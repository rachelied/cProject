#include "handle-text.h"
#include "handle-file.h"
#include "errors.h"
#include "coding.h"
#include "Parser-step1.h"

int coding_data(char** line, char* suffix, char* file_name, int number_line, data_table** tableD, int* data_size, char* original_line, int DC)
{
	int* code_addresses, * temp_addresses;
	char* source_code, * word, ** data_binary_code, * binary_rep;
	int size = 0, result, check;
	code_addresses = NULL;
	data_binary_code = NULL;
	source_code = strdup1(original_line);
	if (source_code == NULL)
		return memory_errors;
/*Process the line until it is empty*/
	while (strcmp(*line, ""))
	{
		word = read_word(line, suffix, file_name, number_line);
		if (word == NULL)
		{
			cleanup_memory(source_code, data_binary_code, code_addresses, size);/*Clean up memory in case of error*/
			return memory_errors;
		}
		/*Check if the word is ","*/
		if (strcmp(word, ",") == 0) {
			word = read_word(line, suffix, file_name, number_line);
			if (word == NULL)
			{
				cleanup_memory(source_code, data_binary_code, code_addresses, size);/*Clean up memory in case of error*/
				return memory_errors;
			}
		}
		/*Add address to code_addresses and handle memory allocation*/
		temp_addresses = (int*)realloc(code_addresses, (size + 1) * sizeof(int));
		if (temp_addresses == NULL)
		{
			error_memory(m_memory_failed);
			cleanup_memory(source_code, data_binary_code, code_addresses, size);
			free(word);
			return memory_errors;
		}
		code_addresses = temp_addresses;
		/*Convert the number to a 15-bit binary representation*/
		binary_rep = strdup1(to_binary_15bit(atoi(word)));
		if (binary_rep == NULL)
		{
			cleanup_memory(source_code, data_binary_code, code_addresses, size);/*Clean up memory in case of error*/
			free(word);
			return memory_errors;
		}
		/*Add the binary representation to data_binary_code array*/
		check = add_string_to_array(&data_binary_code, &size, binary_rep);
		if (check == memory_errors)
			return memory_errors;
		/*Update the address of the code*/
		code_addresses[size - 1] = DC + size - 1;
		free(binary_rep);
		free(word);
		skip_whitespace(line);
	}
	/*Add the processed data to the table*/
	result = add_data_to_table(tableD, data_size, code_addresses, size, source_code, data_binary_code);
	/*Clean up all allocated memory*/	
	cleanup_memory(source_code, data_binary_code, code_addresses, size);

	return result;
}

void cleanup_memory(char* source_code, char** data_binary_code, int* code_addresses, int size)
{
	int i;
	free(source_code);
	for (i = 0; i < size; i++)
	{
		free(data_binary_code[i]);
	}
	free(data_binary_code);
	free(code_addresses);
}

char* to_binary_15bit(int number)
{
	int i;
	static char binary[BINARY_WORD + 1];
   /* Ensure the last character in the binary string is the null terminator */
	binary[BINARY_WORD] = '\0';
 /* Loop through each bit to convert the number into a binary string */
	for (i = BINARY_WORD - 1; i >= 0; i--)
	{/* Assign '1' or '0' to the current position based on the least significant bit */
		binary[i] = (number & 1) ? '1' : '0';
		number >>= 1;
	}

	return binary;
}

int add_data_to_table(data_table** tableD, int* data_size, int* code_addresses, int size, char* source_code, char** binary_code)
{
	int i,j;
	data_table* new_row;
	char* temp_str;
	data_table* temp;
	/* Reallocate memory for the data table to add a new row */
	temp = (data_table*)realloc(*tableD, (*data_size + 1) * sizeof(data_table));
	if (temp == NULL)
		return memory_errors;
	*tableD = temp;
	/* Point to the newly allocated row in the table */
	new_row = &((*tableD)[*data_size]);
	/* Allocate memory for the data addresses */
	new_row->data_address = (int*)malloc(size * sizeof(int));
	if (new_row->data_address == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}
	/* Copy the code addresses into the new row */
	memcpy(new_row->data_address, code_addresses, size * sizeof(int));
	/* Set the amount of coding data words */
	new_row->amount_coding_data_words = size;

	new_row->data_source_code = strdup1(source_code);
	if (new_row->data_source_code == NULL)
	{
		free(new_row->data_address);
		return memory_errors;
	}
	/* Allocate memory for the binary code strings */
	new_row->data_binary_code = (char**)malloc(size * sizeof(char*));
	if (new_row->data_binary_code == NULL)
	{
		error_memory(m_memory_failed);
		free(new_row->data_source_code);
		free(new_row->data_address);
		return memory_errors;
	}
	/* Duplicate each binary code string and store it in the new row */
	for ( i = 0; i < size; i++)
	{
		temp_str = strdup1(binary_code[i]);
		if (temp_str == NULL) {
			for ( j = 0; j < i; j++)
				free(new_row->data_binary_code[j]);
			free(new_row->data_binary_code);
			free(new_row->data_source_code);
			free(new_row->data_address);
			return memory_errors;
		}
		new_row->data_binary_code[i] = temp_str;
	}
	/* Increment the data size */
	(*data_size)++;

	return no_errors;
}

int coding_string(char** line, data_table** tableD, int* data_size, char* original_line, int DC)
{
	int* code_addresses, * temp_addresses;
	char* source_code, ** data_binary_code;
	int size, result, i;
	size = 0;
	code_addresses = NULL;
	source_code = strdup1(original_line);
	if (source_code == NULL)
		return memory_errors;
	data_binary_code = NULL;
	size = 0;
	/* Skip leading and trailing whitespace from the line */
	skip_whitespace(line);
	skip_whitespace_end(line);
	/* Remove quotation marks */
	(*line)++;
	(*line)[strlen((*line)) - 1] = '\0';
	data_binary_code = encode_binary_strings(*line, &size); /* Encode the string into binary and get the size */
	if (data_binary_code == NULL)
		return memory_errors;
	/* Allocate and assign addresses for the encoded binary strings */
	for (i = 0; i < size; i++)
	{
		temp_addresses = (int*)realloc(code_addresses, (size + 1) * sizeof(int));
		if (temp_addresses == NULL)
		{
			error_memory(m_memory_failed);
			cleanup_memory(source_code, data_binary_code, code_addresses, size);
			return memory_errors;
		}
		code_addresses = temp_addresses;
	/* Set the code address for each binary code */
		code_addresses[i] = DC + i;
	}
	result = add_data_to_table(tableD, data_size, code_addresses, size, source_code, data_binary_code);/* Add the data to the data table */
	cleanup_memory(source_code, data_binary_code, code_addresses, size); /* Cleanup memory used during the process */

	return result;
}

char** encode_binary_strings(const char* str, int* size)
{
	unsigned char c;
	int i, len, j;
	char** result = NULL, ** temp;
	char binary[BINARY_WORD + 1];/* Array to store the 15-bit binary representation */
	len = strlen(str);
	/* Loop through each character in the string */
	for (i = 0; i <= len; i++)
	{ 
		/* Check if the end of the string is reached or if the current character is not a space or quotation mark */
		if (i == len || (!isspace(str[i]) && str[i] != '"'))
		{
			if (i < len)
			{
				/* Convert the current character to its binary representation */
				 c = str[i];
				for (j = BINARY_WORD - 1; j >= 0; j--)
				{
					binary[j] = (c & 1) + '0';/* Extract the least significant bit and store as '0' or '1' */
					c >>= 1;
				}
				binary[BINARY_WORD] = '\0';
			}
			else
			{
				strcpy(binary, "000000000000000");
			}
			/* Reallocate memory to store the new binary string */
			temp = realloc(result, ((*size) + 1) * sizeof(char*));
			if (temp == NULL)
			{
				error_memory(m_memory_failed);
				free(result);
				return NULL;
			}
			result = temp;
			result[(*size)] = strdup1(binary);
			if (result[(*size)] == NULL)
			{
				error_memory(m_memory_failed);
				free(result);
				return NULL;
			}
			(*size)++;/* Increment the size counter */
		}
	}

	return result;
}


int coding_instructions(char* line, char* suffix, char* file_name, int number_line, code_table** tableC, int* code_size, char* original_line, int IC, func instructions[], macro_table* tableM, int macro_size, char* directive[], char* regs[], int index)
{
	char* word, * coding_word, * temp;
	int* addresses_array;
	int  addressing_method, size, check, reg_operand;
	char** coding_array;
	size = 0;
	reg_operand = false;
	coding_array = NULL;
	addresses_array = NULL;
	/* Generate the first coding word */
	coding_word = coding_first_word(line, index, suffix, file_name, number_line, instructions, tableM, macro_size, directive, regs);
	if (coding_word == NULL)
		return memory_errors;
	check = add_string_to_array(&coding_array, &size, coding_word); /* Add the first coding word to the coding array */
	if (check == memory_errors)
		return memory_errors;
	check = add_int_to_array(&addresses_array, size - 1, IC + size - 1); /* Add the corresponding address to the addresses array */
	if (check == memory_errors)
		return memory_errors;
	if (instructions[index].source_operand != nothing) {/* Check if there is a source operand */
		word = read_word(&line, suffix, file_name, number_line);
		if (word == NULL)
			return memory_errors;
		/* Determine the addressing method for the source operand */
		addressing_method = get_addressing_method(word, instructions, tableM, macro_size, directive, regs, instructions[index].source_operand);
		if (addressing_method == first_method)
		{
			coding_word = direct_address(word);
			if (coding_word == NULL)
				return memory_errors;
		}
		else if (addressing_method == fourth_method || addressing_method == third_method)
		{
			coding_word = register_addressing(source, word);
			if (coding_word == NULL)
				return memory_errors;
			reg_operand = true;
		}
		else if (addressing_method == second_method)
		{
			strcpy(coding_word, word); /* Immediate addressing */
		}
		/* Add the source operand's coding word to the coding array if applicable */
		if (addressing_method != no_method)
		{
			check = add_string_to_array(&coding_array, &size, coding_word);
			if (check == memory_errors)
				return memory_errors;
			check = add_int_to_array(&addresses_array, size - 1, IC + size - 1); /*Add the corresponding address to the addresses array */
			if (check == memory_errors)
				return memory_errors;
		}
		word = read_word(&line, suffix, file_name, number_line);
		if (word == NULL)
			return memory_errors;
	}

	word = read_word(&line, suffix, file_name, number_line);
	if (word == NULL)
		return memory_errors;
	/* Determine the addressing method for the destination operand */
	addressing_method = get_addressing_method(word, instructions, tableM, macro_size, directive, regs, instructions[index].destination_operand);
	if (addressing_method == first_method)
	{
		coding_word = direct_address(word);
		if (coding_word == NULL)
			return memory_errors;
	}
	else if (addressing_method == fourth_method || addressing_method == third_method)
	{
		coding_word = register_addressing(destination, word);
		if (coding_word == NULL)
			return memory_errors;
		if (reg_operand == true) /* Combine register coding if both source and destination are registers */
		{
			temp = bitwise_or(coding_array[size - 1], coding_word);
			if (temp == NULL)
				return memory_errors;
			strcpy(coding_array[size - 1], temp);
		}
	}
	else if (addressing_method == second_method)
	{
		strcpy(coding_word, word);
	}
	/* Add the destination operand's coding word to the coding array if applicable */
	if (!((reg_operand == true && (addressing_method == fourth_method || addressing_method == third_method)) || addressing_method == no_method))
	{
		check = add_string_to_array(&coding_array, &size, coding_word);
		if (check == memory_errors)
			return memory_errors;
		check = add_int_to_array(&addresses_array, size - 1, IC + size - 1);
		if (check == memory_errors)
			return memory_errors;
	}
	free(word);
	/* Add the corresponding address to the addresses array */
	return add_to_code_table(tableC, code_size, number_line, original_line, size, addresses_array, coding_array);
}

char* coding_first_word(char* line, int index, char* suffix, char* file_name, int number_line, func instructions[], macro_table* tableM, int macro_size, char* directive[], char* regs[])
{
	char* word;
	int so_addressing_method, do_addressing_method;
	do_addressing_method = 0;
	so_addressing_method = 0;
	if (instructions[index].source_operand != nothing) /* Check if the instruction has a source operand */
	{
		word = read_word(&line, suffix, file_name, number_line);
		if (word == NULL)
			return NULL;
		/* Determine the addressing method for the source operand */
		so_addressing_method = get_addressing_method(word, instructions, tableM, macro_size, directive, regs, instructions[index].source_operand);
		word = read_word(&line, suffix, file_name, number_line);
		if (word == NULL)
			return NULL;
	}
	word = read_word(&line, suffix, file_name, number_line);
	if (word == NULL)
		return NULL;
	/* Determine the addressing method for the destination operand */
	do_addressing_method = get_addressing_method(word, instructions, tableM, macro_size, directive, regs, instructions[index].destination_operand);
	/* Create and return the binary first word for the instruction */
	free(word);
	return create_binary_first_word(instructions[index].opcode, so_addressing_method, do_addressing_method);
}

int get_addressing_method(char* word, func instructions[], macro_table* tableM, int macro_size, char* directive[], char* regs[], int operand)
{

	if (operand == nothing)
		return no_method;
	if (is_integer(word))
		return first_method;
	if (can_be_lable_name(word, tableM, macro_size, instructions, directive, regs))
		return second_method;
	if (is_pointer(word, regs))
		return third_method;
	return fourth_method;
}

char* create_binary_first_word(const char* opcode, int source_operand, int destination_operand)
{
	char* result;
	/* Allocate memory for the result string, including space for the null terminator */
	result = (char*)malloc(BINARY_WORD + 1 * sizeof(char) + 1);
	if (result == NULL)
	{
		error_memory(m_memory_failed);
		return NULL;
	}
	/* Initialize the result string with '0's and set the null terminator */
	memset(result, '0', BINARY_WORD + 1);
	result[BINARY_WORD] = '\0';
	/* Copy the opcode into the first part of the result string */
	strncpy(result, opcode, LENGTH_OPCODE);
	/* Set the bit for the source operand, if it exists */
	if (source_operand != 0)
	{
		result[7 - (source_operand - 1)] = '1';
	}

	/* Set the bit for the destination operand, if it exists */
	if (destination_operand != 0)
	{
		result[11 - (destination_operand - 1)] = '1';
	}
	/* Set the 12th bit as '1' */
	result[LAST_BIT_INDEX] = '1';

	return result;
}

char* direct_address(const char* num)
{
	int number;
	char* binary, * result;
	number = atoi(num + 1); /* Convert the numeric string (skipping the first character, assumed to be a symbol like '#') to an integer */
	binary = to_binary_15bit(number);/* Convert the integer to a 15-bit binary string */
	/* Allocate memory for the result string (15 bits + null terminator) */
	result = (char*)malloc(BINARY_WORD + 1 * sizeof(char));
	if (result == NULL)
	{
		error_memory(m_memory_failed);
		return NULL;
	}
	/* Copy the middle 12 bits of the binary representation into the result string */
	strncpy(result, binary + 3, 12);

	result[12] = '1';
	result[13] = '0';
	result[14] = '0';
	result[15] = '\0';

	return result;
}

void to_3bit_binary(int number, char* binary)
{
	int i;
	/* Convert the integer to a 3-bit binary representation */
	for (i = 2; i >= 0; i--)
	{
		binary[i] = (number & 1) ? '1' : '0'; /* Set the current bit in the binary string based on the least significant bit of the number */
		number >>= 1;
	}
}


char* register_addressing(int n, const char* reg)
{
	char* result;
	char binary[4] = "000";/* Initialize a 3-bit binary string with '000' */
	int reg_num;
	/* Determine the register number based on the input string */
	if (reg[0] == '*')
	{
		reg_num = atoi(reg + 2);/* Handle the case where the register is prefixed by '*/
	}
	else
	{
		reg_num = atoi(reg + 1);/* Handle the standard case */
	}
	/* Allocate memory for the result string (15 bits + null terminator) */
	result = (char*)malloc(BINARY_WORD + 1 * sizeof(char));
	if (result == NULL)
	{
		return NULL;
	}
	/* Initialize the result string with '0's and set the null terminator */
	memset(result, '0', BINARY_WORD + 1);
	result[BINARY_WORD] = '\0';
	/* Convert the register number to a 3-bit binary string */
	to_3bit_binary(reg_num, binary);
	/* Insert the 3-bit binary string into the result string based on whether it's a source or destination */
	if (n == destination)
	{
		strncpy(result + DESTINATION_INDEX, binary, 3);/* Place at the destination index */
	}
	else if (n == source)
	{
		strncpy(result + SOURCE_INDEX, binary, 3);/* Place at the source index */
	}
	result[LAST_BIT_INDEX] = '1';

	return result;
}

int add_string_to_array(char*** array, int* size, char* newString)
{
	char** temp;
	/* Reallocate memory to expand the array to hold one more string */
	temp = realloc(*array, (*size + 1) * sizeof(char*));
	if (temp == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}
	*array = temp;
	/* Allocate memory for the new string in the array */
	(*array)[*size] = malloc((strlen(newString) + 1) * sizeof(char));
	if ((*array)[*size] == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}
	/* Copy the new string into the allocated memory in the array */
	strcpy((*array)[*size], newString);
	/* Increment the size of the array */
	(*size)++;
	return no_errors;
}

char* bitwise_or(char* str1, char* str2)
{
	int i;
	char* result;
	if (str1 == NULL || str2 == NULL)
		return NULL;
	/* Allocate memory for the result string (15 bits + null terminator) */
	 result = malloc((BINARY_WORD + 1) * sizeof(char));
	if (result == NULL)
	{
		error_memory(m_memory_failed);
		return NULL;
	}
	/* Perform the bitwise OR operation for each bit in the input strings */
	for (i = 0; i < BINARY_WORD; i++)
	{
	/* If either bit is '1', set the corresponding bit in the result to '1' */
		if (str1[i] == '1' || str2[i] == '1')
		{
			result[i] = '1';
		}
		else
		{
			result[i] = '0';
		}
	}
	result[BINARY_WORD] = '\0';

	return result;
}

int add_to_code_table(code_table** tableC, int* size, int number_line,  char* code_source_code, int amount_coding_words,  int* code_addresses,  char** binary_code)
{
	int i, j;
	code_table* new_entry;
	code_table* new_table;
	/* Reallocate memory to expand the code table by one entry */
	new_table = realloc(*tableC, (*size + 1) * sizeof(code_table));
	if (new_table == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}
	*tableC = new_table;
	/* Set up the new entry in the code table */
	new_entry = &((*tableC)[*size]);
	new_entry->number_line = number_line;
	new_entry->amount_coding_words = amount_coding_words;
	/* Duplicate the source code string */
	new_entry->code_source_code = strdup1(code_source_code);
	if (new_entry->code_source_code == NULL)
		return memory_errors;
	/* Allocate memory for the code addresses */
	new_entry->code_addresses = malloc(amount_coding_words * sizeof(int));
	if (new_entry->code_addresses == NULL)
	{
		free(new_entry->code_source_code);
		error_memory(m_memory_failed);
		return memory_errors;
	}
	/* Copy the code addresses to the new entry */
	memcpy(new_entry->code_addresses, code_addresses, amount_coding_words * sizeof(int));	
	/* Allocate memory for the binary code */
	new_entry->binary_code = malloc(amount_coding_words * sizeof(char*));	
	if (new_entry->binary_code == NULL)
	{
		free(new_entry->code_source_code);
		free(new_entry->code_addresses);
		error_memory(m_memory_failed);
		return memory_errors;
	}
	/* Duplicate each binary code string */
	for (i = 0; i < amount_coding_words; i++)
	{
		new_entry->binary_code[i] = strdup1(binary_code[i]);
		if (new_entry->binary_code[i] == NULL)
		{

			for (j = 0; j < i; j++)
			{
				free(new_entry->binary_code[j]);
			}
			free(new_entry->binary_code);
			free(new_entry->code_addresses);
			free(new_entry->code_source_code);
			return memory_errors;
		}
	}

	(*size)++; /* Increment the size of the code table */
	return no_errors;
}

int add_int_to_array(int** array, int size, int newInt)
{
	int* temp;
	/* Reallocate memory to expand the integer array by one element */
	temp = realloc(*array, (size + 1) * sizeof(int));
	if (temp == NULL)
	{
		error_memory(m_memory_failed);
		return memory_errors;
	}
	*array = temp;
	/* Assign the new integer to the last position in the array */
	(*array)[size] = newInt;


	return no_errors;
}

