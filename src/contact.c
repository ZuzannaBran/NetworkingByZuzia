#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/contact.h"

// Global head pointer for the contact list
Contact *contact_list_head = NULL;

errors newcontact(char *name, char *surname, char *note, char *advantage, char *source_type, char *source_value, char *hashtags[])
{
    if (!name || !surname || !note)
    {
        return ERROR_NULL_PARAMETRS;
    }

    // Read existing contacts to get the next ID
    int next_id = 1;
    FILE *temp_file = fopen("contacts.json", "r");
    if (temp_file)
    {
        char line[1024];
        while (fgets(line, sizeof(line), temp_file))
        {
            if (strstr(line, "\"id\""))
            {
                char *colon = strchr(line, ':');
                if (colon)
                {
                    int id = atoi(colon + 1);
                    if (id >= next_id)
                    {
                        next_id = id + 1;
                    }
                }
            }
        }
        fclose(temp_file);
    }

    FILE *file = fopen("contacts.json", "a");
    if (!file)
    {
        return ERROR_FILE_OPEN;
    }

    // Write contact in JSON format
    fprintf(file, "{\n");
    fprintf(file, "  \"id\": %d,\n", next_id);
    fprintf(file, "  \"name\": \"%s\",\n", name);
    fprintf(file, "  \"surname\": \"%s\",\n", surname);
    fprintf(file, "  \"note\": \"%s\",\n", note);
    fprintf(file, "  \"advantage\": \"%s\",\n", advantage ? advantage : "");
    fprintf(file, "  \"source_type\": \"%s\",\n", source_type ? source_type : "");
    fprintf(file, "  \"source_value\": \"%s\"", source_value ? source_value : "");

    // Add hashtags if provided
    if (hashtags && hashtags[0])
    {
        fprintf(file, ",\n  \"hashtags\": [");
        int i = 0;
        while (hashtags[i] != NULL)
        {
            if (i > 0)
                fprintf(file, ", ");
            fprintf(file, "\"%s\"", hashtags[i]);
            i++;
        }
        fprintf(file, "]");
    }

    fprintf(file, "\n}\n");

    if (fclose(file) != 0)
    {
        return ERROR_FILE_CLOSE;
    }

    return SUCCESS;
}

// Read contacts from JSON file into linked list
errors read_existing_contacts(void)
{
    FILE *file = fopen("contacts.json", "r");
    if (!file)
    {
        return ERROR_FILE_OPEN;
    }

    char line[1024];
    Contact *current_contact = NULL;
    int in_contact = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Check if we're starting a new contact
        if (strstr(line, "{"))
        {
            in_contact = 1;
            current_contact = (Contact *)malloc(sizeof(Contact));
            if (!current_contact)
            {
                fclose(file);
                return ERROR_MEMORY_ALLOCATION;
            }
            current_contact->id = 0;
            current_contact->hashtag_count = 0;
            current_contact->advantage[0] = '\0';
            current_contact->source_type[0] = '\0';
            current_contact->source_value[0] = '\0';
            current_contact->next = NULL;
        }
        // Parse ID
        else if (strstr(line, "\"id\""))
        {
            char *colon = strchr(line, ':');
            if (colon)
            {
                current_contact->id = atoi(colon + 1);
            }
        }
        // Parse name
        else if (strstr(line, "\"name\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->name, start, len);
                        current_contact->name[len] = '\0';
                    }
                }
            }
        }
        // Parse surname
        else if (strstr(line, "\"surname\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->surname, start, len);
                        current_contact->surname[len] = '\0';
                    }
                }
            }
        }
        // Parse note
        else if (strstr(line, "\"note\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->note, start, len);
                        current_contact->note[len] = '\0';
                    }
                }
            }
        }
        // Parse advantage
        else if (strstr(line, "\"advantage\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->advantage, start, len);
                        current_contact->advantage[len] = '\0';
                    }
                }
            }
        }
        // Parse source_type
        else if (strstr(line, "\"source_type\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->source_type, start, len);
                        current_contact->source_type[len] = '\0';
                    }
                }
            }
        }
        // Parse source_value
        else if (strstr(line, "\"source_value\""))
        {
            char *start = strchr(line, ':');
            if (start)
            {
                start = strchr(start, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start + 1, '"');
                    if (end)
                    {
                        int len = end - start;
                        strncpy(current_contact->source_value, start, len);
                        current_contact->source_value[len] = '\0';
                    }
                }
            }
        }
        // Parse hashtags
        else if (strstr(line, "\"hashtags\""))
        {
            // Read hashtags array
            char *ptr = strchr(line, '[');
            if (ptr)
            {
                ptr++; // Move past the [
                while (*ptr && *ptr != ']')
                {
                    // Find opening quote
                    if (*ptr == '"')
                    {
                        ptr++;
                        char *end = strchr(ptr, '"');
                        if (end && current_contact->hashtag_count < 10)
                        {
                            int len = end - ptr;
                            strncpy(current_contact->hashtags[current_contact->hashtag_count], ptr, len);
                            current_contact->hashtags[current_contact->hashtag_count][len] = '\0';
                            current_contact->hashtag_count++;
                            ptr = end + 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        ptr++;
                    }
                }
            }
        }
        // Check if we're ending a contact
        else if (strstr(line, "}") && in_contact)
        {
            in_contact = 0;
            // Add to linked list
            if (!contact_list_head)
            {
                contact_list_head = current_contact;
            }
            else
            {
                Contact *temp = contact_list_head;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = current_contact;
            }
        }
    }

    fclose(file);
    return SUCCESS;
}

// Function to clear and free the contact list
void clear_contact_list(void)
{
    Contact *current = contact_list_head;
    Contact *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    contact_list_head = NULL;
}

// Function to rewrite contacts.json file with renumbered contacts
errors save_contacts_to_file(void)
{
    FILE *file = fopen("contacts.json", "w");
    if (!file)
    {
        return ERROR_FILE_OPEN;
    }

    Contact *current = contact_list_head;
    int new_id = 1;

    while (current)
    {
        // Write contact with new ID
        fprintf(file, "{\n");
        fprintf(file, "  \"id\": %d,\n", new_id++);
        fprintf(file, "  \"name\": \"%s\",\n", current->name);
        fprintf(file, "  \"surname\": \"%s\",\n", current->surname);
        fprintf(file, "  \"note\": \"%s\",\n", current->note);
        fprintf(file, "  \"advantage\": \"%s\",\n", current->advantage);
        fprintf(file, "  \"source_type\": \"%s\",\n", current->source_type);
        fprintf(file, "  \"source_value\": \"%s\"", current->source_value);

        if (current->hashtag_count > 0)
        {
            fprintf(file, ",\n  \"hashtags\": [");
            for (int i = 0; i < current->hashtag_count; i++)
            {
                if (i > 0)
                    fprintf(file, ", ");
                fprintf(file, "\"%s\"", current->hashtags[i]);
            }
            fprintf(file, "]");
        }

        fprintf(file, "\n}\n");
        current = current->next;
    }

    if (fclose(file) != 0)
    {
        return ERROR_FILE_CLOSE;
    }

    return SUCCESS;
}

// Function to delete contact by ID or by name and surname
// Returns the ID of deleted contact, or -1 if not found
// If multiple contacts match name/surname, returns -2
errors delete_contact(int id, char *name, char *surname)
{
    // Read all contacts
    errors err = read_existing_contacts();
    if (err != SUCCESS && err != ERROR_FILE_OPEN)
    {
        return err;
    }

    if (!contact_list_head)
    {
        return ERROR_INVALID_INPUT;
    }

    Contact *current = contact_list_head;
    Contact *prev = NULL;
    int found = 0;

    // Find and remove the contact
    while (current)
    {
        int should_delete = 0;

        if (id > 0 && current->id == id)
        {
            should_delete = 1;
        }
        else if (id == -1 && name && surname)
        {
            if (strcmp(current->name, name) == 0 && strcmp(current->surname, surname) == 0)
            {
                should_delete = 1;
            }
        }

        if (should_delete)
        {
            found = 1;
            if (prev == NULL)
            {
                contact_list_head = current->next;
                free(current);
                current = contact_list_head;
            }
            else
            {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            break;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }

    if (!found)
    {
        clear_contact_list();
        return ERROR_INVALID_INPUT;
    }

    err = save_contacts_to_file();
    clear_contact_list();

    return err;
}
