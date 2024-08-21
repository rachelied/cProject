#ifndef HANDALE_TEXT_H
#define HANDALE_TEXT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define BUFFER_SIZE (2)
#define LINE_CHARACTERS (82)

/*
 * @brief Reads a line from a file, checks for errors, and handles memory allocation.
 * @param file A pointer to the file from which to read the line.
 * @param suffix A suffix string used in error messages.
 * @param file_name The name of the file being read from.
 * @param number_line The line number in the file being read.
 * @param check A pointer to an integer where error codes are stored. It will be
 * @return A pointer to the line read from the file, or NULL if an error occurs
 * or the end of the file is reached.
 */
char* read_line(FILE*, char*, char*, int, int*);
/*
 * @brief Skips whitespace characters in a string.
 * @param line A pointer to a pointer to the string. The function updates this
 * pointer to point to the first non-whitespace character in the string.
 */
void skip_whitespace(char**);
/*
 * @brief Reads a word from the given line of text.
 * @param line A pointer to a pointer to the line of text. The function updates this pointer to
 *     point to the next character after the word.
 * @param suffix A string suffix for error messages.
 * @param file_name The name of the file being processed, used in error messages.
 * @param number_line The line number in the file, used in error messages.
 * @return A pointer to a dynamically allocated string containing the word or comma, or NULL if an error occurs.
 */
char* read_word(char**, char*, char*, int);
/*
 * @brief Checks if there is any non-whitespace garbage remaining in the line.
 * @param line A pointer to a pointer to the line of text. The function updates this pointer to
 *             skip leading whitespace.
 * @return 1 if the line is empty or contains only whitespace after skipping; 0 otherwise.
 */
int check_garbage(char**);
/*
 * @brief Duplicates a string by allocating new memory and copying the content.
 * @param s A pointer to the null-terminated string to be duplicated.
 * @return A pointer to the newly allocated string that is a duplicate of the input string, or NULL if memory allocation fails.
 */
char* strdup1(char*);
/*
 * @brief Checks if the given string contains only whitespace characters.
 * @param line A pointer to the null-terminated string to check.
 * @return 1 if the string contains only whitespace characters or is empty, 0 otherwise.
 */
int is_line_whitespace_only(char*);
/*
 * @brief Trims trailing whitespace from the end of a string.
 * @param str A pointer to a pointer to the null-terminated string to be trimmed. The function modifies
 *        the original string in place.
 */
void skip_whitespace_end(char**);
#endif
