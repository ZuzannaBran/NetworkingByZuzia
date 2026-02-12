#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/utils.h"

// Function to capitalize first letter and lowercase the rest
void capitalize_first(char *str)
{
    if (str == NULL || strlen(str) == 0)
        return;

    // First character to uppercase
    str[0] = toupper(str[0]);

    // Rest to lowercase
    for (int i = 1; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// Function to convert entire string to lowercase
void to_lowercase(char *str)
{
    if (str == NULL)
        return;

    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// Function to wait for user to press Enter
void wait_for_enter(void)
{
    printf("\nPress Enter to return to menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
