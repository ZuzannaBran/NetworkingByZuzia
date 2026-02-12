#ifndef INPUT_H
#define INPUT_H

#include "utils.h"

// Buffer input functions
void read_string(char *buffer, int max_length, const char *prompt);
void read_name(char *buffer);
void read_surname(char *buffer);
void read_note(char *buffer);
void read_advantage(char *buffer);
void read_source_type(char *buffer);
void read_source_value(char *buffer, const char *source_type);
char **read_hashtags(void);
void free_hashtags(char **hashtags);
int read_choice(void);

#endif // INPUT_H
