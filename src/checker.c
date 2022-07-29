#include "checker.h"
#include "io.h"

static inline int min(const int x, const int y) {
  if (x > y)
    return y;
  return x;
}

static inline int min3(int x, int y, int z) {
  return min(min(x, y), z);
}

static inline void lower_str(char *str) {
  for (; *str != '\0'; str++)
    *str = tolower(*str);
}

typedef struct _WordDistance {
  char *word;
  unsigned distance;
} *WordDistance;

WordDistance init_wd(char *str, unsigned dist) {
  WordDistance w = malloc(sizeof(struct _WordDistance));
  assert(w != NULL);
  w->word = copy_str(str);
  w->distance = dist;
  return w;
}

int compare_wd(WordDistance w1, WordDistance w2) {
  // Aquel con distancia mayor tiene menor prioridad
  return w2->distance - w1->distance;
}

void destroy_wd(WordDistance w) {
  free(w->word);
  free(w);
}

unsigned distance(char *str1, char *str2, unsigned len1, unsigned len2) {
  // Memoria O(m), m = len2
  // Tiempo O(n * m), n = len1, m = len2
  //if ( > MAX_SEARCH_DISTANCE || len2 - len1 > MAX_SEARCH_DISTANCE)
    // Retornamos un valor que nos asegure que no se sugiera
  //  return MAX_SEARCH_DISTANCE + 2;

  unsigned *distances = malloc(sizeof(unsigned) * (len2 + 1));
  unsigned *buffer = malloc(sizeof(unsigned) * (len2 + 1));
  
  for (unsigned i = 0; i <= len2; ++i)
    distances[i] = i;
  for (unsigned i = 0; i < len1; ++i) {
    buffer[0] = i + 1;
    for (unsigned j = 1; j < len2 + 1; ++j) {
      if (str1[i] == str2[j - 1])
        buffer[j] = distances[j - 1];
      else
        buffer[j] = min3(buffer[j - 1], distances[j], distances[j - 1]) + 1;
    }
    unsigned *tmp = distances;
    distances = buffer;
    buffer = tmp;
  }

  unsigned result = buffer[len2];
  free(distances);
  free(buffer);

  return result;
}

/*void make_suggests(WrongWord word, Trie dictionary) {
  GQueue q = distance1(word->word);
  while (word->num < NUM_SUGGESTS && !gqueue_empty(q)) {
    char *tmp = gqueue_start(q, id);
    if (trie_search(dictionary, tmp)) {
      word->suggests[word->num++] = tmp;
      gqueue_pop(q, null);
    } else
      gqueue_pop(q, free);
  }
  gqueue_free(q, free);
}*/

void make_suggests(WrongWord word, Trie dictionary) {
  BHeap distances_word = 
    print_distances(dictionary, word->word, strlen(word->word));
  while (word->num < NUM_SUGGESTS) {
    WordDistance suggest = bheap_remove_max(distances_word);
    if (suggest->distance <= MAX_SEARCH_DISTANCE)
      add_suggestion_wrongword(word, suggest->word);
    destroy_wd(suggest);
  }
  bheap_destroy(distances_word);
}

HashTable check_file(const char* input, Trie dictionary) {
  FILE *fp = fopen(input, "r");
  assert(fp != NULL);
  char buf[MAX_LEN_WORD + 1];
  
  // Estructura temporal usada para buscar en la tabla hash
  struct _WrongWord w1;
  w1.word = buf;
  
  HashTable incorrect_words = hashtable_init(500, (CopyFunction) id,
              (CompareFunction) cmp_wrongword, (DestroyFunction) free_wrongword,
              (HashFunction) hash_wrongword);
  
  unsigned line_n = 1;

  while (read_word(fp, buf, &line_n)) {
    lower_str(buf);

    if (trie_search(dictionary, buf))
      continue;

    WrongWord w2 = hashtable_search(incorrect_words, &w1);

    if (w2 == NULL) {
      w2 = init_wrongword(buf);
      make_suggests(w2, dictionary);
      hashtable_insert(incorrect_words, w2);
    }
    queue_push(w2->lines, line_n);
  }

  fclose(fp);
  return incorrect_words;
}

void __print_distances(Trie root, BHeap heap, int depth, char buf[MAX_LEN_WORD],
                      char *str, int len) {
  buf[depth] = root->c;
  if (root->end_of_word && (len - depth) <= MAX_SEARCH_DISTANCE) {
    buf[depth + 1] = '\0';
    bheap_insert(heap, init_wd(buf, distance(str, buf, len, depth + 1)));
  }

  if (depth - len > MAX_SEARCH_DISTANCE)
    return;

  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL)
      __print_distances(root->children[i], heap, depth + 1, buf, str, len);
  }
}

BHeap print_distances(Trie root, char *str, int len) {
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(10000, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS; ++i)
    if (root->children[i])
      __print_distances(root->children[i], heap, 0, buf, str, len);
  return heap;
}
