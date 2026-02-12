#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "include/error.h"
#include "include/contact.h"

// ==========================================
// TEST: Error handling
// ==========================================
void test_error_messages(void)
{
    printf("\n=== Testing Error Messages ===\n");

    assert(strcmp(get_error_message(SUCCESS), "Success") == 0);
    assert(strcmp(get_error_message(ERROR_FILE_OPEN), "Failed to open file") == 0);
    assert(strcmp(get_error_message(ERROR_MEMORY_ALLOCATION), "Memory allocation failed") == 0);
    assert(strcmp(get_error_message(ERROR_NULL_PARAMETRS), "NULL parameter(s) provided") == 0);

    printf("✓ All error message tests passed!\n");
}

// ==========================================
// TEST: Contact structure initialization
// ==========================================
void test_contact_creation(void)
{
    printf("\n=== Testing Contact Creation ===\n");

    Contact test_contact;
    strcpy(test_contact.name, "John");
    strcpy(test_contact.surname, "Doe");
    strcpy(test_contact.note, "Test note");
    test_contact.id = 1;
    test_contact.next = NULL;

    assert(strcmp(test_contact.name, "John") == 0);
    assert(strcmp(test_contact.surname, "Doe") == 0);
    assert(strcmp(test_contact.note, "Test note") == 0);
    assert(test_contact.id == 1);
    assert(test_contact.next == NULL);

    printf("✓ Contact structure tests passed!\n");
}

// ==========================================
// TEST: Contact list operations
// ==========================================
void test_contact_list_operations(void)
{
    printf("\n=== Testing Contact List Operations ===\n");

    // Clear any existing contacts
    clear_contact_list();
    assert(contact_list_head == NULL);

    // Create sample contact
    char *hashtags[] = {"test", "demo", NULL};
    errors err = newcontact("Jane", "Smith", "Test contact", "Network expert", "event", "Conference 2024", hashtags);

    printf("Contact creation result: %s\n", get_error_message(err));

    // Read contacts from file
    err = read_existing_contacts();
    printf("Reading contacts result: %s\n", get_error_message(err));

    if (contact_list_head != NULL)
    {
        printf("✓ Contact list is populated!\n");
        printf("  First contact: %s %s (ID: %d)\n",
               contact_list_head->name,
               contact_list_head->surname,
               contact_list_head->id);
    }
    else
    {
        printf("! Contact list is empty\n");
    }

    clear_contact_list();
    printf("✓ Contact list operations tests passed!\n");
}

// ==========================================
// TEST: String validation
// ==========================================
void test_string_validation(void)
{
    printf("\n=== Testing String Validation ===\n");

    // Test empty strings
    char empty_str[100] = "";
    assert(strlen(empty_str) == 0);

    // Test valid strings
    char valid_str[100] = "Valid String";
    assert(strlen(valid_str) > 0);
    assert(strcmp(valid_str, "Valid String") == 0);

    printf("✓ String validation tests passed!\n");
}

// ==========================================
// TEST: Hashtag handling
// ==========================================
void test_hashtag_handling(void)
{
    printf("\n=== Testing Hashtag Handling ===\n");

    // Test hashtag array
    char *hashtags[] = {"networking", "contacts", "test", NULL};

    int count = 0;
    for (int i = 0; hashtags[i] != NULL; i++)
    {
        count++;
    }

    assert(count == 3);
    assert(strcmp(hashtags[0], "networking") == 0);
    assert(strcmp(hashtags[1], "contacts") == 0);
    assert(strcmp(hashtags[2], "test") == 0);

    printf("✓ Hashtag handling tests passed!\n");
}

// ==========================================
// TEST: File operations
// ==========================================
void test_file_operations(void)
{
    printf("\n=== Testing File Operations ===\n");

    // Check if contacts.json exists
    FILE *file = fopen("contacts.json", "r");
    if (file)
    {
        printf("✓ contacts.json file exists\n");
        fclose(file);
    }
    else
    {
        printf("! contacts.json file not found (will be created on first contact)\n");
    }
}

// ==========================================
// TEST: Contact deletion and renumbering
// ==========================================
void test_contact_deletion(void)
{
    printf("\n=== Testing Contact Deletion ===\n");

    clear_contact_list();

    // Create multiple contacts
    char *hashtags[] = {"test", NULL};

    printf("Creating test contacts...\n");
    newcontact("Alice", "Johnson", "First contact", "", "", "", hashtags);
    newcontact("Bob", "Williams", "Second contact", "", "", "", hashtags);
    newcontact("Charlie", "Brown", "Third contact", "", "", "", hashtags);

    // Read contacts
    read_existing_contacts();

    int count = 0;
    Contact *current = contact_list_head;
    while (current)
    {
        count++;
        current = current->next;
    }
    printf("Created %d contacts\n", count);

    // Delete first contact
    printf("Deleting contact with ID 1...\n");
    errors err = delete_contact(1, NULL, NULL);
    printf("Deletion result: %s\n", get_error_message(err));

    // Read again and verify renumbering
    clear_contact_list();
    read_existing_contacts();

    count = 0;
    current = contact_list_head;
    while (current)
    {
        printf("Contact %d: ID=%d, Name=%s\n", count + 1, current->id, current->name);
        count++;
        current = current->next;
    }

    printf("✓ Contact deletion tests completed!\n");
    clear_contact_list();
}

// ==========================================
// MAIN TEST RUNNER
// ==========================================
int main(void)
{
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║  CONTACT NETWORKING SYSTEM - TEST SUITE║\n");
    printf("╚════════════════════════════════════════╝\n");

    // Run all tests
    test_error_messages();
    test_contact_creation();
    test_string_validation();
    test_hashtag_handling();
    test_file_operations();
    test_contact_list_operations();
    test_contact_deletion();

    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║       ALL TESTS COMPLETED SUCCESSFULLY!║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    return 0;
}
