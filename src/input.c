#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/input.h"

// Generic string reader from buffer
void read_string(char *buffer, int max_length, const char *prompt)
{
    printf("%s", prompt);
    if (fgets(buffer, max_length, stdin) != NULL)
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

// Read contact name
void read_name(char *buffer)
{
    read_string(buffer, 100, "Enter name: ");
    capitalize_first(buffer);
}

// Read contact surname
void read_surname(char *buffer)
{
    read_string(buffer, 100, "Enter surname: ");
    capitalize_first(buffer);
}

// Read contact note
void read_note(char *buffer)
{
    read_string(buffer, 1024, "Enter note: ");
}

// Read advantage
void read_advantage(char *buffer)
{
    read_string(buffer, 1024, "Enter advantage (optional, press Enter to skip): ");
}

// Read source type with validation
void read_source_type(char *buffer)
{
    int valid = 0;

    while (!valid)
    {
        printf("\n--- How did you meet this person? ---\n");
        printf("1. Event (conference, meeting, university, etc.)\n");
        printf("2. Contact (introduced by someone)\n");
        printf("3. Skip this information\n");
        printf("Enter choice (1/2/3): ");

        if (fgets(buffer, 20, stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "1") == 0)
            {
                strcpy(buffer, "event");
                valid = 1;
            }
            else if (strcmp(buffer, "2") == 0)
            {
                strcpy(buffer, "contact");
                valid = 1;
            }
            else if (strcmp(buffer, "3") == 0)
            {
                strcpy(buffer, "");
                valid = 1;
            }
            else
            {
                printf("Invalid choice! Please enter 1, 2, or 3.\n");
            }
        }
    }
}

// Read source value with context-aware prompts
void read_source_value(char *buffer, const char *source_type)
{
    if (source_type == NULL || strlen(source_type) == 0)
    {
        strcpy(buffer, "");
        return;
    }

    if (strcmp(source_type, "event") == 0)
    {
        read_string(buffer, 200, "Enter event/place name (e.g., 'University of Warsaw', 'Tech Conference 2024'): ");
        capitalize_first(buffer);
    }
    else if (strcmp(source_type, "contact") == 0)
    {
        read_string(buffer, 200, "Enter person's name who introduced you (e.g., 'John Smith'): ");
        capitalize_first(buffer);
    }
}

// Read hashtags - returns dynamically allocated array
char **read_hashtags(void)
{
    char **hashtags = (char **)malloc(11 * sizeof(char *)); // Max 10 + NULL terminator
    if (!hashtags)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    printf("Enter hashtags (press Enter to finish):\n");
    int count = 0;

    while (count < 10)
    {
        char buffer[100];
        printf("Hashtag %d (or press Enter to finish): ", count + 1);

        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strlen(buffer) == 0)
            {
                break;
            }

            hashtags[count] = (char *)malloc(strlen(buffer) + 1);
            if (!hashtags[count])
            {
                printf("Memory allocation failed.\n");
                free_hashtags(hashtags);
                return NULL;
            }

            strcpy(hashtags[count], buffer);
            to_lowercase(hashtags[count]);
            count++;
        }
    }

    hashtags[count] = NULL; // NULL terminate the array
    return hashtags;
}

// Free hashtags array
void free_hashtags(char **hashtags)
{
    if (!hashtags)
        return;

    for (int i = 0; hashtags[i] != NULL; i++)
    {
        free(hashtags[i]);
    }
    free(hashtags);
}

// Read user menu choice
int read_choice(void)
{
    char buffer[10];
    printf("\nEnter your choice: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        return atoi(buffer);
    }
    return -1;
}
