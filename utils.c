#include "utils.h"

void *id(void *p) { return p; }

void null(void *p) { (void) p; return; }

int min(const int x, const int y) {
  if (x > y)
    return y;
  return x;
}

int min4(int x, int y, int z, int w) {
  return min(min(x, y), min(z, w));
}

void lower_str(char *str) {
  for (; *str != '\0'; str++)
    *str = tolower(*str);
}

unsigned KRHash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}

char* copy_str(const char *str) {
  char *copy = malloc(strlen(str) + 1);
  assert(copy != NULL);
  strcpy(copy, str);
  return copy;
}
