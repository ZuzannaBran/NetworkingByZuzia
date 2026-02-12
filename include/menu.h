#ifndef MENU_H
#define MENU_H

#include "error.h"

// Menu functions
void show_main_menu(void);
void show_search_menu(void);
void show_list_menu(void);
void handle_menu_choice(int choice);
void menu_search(void);
void menu_list(void);
void menu_add_contact(void);
void menu_list_all_contacts(void);
void menu_list_brief_contacts(void);
void menu_delete_contact(void);
void menu_find_by_hashtag(void);
void menu_find_by_event(void);
void display_error(errors err);
void display_success(const char *message);

#endif // MENU_H
