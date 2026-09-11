#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t StrLen(const char * str);
int Puts(const char * str);
char * StrCpy(char * dest, const char * src);
char * StrCat(char * dest, const char * src);
int StrCmp(const char * lhs, const char * rhs);
char * StrChr(const char * str, int ch);
char * StrRChr(const char * str, int ch);
char * StrRvs(char * str);
char * StrDup(const char * str);
char * Gets(char * str);
char * FGets(char * str, int count, FILE * stream);
void QSortForInts(int * ptr, size_t count, size_t size);

int main(void)
{
    unsigned int size = 0;
    int * numbers = 0;

    printf("Size of massive: ");
    scanf("%d", &size);
    numbers = (int *) calloc(size, sizeof(int));

    printf("Elements of massive (ints): ");
    for (unsigned int i = 0; i < size; i++)
    {
        scanf("%d", numbers + i);
    }

    QSortForInts(numbers, 0, size-1);
    printf("Sorted massive: ");
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%d ", numbers[i]);
    }

    return 0;
}

size_t StrLen(const char * str)
{
    size_t number_of_symbols = 0;

    while (str[number_of_symbols++] != '\0');

    return --number_of_symbols;
}

int Puts(const char * str)
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

char * StrCpy(char * dest, const char * src)
{
    if (dest == NULL && src == NULL)
    {
        return dest;
    }

    unsigned int number_of_symbol = 0;

    do {
        dest[number_of_symbol] = src[number_of_symbol];
    } while (src[++number_of_symbol] != '\0');

    return dest;
}
char * StrCat(char * dest, const char * src)
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

int StrCmp(const char * lhs, const char * rhs)
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

char * StrChr(const char * str, int ch)
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

char * StrRChr(const char * str, int ch)
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

char * StrRvs(char * str)
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

char * StrDup(const char * str)
{
    char * str_dup = (char *) calloc(StrLen(str) + 1, sizeof(char));

    return StrCpy(str_dup, str);
}

char * Gets(char * str)
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

char * FGets(char * str, int count, FILE * stream)
{
    int ch = 0;
    unsigned int number_of_symbol = 0;

    while (number_of_symbol < count - 1 && (ch = getc(stream)) != '\n' && ch != EOF) //в слачае неудачи getc и так поставит ошибку в stdin
    {
        str[number_of_symbol++] = ch;
    }

    str[number_of_symbol] = '\0';

    return str;
}

void QSortForInts(int * ptr, size_t first, size_t last)
{
    if (first < last)
    {
        float med = (ptr[first] + ptr[last] + ptr[(first + last) / 2]) / 3.0;

        size_t left_index = first, right_index = last, med_index = 0;
        int temp = 0;

        while (1)
        {
            while (ptr[left_index] <= med && left_index++ < right_index);

            while (ptr[right_index] > med && left_index < right_index--);

            if (left_index >= right_index)
            {
                if (ptr[left_index] < med)
                {
                    med_index = left_index;
                }

                else
                {
                    med_index = --left_index;
                }

                break;
            }

            else
            {
                temp = ptr[left_index];
                ptr[left_index] = ptr[right_index];
                ptr[right_index] = temp;
            }
        }
        if (med_index != last)
        {
            QSortForInts(ptr, first, med_index);
        }

        if (med_index + 1 != first)
        {
            QSortForInts(ptr, med_index + 1, last);
        }
    }
}
