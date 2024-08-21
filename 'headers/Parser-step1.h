#ifndef PARSER_STEP1_H
#define PARSER_STEP1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "table.h"
#define LABEL_LENGTH (31)
#define MEMORY_SIZE (4096)
/*
 * @brief Processes the first stage of parsing an assembly source file.
 * @param file_name The name of the assembly source file to be parsed.
 * @param tableM Pointer to the macro table.
 * @param macro_size Size of the macro table.
 * @param instructions Array of functions representing instructions.
 * @param directive Array of directive names.
 * @param regs Array of register names.
 * @param tableL Pointer to a pointer to the label table.
 * @param lable_size Pointer to the size of the label table.
 * @param tableD Pointer to a pointer to the data table.
 * @param data_size Pointer to the size of the data table.
 * @param tableEX Pointer to a pointer to the extern table.
 * @param extern_size Pointer to the size of the extern table.
 * @param tableEN Pointer to a pointer to the entry table.
 * @param entry_size Pointer to the size of the entry table.
 * @param tableC Pointer to a pointer to the code table.
 * @param code_size Pointer to the size of the code table.
 * @return int Error code indicating the result of the parsing process.
 * - Returns no_errors if parsing is successful.
 * - Returns memory_errors if there is a memory allocation failure.
 * - Returns code_errors if there are syntax or semantic errors in the assembly file.
 */
int parser_step1(char*, macro_table*, int, func[], char* [], lable_table**, int*, char* [], data_table**, int*, extern_table**, int*, entry_table**, int*, code_table**, int*);
/*
 * @brief Checks if a given label name is valid according to various criteria.
 * @param nameL The label name to be checked.
 * @param tableM Pointer to the macro table.
 * @param macro_size The number of macros in the macro table.
 * @param instructions Array of instruction names.
 * @param directive Array of directive names.
 * @param regs Array of register names.
 * @param tableE Pointer to the extern table.
 * @param extern_size The number of entries in the extern table.
 * @param tableL Pointer to the label table.
 * @param lable_size The number of labels in the label table.
 * @param file_name The name of the file being processed.
 * @param number_line The line number where the label is defined.
 * @return code_errors if an error is found, otherwise no_errors.
 */
int check_lable(char* , macro_table* , int , func [], char* [], char* [], extern_table* , int , lable_table* , int , char* , int );
/*
 * @brief Checks if a given string contains only alphanumeric characters.
 * @param str The string to be checked.
 * @return 1 if the string is alphanumeric; 0 otherwise.
 */
int isAlphaNumeric(const char*);
/*
 * @brief Adds a new label to the label table.
 * @param tableL A pointer to the label table to which the new label will be added.
 * @param size A pointer to the current size of the label table. This will be updated after adding the new label.
 * @param nameL The name of the label to be added.
 * @param addressL The address associated with the label.
 * @param feature The feature associated with the label.
 * @return Returns no_errors on success, or memory_errors if memory allocation fails.
 */
int add_lable(lable_table**, int*, char*, int, int);
/*
 * @brief Checks if a given word matches a valid operand based on the specified addressing method.
 * @param word The word to be checked.
 * @param addressing_method The addressing method to be used for validation (_1, _1_2, _1_2_3, etc.).
 * @param tableM A pointer to the macro table.
 * @param macro_size The size of the macro table.
 * @param instructions An array of valid instruction names.
 * @param directive An array of valid directive names.
 * @param regs An array of valid register names.
 * @return Returns 1 if the word is a valid operand for the given addressing method, 0 otherwise.
 */
int check_operand(char*, int, macro_table*, int, func[], char* [], char* []);
/*
 * @brief Validates an instruction line by checking the presence and correctness of operands.
 * @param index Pointer to an integer that will store the index of the instruction.
 * @param word The current word being processed in the instruction line.
 * @param instructions An array of valid instructions.
 * @param line The entire line of code being processed.
 * @param file_name The name of the file being processed.
 * @param number_line The line number in the file.
 * @param regs An array of valid register names.
 * @param tableM A pointer to the macro table.
 * @param macro_size The size of the macro table.
 * @param directive An array of valid directive names.
 * @return Returns no_errors if the instruction is valid, otherwise returns an error code such as code_errors or memory_errors.
 */
int check_instructions(int*, char*, func[], char*, char*, int, char* [], macro_table*, int, char* []);
/*
 * @brief Validates a line of data, ensuring proper format and correctness of numbers.
 * @param word A pointer to the current word being processed.
 * @param line The line of data being validated.
 * @param file_name The name of the file being processed.
 * @param number_line The line number in the file being processed.
 * @param suffix The suffix used for identifying the word's context.
 * @return Returns no_errors if the data line is correctly formatted, otherwise returns an error code such as code_errors.
 */
int check_data(char*, char*, char*, int, char*);
/*
 * @brief Adds a new external label to the external table if it doesn't already exist.
 * @param tableE A double pointer to the external table.
 * @param extern_size A pointer to the size of the external table.
 * @param name_extern The name of the external label to be added.
 * @param file_name The name of the file being processed.
 * @param number_line The line number in the file being processed.
 * @return Returns no_errors if the label is successfully added, code_errors if the label already exists, or memory_errors if memory allocation fails.
 */
int add_extern_label(extern_table** , int* , char* , char* , int );
/*
 * @brief Adds a new entry label to the entry table if it doesn't already exist.
 * @param tableEN A double pointer to the entry table.
 * @param entry_size A pointer to the size of the entry table.
 * @param name_entry The name of the entry label to be added.
 * @param file_name The name of the file being processed.
 * @param number_line The line number in the file being processed.
 * @return Returns no_errors if the label is successfully added, code_errors if the label already exists, or memory_errors if memory allocation fails.
 */
int add_entry_label(entry_table**, int*, char*, char*, int);
/*
 * @brief Updates the addresses of labels marked as data in the label table.
 * @param tableL Pointer to the label table.
 * @param table_size The number of labels in the table.
 * @param IC The instruction count used to adjust the label addresses.
 */
void update_lable_addresses(lable_table*, int, int);
/*
 * @brief Updates the addresses of all data words in the data table by adding the instruction count.
 * @param tableD Pointer to the data table.
 * @param table_size The number of entries in the data table.
 * @param IC The instruction count used to adjust the data addresses.
 */
void update_data_addresses(data_table*, int, int);
/*
 * @brief Checks if a given name can be used as a label name.
 * @param nameL The name to check.
 * @param tableM Pointer to the macro table.
 * @param macro_size The number of entries in the macro table.
 * @param instructions Array of function pointers to check against instruction names.
 * @param directive Array of directive names to check against.
 * @param regs Array of register names to check against.
 * @return 1 if the name can be used as a label name; 0 otherwise.
 */
int can_be_lable_name(char*, macro_table*, int, func[], char* [], char* []);
/*
 * @brief Updates the addresses of all coding words in the code table by adding a base address.
 * @param tableC Pointer to the code table.
 * @param code_size The number of entries in the code table.
 */
void update_code_addresses(code_table*, int);
#endif
