# Contact Networking System Makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
LDFLAGS = 

# Output executables
TARGET = contact_system.exe
TEST_TARGET = test.exe

# Source files
SOURCES = src/main.c \
          src/error.c \
          src/contact.c \
          src/display.c \
          src/input.c \
          src/menu.c \
          src/utils.c

# Test sources (excludes main.c)
TEST_SOURCES = test.c \
               src/error.c \
               src/contact.c \
               src/display.c \
               src/input.c \
               src/utils.c

# Object files (replace .c with .o)
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the main program
$(TARGET): $(OBJECTS)
	@echo Linking $(TARGET)...
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	@echo Build complete! Executable: $(TARGET)

# Build the test program
$(TEST_TARGET): $(TEST_OBJECTS)
	@echo Linking $(TEST_TARGET)...
	$(CC) $(LDFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)
	@echo Test executable created: $(TEST_TARGET)

# Test target - builds and runs tests
test: $(TEST_TARGET)
	@echo Running tests...
	@$(TEST_TARGET)

# Compile source files into object files
%.o: %.c
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo Cleaning up...
	del /Q $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET) 2>nul || true
	@echo Clean complete!

# Clean and rebuild
rebuild: clean all

# Phony targets (not actual files)
.PHONY: all clean rebuild test

