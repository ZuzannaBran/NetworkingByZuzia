#ifndef CONTACT_H
#define CONTACT_H

#include "error.h"

// Contact structure for linked list
typedef struct Contact
{
    int id;
    char name[100];
    char surname[100];
    char note[1024];
    char advantage[1024];
    char source_type[20];   // "event" or "contact"
    char source_value[200]; // event name or "name surname" of contact
    char hashtags[10][100];
    int hashtag_count;
    struct Contact *next;
} Contact;

// Global head pointer for the contact list
extern Contact *contact_list_head;

// Contact management functions
errors newcontact(char *name, char *surname, char *note, char *advantage, char *source_type, char *source_value, char *hashtags[]);
errors read_existing_contacts(void);
errors delete_contact(int id, char *name, char *surname);
errors save_contacts_to_file(void);
void clear_contact_list(void);

#endif // CONTACT_H
