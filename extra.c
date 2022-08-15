/*
void make_suggests_distances(WrongWord wword, Trie dictionary) {
  unsigned last_dist = 1;
  BHeap distances_word =(dictionary, wword->word);
  add_split_suggests(wword, distances_word, dictionary);

  while (wword->num < NUM_SUGGESTS) {
    WordDistance suggest = bheap_remove_max(distances_word);
    if (suggest == NULL)
      break;
    if (suggest->distance <= MAX_SEARCH_DISTANCE)
      add_suggestion_wrongword(wword, suggest->word);
    destroy_wd(suggest);
  }
  bheap_destroy(distances_word);
}

void make_suggests(WrongWord wword, Trie dictionary) {
  if (strlen(wword->word) > 12)
    make_suggests_distances(wword, dictionary);
  else
    make_suggests_bf(wword, dictionary);
}

GList __brute_force(char* word, unsigned distance, GList list, HashTable __attribute__((unused)) tried) {
  GList list2 = glist_init();
  
  return list2;
}

GList brute_force(char* word, unsigned distance, GList previous_distances, HashTable tried) {
  assert(distance > 0);
  return __brute_force(word, distance, previous_distances, tried, 1);
}
*/


/*
void insert(char *word, unsigned const len, GQueue q) {
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len + 1; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      buf[i] = c;
      gqueue_push(q, buf, (CopyFunction) copy_str);
    }
    buf[i] = buf[i + 1];
  }
  free(buf);
}

void replace(char *word, unsigned const len, GQueue q) {
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  char c;
  strcpy(buf, word);
  for (unsigned i = 0; i < len; ++i) {
    c = word[i];
    for (char x = 'a'; x < 'z'; ++x) {
      buf[i] = x < c ? x : x + 1;
      gqueue_push(q, buf, (CopyFunction) copy_str);
    }
    buf[i] = c;
  }
  free(buf);
}

void swap(char *word, unsigned const len, GQueue q) {
  char *buf = malloc(len + 1);
  assert(buf != NULL);
  strcpy(buf, word);
  for (unsigned i = 0; i < len - 1; ++i) {
    if (word[i] != word[i + 1]) {
      buf[i] = word[i + 1];
      buf[i + 1] = word[i];
      gqueue_push(q, buf, (CopyFunction) copy_str);
      buf[i] = word[i];
    }
  }
  free(buf);
}

void delete(char *word, unsigned const len, GQueue q) {
  char *buf = malloc(len);
  assert(buf != NULL);
  strcpy(buf, word + 1);
  gqueue_push(q, buf, (CopyFunction) copy_str);
  for (unsigned i = 0; i < len - 1; ++i) {
    buf[i] = word[i];
    gqueue_push(q, buf, (CopyFunction) copy_str);
  }
  free(buf);
}

void split(char *word, unsigned const len, GQueue q) {
  char *buf = malloc(len + 2);
  assert(buf != NULL);
  strcpy(buf + 1, word);
  for (unsigned i = 0; i < len - 1; ++i) {
    buf[i] = buf[i + 1];
    buf[i + 1] = ' ';
    gqueue_push(q, buf, (CopyFunction) copy_str);
  }
  free(buf);
}

GQueue distance1(char *word) {
  GQueue q = gqueue_init();
  unsigned len = strlen(word);
  insert(word, len, q);
  replace(word, len, q);
  swap(word, len, q);
  delete(word, len, q);
  split(word, len, q);
  return q;
}


GList brute_force(Trie dictionary, char* word, GList list,
                  Trie repeated_words) {
  if (max_distance == MAX_)
    return NULL;
  else if (max_distance == 1)
    return distance1(word);
}

*/