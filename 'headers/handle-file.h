#ifndef HANDALE_FILE_H
#define HANDALE_FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief Opens a file with a specified suffix and permission mode.
 * @param name_file The original file name.
 * @param suffix The suffix to append to the file name (e.g., ".as").
 * @param premission The permission mode for opening the file (e.g., "r" for read, "w" for write).
 * @return A pointer to the opened file stream if successful, or NULL if the file could not be opened.
 */
FILE* open_file(const char*, char*, char*);
#endif
