#include "Parser-step2.h"
#include "handle-text.h"
#include "handle-file.h"
#include "errors.h"


int parser_step2(code_table** tableC, int code_size, lable_table* tableL, int lable_size, data_table* tableD, int data_size, extern_table* tableEX, int extern_size, entry_table* tableEN, int entry_size, char* file_name,int parser_1_error)
{
	char* found_word;
	int IC, error_status, DC, extern_index,i,j;
	char* binary_str;
	int address;
	/* Initialize instruction counter and error status */
	 IC = 0;
	found_word = NULL;
	error_status = parser_1_error; /* Initialize error status from the first parsing step */
	DC = return_DC(tableD, data_size);/* Calculate total data count */


	for ( i = 0; i < entry_size; i++)  /* Validate entries in the entry table */
	{
		if (find_in_symbol_table(tableL, lable_size, tableEN[i].entry_name) == -1)/* Check if the label in entry is not found in the symbol table*/
		{
			error(e_no_entry_definition,".am", file_name, tableEN[i].number_line);
			error_status = code_errors; 
		}
		if (find_in_extern_table(tableEX, extern_size, tableEN[i].entry_name) != -1)/* Check if the label in entry is also found in the extern table*/
		{
			error(e_both_internaland_external, ".am",file_name, tableEN[i].number_line);
			error_status = code_errors; 
		}
	}
	for ( i = 0; i < code_size; i++)/* Iterate through the code table*/
	{
		for ( j = 0; j < (*tableC)[i].amount_coding_words; j++)
		{
			if (starts_with_alpha((*tableC)[i].binary_code[j]) == no_errors)
			{
				found_word = strdup1((*tableC)[i].binary_code[j]);

				address = find_in_symbol_table(tableL, lable_size, found_word);

				if (address != -1)
				{
					 binary_str = encode_address(address);/* If found_word is in the symbol table, encode the address*/                             	
					 if(binary_str==NULL)
						return memory_errors;					
					(*tableC)[i].binary_code[j] = binary_str;
				}
				else
				{
					 extern_index = find_in_extern_table(tableEX, extern_size, found_word);
					if (extern_index != -1) {
						 binary_str = strdup1(EXTERN_BINARY_STR);/* Update the binary code and add address if found in the extern table */
						free((*tableC)[i].binary_code[j]);
						 (*tableC)[i].binary_code[j] = (char*)malloc(strlen(binary_str) + 1);  
						 if ((*tableC)[i].binary_code[j] == NULL) 					  							{							
							 error_memory(m_memory_failed);
							 return memory_errors;
						 }
						strcpy((*tableC)[i].binary_code[j], binary_str);
						if (add_to_extern_addresses(&tableEX[extern_index], IC + BASE_ADDRESS) == memory_errors) 
							return memory_errors;
					}
					else
					{
						/* Report error if the label is not defined */
						error(e_label_not_defined, ".am", file_name, (*tableC)[i].number_line);
						error_status = code_errors; 
					}
				}
			}
			IC++;
		}
	}

		if (error_status == no_errors)/* Generate output files if no errors are found */
		{
			if (generate_ob_file(file_name, tableC, code_size, tableD, data_size, IC, DC) == memory_errors)
				return memory_errors;
			if (entry_size > 0)
			{
				if (generate_ent_file(file_name, tableEN, entry_size, tableL, lable_size) == memory_errors)
				{
					return memory_errors;
				}
			}
			if (extern_size > 0)
			{
				if (generate_ext_file(file_name, tableEX, extern_size) == memory_errors)
				{
					return memory_errors;
				}
			}
		}
		return error_status;
}


int starts_with_alpha(const char* str)
	{
		if (str != NULL && isalpha(str[0]))
			return no_errors;
		return code_errors;
	}

int find_in_symbol_table(lable_table * tableL, int lable_size,  char* found_word)
	{
		int i;
		for (i = 0; i < lable_size; i++)
		{
			if (strcmp(tableL[i].lable_name, found_word) == 0)
			{
				return tableL[i].lable_address; /* Return the address associated with the label*/
			}
		}
		return -1; /*Label not found*/
	}

char* encode_address(int address)
	{
		int encoded_value,i;
		char* binary_str;
		 encoded_value = (address << OFFSET_BITS) | OFFSET_VALUE; /* The 3-15 bits are the address, 0-2 bits are 010*/

		/* Create a 15-bit binary string*/
		binary_str = (char*)malloc(BINARY_CODE_LENGTH); /* 15 bits + 1 for the null terminator*/
		if (binary_str == NULL)
		{
			error_memory(m_memory_failed);
			return NULL; 
		}

		for ( i = 0; i < ADDRESS_BITS; i++)
		{
			binary_str[ADDRESS_BITS - 1 - i] = (encoded_value & (1 << i)) ? '1' : '0';
		}
		binary_str[ADDRESS_BITS] = '\0';  /* Null-terminate the string*/

		return binary_str;
	}

int find_in_extern_table(extern_table * tableEX, int extern_size, const char* found_word)
	{
		int i;
		for ( i = 0; i < extern_size; i++)
		{
			if (strcmp(tableEX[i].extern_name, found_word) == 0)
			{
				return i; /*Return the index if found*/
			}
		}
		return -1; /* Label not found*/
	}

int add_to_extern_addresses(extern_table * extern_entry, int address) {
		int* temp;
		
		temp = (int*)realloc(extern_entry->extern_addresses, (extern_entry->size_address + 1) * sizeof(int));
		if (temp == NULL) {
			error_memory(m_memory_failed);
			return memory_errors;
		}
		extern_entry->extern_addresses = temp;/*Update the extern_addresses pointer with the newly allocated memory*/

		extern_entry->extern_addresses[extern_entry->size_address] = address;/*Add the new address to the array*/

		extern_entry->size_address++;/*Increment the size_address counter to reflect the addition*/
		return no_errors;
	}
int generate_ob_file(char* file_name, code_table **tableC, int code_size, data_table *tableD, int data_size, int IC, int DC)
{
    int i, j;
    FILE* ob_file;
    ob_file = open_file(file_name, ".ob", "w");/* Open the .ob file for writing */

    if (ob_file == NULL)
    {
        return memory_errors;
    }

    /* Print IC and DC at the top of the file */
    fprintf(ob_file, "  %d %d\n", IC, DC);

    /* Write code table instructions */
    for (i = 0; i < code_size; i++)
    {
        for (j = 0; j < (*tableC)[i].amount_coding_words; j++)
        {
	/* Convert binary code to octal and write it along with the address */
            fprintf(ob_file, "%04d %05lo\n", (*tableC)[i].code_addresses[j], strtol((*tableC)[i].binary_code[j], NULL, 2));
        }
    }

    /* Write data table instructions */
    for (i = 0; i < data_size; i++)
    {
        for (j = 0; j < tableD[i].amount_coding_data_words; j++)
        {
             fprintf(ob_file, "%04d %05lo\n", tableD[i].data_address[j], strtol(tableD[i].data_binary_code[j], NULL, 2));
        }
    }

    fclose(ob_file);
    return no_errors;
}


	int generate_ent_file(char* file_name, entry_table * tableEN, int entry_size, lable_table * tableL, int lable_size)
	{
		FILE* ent_file;
		int i, address;
		ent_file = open_file(file_name, ".ent", "w");/* Open the .ent file for writing */
		if (ent_file == NULL) {
			return memory_errors;
		}
		for ( i = 0; i < entry_size; i++)
		{
			/* Find the address of the entry name in the label table */
			address = find_in_symbol_table(tableL, lable_size, tableEN[i].entry_name);
			fprintf(ent_file, "%s %d\n", tableEN[i].entry_name, address);

		}

		fclose(ent_file);
		return no_errors;
	}

	int generate_ext_file(char* file_name, extern_table * tableEX, int extern_size)
	{
		FILE* ext_file;
		int i, j;
		ext_file = open_file(file_name, ".ext", "w");/* Open the .ext file for writing */
		if (ext_file == NULL) {
			return memory_errors;
		}
		/* Iterate over each entry in the external table */
		for ( i = 0; i < extern_size; i++)
		{
			/* Write each external symbol name and its associated addresses to the file */
			for ( j = 0; j < tableEX[i].size_address; j++)
			{
				fprintf(ext_file, "%s %04d\n", tableEX[i].extern_name, tableEX[i].extern_addresses[j]);
			}
		}

		fclose(ext_file);
		return no_errors;
	}

	int return_DC(data_table* table, int size) {
		int DC,i;
		DC = 0;
		/* Iterate over each entry in the data table */
		for ( i = 0; i < size; i++) {
			DC += table[i].amount_coding_data_words;/* Add the number of coding data words from each entry to the total DC */
		}
		return DC;
	}
