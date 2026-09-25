#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <sys/stat.h>

typedef int (*comporator_t)(const void*, const void*);

struct line {
	char* line;
	size_t length;
};

void buble_sort(void* arr, size_t n, size_t el_size, comporator_t comporator);
void print_lines(struct line* lines, size_t count, FILE* buffer);
void swap(void* a, void* b, size_t el_size, char* buffer);
int compare_up_s(const void* a, const void* b);
int compare_down_s(const void* a, const void* b);
int my_strcmp_down(const char *s1, const char *s2, size_t len1, size_t len2);
struct line* read_file_from_buffer(const char* file_name, size_t* line_n);
int my_strcmp(const char *s1, const char *s2, size_t len1, size_t len2);
struct line* calloc_orig(size_t line_n, struct line* lines);
void write_to_file(const char* output_n, struct line* lines, size_t line_n, struct line* orig_lines);


int main() {
	const char* file = "in.txt";
	const char* output_n = "out.txt";
	size_t line_n = 0;
	
	struct line* lines = read_file_from_buffer(file, &line_n);
	struct line* orig_lines = calloc_orig(line_n, lines);
	
	write_to_file(output_n, lines, line_n, orig_lines);
	
	free(orig_lines[0].line);
	free(lines);
	free(orig_lines);
	return 0;
}

void write_to_file(const char* output_n, struct line* lines, size_t line_n, struct line* orig_lines) {
	FILE* output = fopen(output_n, "w");
	assert(output != NULL);
	
	buble_sort(lines, line_n, sizeof(struct line), compare_up_s);
	fprintf(output, "==================AFTER_UP_BUBBLE_SORT============\n\n");
	print_lines(lines, line_n, output);
	
	qsort(lines, line_n, sizeof(struct line), compare_down_s);
	fprintf(output, "======================AFTER_DOWN_QSORT============\n\n");
	print_lines(lines, line_n, output);
	
	fprintf(output, "======================ORIG========================\n\n");
	print_lines(orig_lines, line_n, output);
	
	fclose(output);
}

struct line* calloc_orig(size_t line_n, struct line* lines) {
	assert(lines != NULL);
	
	struct line* orig_lines = (struct line*)calloc(line_n, sizeof(struct line));
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
	assert(a != NULL); assert(b != NULL); assert(buffer != NULL);
	memcpy(buffer, a, el_size);
	memcpy(a, b, el_size);
	memcpy(b, buffer, el_size);
}

int compare_up_s(const void* a, const void* b) {
	assert(a != NULL); assert(b != NULL);
	const struct line* la = (const struct line*)a;
	const struct line* lb = (const struct line*)b;
	return my_strcmp(la->line, lb->line, la->length, lb->length);
}

int compare_down_s(const void* a, const void* b) {
	assert(a != NULL); assert(b != NULL);
	const struct line* la = (const struct line*)a;
	const struct line* lb = (const struct line*)b;
	return my_strcmp_down(la->line, lb->line, la->length, lb->length);
}

void print_lines(struct line* lines, size_t count, FILE* buffer) {
	assert(lines != NULL);
	for (size_t i = 0; i < count; i++) {
		fprintf(buffer, "<%s>\n", lines[i].line);
	}
}

int my_strcmp(const char *s1, const char *s2, size_t len1, size_t len2) {
	assert(s1 != NULL); assert(s2 != NULL);
	
	size_t i1 = 0, i2 = 0;
	
	while (i1 < len1 || i2 < len2) {
		while (i1 < len1 && !isalpha((unsigned char)s1[i1])) i1++;
		while (i2 < len2 && !isalpha((unsigned char)s2[i2])) i2++;
		
		int c1 = (i1 < len1) ? tolower((unsigned char)s1[i1]) : 0;
		int c2 = (i2 < len2) ? tolower((unsigned char)s2[i2]) : 0;
		
		if (c1 != c2) {
			return c1 - c2;
		}
		
		if (i1 < len1) i1++;
		if (i2 < len2) i2++;
	}
	return 0;
}

int my_strcmp_down(const char *s1, const char *s2, size_t len1, size_t len2) {
	assert(s1 != NULL); assert(s2 != NULL);
	
	size_t i1 = len1;
	size_t i2 = len2;
	
	while (i1 > 0 || i2 > 0) {
		while (i1 > 0 && !isalpha((unsigned char)s1[i1 - 1])) i1--;
		while (i2 > 0 && !isalpha((unsigned char)s2[i2 - 1])) i2--;
		
		int c1 = (i1 > 0) ? tolower((unsigned char)s1[i1 - 1]) : 0;
		int c2 = (i2 > 0) ? tolower((unsigned char)s2[i2 - 1]) : 0;
		
		if (c1 != c2) {
			return c1 - c2;
		}
		
		if (i1 > 0) i1--;
		if (i2 > 0) i2--;
	}
	return 0;
}

size_t count_lines(const char* text) {
	assert(text != NULL);
	size_t n = 0;
	for (int i = 0; *(text + i + 1); i++) {
		if (text[i] == '\n') {
			if (text[i + 1] == '\n') n--;
			n++;
		}
	}
	
	return n;
}

size_t calc_file_size(const char* file_name) {
	struct stat st{};
	stat(file_name, &st);
	size_t file_size = st.st_size;
	
	return file_size;
}

struct line* split_into_lines(const size_t* line_n, char* text) {
	struct line* lines = (struct line*)calloc(*line_n, sizeof(struct line));
	
	char* line_start = text;
	size_t n_curr_line = 0;
	size_t predictable_len = 0;
	
	while (true) {
		text++;
		if (*text == '\0') {
			break;
		}
		predictable_len++;
		
		if (*text == '\n') {
			*text = '\0';
			if (predictable_len > 1) {
				lines[n_curr_line].line = line_start;
				lines[n_curr_line].length = predictable_len - 1;
				n_curr_line++;
			}
			line_start = text + 1;
			predictable_len = 0;
		}
	}
	
	return lines;
}

struct line* read_file_from_buffer(const char* file_name, size_t* line_n) {
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
	struct line* lines = split_into_lines(line_n, text);
	
	int status = fclose(file);
	assert(status == 0);
	return lines;
}
