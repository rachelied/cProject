#ifndef PRE_PROCCESOR_H
#define PRE_PROCCESOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
/*
 * @brief Processes the source assembly file, handling macros and generating a new file with macro expansions.
 * @param file_name The name of the source file (with ".as" extension).
 * @param instructions Array of valid instruction names.
 * @param directive Array of valid directive names.
 * @param tableM Pointer to the macro table where macros will be stored.
 * @param macro_size Pointer to the size of the macro table.
 * @param regs Array of valid register names.
 * @return Returns no_errors if processing is successful, or memory_errors or code_errors in case of errors.
 */
int pre_processor(char*, func[], char* [], macro_table**, int*, char* []);
/*
 * @brief Adds a macro to the macro table if it is valid and not a duplicate.
 * @param table Pointer to a pointer to the macro table. The table will be reallocated if necessary.
 * @param size Pointer to the size of the macro table. This will be incremented if a new macro is added.
 * @param macro_name The name of the macro to be added.
 * @param data The data associated with the macro.
 * @param instructions Array of function pointers representing the macro instructions.
 * @param directive Array of strings representing directives.
 * @param file_name The name of the file where errors should be reported.
 * @param number_line The line number in the file where the macro is defined.
 * @param regs Array of strings representing registers used in the macro.
 * @return Returns no_errors if the macro was successfully added, or an error code if validation or allocation failed.
 */
int add_macro(macro_table**, int*,  char*, char*, func[], char* [], char*, int, char* []);
/*
 * @brief Reads macro data from a file and adds it to the macro table.
 * @param source_file The file to read the macro data from.
 * @param table Pointer to a pointer to the macro table. The table will be reallocated if necessary.
 * @param file_name The name of the file where errors should be reported.
 * @param number_line Pointer to the current line number in the file. This will be updated as lines are read.
 * @param size Pointer to the size of the macro table. This will be incremented if a new macro is added.
 * @param macro_name The name of the macro to be added.
 * @param instructions Array of function pointers representing the macro instructions.
 * @param directive Array of strings representing directives.
 * @param regs Array of strings representing registers used in the macro.
 * @return Returns no_errors if the macro data was successfully read and added, or an error code if an issue occurred.
 */
int get_feild_data(FILE*, macro_table**, char*, int*, int*, char*, func[], char* [], char* []);
/*
 * @brief Checks if a macro name already exists in the macro table.
 * @param table Pointer to the macro table.
 * @param size The number of macros currently in the table.
 * @param macro_name The name of the macro to check for duplicates.
 * @param file_name The name of the file where errors should be reported.
 * @param number_line The line number in the file where the macro is defined.
 * @return Returns no_errors if the macro name does not exist, or code_errors if the name already exists.
 */

int is_macro_name_duplicate(macro_table*, int,  char*, char*, int);
/*
 * @brief Validates if a macro name is acceptable.
 * @param macro_name The name of the macro to be validated.
 * @param instructions Array of valid instruction names.
 * @param directive Array of valid directive names.
 * @param file_name The name of the file where errors should be reported.
 * @param number_line The line number in the file where the macro is defined.
 * @param regs Array of valid register names.
 * @return Returns no_errors if the macro name is valid, or code_errors if it is not.
 */
int is_macro_valid(char*, func[], char* [], char*, int, char* []);

#endif
