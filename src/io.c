#include "io.h"

void get_dict_path(char path[]) {
  printf("Inserte el path del diccionario: ");
  scanf("%s", path);
}

int read_word(FILE* fp, char buf[], unsigned *line_number) {
  int c, i = 0, stop = 0;
  // Se avanza hasta la proxima palabra
  do {
    c = fgetc(fp);
    if (c == '\n')
      ++(*line_number);
  } while (c != EOF && !isalpha(c));

  if (c == EOF)
    return 0;

  ungetc(c, fp);
  
  // Se llena el buffer hasta completar la palabra o finalizar el archivo
  while (i < MAX_LEN_WORD && !stop) {
    c = getc(fp);
    if (c == EOF) {
      buf[i] = '\0';
      return 1;
    } else if (isalpha(c))
      buf[i++] = c;
    else
      stop = 1;
  }
  if (c == '\n')
    ++(*line_number);

  buf[i] = '\0';

  return 1;
}

int read_number(FILE *fp) {
  const int num_digits = 2; // Numero de digitos de NUM_SUGGESTS mas uno
  int c, i = 0, stop = 0;
  char num_buf[num_digits];
  do
    c = fgetc(fp);
  while (!isdigit(c));
  
  num_buf[i++] = c;

  while (i < num_digits - 1 && !stop) {
    c = getc(fp);
    if (isdigit(c))
      num_buf[i++] = c;
    else
      stop = 1;
  }

  num_buf[i] = '\0';
  return atoi(num_buf);
}

void read_suggestion(FILE *fp, char buf[]) {
  int c, i = 0, stop = 0;
  // Se avanza hasta la proxima palabra
  do
    c = fgetc(fp);
  while (!isalpha(c));

  buf[i++] = c;

  while (i < MAX_LEN_WORD && !stop) {
    c = getc(fp);
    if (isalpha(c) || c == ' ')
      buf[i++] = c;
    else
      stop = 1;
  }
  buf[i] = '\0';
}

void write_correction(WrongWord w, FILE *fp) {
  fputs("Lineas ", fp);
  while (!gqueue_empty(w->lines)) {
    fprintf(fp, "%d, ", *((int*) gqueue_start(w->lines, id)));
    gqueue_pop(w->lines, free);
  }
  fprintf(fp, "\"%s\" no esta en el diccionario\n", w->word);
  if (w->num == 0)
    fputs("No se han encontrado sugerencias para la palabra", fp);
  else {
    fputs("Quizas quiso decir:", fp);
    for (int i = 0; i < w->num; ++i)
      fprintf(fp, " %s,", w->suggests[i]);
  }
  fputc('\n', fp);
  fputc('\n', fp);
}

void write_corrections(const char *path, HashTable corrections) {
  FILE *fp = fopen(path, "w");
  assert(fp != NULL);
  hashtable_iterate(corrections, (VisitFunctionExtra) write_correction, fp);
  fclose(fp);
}

void add_to_cachefile(WrongWord w, FILE *fp) {
  if (w->from_cache)
    return ;
  fprintf(fp, "%s, %u", w->word, w->num);
  for (int i = 0; i < w->num; i++) {
    fprintf(fp, ", %s", w->suggests[i]);
  }
  fputc('\n', fp);
}

void update_cachefile(const char *path, HashTable corrections) {
  FILE *fp = fopen(path, "a");
  assert(fp != NULL);
  hashtable_iterate(corrections, (VisitFunctionExtra) add_to_cachefile, fp);
  fclose(fp);
}

void usage() {
  puts("Uso del programa:");
  puts("./main <archivoEntrada> <archivoSalida> <archivoIntermedio>");
  exit(EXIT_FAILURE);
}

