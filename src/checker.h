#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "../structures/hashtable.h"
#include "../structures/trie.h"
#include "dictionary.h"

/*
 * Algoritmo para obtener distancia de edicion de dos strings.
 * Nota: Para los casos evidentes de distancia mayor a 3, el algoritmo
 * se detiene antes de ejecutarse, devolviendo un entero grande

 * Tiempo: O(n * m) donde len(str1) = n y len(str2) = m
 * Memoria: O(n * m) donde len(str1) = n y len(str2) = m
*/
unsigned edit_distance(const char* str1, const char* str2);

/*
 * Analiza si una palabra esta en el diccionario, si no lo esta, busca en la
 * tabla para ver si la palabra ya fue previamente corregida, y sino busca
 * 5 sugerencias (en lo posible) agregandolas a la tabla
*/
void check_word(const char* str, HashTable corrected_words, Trie dict);


/*
 * Chequea palabra por palabra del archivo path si esta es valida tomando
 * como referencia el diccionario. Escribe un archivo de salida con las
 * sugerencias hechas para las palabras con errores de escritura
*/
HashTable check_file(const char* input, Trie dictionary);



#endif /* __CHECKER_H__ */