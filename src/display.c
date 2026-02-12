#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/display.h"
#include "../include/error.h"

// Function to print all contacts from the linked list
void list_contacts(void)
{
    Contact *current = contact_list_head;
    int count = 1;

    if (!current)
    {
        printf("No contacts found.\n");
        return;
    }

    while (current)
    {
        printf("\n=== Contact %d (ID: %d) ===\n", count++, current->id);
        printf("Name: %s\n", current->name);
        printf("Surname: %s\n", current->surname);
        printf("Note: %s\n", current->note);
        if (current->advantage[0] != '\0')
        {
            printf("Advantage: %s\n", current->advantage);
        }
        if (current->source_type[0] != '\0')
        {
            printf("Source: %s", current->source_type);
            if (current->source_value[0] != '\0')
            {
                if (strcmp(current->source_type, "event") == 0)
                {
                    printf(" - %s", current->source_value);
                }
                else if (strcmp(current->source_type, "contact") == 0)
                {
                    printf(" - %s", current->source_value);
                }
            }
            printf("\n");
        }

        if (current->hashtag_count > 0)
        {
            printf("Hashtags: ");
            for (int i = 0; i < current->hashtag_count; i++)
            {
                printf("#%s ", current->hashtags[i]);
            }
            printf("\n");
        }

        current = current->next;
    }
}

// Function to list only ID, name, and surname of all contacts (brief list)
void list_contacts_brief(void)
{
    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Error reading contacts: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("No contacts found.\n");
        clear_contact_list();
        return;
    }

    Contact *current = contact_list_head;

    printf("\n=== All Contacts (Brief) ===\n");
    printf("%-5s %-20s %-20s\n", "ID", "Name", "Surname");
    printf("---------------------------------------------------\n");

    while (current)
    {
        printf("%-5d %-20s %-20s\n", current->id, current->name, current->surname);
        current = current->next;
    }

    clear_contact_list();
}

// Function to display full details of a specific contact by ID or name+surname
void show_contact_details(int id, char *name, char *surname)
{
    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Error reading contacts: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("No contacts found.\n");
        clear_contact_list();
        return;
    }

    Contact *current = contact_list_head;
    int found = 0;

    while (current)
    {
        int is_match = 0;

        if (id > 0 && current->id == id)
        {
            is_match = 1;
        }
        else if (id == -1 && name && surname)
        {
            if (strcmp(current->name, name) == 0 && strcmp(current->surname, surname) == 0)
            {
                is_match = 1;
            }
        }

        if (is_match)
        {
            found = 1;
            printf("\n========================================\n");
            printf("           CONTACT DETAILS\n");
            printf("========================================\n");
            printf("ID:       %d\n", current->id);
            printf("Name:     %s\n", current->name);
            printf("Surname:  %s\n", current->surname);
            printf("Note:     %s\n", current->note);
            if (current->advantage[0] != '\0')
            {
                printf("Advantage: %s\n", current->advantage);
            }
            if (current->source_type[0] != '\0')
            {
                printf("Source:   %s", current->source_type);
                if (current->source_value[0] != '\0')
                {
                    if (strcmp(current->source_type, "event") == 0)
                    {
                        printf(" - Event: %s", current->source_value);
                    }
                    else if (strcmp(current->source_type, "contact") == 0)
                    {
                        printf(" - Contact: %s", current->source_value);
                    }
                }
                printf("\n");
            }

            if (current->hashtag_count > 0)
            {
                printf("Hashtags: ");
                for (int i = 0; i < current->hashtag_count; i++)
                {
                    printf("#%s ", current->hashtags[i]);
                }
                printf("\n");
            }
            else
            {
                printf("Hashtags: None\n");
            }
            printf("========================================\n");
            break;
        }

        current = current->next;
    }

    if (!found)
    {
        if (id > 0)
        {
            printf("Contact with ID %d not found.\n", id);
        }
        else if (name && surname)
        {
            printf("Contact '%s %s' not found.\n", name, surname);
        }
    }

    clear_contact_list();
}

// Function to list all unique hashtags from all contacts
void list_all_hashtags(void)
{
    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Error reading contacts: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("No contacts found.\n");
        clear_contact_list();
        return;
    }

    // Array to store unique hashtags
    char unique_hashtags[1000][100];
    int unique_count = 0;

    Contact *current = contact_list_head;

    // Collect all unique hashtags
    while (current)
    {
        for (int i = 0; i < current->hashtag_count; i++)
        {
            int exists = 0;
            for (int j = 0; j < unique_count; j++)
            {
                if (strcmp(unique_hashtags[j], current->hashtags[i]) == 0)
                {
                    exists = 1;
                    break;
                }
            }

            if (!exists && unique_count < 1000)
            {
                strcpy(unique_hashtags[unique_count], current->hashtags[i]);
                unique_count++;
            }
        }
        current = current->next;
    }

    // Print all unique hashtags
    if (unique_count == 0)
    {
        printf("No hashtags found.\n");
    }
    else
    {
        printf("\n=== All Hashtags (%d unique) ===\n", unique_count);
        for (int i = 0; i < unique_count; i++)
        {
            printf("#%s\n", unique_hashtags[i]);
        }
    }

    clear_contact_list();
}

// Function to find and list all contacts with a specific hashtag
void find_contacts_by_hashtag(char *hashtag)
{
    if (!hashtag)
    {
        printf("Error: NULL hashtag provided.\n");
        return;
    }

    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Error reading contacts: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("No contacts found.\n");
        clear_contact_list();
        return;
    }

    Contact *current = contact_list_head;
    int found_count = 0;

    printf("\n=== Contacts with hashtag #%s ===\n", hashtag);

    while (current)
    {
        int has_hashtag = 0;
        for (int i = 0; i < current->hashtag_count; i++)
        {
            if (strcmp(current->hashtags[i], hashtag) == 0)
            {
                has_hashtag = 1;
                break;
            }
        }

        if (has_hashtag)
        {
            found_count++;
            printf("\n--- Contact ID: %d ---\n", current->id);
            printf("Name: %s %s\n", current->name, current->surname);
            printf("Note: %s\n", current->note);

            if (current->hashtag_count > 0)
            {
                printf("Hashtags: ");
                for (int i = 0; i < current->hashtag_count; i++)
                {
                    printf("#%s ", current->hashtags[i]);
                }
                printf("\n");
            }
        }

        current = current->next;
    }

    if (found_count == 0)
    {
        printf("No contacts found with hashtag #%s\n", hashtag);
    }

    clear_contact_list();
}

// Function to display a simple network diagram showing how you know a contact
void show_contact_network(char *name, char *surname)
{
    if (!name || !surname)
    {
        printf("Error: Name and surname required.\n");
        return;
    }

    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Error reading contacts: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("No contacts found.\n");
        clear_contact_list();
        return;
    }

    // Find the target contact
    Contact *target = contact_list_head;
    int found = 0;

    while (target)
    {
        if (strcmp(target->name, name) == 0 && strcmp(target->surname, surname) == 0)
        {
            found = 1;
            break;
        }
        target = target->next;
    }

    if (!found)
    {
        printf("Contact '%s %s' not found.\n", name, surname);
        clear_contact_list();
        return;
    }

    // Display network diagram
    printf("\n");
    printf("========================================\n");
    printf("       CONTACT NETWORK DIAGRAM\n");
    printf("========================================\n");

    if (target->source_type[0] == '\0')
    {
        printf("No source information available.\n");
    }
    else if (strcmp(target->source_type, "event") == 0)
    {
        printf("EVENT: %s\n", target->source_value);
        printf("   |\n");
        printf("   v\n");
        printf("%s %s (ID: %d)\n", target->name, target->surname, target->id);
    }
    else if (strcmp(target->source_type, "contact") == 0)
    {
        printf("%s\n", target->source_value);
        printf("   |\n");
        printf("   v\n");
        printf("%s %s (ID: %d)\n", target->name, target->surname, target->id);
    }

    printf("========================================\n");

    clear_contact_list();
}

// Function to find and list all contacts by event
void find_contacts_by_event(char *event)
{
    if (!event)
    {
        printf("Błąd: Nie podano eventu.\n");
        return;
    }

    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        printf("Błąd odczytu kontaktów: %s\n", get_error_message(err));
        return;
    }

    if (!contact_list_head)
    {
        printf("Nie znaleziono kontaktów.\n");
        clear_contact_list();
        return;
    }

    Contact *current = contact_list_head;
    int found_count = 0;

    printf("\n=== Kontakty z eventu: %s ===\n", event);

    while (current)
    {
        if (strcmp(current->source_type, "event") == 0 &&
            strcmp(current->source_value, event) == 0)
        {
            found_count++;
            printf("\n--- Kontakt ID: %d ---\n", current->id);
            printf("Imię i nazwisko: %s %s\n", current->name, current->surname);
            printf("Notatka: %s\n", current->note);

            if (current->hashtag_count > 0)
            {
                printf("Hashtagi: ");
                for (int i = 0; i < current->hashtag_count; i++)
                {
                    printf("#%s ", current->hashtags[i]);
                }
                printf("\n");
            }
        }

        current = current->next;
    }

    if (found_count == 0)
    {
        printf("Nie znaleziono kontaktów z eventu '%s'\n", event);
    }
    else
    {
        printf("\nZnaleziono kontaktów: %d\n", found_count);
    }

    clear_contact_list();
}
