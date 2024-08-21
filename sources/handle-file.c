#include "handle-file.h"
#include "errors.h"
FILE* open_file(const char* name_file, char* suffix, char* premission) {
    /* Allocate memory for name_file_with_suffix */
    FILE* file;
    char* name_file_with_suffix;
    name_file_with_suffix = (char*)malloc((strlen(name_file) + strlen(suffix) + 1) * sizeof(char));
    if (name_file_with_suffix == NULL) {
        error_memory(m_memory_failed);
        return NULL;
    }

    /* Copy the original file name and append the suffix ".as" */
    strcpy(name_file_with_suffix, name_file);
    strcat(name_file_with_suffix, suffix);


    /* Open the file with the new suffix */
    file = fopen(name_file_with_suffix, premission); /* Change mode as needed (read/write) */
    if (file == NULL) {
        error_memory(m_open_file);
        free(name_file_with_suffix);
        return NULL;
    }

    /* Free the allocated memory for name_file_with_suffix */
    return file;
}
