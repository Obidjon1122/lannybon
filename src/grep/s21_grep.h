#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 4096

enum { SUCCESS = 0, NO_FILE = 1, NON_EXISTENT_FLAG = 2, MALLOC_ERROR = 3 };

// new
void grep(const int *const flags, const char *const filename, char *patterns[],
          const int file_count);
int parse_args(int *const flags, const char **const argv, char *filenames[],
               char *patterns[], const int argc);
bool parse_flags(int *const flags, const char *const str);
bool checking_flags(const char *const argv);
int get_patterns_from_file(const char *const filename, char *patterns[],
                           int *idx);
bool match_patterns(const int *const flags, const char *const str,
                    char *patterns[], char *matches[]);
void handler_header(const int *const flags, const char *const filename,
                    const int file_count);
void handler_number(const int *const flags, const int line_num);
void handler_header_cl(const int *const flags, const char *const filename,
                       const int file_count);
void handler_count(const int *const flags, const int matched_lines);
void handler_list_files(const int *const flags, const char *const filename,
                        const int matched_lines);
// mini-helper
int get_file_count(char *const filenames[]);
void reset_e_flags(int *const flags);

// err messages
void print_err(const int err_code);
void print_malloc_error();
void print_usage_error();
void print_no_file_error(const char *const filename);

#endif  // SRC_GREP_S21_GREP_H_
