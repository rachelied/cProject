#ifndef CODING_H
#define CODING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "table.h"
#define   BINARY_WORD (15)
#define DESTINATION_INDEX (9)
#define SOURCE_INDEX (6)
#define LAST_BIT_INDEX (12)
 /* This enumeration defines two types of operands used in instructions:
 * - `source`: Represents the source operand.
 * - `destination`: Represents the destination operand
*/
enum operand {
    source,
    destination
};
/**
 * @brief Encodes a line of data into binary and stores it in the data table.
 * Reads words from the given line, converts them to 15-bit binary, and stores the binary data 
 * and addresses in the `data_table`. Manages memory allocation and cleanup.
 * @param line         Pointer to the line being processed, updated as words are read.
 * @param suffix       File suffix for error reporting.
 * @param file_name    Name of the file being processed, used for error reporting.
 * @param number_line  Line number for error reporting.
 * @param tableD       Pointer to the data table for storing processed data.
 * @param data_size    Pointer to the current size of the data table, updated with new data.
 * @param original_line Original line of code being processed.
 * @param DC           Data counter representing the starting address for the data.
 * @return Returns `memory_errors` on memory failure, or the result of adding data to the table.
 */

int coding_data(char**, char*, char*, int, data_table**, int*, char*, int);
/**
 * @brief Frees allocated memory for source code, binary data, and code addresses.
 * This function releases the memory allocated for the `source_code` string, the `data_binary_code` 
 * array of strings, and the `code_addresses` array of integers. It iterates through the `data_binary_code` 
 * array to free each binary string individually before freeing the array itself.
 * @param source_code     Pointer to the source code string to be freed.
 * @param data_binary_code Array of binary code strings to be freed.
 * @param code_addresses  Array of code addresses to be freed.
 * @param size            The number of binary strings in the `data_binary_code` array.
 */

void cleanup_memory(char*, char**, int*, int);
/**
 * @brief Converts an integer to a 15-bit binary string.
 * This function takes an integer and converts it to a binary string representation 
 * of 15 bits. The resulting string is null-terminated and stored in a static buffer.
 * @param number The integer to be converted to a 15-bit binary string.
 * @return A pointer to the static buffer containing the 15-bit binary string.
 * The buffer is overwritten on each call, so the value should be used or copied immediately.
 */

char* to_binary_15bit(int);
/**
 * @brief Adds a new row of data to the data table.
 * This function allocates memory for a new row in the `data_table`, stores the given code addresses, 
 * binary code, and source code in the new row, and updates the table size. If any memory allocation 
 * fails, the function returns an error code and performs necessary cleanup.
 * @param tableD        Pointer to the data table where the new row will be added.
 * @param data_size     Pointer to the current size of the data table, which will be incremented.
 * @param code_addresses Array of code addresses corresponding to the binary code.
 * @param size          The number of binary code words to add to the table.
 * @param source_code   The source code associated with the data.
 * @param binary_code   Array of binary code strings to be added to the table.
 * 
 * @return Returns `no_errors` on success, or `memory_errors` if a memory allocation fails.
 */

int add_data_to_table(data_table**, int*, int*, int, char*, char**);
/**
 * @brief Encodes a string into binary and stores it in the data table.
 * 
 * This function processes a string, converts it into binary representations, 
 * and stores the resulting binary data and corresponding addresses in the `data_table`. 
 * It manages memory allocation for the binary data, addresses, and source code. 
 * The function handles any necessary cleanup in case of errors.
 * @param line         Pointer to the line containing the string to be processed.
 * @param tableD       Pointer to the data table where the processed data will be stored.
 * @param data_size    Pointer to the current size of the data table, updated with new data.
 * @param original_line Original line of code being processed.
 * @param DC           Data counter representing the starting address for the data.
 * 
 * @return Returns `no_errors` on success, or `memory_errors` if a memory allocation fails.
 */

int coding_string(char**,data_table**, int*, char*, int);
/**
 * @brief Encodes a string into an array of 15-bit binary strings.
 * This function takes an input string, converts each non-space and non-quote character 
 * into its 15-bit binary representation, and stores each binary string in an array. 
 * The resulting array of binary strings is dynamically allocated and returned.
 * @param str  The input string to be encoded into binary.
 * @param size Pointer to an integer that will store the number of binary strings produced.
 * @return A pointer to an array of binary strings. Returns `NULL` if a memory allocation fails.
 */

char** encode_binary_strings(const char*, int*);
/**
 * @brief Encodes assembly instructions into binary and stores them in the code table.
 * Processes a line of assembly code, encodes instructions and operands into binary, and 
 * stores the results in `code_table`. Handles different addressing methods and manages 
 * memory allocation.
 * @param line          The assembly code line to process.
 * @param suffix        File name suffix for error handling.
 * @param file_name     Source file name.
 * @param number_line   Current line number in the file.
 * @param tableC        Pointer to the code table.
 * @param code_size     Pointer to the code table size, updated after processing.
 * @param original_line The original code line.
 * @param IC            Instruction counter for the starting address.
 * @param instructions  Array of instruction functions.
 * @param tableM        Pointer to the macro table.
 * @param macro_size    Size of the macro table.
 * @param directive     Array of directive strings.
 * @param regs          Array of register names.
 * @param index         Instruction index in the array.
 * @return `no_errors` on success, `memory_errors` on failure.
 */

int coding_instructions(char*, char*, char*, int, code_table**, int*, char*, int, func[], macro_table*, int, char* [], char* [],int);
/**
 * @brief Encodes the first word of an assembly instruction.
 * Reads the source and destination operands from the given line of code, determines their 
 * addressing methods, and encodes the first word of the instruction in binary format 
 * based on the opcode and addressing methods.
 * @param line           The line of assembly code.
 * @param index          The index of the instruction in the `instructions` array.
 * @param suffix         File name suffix for error handling.
 * @param file_name      Source file name.
 * @param number_line    Current line number in the source file.
 * @param instructions   Array of instruction functions.
 * @param tableM         Pointer to the macro table.
 * @param macro_size     Size of the macro table.
 * @param directive      Array of directive strings.
 * @param regs           Array of register names.
 * 
 * @return A string containing the binary representation of the first word of the instruction, 
 *         or `NULL` on failure.
 */

char* coding_first_word(char*, int, char*, char*, int, func[], macro_table*, int, char* [], char* []);
/**
 * @brief Determines the addressing method for a given operand.
 * Evaluates the operand (`word`) to determine its addressing method based on whether it's an 
 * integer, label, pointer, or requires indirect addressing. The method is determined using 
 * the provided arrays and tables for labels, directives, and registers.
 * @param word          The operand to evaluate.
 * @param instructions  Array of instruction functions (not used directly in this function).
 * @param tableM        Pointer to the macro table.
 * @param macro_size    Size of the macro table.
 * @param directive     Array of directive strings.
 * @param regs          Array of register names.
 * @param operand       The operand type to determine the addressing method for.
 * @return An integer representing the addressing method:
 *         - `no_method` if no valid method is found,
 *         - `first_method` for immediate addressing,
 *         - `second_method` for label addressing,
 *         - `third_method` for register addressing,
 *         - `fourth_method` for indirect addressing.
 */

int get_addressing_method(char*, func[], macro_table*, int, char* [], char* [], int);
/**
 * @brief Creates the binary representation of the first word of an instruction.
 * 
 * Constructs a binary string based on the opcode and addressing methods for source and 
 * destination operands. The binary string is of fixed length with specific positions set 
 * according to the provided operands.
 * @param opcode              The opcode for the instruction.
 * @param source_operand      Addressing method for the source operand (0 if not used).
 * @param destination_operand Addressing method for the destination operand (0 if not used).
 * 
 * @return A binary string representation of the first word, or `NULL` on failure.
 */

char* create_binary_first_word(const char*, int, int);
/**
 * @brief Creates the binary representation for a direct addressing mode.
 * Converts a direct address operand (formatted as a string) to its 15-bit binary representation. 
 * The result is a binary string of fixed length, with specific bits set for the direct address.
 * @param num  The direct address operand as a string (assumed to be prefixed with a character such as '@').
 * @return A binary string representation of the direct address, or `NULL` on failure.
 */

char* direct_address(const char*);
/**
 * @brief Converts an integer to a 3-bit binary representation.
 * Converts the given integer number to its binary form, represented as a 3-bit string. 
 * The result is stored in the provided `binary` buffer.
 * @param number The integer number to convert.
 * @param binary A buffer of at least 4 characters (3 bits + null terminator) 
 *               to store the binary representation.
 */

void to_3bit_binary(int, char*);
/**
 * @brief Creates the binary representation for register addressing mode.
 * 
 * Converts a register operand to its binary form and formats it according to the 
 * register addressing mode. The result is a binary string of fixed length with 
 * specific bits set for the register.
 * 
 * @param n   Indicates if the register is a source or destination operand.
 *            Use `source` for source operand and `destination` for destination operand.
 * @param reg The register operand as a string (e.g., "*1" or "R2").
 * 
 * @return A binary string representation of the register addressing mode, or `NULL` on failure.
 */

char* register_addressing(int, const char*);
/**
 * @brief Adds a new string to a dynamically allocated array of strings.
 * Reallocates memory for the array to accommodate an additional string and copies the 
 * new string into the newly allocated space. Updates the array and size accordingly.
 * @param array A pointer to the pointer to the array of strings to modify.
 * @param size  A pointer to the size of the array, which will be incremented.
 * @param newString The string to add to the array.
 * 
 * @return `no_errors` on success, `memory_errors` on failure.
 */

int add_string_to_array(char***, int*, char*);
/**
 * @brief Performs a bitwise OR operation between two binary strings.
 * Computes the bitwise OR of two binary strings of equal length and returns the result as a new binary string.
 * The result string will be of the same length as the input strings, with each bit set to '1' if either 
 * corresponding bit in the input strings is '1', and '0' otherwise.
 * @param str1 The first binary string.
 * @param str2 The second binary string.
 * @return A new binary string representing the result of the bitwise OR operation, or `NULL` on failure.
 */

char* bitwise_or( char*, char*);
/**
 * @brief Adds a new entry to a dynamically allocated code table. 
 * Reallocates memory for the code table to accommodate a new entry, and copies the provided
 * code information into the newly allocated space. Updates the size of the table accordingly.
 * @param tableC Pointer to the pointer to the code table to modify.
 * @param size   Pointer to the size of the table, which will be incremented.
 * @param number_line The line number where the code is located.
 * @param code_source_code The source code corresponding to the entry.
 * @param amount_coding_words Number of coding words in the entry.
 * @param code_addresses Array of addresses associated with the code.
 * @param binary_code Array of binary code strings corresponding to the addresses.
 * @return `no_errors` on success, `memory_errors` on failure.
 */
int add_to_code_table(code_table**, int*, int,  char*, int,  int*,  char**);
/**
 * @brief Adds a new integer to a dynamically allocated integer array.
 * Reallocates memory for the array to accommodate one more integer and adds the new integer
 * to the end of the array. Updates the array pointer accordingly.
 * @param array Pointer to the pointer to the integer array to modify.
 * @param size  Current size of the array (before adding the new integer).
 * @param newInt The integer to add to the array.
 * @return `no_errors` on success, `memory_errors` on failure.
 */

int add_int_to_array(int**, int, int);

#endif
