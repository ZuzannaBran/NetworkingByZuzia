#ifndef ERROR_H
#define ERROR_H

// Error codes enum
typedef enum
{
    SUCCESS = 0,
    ERROR_FILE_OPEN = 1,
    ERROR_FILE_READ = 2,
    ERROR_FILE_WRITE = 3,
    ERROR_FILE_CLOSE = 4,
    ERROR_MEMORY_ALLOCATION = 5,
    ERROR_NULL_PARAMETRS = 6,
    ERROR_INVALID_INPUT = 7,
    ERROR_PARSE_JSON = 8
} errors;

// Error handling functions
const char *get_error_message(errors error);

#endif // ERROR_H
