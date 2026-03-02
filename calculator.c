/*
 * A simple recursive-descent parser
 * for evaluating the + and * mathematical functions
 *
 * Functions:
 * parseSum()     - handles addition
 * parseProduct() - handles multiplication
 * parseFactor()  - detects a digit and transforms it into an integer
 */

#include <stdio.h>
#include <stdlib.h>

/* Function declarations */
int parseSum();
int parseProduct();
int parseFactor();

/* Global variable pointing to current position in input */
char *x;

int parseSum()
{
    int pro1 = parseProduct();
    while (*x == '+')
    {
        ++x;
        int pro2 = parseProduct();
        pro1 = pro1 + pro2;
    }
    return pro1;
}

int parseProduct()
{
    int fac1 = parseFactor();
    while (*x == '*')
    {
        ++x;
        int fac2 = parseFactor();
        fac1 = fac1 * fac2;
    }
    return fac1;
}

int parseFactor()
{
    if (*x >= '0' && *x <= '9')
    {
        return *x++ - '0';
    }
    else
    {
        printf("Expected digit but found %c\n", *x);
        exit(1);
    }
}

int main()
{
    char input[100];
    printf("Enter an expression (digits with + and * only, e.g. 2*3+4): ");
    scanf("%s", input);
    x = input;
    int result = parseSum();
    printf("Result: %d\n", result);
    return 0;
}