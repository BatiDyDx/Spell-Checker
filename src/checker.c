#include "checker.h"
#include "io.h"
#include "distances.h"
#include "../structures/hashtable.h"

void make_suggests(WrongWord wword, Trie dictionary) {
  int stop = 0;
  HashTable attempts = hashtable_init(100U, (CopyFunction) id,
                  (CompareFunction) compare_wd, (DestroyFunction) destroy_wd,
                  (HashFunction) hash_wd);
  WordDistance wd = init_wd(wword->word, 0);
  hashtable_insert(attempts, wd);
  stop = get_distance_1(wword, wd, dictionary, attempts);
  
  for (unsigned d = 1; !stop && d <= MAX_SEARCH_DISTANCE; ++d) {
    for (unsigned i = 0; i < hashtable_size(attempts); ++i) {
      WordDistance wd = hashtable_elems(attempts)[i];
      if (wd == NULL || wd->distance != d - 1 || strchr(wd->word, ' ') != NULL)
        continue;
      if (d == MAX_SEARCH_DISTANCE)
        // No se inserta la palabra ya que no interesa la siguiente distancia
        stop = get_distance_1(wword, wd, dictionary, NULL);
      else
        stop = get_distance_1(wword, wd, dictionary, attempts);
      
      if (stop)
        break;
    }
  }
  hashtable_free(attempts);
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
    gqueue_push(w2->lines, &line_n, (CopyFunction) copy_int);
  }

  fclose(fp);
  return incorrect_words;
}


// Calculo de distancias

/*(Trie root, BHeap heap, int depth,
              char buf[MAX_LEN_WORD + 1], char *str, int len, unsigned *dist1) {
  buf[depth] = root->c;
  if (root->end_of_word && (len - depth) <= MAX_SEARCH_DISTANCE) {
    unsigned distance = edit_distance(str, buf, len, depth + 1);
    buf[depth + 1] = '\0';
    bheap_insert(heap, init_wd(buf, distance));
    if (distance == 1)
      if (++(*dist1) == NUM_SUGGESTS)
        return ;
    }

  if (depth - len > MAX_SEARCH_DISTANCE)
    return ;

  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL) (root->children[i], 
                        heap, depth + 1, buf, str, len, dist1);
    if (*dist1 == NUM_SUGGESTS)
      return ;
    }
  }
  return ;
}(Trie root, char *str) {
  unsigned dist1 = 0; // Keeps track of distance 1 words found
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(100, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS && dist1 < NUM_SUGGESTS; ++i)
    if (root->children[i](root->children[i], heap, 0, buf, str, strlen(str), &dist1);
  return heap;
}
*/