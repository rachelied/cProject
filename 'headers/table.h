#ifndef TABLE_H
#define TABLE_H
#include <stdlib.h>
#include <stdio.h>
#define INSTRUCTIONS_AMOUNT (16)
#define LENGTH_OPCODE (4)
#define DIRECTIVES_AMOUNT (4)
#define REGISTERS_AMOUNT (8)
#define BASE_ADDRESS 100

#define MIN_12_BITS (-2048)
#define MAX_12_BITS (2047)

#define MIN_15_BITS (-16384)
#define MAX_15_BITS (16383)

#define IS_IN_12_BIT_RANGE(x) ((x) >= MIN_12_BITS && (x) <= MAX_12_BITS)
#define IS_IN_15_BIT_RANGE(x) ((x) >= MIN_15_BITS && (x) <= MAX_15_BITS)

#ifndef FUNC_H
#define FUNC_H

/*
 * Represents an assembly instruction with its operands and opcode.
 *
 * This struct contains information about an assembly instruction, including its name,
 * the types of source and destination operands it accepts, and its opcode.
 */
typedef struct {
	char* func_name;
	int source_operand;
	int destination_operand;
	char* opcode;
} func;

#endif
/*
 * Enumerates the combinations of addressing methods used in an instruction.
 *
 * This enum represents the different combinations of addressing methods that can be used
 * in an assembly instruction, ranging from no method to multiple methods.
 */
typedef enum{
	nothing,
	_1,
	_0_1_2_3,
	_1_2_3,
	_1_2
}addressing_methods_used;
/*
 * Enumerates the individual addressing methods available.
 *
 * This enum lists the different addressing methods that can be used in assembly instructions,
 * providing a clear way to identify and refer to each method.
 */
 typedef enum{
	no_method,
	first_method,
	second_method,
	third_method,
	fourth_method
}addressing_methods_exist;
/*
 * Enumerates the types of commands encountered in the assembly code.
 *
 * This enum categorizes the different types of commands that can be found in the assembly code,
 * including macros, labels, and direct commands.
 */
 typedef enum{
	unknown,
	macro,
	endmacro,
	name_macro,
	lable,
	direct
}types_of_commands;
/*
 * Enumerates the modes for parsing lines of assembly code.
 *
 * This enum defines the different modes that can be used when parsing lines of assembly code,
 * specifically for handling commas and variables.
 */
 typedef enum{
	comma,
	variable
}line_mode;
/*
 * Enumerates the features of commands in the assembly code.
 *
 * This enum distinguishes between data commands and code commands in the assembly,
 * allowing for different handling of these two types of commands.
 */
 typedef enum{
	data,
	code
}command_feature;
/*
 * Represents a macro definition in the assembly code.
 *
 * This struct stores information about a macro, including its name and the content
 * that should be expanded when the macro is used.
 */
typedef struct
{
	char* macr_name;
	char* macr_data;
} macro_table;
/*
 * Represents a label in the assembly code.
 *
 * This struct stores information about a label, including its name, memory address,
 * and whether it's associated with data or code.
 */
typedef struct
{
	char* lable_name;
	int lable_address;
	int lable_feature;
} lable_table;
/*
 * Represents an external label reference in the assembly code.
 *
 * This struct stores information about an external label, including its name and
 * the addresses where it's referenced in the code.
 */
typedef struct {
	char* extern_name;
	int* extern_addresses;
	int size_address;
} extern_table;
/*
 * Represents an entry point label in the assembly code.
 *
 * This struct stores information about an entry point label, including its name
 * and the line number where it's defined in the source code.
 */
typedef struct {
	char* entry_name;
	int number_line;
} entry_table;
/*
 * Represents data segment information in the assembly code.
 *
 * This struct stores information about a data segment, including its address,
 * size, original source code, and binary representation.
 */
typedef struct
{
	int* data_address;
	int amount_coding_data_words;
	char* data_source_code;
	char** data_binary_code;
}data_table;
/*
 * Represents code segment information in the assembly code.
 *
 * This struct stores information about a code segment, including its line number,
 * original source code, size, addresses, and binary representation.
 */
typedef struct
{
	int number_line;
	char* code_source_code;
	int amount_coding_words;
	int* code_addresses;
	char** binary_code;
}code_table;
/*
 * Represents boolean values in the assembly code.
 *
 * This enum provides a simple way to represent true and false values in the code,
 * enhancing readability and type safety.
 */
enum bool{
	false,
	true
};


/*
 * @brief Checks if a given string is a macro name in the macro table.
 * @param str The string to check.
 * @param tableM Pointer to the macro table.
 * @param macro_size The number of macros in the table.
 * @return 1 if the string matches a macro name in the table; 0 otherwise.
 */
int is_macro_name(char*, macro_table*, int);
/*
 * @brief Checks if a given string matches any of the directive names.
 * @param str The string to check.
 * @param directive Array of strings representing directive names.
 * @return 1 if the string matches any directive name; 0 otherwise.
 */
int is_directive_name(char*, char* []);
/*
 * @brief Checks if a given string matches any of the instruction names.
 * @param str The string to check.
 * @param instructions Array of func structures representing instructions.
 * @return 1 if the string matches any instruction name; 0 otherwise.
 */
int is_instructions_name(char*, func[]);
/*
 * @brief Checks if a given string matches any of the register names.
 * @param str The string to check.
 * @param regs Array of strings representing register names.
 * @return 1 if the string matches any register name; 0 otherwise.
 */
int is_register_name(char*, char* []);
/*
 * @brief Retrieves the index of a given function name in the instructions array.
 * @param str The function name to find.
 * @param instructions Array of func structures representing the available instructions.
 * @return The index of the function name in the array if found; -1 if not found.
 */
int get_index(char*, func []);
/*
 * @brief Checks if a given string represents a valid integer in a specific format.
 * @param str The string to check.
 * @return 1 if the string is a valid integer in the specified format; 0 otherwise.
 */
int is_integer(const char*);
/*
 * @brief Checks if a given string matches any label name in the label table.
 * @param tableL Pointer to the label table.
 * @param str The string to check against label names.
 * @param size The number of labels in the table.
 * @return 1 if the string matches a label name in the table; 0 otherwise.
 */
int is_lable_name(lable_table*, char*, int);
/*
 * @brief Checks if a given string represents a valid pointer to a register.
 * @param str The string to check.
 * @param regs Array of valid register names.
 * @return 1 if the string is a valid pointer to a register; 0 otherwise.
 */
int is_pointer(char*, char* []);
/*
 * @brief Checks if a given string is a valid string literal enclosed in double quotes. 
 * @param str The string to check.
 * @return 1 if the string is a valid string literal; 0 otherwise.
 */
int is_string( char*);
/*
 * @brief Checks if a given string represents a valid integer number within a specific range.
 * @param str The string to check.
 * @return 1 if the string is a valid integer within the 15-bit range; 0 otherwise.
 */
int is_valid_number(const char*);
/*
 * @brief Checks if a given string is an entry label in the entry table.
 * @param tableEN Pointer to the entry table.
 * @param entry_size The number of entry labels in the table.
 * @param str The string to check.
 * @return 1 if the string matches an entry label in the table; 0 otherwise.
 */
int is_entry_label(entry_table*, int, char*);
/*
 * @brief Checks if a given string is an extern label in the extern table.
 * @param str The string to check.
 * @param tableE Pointer to the extern table.
 * @param extern_size The number of extern labels in the table.
 * @return 1 if the string matches an extern label in the table; 0 otherwise.
 */
int is_extern_label(char*, extern_table*, int);
/*
 * @brief Frees all allocated memory for the various tables used in the program.
 * @param tableM Pointer to the macro table.
 * @param macro_size The number of macros in the macro table.
 * @param tableL Pointer to the label table.
 * @param lable_size The number of labels in the label table.
 * @param tableD Pointer to the data table.
 * @param data_size The number of data entries in the data table.
 * @param tableEX Pointer to the extern table.
 * @param extern_size The number of extern entries in the extern table.
 * @param tableEN Pointer to the entry table.
 * @param entry_size The number of entries in the entry table.
 * @param tableC Pointer to the code table.
 * @param code_size The number of code entries in the code table.
 */
void free_all_tables(macro_table*, int, lable_table*, int, data_table*, int, extern_table*, int, entry_table*, int, code_table*, int);
#endif
