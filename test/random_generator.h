// Simple utility class for random generation.

#include <cstdlib> // srand, rand
#include <utility>

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_RANDOM_GENERATOR_H
#define ADVANCED_PROGRAMMING_EXAM_2020_RANDOM_GENERATOR_H

/**Generates and returns a random integer between 0 and max_value_excluded-1.*/
inline int random_int(const int max_value_excluded) { return rand() % max_value_excluded; }

/**Generates and returns a random char between 'a' and 'z'.*/
inline char random_char() { return static_cast<char>(rand() % 26 + static_cast<int>('a')); }

/** Generates and returns an std::pair<int, char> with random generated
 * key and value.*/
inline std::pair<int, char> generate_pair_random() { return std::pair<int, char>(rand(), random_char()); }

#endif // ADVANCED_PROGRAMMING_EXAM_2020_RANDOM_GENERATOR_H