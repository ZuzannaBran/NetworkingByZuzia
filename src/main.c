#include <stdio.h>
#include "../include/menu.h"
#include "../include/input.h"

int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf("   CONTACT NETWORKING SYSTEM v1.0\n");
    printf("========================================\n\n");

    int choice;

    while (1)
    {
        show_main_menu();
        choice = read_choice();
        handle_menu_choice(choice);
    }

    return 0;
}
