#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum Comparing
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

const char name_of_input[]  = "input.txt";
const char name_of_output[]  = "ouput.txt";
const int number_of_str = 12000;;
const int size_of_buffer = 10000;

size_t StrLen(const char *str);
size_t PtrStrLen(const char *str[]);
int Puts(const char *str);
char* StrCpy(char *dest, const char *src);
char** PtrStrCpy(char *dest[], const char *src[]);
char* StrCat(char *dest, const char *src);
int StrCmp(const char *lhs, const char *rhs);
char* StrChr(const char *str, int ch);
char* StrRChr(const char *str, int ch);
char* StrRvs(char *str);
char* StrDup(const char *str);
char* Gets(char *str);
char* FGets(char *str, int count, FILE *stream);

void QSort(void *ptr, size_t first, size_t last, size_t size_of_elem, int (*Compare)(const void *ptr_a, const void *ptr_b));
void Swap(void *a, void *b, size_t size_of_elem);
int ReadFromFile(FILE *ptr_file, char* text[]);
int WriteToFile(FILE *ptr_file, char* text[]);
int Compare(const void *ptr_a, const void *ptr_b);
int CompareRev(const void *ptr_a, const void *ptr_b);

int main(void)
{
    unsigned int size = 0;
    FILE * ptr_input = fopen(name_of_input, "r");
    FILE * ptr_output = fopen(name_of_output, "w");
    char * poem[number_of_str] = {};
    char * original_poem[number_of_str] = {};

    ReadFromFile(ptr_input, original_poem);
    size = PtrStrLen((const char **) original_poem);
    PtrStrCpy(poem, (const char **) original_poem);
    QSort((void *) poem, 0, size-1, sizeof(poem[0]), &Compare);
    WriteToFile(ptr_output, poem);
    qsort((void *) poem, size, sizeof(poem[0]), &CompareRev);
    WriteToFile(ptr_output, poem);
    WriteToFile(ptr_output, original_poem);

    fclose(ptr_input);
    fclose(ptr_output);
    for (unsigned int index = 0; index < size; index++)
    {
        free(poem[index]);
    }

    return 0;
}

size_t StrLen(const char *str)
{
    size_t number_of_symbols = 0;

    while (str[number_of_symbols++] != '\0');

    return --number_of_symbols;
}

int Puts(const char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    unsigned int number_of_symbol = 0;
    int returned_value = 0;

    while (str[number_of_symbol++] != '\0')
    {
        returned_value = putchar(str[number_of_symbol]); //в слачае неудачи putchar и так поставит ошибку в stdin
    }

    returned_value = putchar('\n');

    return returned_value;
}

char* StrCpy(char *dest, const char *src)
{
    if (dest == NULL && src == NULL)
    {
        return dest;
    }

    unsigned int number_of_symbol = 0;

    do {
        dest[number_of_symbol] = src[number_of_symbol];
    } while (src[number_of_symbol++] != '\0');

    return dest;
}
char* StrCat(char *dest, const char *src)
{
    unsigned int number_of_symbol_dest = 0;
    unsigned int number_of_symbol_src = 0;

    while (dest[number_of_symbol_dest++] != '\0');

    while (src[number_of_symbol_src] != '\0')
    {
        dest[number_of_symbol_dest++] = src[number_of_symbol_src++];
    }

    return dest;
}

int StrCmp(const char *lhs, const char *rhs)
{
    if (lhs == NULL && rhs == NULL)
    {
        return 0;
    }

    unsigned int number_of_symbol_lhs = 0;
    unsigned int number_of_symbol_rhs = 0;

    while (lhs[number_of_symbol_lhs] == rhs[number_of_symbol_rhs]
    && lhs[number_of_symbol_lhs++] != '\0' && rhs[number_of_symbol_rhs++] != '\0');

    return lhs[number_of_symbol_lhs] - rhs[number_of_symbol_rhs];
}

char* StrChr(const char *str, int ch)
{
    unsigned int number_of_symbol = 0;
    char * returned_value = (char *) str;

    while (str[number_of_symbol] != '\0' && str[number_of_symbol++] != ch);

    if (str[number_of_symbol--] != '\0')
    {
        returned_value += number_of_symbol;
        return returned_value;
    }

    else
    {
        return 0;
    }
}

char* StrRChr(const char *str, int ch)
{
    unsigned int number_of_symbol = 0;
    char * returned_value = 0;

    while (str[number_of_symbol] != '\0')
    {
        if (str[number_of_symbol] == ch)
        {
            returned_value = (char *) str + number_of_symbol;
        }

        number_of_symbol++;
    }

    return returned_value;
}

char* StrRvs(char *str)
{
    unsigned int number_of_left_symbol = 0, number_of_right_symbol = StrLen(str) - 1;
    char temp = 0;

    while (number_of_left_symbol < number_of_right_symbol)
    {
        temp = str[number_of_left_symbol];
        str[number_of_left_symbol++] = str[number_of_right_symbol];
        str[number_of_right_symbol--] = temp;
    }

    return str;
}

char* StrDup(const char *str)
{
    char * str_dup = (char *) calloc(StrLen(str) + 1, sizeof(char));

    return StrCpy(str_dup, str);
}

char* Gets(char *str)
{
    int ch = 0;
    unsigned int number_of_symbol = 0;

    while ((ch = getchar()) != '\n' && ch != EOF) //в слачае неудачи getchar и так поставит ошибку в stdin
    {
        str[number_of_symbol++] = ch;
    }

    str[number_of_symbol] = '\0';

    return str;
}

char* FGets(char *str, int count, FILE *stream)
{
    int ch = 0;
    unsigned int number_of_symbol = 0;

    while (number_of_symbol < count - 1 && (ch = getc(stream)) != '\n' && ch != EOF) //в слачае неудачи getc и так поставит ошибку в stdin
    {
        str[number_of_symbol++] = ch;
    }

    str[number_of_symbol] = '\0';

    if (ch == EOF) return NULL;
    else return str;
}

void QSort(void *ptr, size_t first, size_t last, size_t size_of_elem, int (*Compare)(const void *ptr_a, const void *ptr_b))
{
    if (first < last)
    {
        char med_value[size_of_elem] = {};
        //printf("\nfirst = %zu, last = %zu\n", first, last);
        void *med = (char *) ptr + ((first + last) / 2) * size_of_elem;
        for (size_t index = 0; index < size_of_elem; index++) med_value[index] = *((char *) med + index);
        //printf("med = \n");
        size_t left_index = first, right_index = last, med_index = 0;
        int temp = 0;

        while (1)
        {
            while ((*Compare)((char *) ptr + left_index * size_of_elem, (void *) med_value) == SMALLER && left_index < right_index && left_index != last) left_index++;
            //printf("left_index = %zu\n", left_index);

            while ((*Compare)((char *) ptr + right_index * size_of_elem, (void *) med_value) == BIGGER && left_index < right_index && right_index != first) right_index--;
            //printf("right_index = %zu\n", right_index);

            if (left_index >= right_index)
            {
                break;
            }

            //printf("before swap: left = '%c', right = '%c'\n", (*((char **) (ptr + left_index * size_of_elem)))[0], (*((char **) (ptr + right_index * size_of_elem)))[0]);
            Swap((char *) ptr + left_index * size_of_elem, (char *) ptr + right_index * size_of_elem, size_of_elem);
            //printf("after swap: left = '%c', right = '%c'\n", (*((char **) (ptr + left_index * size_of_elem)))[0], (*((char **) (ptr + right_index * size_of_elem)))[0]);

            if ((*Compare)((char *) ptr + left_index * size_of_elem, (void *) med_value) == EQUAL && left_index != last) left_index++;
            if ((*Compare)((char *) ptr + right_index * size_of_elem, (void *) med_value) == EQUAL && right_index != first) right_index--;
        }
        if (left_index != last) QSort(ptr, first, left_index, size_of_elem, Compare);
        if (left_index != first) QSort(ptr, left_index, last, size_of_elem, Compare);
    }
}

int Compare(const void *ptr_a, const void *ptr_b)
{
    const char **a = (const char **) ptr_a;
    const char **b = (const char **) ptr_b;
    unsigned int index_a = 0, index_b = 0;

    while ((*a)[index_a] != '\0' && (*b)[index_b] != '\0')
    {
        if (!isalpha((*a)[index_a])) index_a++;
        else if (!isalpha((*b)[index_b])) index_b++;
        else
        {
            if (tolower((*a)[index_a]) > tolower((*b)[index_b])) return BIGGER;
            else if (tolower((*a)[index_a]) < tolower((*b)[index_b])) return SMALLER;
            index_a++;
            index_b++;
        }
    }

    if ((*a)[index_a] == '\0' && (*b)[index_b] == '\0') return EQUAL;
    else if ((*a)[index_a] == '\0') return SMALLER;
    else return BIGGER;
}

int CompareRev(const void *ptr_a, const void *ptr_b)
{
    const char **a = (const char **) ptr_a;
    const char **b = (const char **) ptr_b;
    int index_a = 0, index_b = 0;

    while ((*a)[index_a] != '\0') index_a++;
    while ((*b)[index_b] != '\0') index_b++;

    while (index_a >= 0 && index_b >= 0)
    {
        if (!isalpha((*a)[index_a])) index_a--;
        else if (!isalpha((*b)[index_b])) index_b--;
        else
        {
            if (tolower((*a)[index_a]) > tolower((*b)[index_b])) return BIGGER;
            else if (tolower((*a)[index_a]) < tolower((*b)[index_b])) return SMALLER;
            index_a--;
            index_b--;
        }
    }

    if (index_a == 0 && index_b == 0) return EQUAL;
    else if (index_a == 0) return SMALLER;
    else return BIGGER;
}

void Swap(void *a, void* b, size_t size_of_elem)
{
    char temp = 0;
    for (size_t index = 0; index < size_of_elem; index++)
    {
        temp = *((char *) a + index);
        *((char *) a + index) = *((char *) b + index);
        *((char *) b + index) = temp;
    }
}

int ReadFromFile(FILE * ptr_file, char * text[])
{
    char buffer[size_of_buffer] = "";
    char clean_buffer[size_of_buffer] = "";
    int success_str = 0;
    unsigned int index = 0;

    //printf("Zashel v reading function\n");

    while (FGets(buffer, size_of_buffer - 1, ptr_file) != NULL)
    {
        text[success_str] = (char *) calloc(StrLen(buffer) + 1, sizeof(char));
        StrCpy(text[success_str++], buffer);
        //printf("success_str = %d, ", success_str);
        //index = 0;
        //printf("'");
        //while (buffer[index] != '\0') printf("%c", buffer[index++]);
        //printf("'\n");
        StrCpy(buffer, clean_buffer);
    }

    return success_str;
}

int WriteToFile(FILE *ptr_file, char *text[])
{
    int success_str = 0;
    //printf("In WTF func\n");
    fprintf(ptr_file, "\nc\ne\ng\nf\no\nl\nt\na\nr\no\nt\ne\nb\na\nl\n\n");
    while (text[success_str] != 0)
    {
        fprintf(ptr_file, "%s\n", text[success_str++]);
    }

    return success_str;
}

size_t PtrStrLen(const char *str[])
{
    size_t number_of_symbols = 0;

    while (str[number_of_symbols++] != 0);

    return --number_of_symbols;
}

char** PtrStrCpy(char *dest[], const char *src[])
{
    if (dest == NULL && src == NULL)
    {
        return dest;
    }

    unsigned int number_of_symbol = 0;

    do {
        dest[number_of_symbol] = (char *) src[number_of_symbol];
    } while (src[number_of_symbol++] != 0);

    return dest;
}
