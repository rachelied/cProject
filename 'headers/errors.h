#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*Enum representing general error codes. */
typedef enum 
{
	no_errors,
	code_errors,
	memory_errors

}e_error;
/* Enum representing specific error codes related to macros, labels, and other syntax errors*/
typedef enum 
{
	e_macro_macr,
	e_macro_endmacr,
	e_additional_characters_in_macro,
	e_macro_not_finished,
	e_macro_name_exists,
	e_macro_name_directive,
	e_macro_name_instruction,
	e_macro_name_register,
	e_more_80_characters,
	e_invalid_string,
	e_additional_characters_in_extern,
	e_additional_characters_in_entry,
	e_invalid_command,
	e_more_31_characters,
	e_not_alphabetic,
	e_non_alphanumeric_characters,
	e_lable_name_macro,
	e_lable_name_directive,
	e_lable_name_instruction,
	e_lable_name_register,
	e_lable_name_exists,
	e_lable_name_extern,
	e_lable_name_entry,
	e_invalid_source_operand,
	e_missing_a_comma,
	e_invalid_destination_operand,
	e_additional_characters_in_command,
	e_invalid_number,
	e_unnecessary_comma,
	e_blank_line,
	e_no_data,
	e_no_name_label,
	e_missing_operand,
	e_no_entry_definition,
	e_both_internaland_external,
	e_label_not_defined,
	e_out_of_memory
}ERROR_CODES;
/*Enum representing memory and file-related error codes.*/
typedef enum 
{
	m_memory_failed,
	m_open_file,
	m_fgets_read
}ERROR_MEMORY;
/* Enum representing warning codes*/
typedef enum 
{
	w_label_before_extern,
	w_label_before_entry

}WARNING;
/**
 * @brief Prints an error message to the console.
 * This function prints an error message based on the provided error code.
 * The message includes the line number, file name, and an optional suffix.
 * @param msg         The error code corresponding to a message in the `errors` array.
 * @param suffix      An optional string suffix to add to the file name.
 * @param file_name   The name of the file where the error occurred.
 * @param number_line The line number where the error occurred.
 */
void error( int, const char*, const char*, const int);
/**
 * @brief Prints a memory-related error message to the console.
 * This function prints a memory or file-related error message based on the provided error code.
 * @param error The error code corresponding to a message in the `errors_memory` array.
 */
void error_memory( int);
/**
 * @brief Prints a warning message to the console.
 * This function prints a warning message based on the provided warning code.
 * The message includes the line number, file name, and an optional suffix.
 * @param msg         The warning code corresponding to a message in the `warnings` array.
 * @param suffix      An optional string suffix to add to the file name.
 * @param file_name   The name of the file where the warning occurred.
 * @param number_line The line number where the warning occurred.
 */
void warning( int, const char*, const char*, const int);
