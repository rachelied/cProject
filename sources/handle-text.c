#include "handle-text.h"
#include "errors.h"
char* read_line(FILE* file, char* suffix, char* file_name, int number_line, int* check)
{
	char* line;
	int size;
	(*check) = no_errors;/*Initialize the error check variable to no errors*/
	line = (char*)malloc(LINE_CHARACTERS * sizeof(char));/*Allocate memory for the line buffer*/
	if (line == NULL) {
		*check = memory_errors;
		error_memory(m_memory_failed);
		return NULL;
	}
	if (fgets(line, LINE_CHARACTERS, file) == NULL) {
		if (feof(file)) {
			return NULL;
		}
		*check = memory_errors;
		error_memory(m_fgets_read);
		free(line);
		return NULL;
	}
	size = strlen(line);
	if (line[size - 1] != '\n') {
		*check = code_errors;
		error(e_more_80_characters, suffix, file_name, number_line);
		free(line);
		return NULL;
	}
	line[size] = '\0';
	return line;
}

void skip_whitespace(char** line) {
	if (*line == NULL)
		return;
	/* Loop through the string, advancing the pointer past any whitespace characters */
	while (**line == ' ' || **line == '\t' || **line == '\n' || **line == '\r') {
		(*line)++;
	}
}


char* read_word(char** line, char* suffix, char* file_name, int number_line) {
	int i;
	char* temp;
	int bufferSize;
	char* buffer;
	bufferSize = BUFFER_SIZE;/* Initial buffer allocation */
	buffer = (char*)malloc(bufferSize * sizeof(char));
	if (!buffer) {
		error_memory(m_memory_failed);
		return NULL;
	}
	i = 0;
	skip_whitespace(line);/* Skip leading whitespace */
	/* Check if the next character is a comma */
	if (**line == ',') {
		buffer[i] = **line;
		buffer[++i] = '\0';
		(*line)++;
		return buffer;
	}
	skip_whitespace(line);/* Skip whitespace again */
	/* Read characters until a comma, end of string, or whitespace is found */
	while ((**line) != ',' && (**line) != '\0' && !isspace(**line)) {
		buffer[i] = **line;
		(*line)++;
		i++;
		/* If the buffer is full, reallocate to increase its size*/
		if (i >= bufferSize) {
			bufferSize += BUFFER_SIZE;
			temp = realloc(buffer, bufferSize * sizeof(char));
			if (temp == NULL) {
				error_memory(m_memory_failed);
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
	}
	buffer[i] = '\0';
	return buffer;
}



int check_garbage(char** line) {
	skip_whitespace(line);
  /* Check if the line is empty or contains only whitespace characters */
	if (*line == NULL || **line == '\0') {
		return 1;
	}
	else {
		return 0;
	}
}

char* strdup1( char* s) {
	char* d;
	d= malloc(strlen(s) + 1);/* Allocate memory for the duplicate string, including space for the null terminator */
	if (d == NULL)
	{
		error_memory(m_memory_failed);
		return NULL; 
	}
	strcpy(d, s);/* Copy the contents of the original string into the newly allocated memory */
	return d;
}

int is_line_whitespace_only( char* line) {
	while (*line) {
	/* Check if the current character is not a whitespace character */
		if (!isspace(*line)) {
			return 0;
		}
		line++;
	}
	return 1;
}

void skip_whitespace_end(char** str) {
	int len, end;
	if (*str == NULL)
		return;

	 len = strlen(*str);/* Calculate the length of the string and determine the end index */
	 end = len - 1;
	/* Move backward through the string while the current character is whitespace */
	while (end >= 0 && isspace((unsigned char)(*str)[end])) {
		end--;
	}

	(*str)[end + 1] = '\0';/* Set the character after the last non-whitespace character to the null terminator */
}
