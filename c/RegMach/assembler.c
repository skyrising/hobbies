#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define DEBUG 1

#define I_NOP 0
#define I_LOAD 1
#define I_DLOAD 2
#define I_STORE 3
#define I_ADD 4
#define I_SUB 5
#define I_MULT 6
#define I_DIV 7
#define I_JUMP 8
#define I_JGE 9
#define I_JGT 10
#define I_JLE 11
#define I_JLT 12
#define I_JEQ 13
#define I_JNE 14
#define I_END 15

char * program_name = "as";
int linenum = 0;
char * filename;

void error_at_line(int status, int errnum, char * fname, unsigned int lineno, char * format, ...) {
  fprintf(stderr, "%s:%s:%d: ", program_name, fname, lineno);
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputc('\n', stderr);
  if(status) exit(status);
}

void strip_comments(char * line) {
  int i;
  for(i = 0; line[i] != 0; i++) {
    if(line[i] == '#' || line[i] == ';') {
      line[i] = 0;
      break;
    }
  }
}

int tolower(int c) {
  return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
}

int get_op(char * tok, int * op) {
  int i;
  for(i = 0; tok[i]; i++)
    tok[i] = tolower(tok[i]);
  if(strcmp(tok, "nop") == 0){*op = I_NOP; return 0;}
  if(strcmp(tok, "load") == 0){*op = I_LOAD; return 0;}
  if(strcmp(tok, "dload") == 0){*op = I_DLOAD; return 0;}
  if(strcmp(tok, "store") == 0){*op = I_STORE; return 0;}
  if(strcmp(tok, "add") == 0){*op = I_ADD; return 0;}
  if(strcmp(tok, "sub") == 0){*op = I_SUB; return 0;}
  if(strcmp(tok, "mult") == 0){*op = I_MULT; return 0;}
  if(strcmp(tok, "div") == 0){*op = I_DIV; return 0;}
  if(strcmp(tok, "jump") == 0){*op = I_JUMP; return 0;}
  if(strcmp(tok, "jge") == 0){*op = I_JGE; return 0;}
  if(strcmp(tok, "jgt") == 0){*op = I_JGT; return 0;}
  if(strcmp(tok, "jle") == 0){*op = I_JLE; return 0;}
  if(strcmp(tok, "jlt") == 0){*op = I_JLT; return 0;}
  if(strcmp(tok, "jeq") == 0){*op = I_JEQ; return 0;}
  if(strcmp(tok, "jne") == 0){*op = I_JNE; return 0;}
  if(strcmp(tok, "end") == 0){*op = I_END; return 0;}
  error_at_line(0, EINVAL, filename, linenum, "Invalid op-code %s", tok);
  return 1;
}

int parse_num(char * tok, uint32_t * num) {
  char * endptr;
  errno = 0;
  *num = strtol(tok, &endptr, 0);
  if(errno == ERANGE || errno != 0) {
    error_at_line(0, errno, filename, linenum, "Invalid number %s", tok);
    return 1;
  }
  if(endptr == tok || *endptr != 0) {
    error_at_line(0, EINVAL, filename, linenum, "Invalid number %s", tok);
    return 1;
  }
  return 0;
}

int compile(char * line, uint16_t * code) {
  strip_comments(line);
  char * tok;
  tok = strtok(line, " ");
  int ret = 0;
  int op = -1;
  uint32_t arg = -1;
  int i;
  for(i = 0; tok != NULL; i++) {
    printf("%s\n", tok);
    if(i == 0) {
       ret |= get_op(tok, &op);
    }else if(i == 1) {
       ret |= parse_num(tok, &arg);
    }else{
      return 0;
    }
    tok = strtok(NULL, " ");
  }
  code[0] = op;
  code[1] = (op == I_DLOAD || (op >= I_JUMP && op <= I_JNE) ? (arg >> 4)&0xF : 0) << 4;
  code[1] |= op == I_END || op == I_NOP ? 0 : arg&0xF;
  return ~ret;
}

int main(int argc, char** argv) {
  int m, n;
  program_name = argv[0];
  char * argn;
  char * output_filename = "a.out";
  char * input_filename = NULL;
  for(n = 1; n < argc; n++) {
    if(argv[n][0] == '-') {
      argn = argv[n];
      for(m = 1; m < strlen(argn); m++) {
        switch(argn[m]) {
          case 'o':
            if(n + 1 >= argc) {
              puts("Argument to -o is missing!\n");
              return 1;
            }
            output_filename = argv[++n];
            break; 
        }
      }
    } else {
      input_filename = argv[n];
      break;
    }
  }
  if(input_filename == NULL) {
    puts("No input file!\n");
    return 1;
  }
  FILE * fin;
  FILE * fout;
  filename = input_filename;
  fin = fopen(input_filename, "r");
  if(fin == NULL) {
    puts("Can't open input file.\n");
    return 1;
  }
  fout = fopen(output_filename, "w");
  if(fout == NULL) {
    puts("Can't open output file.\n");
    return 1;
  }
  char line[80];
  uint16_t code;
  while(fgets(line, 80, fin)!=NULL) {
    linenum++;
    if(compile(line, &code)) {
      fwrite(&code, 1, sizeof(code), fout);
    } else {
      //printf("Error in line %d: %s", ln, line);
      //break;
    }
  }
  fclose(fin);
  fclose(fout);
}
