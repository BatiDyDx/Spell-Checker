#include "checker.h"
#include "io.h"
#include "distances.h"
#include "../structures/hashtable.h"

void make_suggests_bf(WrongWord wword, Trie dictionary) {
  int stop = 0;
  HashTable attempts[MAX_SEARCH_DISTANCE - 1];
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; ++i) {
    attempts[i] = hashtable_init(100U, (CopyFunction) copy_str,
      (CompareFunction) strcmp, (DestroyFunction) free, (HashFunction) KRHash);
    assert(attempts[i - 1] != NULL);
  }

  stop = get_distance_1(wword, wword->word, dictionary, attempts[0]);// , 0, NULL);
  for (unsigned d = 1; !stop && d < MAX_SEARCH_DISTANCE; ++d) {
    for (unsigned i = 0; i < hashtable_size(attempts[d - 1]); ++i) {
      if (hashtable_elems(attempts[d - 1])[i] == NULL)
        continue;
      char* suggest = hashtable_elems(attempts[d - 1])[i];
      if (strchr(suggest, ' ') != NULL)
        continue;
      if (d == MAX_SEARCH_DISTANCE - 1)
         // No se inserta la palabra ya que no interesa la siguiente distancia
        stop = get_distance_1(wword, suggest, dictionary, NULL);//, 0, NULL);
      else
        stop = get_distance_1(wword, suggest, dictionary, attempts[d]); //, d, attempts);
      if (stop)
        break;
    }
  }
  for (int i = 0; i < MAX_SEARCH_DISTANCE - 1; ++i)
    hashtable_free(attempts[i]);
}


/*
void make_suggests_ed(WrongWord wword, Trie dictionary) {
  int stop;
  BHeap distances_word = calculate_distances(dictionary, wword->word);
  stop = wword->num == NUM_SUGGESTS;
  for (int d = 1; d <= MAX_SEARCH_DISTANCE && !stop; ++d) {
    WordDistance top = bheap_get_max(distances_word);
    while (top != NULL && top->distance != d && !stop) {
      stop = add_suggestion_wrongword(wword, top->word);
      bheap_remove_max(distances_word, heap);
        
    }
  }
  add_split_suggests(wword, distances_word, dictionary);
  while (wword->num < NUM_SUGGESTS) {

    WordDistance suggest = bheap_remove_max(distances_word);
    if (suggest == NULL)
      break;
    if (suggest->distance <= MAX_SEARCH_DISTANCE)
      add_suggestion_wrongword(wword, suggest->word);
    destroy_wd(suggest);
  }
  //bheap_destroy(distances_word);
}
}*/

void make_suggests(WrongWord wword, Trie dictionary) {
  if (strlen(wword->word) < 12)
    make_suggests_bf(wword, dictionary);
  else
    make_suggests_ed(wword, dictionary);
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

void __calculate_distances(Trie root, BHeap heap, int depth,
              char buf[MAX_LEN_WORD + 1], char *str, int len, unsigned *dist1) {
  buf[depth] = root->c;
  if (root->end_of_word && (len - depth) <= MAX_SEARCH_DISTANCE) {
    unsigned distance = edit_distance(str, buf, len, depth + 1);
    if (distance <= MAX_SEARCH_DISTANCE) {
      buf[depth + 1] = '\0';
      bheap_insert(heap, init_wd(buf, distance));
      if (distance == 1)
        if (++(*dist1) == NUM_SUGGESTS)
          return ;
    }
  }

  if (depth - len > MAX_SEARCH_DISTANCE)
    return ;

  for (int i = 0; i < NCHARS; ++i) {
    if (root->children[i] != NULL) 
      __calculate_distances(root->children[i], heap, 
                            depth + 1, buf, str, len, dist1);
    if (*dist1 == NUM_SUGGESTS)
      return ;
  }
  return ;
}

BHeap calculate_distances(Trie root, char *str) {
  unsigned dist1 = 0; // Keeps track of distance 1 words found
  char buf[MAX_LEN_WORD + 1];
  BHeap heap = bheap_init(1000U, (CompareFunction) compare_wd, id,
                          (DestroyFunction) destroy_wd);
  // Check suggestions have the right priority in the heap
  for (int i = 0; i < NCHARS && dist1 < NUM_SUGGESTS; ++i)
    if (root->children[i])
      __calculate_distances(root->children[i], heap, 0, buf, str,
                                strlen(str), &dist1);
  return heap;
}
