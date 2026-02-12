#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/contact.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/utils.h"

// Display error message
void display_error(errors err)
{
    if (err != SUCCESS)
    {
        printf("\n[ERROR] %s\n", get_error_message(err));
    }
}

// Display success message
void display_success(const char *message)
{
    printf("\n[SUCCESS] %s\n", message);
}

// Show main menu
void show_main_menu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("    CONTACT NETWORKING SYSTEM MENU\n");
    printf("========================================\n");
    printf("1. Search contact\n");
    printf("2. List contacts\n");
    printf("3. Delete contact\n");
    printf("4. Add new contact\n");
    printf("5. Exit\n");
    printf("========================================\n");
}

// Show search submenu
void show_search_menu(void)
{
    printf("\n--- SEARCH CONTACT ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by name and surname\n");
    printf("3. Search by hashtags\n");
    printf("4. Search by event\n");
    printf("5. Back\n");
    printf("Choose option: ");
}

// Show list submenu
void show_list_menu(void)
{
    printf("\n--- LIST CONTACTS ---\n");
    printf("1. Full list (all details)\n");
    printf("2. Brief list (ID, name, surname)\n");
    printf("3. Back\n");
    printf("Choose option: ");
}

// Add new contact
void menu_add_contact(void)
{
    char name[100], surname[100], note[1024];
    char advantage[1024], source_type[20], source_value[200];
    char **hashtags;

    printf("\n--- ADD NEW CONTACT ---\n");

    read_name(name);
    if (strlen(name) == 0)
    {
        display_error(ERROR_NULL_PARAMETRS);
        return;
    }

    read_surname(surname);
    if (strlen(surname) == 0)
    {
        display_error(ERROR_NULL_PARAMETRS);
        return;
    }

    read_note(note);
    if (strlen(note) == 0)
    {
        display_error(ERROR_NULL_PARAMETRS);
        return;
    }

    read_advantage(advantage);
    read_source_type(source_type);
    read_source_value(source_value, source_type);

    hashtags = read_hashtags();
    if (!hashtags)
    {
        display_error(ERROR_MEMORY_ALLOCATION);
        return;
    }

    errors err = newcontact(name, surname, note, advantage, source_type, source_value, hashtags);
    display_error(err);
    if (err == SUCCESS)
    {
        display_success("Contact added successfully!");
    }

    free_hashtags(hashtags);
    wait_for_enter();
}

// List all contacts detailed
void menu_list_all_contacts(void)
{
    printf("\n--- ALL CONTACTS ---\n");
    errors err = read_existing_contacts();
    display_error(err);
    if (err == SUCCESS || err == ERROR_FILE_OPEN)
    {
        list_contacts();
        clear_contact_list();
    }
    wait_for_enter();
}

// List all contacts brief
void menu_list_brief_contacts(void)
{
    printf("\n--- ALL CONTACTS (BRIEF) ---\n");
    list_contacts_brief();
    wait_for_enter();
}

// Show contact details
void menu_show_contact_details(void)
{
    char choice_str[10];
    printf("\n--- SHOW CONTACT DETAILS ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name and Surname\n");
    printf("Enter choice (1 or 2): ");
    fgets(choice_str, sizeof(choice_str), stdin);
    int choice = atoi(choice_str);

    if (choice == 1)
    {
        printf("Enter contact ID: ");
        fgets(choice_str, sizeof(choice_str), stdin);
        int id = atoi(choice_str);
        show_contact_details(id, NULL, NULL);
    }
    else if (choice == 2)
    {
        char name[100], surname[100];
        read_name(name);
        read_surname(surname);
        show_contact_details(-1, name, surname);
    }
    else
    {
        printf("Invalid choice.\n");
    }
}

// Delete contact
void menu_delete_contact(void)
{
    char choice_str[10];
    printf("\n--- DELETE CONTACT ---\n");
    printf("1. Delete by ID\n");
    printf("2. Delete by name and surname\n");
    printf("Choose option (1 or 2): ");
    fgets(choice_str, sizeof(choice_str), stdin);
    int choice = atoi(choice_str);

    errors err;
    if (choice == 1)
    {
        printf("Enter contact ID: ");
        fgets(choice_str, sizeof(choice_str), stdin);
        int id = atoi(choice_str);
        err = delete_contact(id, NULL, NULL);
    }
    else if (choice == 2)
    {
        char name[100], surname[100];
        read_name(name);
        read_surname(surname);

        // Check for duplicates first
        errors read_err = read_existing_contacts();
        if (read_err != SUCCESS && read_err != ERROR_FILE_OPEN)
        {
            display_error(read_err);
            return;
        }

        // Count matching contacts
        Contact *matching[100];
        int match_count = 0;
        Contact *current = contact_list_head;

        while (current && match_count < 100)
        {
            if (strcmp(current->name, name) == 0 && strcmp(current->surname, surname) == 0)
            {
                matching[match_count++] = current;
            }
            current = current->next;
        }

        if (match_count == 0)
        {
            printf("Contact '%s %s' not found.\n", name, surname);
            clear_contact_list();
            return;
        }
        else if (match_count == 1)
        {
            clear_contact_list();
            err = delete_contact(-1, name, surname);
        }
        else
        {
            // Multiple matches - let user choose
            printf("\nFound %d contacts with the same name and surname:\n", match_count);
            for (int i = 0; i < match_count; i++)
            {
                printf("\n%d. ID: %d\n", i + 1, matching[i]->id);
                printf("   Note: %s\n", matching[i]->note);
                if (strlen(matching[i]->advantage) > 0)
                    printf("   Advantage: %s\n", matching[i]->advantage);
            }

            printf("\nWhich contact to delete (enter number 1-%d or 0 to cancel): ", match_count);
            fgets(choice_str, sizeof(choice_str), stdin);
            int selected = atoi(choice_str);

            if (selected > 0 && selected <= match_count)
            {
                int id_to_delete = matching[selected - 1]->id;
                clear_contact_list();
                err = delete_contact(id_to_delete, NULL, NULL);
            }
            else
            {
                printf("Operation canceled.\n");
                clear_contact_list();
                return;
            }
        }
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    display_error(err);
    if (err == SUCCESS)
    {
        display_success("Contact deleted successfully!");
    }
    wait_for_enter();
}

// List all hashtags
void menu_list_hashtags(void)
{
    printf("\n--- ALL HASHTAGS ---\n");
    list_all_hashtags();
}

// Find contacts by hashtag
void menu_find_by_hashtag(void)
{
    char hashtag[100];
    printf("\n--- SEARCH BY HASHTAG ---\n");
    read_string(hashtag, 100, "Enter hashtag (without #): ");
    find_contacts_by_hashtag(hashtag);
    wait_for_enter();
}

// Find contacts by event
void menu_find_by_event(void)
{
    char event[200];
    printf("\n--- SEARCH BY EVENT ---\n");
    read_string(event, 200, "Enter event name: ");
    find_contacts_by_event(event);
    wait_for_enter();
}

// Show contact network
void menu_show_network(void)
{
    char name[100], surname[100];
    printf("\n--- CONTACT NETWORK ---\n");
    read_name(name);
    read_surname(surname);
    show_contact_network(name, surname);
}

// Handle search submenu
void menu_search(void)
{
    int choice;
    show_search_menu();
    choice = read_choice();

    switch (choice)
    {
    case 1: // Search by ID
    {
        printf("Enter contact ID: ");
        int id = read_choice();
        show_contact_details(id, NULL, NULL);
        wait_for_enter();
    }
    break;
    case 2: // Wyszukaj po imieniu i nazwisku
    {
        char name[100], surname[100];
        read_name(name);
        read_surname(surname);
        show_contact_details(-1, name, surname);
        wait_for_enter();
    }
    break;
    case 3: // Wyszukaj po hashtagach
        menu_find_by_hashtag();
        break;
    case 4: // Wyszukaj po evencie
        menu_find_by_event();
        break;
    case 5: // Back
        return;
    default:
        printf("\nInvalid choice.\n");
    }
}

// Handle list submenu
void menu_list(void)
{
    int choice;
    show_list_menu();
    choice = read_choice();

    switch (choice)
    {
    case 1: // Lista pełna
        menu_list_all_contacts();
        break;
    case 2: // Lista niepełna
        menu_list_brief_contacts();
        break;
    case 3: // Back
        return;
    default:
        printf("\nInvalid choice.\n");
    }
}

// Handle menu choice
void handle_menu_choice(int choice)
{
    switch (choice)
    {
    case 1: // Wyszukaj
        menu_search();
        break;
    case 2: // Lista
        menu_list();
        break;
    case 3: // Usuń
        menu_delete_contact();
        break;
    case 4: // Dodaj nowy kontakt
        menu_add_contact();
        break;
    case 5: // Exit
        printf("\nGoodbye!\n");
        exit(0);
    default:
        printf("\nInvalid choice. Try again.\n");
    }
}
