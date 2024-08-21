#include "Pre-proccesor.h"
#include "Parser-step1.h"
#include "Parser-step2.h"
#include "errors.h"
/* Define the opcodes */
func instructions[] = {
    {"mov", _0_1_2_3, _1_2_3, "0000"},
    {"cmp", _0_1_2_3, _0_1_2_3,"0001"},
    {"add", _0_1_2_3, _1_2_3, "0010"},
    {"sub", _0_1_2_3, _1_2_3, "0011"},
    {"lea", _1, _1_2_3,"0100"},
    {"clr", nothing, _1_2_3, "0101"},
    {"not", nothing, _1_2_3, "0110"},
    {"inc", nothing, _1_2_3, "0111"},
    {"dec", nothing, _1_2_3, "1000"},
    {"jmp", nothing, _1_2, "1001"},
    {"bne", nothing, _1_2, "1010"},
    {"red", nothing, _1_2_3,"1011"},
    {"prn", nothing, _0_1_2_3, "1100"},
    {"jsr", nothing, _1_2,"1101"},
    {"rts", nothing, nothing,"1110"},
    {"stop", nothing, nothing, "1111"} };
/* Define the registers */
char* regs[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };

/* Define the instructions */
char* directive[] = { ".data", ".string", ".extern", ".entry" };


int main(int argc, char* argv[])
{
    int macro_size, lable_size, data_size, extern_size, entry_size, code_size, result_pre_proc,result_first_pass, result_second_pass;
    macro_table* tableM;/*A table of macros containing original names and the data*/
    lable_table* tableL;/*A label table containing the name, address, and feature for each label*/
    data_table* tableD;/* A table for managing data, including addresses, source code, and binary representations */
    extern_table* tableEX;/* A table for managing external references, including names and associated addresses */
    entry_table* tableEN;/* A table for entries, containing the entry name and the line number of its declaration */
    code_table* tableC;/* A table for code lines, storing the line number, source code, memory addresses, and binary representation */
    /* Loop through each file provided in the command-line arguments */
    while (--argc > 0) {
	/* Initialize table pointers and sizes */
        tableL = NULL;
        tableD = NULL;
        tableM = NULL;
        tableEX = NULL;
        tableEN = NULL;
        tableC = NULL;
        code_size = 0;
        macro_size = 0;
        lable_size = 0;
        data_size = 0;
        extern_size = 0;
        entry_size = 0;
        printf("file: %s\n", argv[argc]);
        printf("---------------Start pre-proc---------------\n");
        result_pre_proc = pre_processor(argv[argc], instructions, directive, &tableM, &macro_size, regs);/* Step 1: Pre-processing the file */
        if (result_pre_proc == no_errors)
        {
            printf("Pre-processing completed successfully.\n");
            printf("---------------Start proc---------------\n");
            result_first_pass=parser_step1(argv[argc], tableM, macro_size, instructions, directive, &tableL, &lable_size, regs, &tableD, &data_size, &tableEX, &extern_size, &tableEN, &entry_size, &tableC, &code_size); /* Step 2: First pass of processing */
            if (result_first_pass!= memory_errors) {
                result_second_pass = parser_step2(&tableC, code_size, tableL, lable_size, tableD, data_size, tableEX, extern_size, tableEN, entry_size, argv[argc], result_first_pass); /* Step 3: Second pass of processing */
                if (result_first_pass==no_errors&& result_second_pass == no_errors)
                    printf("processing completed successfully.\n");
            }
        }
	/* Free all allocated tables and their contents */
         free_all_tables(tableM, macro_size, tableL, lable_size, tableD, data_size, tableEX, extern_size, tableEN, entry_size, tableC, code_size);
    }
    printf("end\n");
    return 0;\
}
