#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <sys/stat.h>


typedef int (*comporator_t)(const void*, const void*);


struct line {
	char* line;
	size_t lenght;
};


void buble_sort(void* arr, size_t n, size_t el_size, comporator_t comporator);
void print_lines(char** lines, size_t count, FILE* buffer);
void swap(void* a, void* b, size_t el_size, char* buffer);
int compare_up_s(const void* a, const void* b);
int compare_down_s(const void* a, const void* b);
int my_strcmp_down(const char *s1, const char *s2);
char** read_file_from_buffer(const char* file_name, size_t* line_n);
int my_strcmp(const char *s1, const char *s2);
char** calloc_orig(size_t line_n, char** lines);
void write_to_file(const char* output_n, char** lines, size_t line_n, char** orig_lines);
void free_lines(char** lines, size_t count);


int main() {
	const char* file = "in.txt";
	const char* output_n = "trash.txt";
	size_t line_n = 0;
	
	char** lines = read_file_from_buffer(file, &line_n);
	char** orig_lines = calloc_orig(line_n, lines);
	
	write_to_file(output_n, lines, line_n, orig_lines);
	
	free_lines(lines, line_n);
	free(orig_lines);
	return 0;
}


void write_to_file(const char* output_n, char** lines, size_t line_n, char** orig_lines) {
	FILE* output = fopen(output_n, "w");
	assert(output != NULL);
	
	buble_sort(lines, line_n, sizeof(char*), compare_up_s);
	fprintf(output, "==================AFTER_UP_BUBBLE_SORT============\n\n");
	print_lines(lines, line_n, output);
	
	qsort(lines, line_n, sizeof(char*), compare_down_s);
	fprintf(output, "======================AFTER_DOWN_QSORT============\n\n");
	print_lines(lines, line_n, output);
	
	fprintf(output, "======================ORIG========================\n\n");
	print_lines(orig_lines, line_n, output);
	
	fclose(output);
}


char** calloc_orig(size_t line_n, char** lines) {
	assert(lines != NULL);
	
	char** orig_lines = (char**)calloc(line_n, sizeof(char*));
	if (orig_lines == NULL) {
		printf("U have a calculator((\n");
		exit(1);
	}
	
	for (size_t i = 0; i < line_n; i++) {
		orig_lines[i] = lines[i];
	}
	return orig_lines;
}


void buble_sort(void* arr, size_t n, size_t el_size, comporator_t comporator) {
	
	assert(arr != NULL);
	assert(comporator != NULL);
	
	char* const base = (char*)arr;
	char* buffer = (char*)calloc(el_size, sizeof(char));
	
	for (size_t i = 0; i < n - 1; i++) {
		for (size_t j = 0; j < n - i - 1; j++) {
			if (comporator(base + j * el_size, base + el_size * (j + 1)) > 0) {
				swap(base + j * el_size, base + (j + 1) * el_size, el_size, buffer);
			}
		}
	}
	
	free(buffer);
}


void swap(void* a, void* b, size_t el_size, char* buffer) {
	memcpy(buffer, a, el_size);
	memcpy(a, b, el_size);
	memcpy(b, buffer, el_size);
}


int compare_up_s(const void* a, const void* b) { // ------
	assert(a != NULL); assert(b != NULL);
	return my_strcmp(*(const char**)a, *(const char**)b);
}


int compare_down_s(const void* a, const void* b) {
	assert(a != NULL); assert(b != NULL);
	return my_strcmp_down(*(const char**)a, *(const char**)b);
}


void free_lines(char** lines, size_t count) {// ----------
	assert(lines != NULL);
	for (size_t i = 0; i < count; i++) {
		free(lines[i]);
	}
	free(lines);
}


void print_lines(char** lines, size_t count, FILE* buffer) {
	assert(lines != NULL);
	for (size_t i = 0; i < count; i++) {
		fprintf(buffer, "<%s>\n", lines[i]);
	}
}


int my_strcmp(const char *s1, const char *s2) {
	assert(s1 != NULL); assert(s2 != NULL);
	while (*s1 || *s2) {
		while (*s1 && !isalpha(*s1)) s1++;
		while (*s2 && !isalpha(*s2)) s2++;
		
		if (tolower(*s1) != tolower(*s2)) {
			return tolower(*s1) - tolower(*s2);
		}
		
		if (*s1) s1++;
		if (*s2) s2++;
	}
	return 0;
}


int my_strcmp_down(const char *s1, const char *s2) {
	
	assert(s1 != NULL); assert(s2 != NULL);
	
	const char *p1 = s1 + strlen(s1);
	const char *p2 = s2 + strlen(s2);
	
	while (p1 > s1 || p2 > s2) {
		while (p1 > s1 && !isalpha(*(p1 - 1))) p1--;
		while (p2 > s2 && !isalpha(*(p2 - 1))) p2--;
		
		int c1 = (p1 > s1) ? tolower(*(p1 - 1)) : 0;
		int c2 = (p2 > s2) ? tolower(*(p2 - 1)) : 0;
		
		if (c1 != c2) {
			return c1 - c2;
		}
		
		if (p1 > s1) p1--;
		if (p2 > s2) p2--;
	}
	return 0;
}


size_t count_lines(const char* text) {
	assert(text != NULL);
	size_t n = 0;
	for (int i = 0; *(text + i + 1); i++) {
		if (text[i] == '\n') {
			if (text[i + 1] == '\n') n --;
			n ++;
		}
	}
	
	return n;
}

size_t calc_file_size (const char* file_name) {
	struct stat st{};
	stat(file_name, &st);
	size_t file_size = st.st_size;
	
	return file_size;
}

char** split_into_lines(const size_t* line_n, char* text) {
	char** lines = (char**)calloc(*line_n, sizeof(char*));
	
	char* line = text;
	size_t n_curr_line = 0;
	size_t predictable_len = 0;
	
	while (true) {
		text ++;
		if (*text == '\0') {
			break;
		}
		predictable_len ++;
		
		if (*text == '\n') {
			*text = '\0';
			if (predictable_len > 1) {
				lines[n_curr_line ++] = line;
			}
			line = text + 1;
			predictable_len = 0;
		}
	}
	
	return lines;
}

char** read_file_from_buffer(const char* file_name, size_t* line_n) {
	assert(file_name != NULL);
	FILE* file = fopen(file_name, "r");
	size_t file_size = calc_file_size(file_name);
	
	if (!file) {
		printf("Cannot open file: %s\n", file_name);
		return NULL;
	}
	
	char* text = (char*)calloc(file_size + 1, sizeof(char));
	
	
	if (!text) {
		printf("Memory error\n");
		fclose(file);
		return NULL;
	}

	
	for (size_t i = 0; i < file_size; i++) {
		text[i] = (char)getc(file);
	}
	
	text[file_size] = '\0';

	*line_n = count_lines(text);
	char** lines = split_into_lines(line_n, text);
	
	int status = fclose(file);
	assert(status == 0);
	return lines;
}
