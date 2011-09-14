

#include "orwl_once.h"
#include "p99_defarg.h"

size_t orwl_keys_total(void);

/** @brief Declare the series of ORWL keys that is used in this executable
 **
 ** This must be used at most once in some header file. If #ORWL_KEYS
 ** is used there must a corresponding #ORWL_KEYS_DEFINE in exactly
 ** one .c file such that the necessary are implemented.
 ** @see ORWL_KEY_SCALE to have several slots reserved per key
 ** @see ORWL_KEY to obtain the key for a particular name and position
 **/
#define ORWL_KEYS(...)                                          \
DECLARE_ONCE(o_rwl_keys);                                       \
enum { O_RWL_KEYS_NB = P99_NARG(__VA_ARGS__), };                \
extern size_t o_rwl_key_offset[O_RWL_KEYS_NB + 1];              \
extern size_t o_rwl_key_nb[O_RWL_KEYS_NB];                      \
typedef enum o_rwl_key_enum { __VA_ARGS__ } o_rwl_key_enum

/** @brief Define the symbols that are necessary for #ORWL_KEYS
 **/
#define ORWL_KEYS_DEFINE()                                              \
DEFINE_ONCE(o_rwl_keys) {                                               \
  o_rwl_key_offset[0] = 0;                                              \
  for (size_t i = 0; i < O_RWL_KEYS_NB; ++i) {                          \
    if (!o_rwl_key_nb[i]) o_rwl_key_nb[i] = 1;                          \
    o_rwl_key_offset[i + 1] = o_rwl_key_offset[i] + o_rwl_key_nb[i];    \
  }                                                                     \
}                                                                       \
size_t orwl_keys_total(void) {                                          \
  INIT_ONCE(o_rwl_keys);                                                \
  return o_rwl_key_offset[O_RWL_KEYS_NB];                               \
}                                                                       \
size_t o_rwl_key_offset[O_RWL_KEYS_NB + 1];                             \
size_t o_rwl_key_nb[O_RWL_KEYS_NB]


/** @brief reserve @a SIZE slots for key @a NAME
 **/
#define ORWL_KEY_SCALE(NAME, SIZE) (o_rwl_key_nb[NAME] = (SIZE))

#define O_RWL_KEY2(NAME, I) (o_rwl_key_offset[NAME] + (I))
#define O_RWL_KEY1(NAME) (o_rwl_key_offset[NAME])

/** @brief obtain the key for name and position
 **
 ** position defaults to 0 if it is omitted 
 **/
#define ORWL_KEY(...) P99_IF_LT(P99_NARG(__VA_ARGS__), 2)(O_RWL_KEY1(__VA_ARGS__))(O_RWL_KEY2(__VA_ARGS__))
