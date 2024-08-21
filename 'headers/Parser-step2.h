#ifndef PARSER_STEP2_H
#define PARSER_STEP2_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "table.h"
#include "Parser-step1.h"
#define OFFSET_BITS 3
#define OFFSET_VALUE 2
#define BINARY_CODE_LENGTH (16) 
#define ADDRESS_BITS 15
#define INITIAL_SIZE 10
#define SIZE_MULTIPLIER 2
#define EXTERN_BINARY_STR "000000000000001"


/*
 * @brief Performs the second parsing step for assembly code, handling labels, externals, and entries, and generates output files.
 * @param tableC Pointer to the code table, which is an array of pointers to code_table structures.
 * @param code_size The number of entries in the code table.
 * @param tableL Pointer to the label table, which is an array of lable_table structures.
 * @param lable_size The number of labels in the label table.
 * @param tableD Pointer to the data table, which is an array of data_table structures.
 * @param data_size The number of entries in the data table.
 * @param tableEX Pointer to the external table, which is an array of extern_table structures.
 * @param extern_size The number of entries in the external table.
 * @param tableEN Pointer to the entry table, which is an array of entry_table structures.
 * @param entry_size The number of entries in the entry table.
 * @param file_name The base name of the files to generate (without extension).
 * @param parser_1_error Error status from the first parsing step.
 * @return no_errors if the parsing and file generation are successful, or code_errors if errors are found during processing.
 */
int parser_step2(code_table** , int , lable_table* , int , data_table* , int , extern_table* , int , entry_table* , int , char* , int);
/*
 * @brief Checks if a string starts with an alphabetic character.
 * @param str The string to check.
 * @return no_errors if the string starts with an alphabetic character, code_errors otherwise.
 */
int starts_with_alpha(const char* );
/*
 * @brief Searches for a label in the symbol table and returns its associated address.
 * @param tableL Pointer to the array of lable_table structures that represents the symbol table.
 * @param lable_size The number of elements in the symbol table.
 * @param found_word The label name to search for in the symbol table.
 * @return The address associated with the label if found, or -1 if the label is not found.
 */
int find_in_symbol_table(lable_table* , int ,  char* );
/*
 * @brief Encodes an address as a 15-bit binary string.
 * @param address The address to encode.
 * @return A 15-bit binary string representing the address.
 */
char* encode_address(int );
/*
 * @brief Searches for a label in the extern table.
 * @param tableEX The extern table to search in.
 * @param extern_size The number of entries in the extern table.
 * @param found_word The label to search for.
 * @return The index of the label if found, -1 if not found.
 */
int find_in_extern_table(extern_table* , int , const char*);
/*
 * @brief Adds an address to the extern table's address list.
 * @param extern_entry Pointer to an extern_table structure where the address will be added.
 * @param address The address to add to the extern table's address list.
 * @return no_errors if the address is successfully added, or memory_errors if memory allocation fails.
 */
int add_to_extern_addresses(extern_table* , int );
/*
 * @brief Generates an .ob file containing the code and data tables.
 * @param file_name The base name of the file to generate (without extension).
 * @param tableC Pointer to the code table, which is an array of pointers to code_table structures.
 * @param code_size The number of code entries in the code table.
 * @param tableD Pointer to the data table, which is an array of data_table structures.
 * @param data_size The number of data entries in the data table.
 * @param IC The instruction counter value to be written at the top of the file.
 * @param DC The data counter value to be written at the top of the file.
 * @return no_errors if the file is generated successfully, or memory_errors if file creation fails.
 */
int generate_ob_file(char* , code_table** , int , data_table* , int , int , int );
/*
 * @brief Generates an .ent file containing the entry names and their addresses.
 * @param file_name The base name of the file to generate (without extension).
 * @param tableEN Pointer to the entry table, which is an array of entry_table structures.
 * @param entry_size The number of entries in the entry table.
 * @param tableL Pointer to the label table, which is an array of lable_table structures.
 * @param lable_size The number of labels in the label table.
 * @return no_errors if the file is generated successfully, or memory_errors if file creation fails.
 */
int generate_ent_file(char* , entry_table* , int , lable_table* , int );
/*
 * @brief Generates an .ext file containing external symbol names and their addresses.
 * @param file_name The base name of the file to generate (without extension).
 * @param tableEX Pointer to the external table, which is an array of extern_table structures.
 * @param extern_size The number of entries in the external table.
 * @return no_errors if the file is generated successfully, or memory_errors if file creation fails.
 */
int generate_ext_file(char* , extern_table* , int );
/*
 * @brief Calculates the total number of data words in the data table.
 * @param table Pointer to the data table, which is an array of data_table structures.
 * @param size The number of entries in the data table.
 * @return The total count of data words across all entries in the table.
 */
int return_DC(data_table* , int );

#endif
