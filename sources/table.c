#include "Pre-proccesor.h"
#include "handle-file.h"
#include "handle-text.h"
int is_macro_name(char* str, macro_table* tableM, int macro_size)
{
    int i;
    for (i = 0; i < macro_size; i++)/* Iterate through each macro in the macro table */
        if (strcmp(tableM[i].macr_name, str) == 0)/* Compare the given string with the macro name in the table */
            return 1;
    return 0;
}

int is_directive_name(char* str, char* directive[])
{
    int i;
    for (i = 0; i < DIRECTIVES_AMOUNT; i++)/* Iterate through the array of directive names */
        if (strcmp(directive[i], str) == 0)/* Compare the current directive name with the given string */
            return 1;
    return 0;
}

int is_instructions_name(char* str, func instructions[])
{
    int i;
    for (i = 0; i < INSTRUCTIONS_AMOUNT; i++)/* Iterate through the array of instructions */
        if (strcmp(instructions[i].func_name, str) == 0)/* Compare the instruction name with the given string */
            return 1;
    return 0;
}

int is_register_name(char* str, char* regs[])
{
    int i;
    for (i = 0; i < REGISTERS_AMOUNT; i++)/* Iterate through the array of register names */
        if (strcmp(regs[i], str) == 0)/* Compare the register name with the given string */
            return 1;
    return 0;
}

int get_index(char* str, func instructions[])
{
    int i;
    for (i = 0; i < INSTRUCTIONS_AMOUNT; i++) /* Iterate through the array of instructions */
        if (strcmp(instructions[i].func_name, str) == 0)/* Compare the function name with the given string */
        {
            return i;
        }
    return -1; 
}

int is_integer(const char* str)
{
    if (*str == '\0') /* Check if the string is empty */
        return 0;
    if (str[0] != '#') /* Check if the first character is '#' */
        return 0;
    str++;/* Move past the '#' character */
    if (!is_valid_number(str))/* Check if the remaining string is a valid number */
        return 0;
    if (!IS_IN_12_BIT_RANGE(atoi(str)))/* Check if the number is within the 12-bit range */
        return 0;
    return 1;
}

int is_lable_name(lable_table* tableL, char* str, int size)
{
    int i;
    for (i = 0; i < size; i++)/* Iterate through each label in the table */
        if (strcmp(str, tableL[i].lable_name) == 0)/* Check if the given string matches the current label name */
            return 1;
    return 0;
}

int is_pointer(char* str, char* regs[])
{
    if (str == NULL || str[0] != '*') /* Check if the input string is null or does not start with '*' */
        return 0;
    str++;/* Skip the '*' character */
    if (!is_register_name(str, regs))/* Check if the remaining part of the string is a valid register name */
        return 0;
    return 1;
}

int is_string(char* str)
{
    char* ptr;
    char* end;
    if (str == NULL)
    {
        return 0;
    }

    while (isspace(*str))/*Skip leading whitespace*/
    {
        str++;
    }

    if (*str != '"')/*Check for opening quote*/
    {
        return 0;
    }
    /*Find the last non-whitespace character*/
     end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
    {
        end--;
    }

    if (*end != '"' || end == str)  /*Check for closing quote*/
    {
        return 0;
    }

    for ( ptr = str + 1; ptr < end; ptr++)/*Check for printable characters between quotes*/
    {
        if (!isprint(*ptr))
        {
            return 0;
        }
    }

    return 1;
}

int is_valid_number(const char* str)
{
    int i ;
    i = 0;


    if (str == NULL || strlen(str) == 0)
    {
        return 0;
    }

   
    if (str[i] == '-' || str[i] == '+')/* Skip optional leading '+' or '-' signs */
    {
        i++;
    }


    if (str[i] == '\0')/* Check if the string contains only a sign */
    {
        return 0;
    }


    for (; i < strlen(str); i++)/* Verify that all remaining characters are digits */
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
    if(!IS_IN_15_BIT_RANGE(atoi(str)))/* Check if the integer value falls within the 15-bit range */
        return 0;
    return 1;
}
int is_extern_label(char* str, extern_table* tableE, int extern_size)
{
    int i;
    for (i = 0; i < extern_size; i++) /* Iterate through the extern table */
    {
        if (strcmp(tableE[i].extern_name, str) == 0)/* Compare the provided string with each extern label in the table */
            return 1;
    }
    return 0;
}

int is_entry_label(entry_table* tableEN, int entry_size, char* str)
{
    int i;
    for (i = 0; i < entry_size; i++)
    {
        if (strcmp(tableEN[i].entry_name, str) == 0)/* Compare the provided string with each entry label in the table */
            return 1;
    }
    return 0;
}
void free_all_tables(macro_table* tableM, int macro_size, lable_table* tableL, int lable_size, data_table* tableD, int data_size, extern_table* tableEX, int extern_size, entry_table* tableEN, int entry_size, code_table* tableC, int code_size)
{
    int i,j;
   
    for (i = 0; i < macro_size; i++) {
        free(tableM[i].macr_name);
        free(tableM[i].macr_data);
    }
    free(tableM);


    for (i = 0; i < lable_size; i++) {
        free(tableL[i].lable_name);
    }
    free(tableL);

    for (i = 0; i < extern_size; i++) {
        free(tableEX[i].extern_name);
        if (tableEX[i].size_address > 0) {
            free(tableEX[i].extern_addresses);
        }     
    }
    free(tableEX);

    for (i = 0; i < entry_size; i++) {
        free(tableEN[i].entry_name);
    }
    free(tableEN);

    for (i = 0; i < data_size; i++) {
        free(tableD[i].data_address);
        free(tableD[i].data_source_code);
        for (j = 0; j < tableD[i].amount_coding_data_words; j++) {
            free(tableD[i].data_binary_code[j]);
        }
        free(tableD[i].data_binary_code);
    }
    free(tableD);


for (i = 0; i < code_size; i++) {
    for (j = 0; j < tableC[i].amount_coding_words; j++) {
        if (tableC[i].binary_code[j] != NULL) {
            free(tableC[i].binary_code[j]);
            tableC[i].binary_code[j] = NULL;
        }
    }
    free(tableC[i].binary_code);
    free(tableC[i].code_addresses);
    free(tableC[i].code_source_code);
}
free(tableC);

}

