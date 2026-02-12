#include "../include/error.h"

// Function to get error message
const char *get_error_message(errors error)
{
    switch (error)
    {
    case SUCCESS:
        return "Success";
    case ERROR_FILE_OPEN:
        return "Failed to open file";
    case ERROR_FILE_READ:
        return "Failed to read from file";
    case ERROR_FILE_WRITE:
        return "Failed to write to file";
    case ERROR_FILE_CLOSE:
        return "Failed to close file";
    case ERROR_MEMORY_ALLOCATION:
        return "Memory allocation failed";
    case ERROR_NULL_PARAMETRS:
        return "NULL parameter(s) provided";
    case ERROR_INVALID_INPUT:
        return "Invalid input data";
    case ERROR_PARSE_JSON:
        return "Failed to parse JSON";
    default:
        return "Unknown error";
    }
}
