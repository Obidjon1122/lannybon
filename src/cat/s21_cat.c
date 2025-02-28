#define _POSIX_C_SOURCE 200809L
#include "s21_cat.h"

#include <math.h>

int main(const int argc, const char **const argv) {
  int ret = 0;
  if (check_args(argc, argv)) {
    flags f = {0};
    int is_set = parse_flag(&f, argv[1]);
    for (int i = 1 + is_set; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      int cnt = 1;
      if (!file) {
        printf("cat: %s: No such file or directory\n", argv[i]);
        continue;
      } else {
        print_file(f, file, &cnt);
        fclose(file);
      }
    }
  } else {
    printf("usage: cat [-benst] [file ...]\n");
    ret = 1;
  }

  return ret;
}

bool check_args(const int count, const char **const strs) {
  bool ret = false;
  if (count == 2 && strs[1][0] != '-') ret = true;
  if (count > 2 && check_flags(strs[1])) ret = true;

  return ret;
}

bool check_flags(const char *const flags) {
  bool ret = flags[0] == '-' ? false : true;
  if (flags[0] == '-') {
    for (int i = 0; strcmp("NULL", pos_fl[i]); i++) {
      if (!strcmp(pos_fl[i], flags)) {
        ret = true;
        break;
      }
    }
  }
  return ret;
}

bool parse_flag(flags *const f, const char *const str) {
  bool ret = *str == '-' ? true : false;
  if (!strcmp(str, pos_fl[b]) || !strcmp(str, pos_fl[gnu_b])) f->b = true;
  if (!strcmp(str, pos_fl[e])) f->e = true;
  if (!strcmp(str, pos_fl[gnu_e])) f->E = true;
  if (!strcmp(str, pos_fl[n]) || !strcmp(str, pos_fl[gnu_n])) f->n = true;
  if (!strcmp(str, pos_fl[s])) f->s = true;
  if (!strcmp(str, pos_fl[t])) f->t = true;
  if (!strcmp(str, pos_fl[v])) f->v = true;
  if (!strcmp(str, pos_fl[gnu_t])) f->T = true;
  return ret;
}

void print_file(flags f, FILE *file, int *const cnt) {
  char *line = NULL;
  size_t lencap = 0;
  ssize_t linelen;
  int prev = 0;
  while ((linelen = getline(&line, &lencap, file)) > 0)
    handler_string(f, line, linelen, cnt, &prev);

  // must free getline does malloc
  free(line);
}

void handler_string(flags f, const char *const str, const size_t len,
                    int *const cnt, int *const prev) {
  if (f.b)
    handler_bb(str, len, cnt);
  else if (f.s)
    handler_ss(str, len, prev);
  else if (f.n)
    handler_nn(str, len, cnt);
  else if (f.e)
    handler_ee(str, len);
  else if (f.E)
    handler_EE(str, len);
  else if (f.T)
    handler_TT(str, len);
  else if (f.t)
    handler_tt(str, len);
  else if (f.v)
    handler_vv(str, len);
  else
    fwrite(str, len, 1, stdout);
}
void handler_tt(const char *const str, const size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (str[i] == '\t')
      printf("^I");
    else
      printf("%s", symb[(int)str[i]]);
  }
}

void handler_vv(const char *const str, const size_t len) {
  for (size_t i = 0; i < len; i++) {
    printf("%s", symb[(int)str[i]]);
  }
}

void handler_ee(const char *const str, const size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (str[i] == '\n')
      printf("$\n");
    else if (str[i] == '\t')
      printf("\t");
    else
      printf("%s", symb[(int)str[i]]);
  }
}

void handler_TT(const char *const str, const size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (str[i] == '\t')
      printf("^I");
    else
      printf("%c", str[i]);
  }
}

void handler_EE(const char *const str, const size_t len) {
  if (str[len - 1] == '\n') {
    char *tmp = malloc(len + 1);
    strcpy(tmp, str);
    tmp[len - 1] = '$';
    tmp[len] = '\n';
    fwrite(tmp, len + 1, 1, stdout);
    free(tmp);
  } else {
    fwrite(str, len, 1, stdout);
  }
}

void num_to_str(int val, char *const num) {
  int idx;
  for (idx = SIZE - 2; val; val /= 10, idx--) num[idx] = val % 10 + '0';
  memmove(num, num + idx + 1, SIZE - idx);
}

void handler_bb(const char *const str, const size_t len, int *const cnt) {
  char num[SIZE] = {'\0'};
  num_to_str(*cnt, num);
  if (*str != '\n') {
    int sp = 6 - log10(*cnt) - 0.0005;
    while (sp--) putchar(' ');
    printf("%s\t", num);
    fwrite(str, len, 1, stdout);
    (*cnt)++;
  } else {
    fwrite(str, len, 1, stdout);
  }
}

void handler_nn(const char *const str, const size_t len, int *const cnt) {
  char num[SIZE] = {'\0'};
  num_to_str(*cnt, num);
  int sp = 6 - log10(*cnt) - 0.0005;
  while (sp--) putchar(' ');
  printf("%s\t", num);
  fwrite(str, len, 1, stdout);
  (*cnt)++;
}

void handler_ss(const char *const str, const size_t len, int *const prev) {
  if (*str == '\n') {
    (*prev)++;
    if (*prev == 1) fwrite(str, len, 1, stdout);
  } else {
    fwrite(str, len, 1, stdout);
    *prev = 0;
  }
}
