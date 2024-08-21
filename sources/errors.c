#include "errors.h"
/**
 * Array of error messages corresponding to various error codes.
 * Each message describes a specific error condition encountered during processing.
 */
char* errors[] = {
"A macro name cannot be macr",
"A macro name cannot be endmacr",
"There are additional characters in the definition of the macro that are not used",
"Macro not finished",
"The name of the macro already exists",
"The name of the macro is the name of a directive",
"The name of the macro is the name of an instruction",
"The name of the macro is the name of a register",
"More than 80 characters in line",
"The string in the instruction is invalid",
"There are additional characters in extern definition that are not used",
"There are additional characters in entry definition that are not used",
"Invalid command",
"Invalid label definition - more than 31 characters",
"Invalid label definition-The first letter is not alphabetic",
"Invalid label definition-Non-alphanumeric characters are present",
"Invalid label definition-There is a macro with this name",
"Invalid label definition-The name of the lable is the name of a directive",
"Invalid label definition-The name of the lable is the name of a instruction",
"Invalid label definition-The name of the lable is the name of a register",
"Invalid label definition-The name of the lable already exists",
"Invalid label definition-The label has already been defined as external",
"Invalid label definition - a label has already been defined as an entry",
"The source operand is invalid",
"missing a comma",
"The destination operand is invalid",
"There are extra characters in the command",
"The number in the data is invalid",
"Unnecessary comma",
"A blank line after a label",
"There is no data after directive",
"There is no name to the label",
"Missing operand",
"A label is defined as an entry label as it has no definition",
"The label was defined as both internal and external",
"The label name was not defined",
"The code is running out of memory"
};
/**
 * Array of error messages related to memory and file operations.
 * Each message describes a specific memory or file-related error condition.
 */
char* errors_memory[] = {
"Memory allocation failed",
"Error opening file",
"The fgets function failed to read from the file"
};
/**
 * Array of warning messages corresponding to various warning codes.
 * Each message describes a specific warning condition encountered during processing.
 */
char* warnings[] = {
"There is a label before the external definition. ",
"There is a label before the entry definition.",
};
void error( int msg, const char* suffix, const char* file_name, const int number_line)
{
	printf("Error in line %d in the file %s%s : %s \n", number_line, file_name, suffix, errors[msg]);
}
void error_memory(int error) {
	printf("%s\n", errors_memory[error]);
}
void warning( int msg, const char* suffix, const char* file_name, const int number_line) {
	printf("warning in line %d in the file %s%s : %s \n", number_line, file_name, suffix, warnings[msg]);
}
