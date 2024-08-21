#include "Pre-proccesor.h"
#include "handle-file.h"
#include "handle-text.h"
#include "errors.h"
#include "Parser-step1.h"
int pre_processor(char* file_name, func instructions[], char* directive[], macro_table** tableM, int* macro_size, char* regs[])
{
	FILE* source_file;
	FILE* macro_file;
	char* line;
	char* buffer = NULL;
	char* original_line;
	int number_line = 0;
	int i;
	int check_error_per_line = no_errors;
	int check_error = no_errors;
	int type_of_command = unknown;
	source_file = open_file(file_name, ".as", "r");/* Open the source file for reading */
	if (source_file == NULL)
	{
		return memory_errors;
	}
	macro_file = open_file(file_name, ".am", "w");;/* Open the macro file for writing */
	if (macro_file == NULL)
	{
		fclose(source_file);
		return memory_errors;
	}
	while (1)
	{
		check_error_per_line = no_errors;
		type_of_command = unknown;
		number_line++;
		line = read_line(source_file, ".as", file_name, number_line, &check_error_per_line);
		if (feof(source_file))/* Break the loop when end-of-file is reached */
			break;
		if (check_error_per_line == memory_errors)
		{
			free(line);
			fclose(source_file);
			return memory_errors;
		}
		if (check_error_per_line == code_errors)/* Process lines that are not comments or empty */
			check_error = code_errors;
		skip_whitespace(&line);
		if (check_error_per_line == no_errors && line[0] != ';' && line != NULL)
		{
			original_line = strdup1(line);
			if (original_line == NULL)
			{
				free(line);
				fclose(source_file);
				return memory_errors;
			}
			buffer = read_word(&line, ".as", file_name, number_line);
			if (buffer == NULL)
			{
				fclose(source_file);
				free(line);
				free(original_line);
				return memory_errors;
			}
			else if (strcmp(buffer, "macr") == 0)
			{
				type_of_command = macro;
				buffer = read_word(&line, ".as", file_name, number_line);/* Read the macro name */
				if (buffer == NULL)
				{
					fclose(source_file);
					free(line);
					free(original_line);
					return memory_errors;
				}
				if (!check_garbage(&line))
				{
					error(e_additional_characters_in_macro, ".as", file_name, number_line);
					check_error = code_errors;
				}
				else
				{
					check_error_per_line = get_feild_data(source_file, tableM, file_name, &number_line, macro_size, buffer, instructions, directive, regs);
					if (check_error_per_line == memory_errors)
					{
						fclose(source_file);
						free(line);
						free(original_line);
						free(buffer);
						return memory_errors;
					}
					if (check_error_per_line == code_errors)
						check_error = code_errors;
				}
			}
			else
			{
				for (i = 0; i < *macro_size; i++)/* Replace macros in the line */
				{
					if (strcmp(buffer, (*tableM)[i].macr_name) == 0)
					{
						if (!check_garbage(&line))
						{
							error(e_additional_characters_in_command, ".as", file_name, number_line);
							check_error = code_errors;
						}
						type_of_command = name_macro;
						fprintf(macro_file, "%s", (*tableM)[i].macr_data);
						break;
					}
				}
			}  /* Write the original line if no macro substitution was done */
			if (type_of_command == unknown && !is_line_whitespace_only(original_line))
				fprintf(macro_file, "%s", original_line);
		}
	}

	/* Close the file */
	fclose(source_file);
	fclose(macro_file);
	return check_error;
}

int add_macro(macro_table** table, int* size,  char* macro_name, char* data, func instructions[], char* directive[], char* file_name, int number_line, char* regs[])
{
	int check_error;
	macro_table* temp;
	check_error = is_macro_valid(macro_name, instructions, directive, file_name, number_line, regs);/* Validate the macro */
	if (check_error != no_errors)
	{
		return check_error;
	}
	check_error = is_macro_name_duplicate(*table, *size, macro_name, file_name, number_line);/* Check for duplicate macro names */
	if (check_error != no_errors)
	{
		return check_error;
	}
	temp = realloc(*table, (*size + 1) * sizeof(macro_table));/* Reallocate memory to add the new macro */
	if (temp == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors; /* Return an error value if the allocation failed*/
	}

	*table = temp;

	/* Allocate memory and store the data in the new structure*/
	(*table)[*size].macr_name = strdup1(macro_name);
	if ((*table)[*size].macr_name == NULL)
	{
		return memory_errors; /*Return an error value if the allocation failed*/
	}
	(*table)[*size].macr_data = strdup1(data);
	if ((*table)[*size].macr_data == NULL)
	{
		free((*table)[*size].macr_name);
		return memory_errors; /* Return an error value if the allocation failed*/
	}

	(*size)++;		  /* Update the size of the array*/
	return no_errors; /* Return a success value*/
}

int get_feild_data(FILE* source_file, macro_table** table, char* file_name, int* number_line, int* size, char* macro_name, func instructions[], char* directive[], char* regs[])
{
	char* macro_data;
	char* line;
	char* temp;
	int macro_data_size , macro_data_length, line_length,check_error, new_length, number_line_macr;
	char* word;
	char* temp_line;
	word = NULL;
	check_error = no_errors;
	/* Define initial sizes for macro_data and its length */
	macro_data_size = LINE_CHARACTERS;
	macro_data_length = 0;

	number_line_macr = *number_line;

	/* Allocate memory for macro_data */
	macro_data = (char*)malloc(macro_data_size * sizeof(char));
	if (macro_data == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}

	/* Initialize macro_data with an empty string */
	macro_data[0] = '\0';

	while (1)
	{
		line = read_line(source_file, ".as", file_name, *number_line, &check_error);
		(*number_line)++;
		if (feof(source_file))
		{
			free(macro_data);
			error(e_macro_not_finished, ".as", file_name, *number_line);
			return code_errors;
		}
		if (check_error != no_errors)
		{
			free(macro_data); 
			return check_error;
		}
		temp_line = strdup1(line);
		if (temp_line == NULL)
		{
			free(line);
			free(macro_data); 
			return memory_errors;
		}
		word = read_word(&temp_line, ".as", file_name, *number_line);
		if (word == NULL)
		{
			free(temp_line);
			free(line);
			free(macro_data);  
			return memory_errors;
		}
		if (strcmp(word, "endmacr") == 0)/* Check for the end of macro definition */
		{
			break;
		}

		/* Calculate the new length of macro_data if the buffer is appended */
		line_length = strlen(line);
		new_length = macro_data_length + line_length;

		/* Reallocate memory if needed */
		if (new_length >= macro_data_size)
		{
			macro_data_size += LINE_CHARACTERS;
			temp = realloc(macro_data, macro_data_size * sizeof(char));
			if (temp == NULL)
			{
				error_memory(m_memory_failed);
				free(word);
				free(temp_line);
				free(line);
				free(macro_data);	  
				return memory_errors; /* Or handle the error appropriately */
			}
			macro_data = temp;
		}

		/* Append the buffer to macro_data */
		strcat(macro_data, line);
		macro_data_length = new_length;
	}

	if (!check_garbage(&temp_line))/* Check for additional characters in the macro definition */
	{
		free(macro_data); 
		error(e_additional_characters_in_macro, ".as", file_name, *number_line);
		return code_errors;
	}
	/* Add the macro to the macro table */
	check_error = add_macro(table, size, macro_name, macro_data, instructions, directive, file_name, number_line_macr, regs);
	if (check_error != no_errors)
	{
		free(macro_data); 
		return check_error;
	}
	free(macro_data);
	return no_errors;
}

int is_macro_name_duplicate(macro_table* table, int size, char* macro_name, char* file_name, int number_line)
{

	if (is_macro_name(macro_name, table, size))/* Check if the macro name already exists in the table */
	{
		error(e_macro_name_exists, ".as", file_name, number_line);
		return code_errors; 
	}
	return no_errors; 
}

int is_macro_valid(char* macro_name, func instructions[], char* directive[], char* file_name, int number_line, char* regs[])
{
	if (!strcmp(macro_name, "macr")) {/* Check if the macro name is "macr", which is invalid */
		error(e_macro_macr, ".as", file_name, number_line);
		return code_errors;
	}
	if (!strcmp(macro_name, "endmacr")) {/* Check if the macro name is "endmacr", which is invalid */
		error(e_macro_endmacr, ".as", file_name, number_line);
		return code_errors;
	}
	if (is_directive_name(macro_name, directive))  /* Check if the macro name is a valid directive name */
	{
		error(e_macro_name_directive, ".as", file_name, number_line);
		return code_errors; 
	}
	if (is_instructions_name(macro_name, instructions))/* Check if the macro name is a valid instruction name */
	{
		error(e_macro_name_instruction, ".as", file_name, number_line);
		return code_errors; 
	}
	if (is_register_name(macro_name, regs)) /* Check if the macro name is a valid register name */
	{
		error(e_macro_name_register, ".as", file_name, number_line);
		return code_errors; 
	}
	return no_errors; 
}
