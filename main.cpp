#define COMPARE_WITH_MED_VALUE(index) (*Compare)(Addressing(ptr, (index), size_of_elem), med_value)
#define GET_FIRST_BYTE(index) (*((char **) Addressing(ptr, (index), size_of_elem)))[0]
#define IS_END_OF_STRING(str) ((str) == '\n' || (str) == '\0')

#ifdef DEBUG
    #define DEBUG_PRINTF(str, ...) printf(str, ##__VA_ARGS__)
#else
    #define DEBUG_PRINTF(str, ...)
    #define NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>

typedef char byte;

const int CODE_OF_CTRL_Z = 26;
const int MAX_LEN_OF_FILENAME = 81;

enum Comparing
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

enum YesNoAnswers
{
    NO = 0,
    YES = 1
};

struct Text
{
    char* text;
    size_t size;
};

struct Index
{
    char** index;
    size_t size;
};

size_t MyStrLen(const char *str);
size_t MyChrCount(const char *str, const int ch);
void MyChrFind(char* index[], const char *str, const int ch);

void QSort(void *ptr, const size_t first, const size_t last, const size_t size_of_elem,
           int (*Compare)(const void *ptr_a, const void *ptr_b));
int Compare(const void *ptr_a, const void *ptr_b);
int CompareRev(const void *ptr_a, const void *ptr_b);
void Swap(void *a, void *b, const size_t size_of_elem);
void* Addressing(const void *ptr, const size_t index, const size_t size_of_elem);

void TextCallocation(const int fd, Text *ptr_text_structure);
void IndexCallocation(const char *text, Index *ptr_index_structure);
void WriteToFile(const int fd, const char* text[], const size_t size);

void PrintOpenError(const char* name_of_file);
void PrintCallocError(const char* name);
void PrintWriteError(const int fd);
void PrintCloseError(const char* name_of_file);

void GetInputOutputNames(char *name_of_input, char *name_of_output);
void FilenameMenu(const char* name_of_file);
void RequestAnswer(void);
int InputOutputMenuAnswer(void);
void InvalidYesOrNoMenuAnswer(void);
void GetFilename(char *name_of_file);
void RequestForFilename(void);
void InvalidGetFilenameAnswer(void);

bool CheckString(void);
void SkipString(void);
void StringClean(char *str);

int main(void)
{
    char name_of_input[MAX_LEN_OF_FILENAME] = "input.txt";
    char name_of_output[MAX_LEN_OF_FILENAME] = "output.txt";
    const int mode = 0666;
    const char *filler = "\n------------------------------------\n\n------------------------------------\n\n";

    GetInputOutputNames(name_of_input, name_of_output);

    int fd_input = open(name_of_input, O_RDONLY | O_BINARY);
    if (fd_input == -1) PrintOpenError(name_of_input);

    int fd_output = open(name_of_output, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, mode);
    if (fd_output == -1) PrintOpenError(name_of_output);

    Text text_structure = {};
    TextCallocation(fd_input, &text_structure);

    Index index_structure = {};
    IndexCallocation(text_structure.text, &index_structure);

    QSort(index_structure.index, 0, index_structure.size - 1, sizeof(index_structure.index[0]), &Compare);
    WriteToFile(fd_output, (const char**) index_structure.index, index_structure.size);

    qsort(index_structure.index, index_structure.size, sizeof(index_structure.index[0]), &CompareRev);
    write(fd_output, filler, strlen(filler));
    WriteToFile(fd_output, (const char**) index_structure.index, index_structure.size);

    write(fd_output, filler, strlen(filler));
    write(fd_output, text_structure.text, text_structure.size);

    if (close(fd_input) == -1) PrintCloseError(name_of_input);
    if (close(fd_output) == -1) PrintCloseError(name_of_output);
    free(text_structure.text);
    free(index_structure.index);

    return 0;
}

size_t MyStrLen(const char *str)
{
    assert(str != NULL);

    size_t number_of_symbols = 0;

    while (str[number_of_symbols] != '\n' && str[number_of_symbols] != '\0')
    {
        number_of_symbols++;
    }

    return ++number_of_symbols;
}

size_t MyChrCount(const char *str, const int ch)
{
    assert(str != NULL);

    size_t number_of_ch = 1;
    unsigned int index = 0;

    while (str[index] != '\0')
    {
        if (str[index] == ch && str[index + 1] != '\0')
        {
            number_of_ch++;
        }

        index++;
    }

    return number_of_ch;
}

void MyChrFind(char* index[], const char *str, const int ch)
{
    assert(index != NULL);
    assert(str != NULL);

    size_t number_of_str = 1;
    unsigned int index_of_elem = 0;

    while (str[index_of_elem] != '\0')
    {
        if (str[index_of_elem] == ch && str[index_of_elem + 1] != '\0')
        {
            index[number_of_str] = (char*) str + index_of_elem + 1;
            number_of_str++;
        }

        index_of_elem++;
    }
}

void QSort(void *ptr, const size_t first, const size_t last, const size_t size_of_elem,
           int (*Compare)(const void *ptr_a, const void *ptr_b))
{
    assert(ptr != NULL);

    if (first < last)
    {
        char *med_value = (char*) calloc(size_of_elem, sizeof(char));
        if (med_value == NULL) PrintCallocError("med value");
        DEBUG_PRINTF("\nfirst = %d, last = %d\n", first, last);

        void *med = (char *) ptr + ((first + last) / 2) * size_of_elem;
        for (size_t index = 0; index < size_of_elem; index++)
        {
            med_value[index] = *((char *) med + index);
        }

        DEBUG_PRINTF("med_value = '%c'\n", (*((char **) med_value))[0]);

        size_t left_index = first, right_index = last;

        while (1)
        {
            while (COMPARE_WITH_MED_VALUE(left_index) == SMALLER
            && left_index < right_index && left_index != last) left_index++;
            DEBUG_PRINTF("left_index = %d ", left_index);

            while (COMPARE_WITH_MED_VALUE(right_index) == BIGGER
            && left_index < right_index && right_index != first) right_index--;
            DEBUG_PRINTF("right_index = %d\n", right_index);

            if (left_index >= right_index)
            {
                break;
            }

            DEBUG_PRINTF("before swap: ptr[left_index] = '%c', ptr[right_index] = '%c'\n",
                          GET_FIRST_BYTE(left_index), GET_FIRST_BYTE(right_index));
            Swap(Addressing(ptr, left_index, size_of_elem), Addressing(ptr, right_index, size_of_elem), size_of_elem);
            DEBUG_PRINTF("after swap: ptr[left_index] = '%c', ptr[right_index] = '%c'\n",
                          GET_FIRST_BYTE(left_index), GET_FIRST_BYTE(right_index));

            if (COMPARE_WITH_MED_VALUE(left_index) == EQUAL && left_index != last) left_index++;
            if (COMPARE_WITH_MED_VALUE(right_index) == EQUAL && right_index != first) right_index--;
        }

        free(med_value);

        if (left_index != last) QSort(ptr, first, left_index, size_of_elem, Compare);
        if (left_index != first) QSort(ptr, left_index, last, size_of_elem, Compare);
    }
}

int Compare(const void *ptr_a, const void *ptr_b)
{
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    const char *a = *((const char **) ptr_a);
    const char *b = *((const char **) ptr_b);
    unsigned int index_a = 0, index_b = 0;

    while (!IS_END_OF_STRING(a[index_a]) && !IS_END_OF_STRING(b[index_b]))
    {
        if (!isalpha(a[index_a])) index_a++;
        else if (!isalpha(b[index_b])) index_b++;
        else
        {
            if (tolower(a[index_a]) > tolower(b[index_b])) return BIGGER;
            if (tolower(a[index_a]) < tolower(b[index_b])) return SMALLER;
            index_a++;
            index_b++;
        }
    }

    if (IS_END_OF_STRING(a[index_a]) && IS_END_OF_STRING(b[index_b])) return EQUAL;
    if (IS_END_OF_STRING(a[index_a])) return SMALLER;
    return BIGGER;
}

int CompareRev(const void *ptr_a, const void *ptr_b)
{
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    const char *a = *((const char **) ptr_a);
    const char *b = *((const char **) ptr_b);
    int index_a = 0, index_b = 0;

    while (!IS_END_OF_STRING(a[index_a])) index_a++;
    while (!IS_END_OF_STRING(b[index_b])) index_b++;

    while (index_a >= 0 && index_b >= 0)
    {
        if (!isalpha(a[index_a])) index_a--;
        else if (!isalpha(b[index_b])) index_b--;
        else
        {
            if (tolower(a[index_a]) > tolower(b[index_b])) return BIGGER;
            if (tolower(a[index_a]) < tolower(b[index_b])) return SMALLER;
            index_a--;
            index_b--;
        }
    }

    if (index_a < 0 && index_b < 0) return EQUAL;
    if (index_a < 0) return SMALLER;
    return BIGGER;
}

void Swap(void *a, void* b, const size_t size_of_elem)
{
    assert(a != NULL);
    assert(b != NULL);

    byte temp = 0;

    for (size_t index = 0; index < size_of_elem; index++)
    {
        temp = *((byte *) a + index);
        *((byte *) a + index) = *((byte *) b + index);
        *((byte *) b + index) = temp;
    }
}

void* Addressing(const void *ptr, const size_t index, const size_t size_of_elem)
{
    assert(ptr != NULL);

    return (char *) ptr + index * size_of_elem;
}

void TextCallocation(const int fd, Text *ptr_text_structure)
{
    assert(ptr_text_structure != NULL);

    struct stat statistics = {};
    fstat(fd, &statistics);

    ptr_text_structure->text = (char *) calloc(statistics.st_size + 1, sizeof(char));
    if (ptr_text_structure->text == NULL) PrintCallocError("text");
    ptr_text_structure->size = statistics.st_size + 1;

    read(fd, ptr_text_structure->text, statistics.st_size);
    if (ptr_text_structure->text[statistics.st_size - 1] != '\n') ptr_text_structure->text[statistics.st_size] = '\n';
}

void IndexCallocation(const char *text, Index *ptr_index_structure)
{
    assert(text != NULL);
    assert(ptr_index_structure != NULL);

    int number_of_strings = MyChrCount(text, '\n');

    ptr_index_structure->index = (char**) calloc(number_of_strings, sizeof(char *));
    if (ptr_index_structure->index == NULL) PrintCallocError("index massive");
    ptr_index_structure->size = number_of_strings;

    ptr_index_structure->index[0] = (char *) text;
    MyChrFind(ptr_index_structure->index, text, '\n');
}

void WriteToFile(const int fd, const char* text[], const size_t size)
{
    assert(text != NULL);

    for (unsigned int index = 0; index < size; index++)
    {
        if (write(fd, text[index], MyStrLen(text[index])) == -1)
        {
            PrintWriteError(fd);
        }
    }
}

void PrintOpenError(const char* name_of_file)
{
    assert(name_of_file != NULL);

    printf("Error opening file \"%s\".\n", name_of_file);
}

void PrintCallocError(const char* name)
{
    assert(name != NULL);

    printf("Error callocation for \"%s\".\n", name);
}

void PrintWriteError(const int fd)
{
    printf("Error writing file (file desc = %d)).\n", fd);
}

void PrintCloseError(const char* name_of_file)
{
    assert(name_of_file != NULL);

    printf("Error closing file \"%s\".\n", name_of_file);
}

void GetInputOutputNames(char *name_of_input, char *name_of_output)
{
    assert(name_of_input != NULL);
    assert(name_of_output != NULL);

    FilenameMenu("input");
    if (InputOutputMenuAnswer() == YES) GetFilename(name_of_input);

    FilenameMenu("output");
    if (InputOutputMenuAnswer() == YES) GetFilename(name_of_output);
}

void FilenameMenu(const char* name_of_file)
{
    assert(name_of_file != NULL);

    printf("\nDo you want to enter custom %s filename (default name is %s.txt)?\n"
           "y) Yes\n"
           "n) No\n", name_of_file, name_of_file);
    RequestAnswer();

}

void RequestAnswer(void)
{
    printf("\nEnter chosen variant: ");
}

int InputOutputMenuAnswer(void)
{
    int answer = 0;

    while (1)
    {
        while (isspace(answer = getchar()));

        if (answer == 'y' || answer == 'Y')
        {
            if (CheckString()) return YES;
        }

        else if (answer == 'n' || answer == 'N')
        {
            if (CheckString()) return NO;
        }

        SkipString();
        InvalidYesOrNoMenuAnswer();
    }
}

void InvalidYesOrNoMenuAnswer(void)
{
    printf("Verify only y(Y)/n(N) answers: ");
}

void GetFilename(char *name_of_file)
{
    assert(name_of_file != NULL);

    int symbol = 0;
    unsigned int index = 0;
    bool need_to_continue = true;

    RequestForFilename();

    while (need_to_continue)
    {
        while (isspace(symbol = getchar()));
        index = 0;
        StringClean(name_of_file);

        while (index < MAX_LEN_OF_FILENAME - 1 && symbol != CODE_OF_CTRL_Z && symbol != EOF && !isspace(symbol))
        {
            name_of_file[index++] = symbol;
            symbol = getchar();
        }

        if (symbol == '\n') need_to_continue = false;

        else
        {
            SkipString();
            InvalidGetFilenameAnswer();
        }
    }
}

void RequestForFilename(void)
{
    printf("\nEnter custom filename (less then %d symbols): ", MAX_LEN_OF_FILENAME);
}

void InvalidGetFilenameAnswer(void)
{
    printf("In entered data more then 2 words or more then %d symbols. Enter data correctly: ", MAX_LEN_OF_FILENAME - 1);
}


bool CheckString(void)
{
    int ch = 0;

    while (isspace(ch = getchar()))
    {
        if (ch == '\n') return true;
    }

    if (ch != CODE_OF_CTRL_Z && ch != EOF) SkipString();

    return false;
}

void SkipString(void)
{
    int ch = 0;

    while ((ch = getchar()) != CODE_OF_CTRL_Z && ch != '\n' && ch != EOF);
}

void StringClean(char *str)
{
    assert(str != NULL);

    unsigned int index = 0;

    while (str[index] != '\0') str[index++] = '\0';
}
