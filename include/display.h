#ifndef DISPLAY_H
#define DISPLAY_H

#include "contact.h"
#include "error.h"

// Display and listing functions
void list_contacts(void);
void list_contacts_brief(void);
void show_contact_details(int id, char *name, char *surname);
void list_all_hashtags(void);
void find_contacts_by_hashtag(char *hashtag);
void find_contacts_by_event(char *event);
void show_contact_network(char *name, char *surname);

#endif // DISPLAY_H
