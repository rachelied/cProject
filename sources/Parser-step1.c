#include "Parser-step1.h"
#include "handle-text.h"
#include "handle-file.h"
#include "errors.h"
#include "coding.h"

int parser_step1(char* file_name, macro_table* tableM, int macro_size, func instructions[], char* directive[], lable_table** tableL, int* lable_size, char* regs[], data_table** tableD, int* data_size, extern_table** tableEX, int* extern_size, entry_table** tableEN, int* entry_size, code_table** tableC, int* code_size)
{
    int number_line, type_of_command, check_error_per_line, check_error, index, IC, DC;
    char* line, * original_line, * word, * label_name;
    FILE* macro_file;
    check_error = no_errors;
    check_error_per_line = no_errors;
    number_line = 0, IC = 0, DC = 0;
    label_name = NULL;
    macro_file = open_file(file_name, ".am", "r");/* Open the input file */
    if (macro_file == NULL)
    {
        return memory_errors;
    }
    while (1)/* Main processing loop */
    {
	/* Reset variables for each line */
        check_error_per_line = no_errors;
        type_of_command = unknown;
        number_line++;
        line = read_line(macro_file, ".am", file_name, number_line, &check_error_per_line);
        if (feof(macro_file))
            break;
        if (check_error_per_line == memory_errors)
        {
            free(line);
            fclose(macro_file);
            return memory_errors;
        }
        if (check_error_per_line == code_errors)
            check_error = code_errors;
        original_line = strdup1(line);
        if (original_line == NULL)
        {
            free(line);
            fclose(macro_file);
            return memory_errors;
        }
        word = read_word(&line, ".am", file_name, number_line);
        if (word == NULL)
            return memory_errors;
        if (word[strlen(word) - 1] == ':')/* Check if the line starts with a label */
        {
            type_of_command = lable;
            word[strlen(word) - 1] = '\0';
            label_name = strdup1(word);
            if (label_name == NULL)
                return memory_errors;
            word = read_word(&line, ".am", file_name, number_line);
            if (word == NULL)
                return memory_errors;
        }
        if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0) /* Handle .data and .string directives */
        {
            if (type_of_command == lable)
            {

                check_error_per_line = check_lable(label_name, tableM, macro_size, instructions, directive, regs, *tableEX, *extern_size, *tableL, *lable_size, file_name, number_line);
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                else {
                    check_error_per_line = add_lable(tableL, lable_size, label_name, DC, data);
                    if (check_error_per_line == memory_errors)
                        return memory_errors;
                }
            }
            if (strcmp(word, ".data") == 0)
            {
                if (check_data(word, line, file_name, number_line, ".am"))
                {
                    check_error = code_errors;
                }
                else
                {

                    if (coding_data(&line, ".am", file_name, number_line, tableD, data_size, original_line, DC) == memory_errors)
                        return memory_errors;
                    DC += (*tableD)[(*data_size) - 1].amount_coding_data_words;
                }
            }
            else
            {
                if (is_line_whitespace_only(line))
                {
                    error(e_no_data, ".am", file_name, number_line);
                    check_error = code_errors;
                }
                else {
                    if (!is_string(line))
                    {
                        error(e_invalid_string, ".am", file_name, number_line);
                        check_error = code_errors;
                    }
                    else
                    {

                        if (coding_string(&line, tableD, data_size, original_line, DC) == memory_errors)
                            return memory_errors;
                        DC += (*tableD)[(*data_size) - 1].amount_coding_data_words;
                    }
                }
            }
            type_of_command = direct;
        }
        else if (strcmp(word, ".extern") == 0)/* Handle .extern directive */
        {
            if (type_of_command == lable)
            {
                warning(w_label_before_extern, ".am", file_name, number_line); 
                check_error_per_line = check_lable(label_name, tableM, macro_size, instructions, directive, regs, *tableEX, *extern_size, *tableL, *lable_size, file_name, number_line);
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                else {
                    check_error_per_line = add_lable(tableL, lable_size, label_name, DC, data);
                    if (check_error_per_line == memory_errors)
                        return memory_errors;
                }
            }
            word = read_word(&line, ".am", file_name, number_line);
            if (word == NULL)
                return memory_errors;
            if (!can_be_lable_name(word, tableM, macro_size, instructions, directive, regs))
                check_error = code_errors;
            else {
                check_error_per_line = add_extern_label(tableEX, extern_size, word, file_name, number_line);
                if (check_error_per_line == memory_errors)
                    return memory_errors;
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                if (!check_garbage(&line))
                {
                    error(e_additional_characters_in_extern, ".am", file_name, number_line);
                    check_error = code_errors;
                }
            }
            type_of_command = direct;
        }
        else if (strcmp(word, ".entry") == 0) /* Handle .entry directive */
        {
            if (type_of_command == lable)
            {
                warning(w_label_before_entry, ".am", file_name, number_line);
                check_error_per_line = check_lable(label_name, tableM, macro_size, instructions, directive, regs, *tableEX, *extern_size, *tableL, *lable_size, file_name, number_line);
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                else {
                    check_error_per_line = add_lable(tableL, lable_size, label_name, DC, data);
                    if (check_error_per_line == memory_errors)
                        return memory_errors;
                }
            }
            word = read_word(&line, ".am", file_name, number_line);
            if (word == NULL)
                return memory_errors;
            if (!can_be_lable_name(word, tableM, macro_size, instructions, directive, regs))
                check_error = code_errors;
            else {
                check_error_per_line = add_entry_label(tableEN, entry_size, word, file_name, number_line);
                if (check_error_per_line == memory_errors)
                    return memory_errors;
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                if (!check_garbage(&line))
                {
                    error(e_additional_characters_in_entry, ".am", file_name, number_line);
                    check_error = code_errors;
                }
            }
            type_of_command = direct;
        }
        else if (type_of_command == lable) /* Handle labels not followed by directives */
        {
            check_error_per_line = check_lable(label_name, tableM, macro_size, instructions, directive, regs, *tableEX, *extern_size, *tableL, *lable_size, file_name, number_line);
            if (check_error_per_line == code_errors)
                check_error = code_errors;
            else {
                check_error_per_line = add_lable(tableL, lable_size, label_name, IC + BASE_ADDRESS, code);
                if (check_error_per_line == code_errors)
                    check_error = code_errors;
                if (check_error_per_line == memory_errors)
                    return memory_errors;
            }
        }
        if (check_error_per_line == no_errors && type_of_command != direct && is_instructions_name(word, instructions))/* Handle instructions */
        {
            check_error_per_line = check_instructions(&index, word, instructions, line, file_name, number_line, regs, tableM, macro_size, directive);
            if (check_error_per_line == memory_errors)
                return memory_errors;
            if (check_error_per_line == code_errors)
                check_error = code_errors;
            else {
                check_error_per_line = coding_instructions(line, ".am", file_name, number_line, tableC, code_size, original_line, IC, instructions, tableM, macro_size, directive, regs, index);
                if (check_error_per_line == memory_errors)
                    return memory_errors;
                IC += (*tableC)[(*code_size) - 1].amount_coding_words;
            }
        } /* Handle invalid commands */
        else if (check_error_per_line == no_errors && (type_of_command == lable || type_of_command == unknown) && !is_line_whitespace_only(word))
        {
            error(e_invalid_command, ".am", file_name, number_line);
	    check_error = code_errors;
        }
        else if (check_error_per_line == no_errors && type_of_command == lable) {  /* Handle blank lines after labels */
            error(e_blank_line, ".am", file_name, number_line);
	    check_error = code_errors;
        }
        if (DC + IC > MEMORY_SIZE - BASE_ADDRESS)/* Check if we've exceeded memory limits */
        {
            error(e_out_of_memory, ".am", file_name, number_line);
            return code_errors;
        }
    }
    update_code_addresses( *tableC,  *code_size);
    update_lable_addresses(*tableL, *lable_size, IC);
    update_data_addresses(*tableD, *data_size, IC + BASE_ADDRESS);   
   return  check_error;
}

int check_lable(char* nameL, macro_table* tableM, int macro_size, func instructions[], char* directive[], char* regs[], extern_table* tableE, int extern_size, lable_table* tableL, int lable_size, char* file_name, int number_line)
{
    /* Check if label name is empty */
    if (!strcmp(nameL, "")) {
        error(e_no_name_label, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name exceeds maximum length */
    if (strlen(nameL) > LABEL_LENGTH)
    {
        error(e_more_31_characters, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name starts with an alphabetic character */
    if (!isalpha(nameL[0]))
    {
        error(e_not_alphabetic, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name contains only alphanumeric characters */
    if (!isAlphaNumeric(nameL))
    {
        error(e_non_alphanumeric_characters, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name conflicts with a macro name */
    if (is_macro_name(nameL, tableM, macro_size))
    {
        error(e_lable_name_macro, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name conflicts with a directive name */
    if (is_directive_name(nameL, directive))
    {
        error(e_lable_name_directive, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name conflicts with an instruction name */
    if (is_instructions_name(nameL, instructions))
    {
        error(e_lable_name_instruction, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name conflicts with a register name */
    if (is_register_name(nameL, regs))
    {
        error(e_lable_name_register, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name already exists */
    if (is_lable_name(tableL, nameL, lable_size))
    {
        error(e_lable_name_exists, ".am", file_name, number_line);
        return code_errors;
    }

    /* Check if label name conflicts with an external label */
    if (is_extern_label(nameL, tableE, extern_size)) 
    {
        error(e_lable_name_extern, ".am", file_name, number_line);
        return code_errors;
    }

    /* If all checks pass, return no errors */
    return no_errors;
}

int isAlphaNumeric(const char* str)
{
    while (*str)/*Loop through each character in the string */
    {
        if (!isalnum((unsigned char)*str))/*Check if the current character is not alphanumeric */
        {
            return 0;
        }
        str++;
    }
    return 1;
}

int add_lable(lable_table** tableL, int* size, char* nameL, int addressL, int feature)
{
    lable_table* temp;
    temp = realloc(*tableL, (*size + 1) * sizeof(lable_table));
    if (temp == NULL)
    {
        error_memory(m_memory_failed);
        return memory_errors; /* Return an error value if the allocation failed*/
    }
    *tableL = temp;

    /* Allocate memory and store the data in the new structure*/
    (*tableL)[*size].lable_name = strdup1(nameL);
    if ((*tableL)[*size].lable_name == NULL)
    {
        return memory_errors; /*Return an error value if the allocation failed*/
    }
    (*tableL)[*size].lable_address = addressL;
    (*tableL)[*size].lable_feature = feature;
    (*size)++;        /* Update the size of the array*/
    return no_errors; /* Return a success value*/
}

int check_operand(char* word, int addressing_method, macro_table* tableM, int macro_size, func instructions[], char* directive[], char* regs[])
{
    if (addressing_method == _1)
        return can_be_lable_name(word, tableM, macro_size, instructions, directive, regs);
    else if (addressing_method == _1_2)
        return can_be_lable_name(word, tableM, macro_size, instructions, directive, regs) || is_pointer(word, regs);
    else if (addressing_method == _1_2_3)
        return can_be_lable_name(word, tableM, macro_size, instructions, directive, regs) || is_pointer(word, regs) || is_register_name(word, regs);
    else
        return can_be_lable_name(word, tableM, macro_size, instructions, directive, regs) || is_pointer(word, regs) || is_register_name(word, regs) || is_integer(word);
}

int check_instructions(int* index, char* word, func instructions[], char* line, char* file_name, int number_line, char* regs[], macro_table* tableM, int macro_size, char* directive[])
{
    *index = get_index(word, instructions);/* Get the index of the instruction from the word */
    if (instructions[*index].source_operand != nothing)/* Check if the source operand is required */
    {
        word = read_word(&line, ".am", file_name, number_line);
        if (word == NULL)
            return memory_errors;
        if (!strcmp(word, "")) {/* Check if the word is empty or invalid */
            error(e_missing_operand, ".am", file_name, number_line);
            return code_errors;
        }
        if (!strcmp(word, ",")) {
            error(e_unnecessary_comma, ".am", file_name, number_line);
            return code_errors;
        }
        if (!check_operand(word, instructions[*index].source_operand, tableM, macro_size, instructions, directive, regs))/* Validate the source operand */
        {
            error(e_invalid_source_operand, ".am", file_name, number_line);
            return code_errors;
        }
        word = read_word(&line, ".am", file_name, number_line);
        if (word == NULL)
            return memory_errors;
        if (!strcmp(word, "")) {/* Check if the word is empty or if it's not a comma */
            error(e_missing_operand, ".am", file_name, number_line);
            return code_errors;
        }
        if (strcmp(word, ","))
        {
            error(e_missing_a_comma, ".am", file_name, number_line);
            return code_errors;
        }
    }
    if (instructions[*index].destination_operand != nothing)/* Check if the destination operand is required */
    {
        word = read_word(&line, ".am", file_name, number_line);
        if (word == NULL)
            return memory_errors;
        if (!strcmp(word, "")) {
            error(e_missing_operand, ".am", file_name, number_line);
            return code_errors;
        }
        if (!strcmp( word, ","))
        {
            error(e_unnecessary_comma, ".am", file_name, number_line);
            return code_errors;
        }
        if (!check_operand(word, instructions[*index].destination_operand, tableM, macro_size, instructions, directive, regs))/* Validate the destination operand */
        {
            error(e_invalid_destination_operand, ".am", file_name, number_line);
            return code_errors;
        }
    }
    if (!check_garbage(&line))/* Check if there are additional unexpected characters after the instruction */
    {
        error(e_additional_characters_in_command, ".as", file_name, number_line);
        return code_errors;
    }
    return no_errors;
}

int check_data(char* word, char* line, char* file_name, int number_line, char* suffix)
{
    int line_status;
    line_status = variable;/* Initialize line status as expecting a variable */
    skip_whitespace(&line);/* Skip initial whitespace in the line */
    if (*line == '\0') {
        error(e_no_data, ".am", file_name, number_line);
        return code_errors;
    }
    while (*line != '\0')/* Process the line character by character */
    {

        word = read_word(&line, suffix, file_name, number_line);
        if (line_status == variable)/* If currently expecting a variable */
        {
            if (*word == ',')/* Check for unnecessary comma */
            {
                error(e_unnecessary_comma, ".am", file_name, number_line);
                return code_errors;
            }
            if (!is_valid_number(word))/* Validate the word as a number */
            {
                error(e_invalid_number, ".am", file_name, number_line);
                return code_errors;
            }
            line_status = comma;
        }else if (line_status == comma)/* If currently expecting a comma */
        {
            if (*word != ',')/* Check for missing comma */
            {
                error(e_missing_a_comma, ".am", file_name, number_line);
                return code_errors;
            }
            line_status = variable;
        }
        skip_whitespace(&line);/* Skip any whitespace after the word */
    }
    return no_errors;
}

int add_extern_label(extern_table** tableE, int* extern_size, char* name_extern,char *file_name,int number_line)
{
    int i;
    extern_table* temp;
   /* Check if the external label already exists in the table */
    for (i = 0; i < *extern_size; i++)
    {
        if (strcmp((*tableE)[i].extern_name, name_extern) == 0)
        {
            error(e_lable_name_extern, ".am", file_name, number_line);
            return code_errors;
        }
    }
  /* Reallocate memory to accommodate the new label */
    temp = realloc(*tableE, (*extern_size + 1) * sizeof(extern_table));
    if (temp == NULL)
    {
        error_memory(m_memory_failed);
        return memory_errors;
    }
    *tableE = temp;
    (*tableE)[*extern_size].extern_name = strdup1(name_extern);/* Allocate memory and store the external label name */
    if ((*tableE)[*extern_size].extern_name == NULL)
    {
        return memory_errors;
    }
/* Initialize the addresses and size for the new external label */
    (*tableE)[*extern_size].extern_addresses = NULL;
    (*tableE)[*extern_size].size_address = 0;
    (*extern_size)++;/* Increment the size of the external table */

    return no_errors; 
}

int add_entry_label(entry_table** tableEN, int* entry_size, char* name_entry,char * file_name,int number_line)
{
    int i;
    entry_table* temp;
  /* Check if the entry label already exists in the table */
    for (i = 0; i < *entry_size; i++)
    {
        if (strcmp((*tableEN)[i].entry_name, name_entry) == 0)
        {
            error(e_lable_name_entry, ".am",file_name, number_line);
            return code_errors;
        }
    }

 
    temp = realloc(*tableEN, (*entry_size + 1) * sizeof(entry_table));/* Reallocate memory to accommodate the new label */
    if (temp == NULL)
    {
        error_memory(m_memory_failed);
        return memory_errors;
    }
    *tableEN = temp;
    (*tableEN)[*entry_size].entry_name = strdup1(name_entry);/* Allocate memory and store the entry label name */
    if ((*tableEN)[*entry_size].entry_name == NULL)
    {
        return memory_errors;
    }
    (*tableEN)[*entry_size].number_line = number_line;/* Store the line number where the entry label is defined */
    (*entry_size)++;/* Increment the size of the entry table */

    return no_errors; 
}

void update_lable_addresses(lable_table* tableL, int table_size, int IC)
{
    int i;
/* Loop through each label in the table */
    for (i = 0; i < table_size; i++)
    {
        if (tableL[i].lable_feature == data)/* Check if the label's feature is marked as data */
        {
            tableL[i].lable_address += IC +  BASE_ADDRESS ;/* Adjust the label's address by adding the instruction count and base address */
        }
    }
}
void update_code_addresses(code_table* tableC, int code_size )
{
    int i, j;
/* Loop through each entry in the code table */
    for (i = 0; i < code_size; i++)
    {/* Loop through each coding word in the current entry */
        for (j = 0; j < tableC[i].amount_coding_words; j++)
            tableC[i].code_addresses[j] += BASE_ADDRESS;/* Adjust the address of the current coding word by adding the base address */
    }
}

void update_data_addresses(data_table* tableD, int table_size, int IC)
{
    int i, j;
/* Loop through each entry in the data table */
    for (i= 0; i < table_size; i++)
    {

        for (j = 0; j < tableD[i].amount_coding_data_words; j++)/* Loop through each data word in the current entry */
            tableD[i].data_address[j] += IC;/* Adjust the address of the current data word by adding the instruction count */
    }
}

int can_be_lable_name(char* nameL, macro_table* tableM, int macro_size, func instructions[], char* directive[], char* regs[])
{
    if (strlen(nameL) > LABEL_LENGTH)/* Check if the length of the name exceeds the maximum allowed length for labels */
        return 0;
    if (!isalpha(nameL[0]))/* Check if the first character of the name is alphabetic */
        return 0;
    if (!isAlphaNumeric(nameL))/* Check if the name contains only alphanumeric characters*/
        return 0;
    if (is_macro_name(nameL, tableM, macro_size)) /* Check if the name is already used as a macro name */
        return 0;
    if (is_directive_name(nameL, directive))/* Check if the name is a directive name */
        return 0;
    if (is_instructions_name(nameL, instructions))/* Check if the name is an instruction name */
        return 0;
    if (is_register_name(nameL, regs))/* Check if the name is a register name */
        return 0;
    return 1;
}
