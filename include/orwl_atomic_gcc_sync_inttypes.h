
/**
 ** @file
 ** @brief Implementation of the atomic operations for the different
 ** integer types.
 **
 ** These are not intended to be used directly but through a
 ** macro. The idea is that they operate on unsigned types, namely
 ** <code>uint8_t, uint16_t, uint32_t, uint64_t</code> but that their
 ** return type is the corresponding signed type. The promotion the
 ** unsigned to the signed type is done such that no integer exception
 ** might trigger. This is only possible because the signed exact
 ** width type are guaranteed to be two's complement without trap
 ** value.
 **
 ** @warning This file is included several times. Don't protect it by
 ** include guards.
 **/

#define ORWL_MANG(NAME) ORWL_MANG_(NAME, ORWL_SYNC_MAX)
#define ORWL_AT ORWL_AT_(ORWL_SYNC_MAX)
#define ORWL_RT ORWL_RT_(ORWL_SYNC_MAX)

inline
_Bool ORWL_MANG(atomic_compare_exchange_weak)(void volatile*object, void*restrict expected, ORWL_AT desired) {
  ORWL_AT val = __sync_val_compare_and_swap((ORWL_AT volatile*)object, *(ORWL_AT*restrict)expected, desired);
  _Bool ret = (val == *(ORWL_AT*restrict)expected);
  if (!ret) *(ORWL_AT*restrict)expected = val;
  return ret;
}

inline
void ORWL_MANG(atomic_store)(void volatile*object, ORWL_AT desired){
  for (ORWL_AT expected = desired;;) {
    ORWL_AT val = __sync_val_compare_and_swap((ORWL_AT volatile*)object, expected, desired);
    if (val == expected) break;
  }
}


p99_inline
ORWL_RT ORWL_MANG(atomic_promote)(ORWL_AT x) {
  register ORWL_AT const max_val = ((ORWL_AT)-1) >> 1;
  register ORWL_AT const max_val1 = max_val + 1;
  register ORWL_RT const min_val = -max_val - 1;
  if (x == max_val1) return min_val;
  else if (x > max_val1) return -(ORWL_RT)-x;
  else return x;
}

inline
ORWL_RT ORWL_MANG(atomic_fetch_add)(void volatile*object, ORWL_AT operand) {
  return ORWL_MANG(atomic_promote)(__sync_fetch_and_add((ORWL_AT volatile*)object, operand));
}

inline
ORWL_RT ORWL_MANG(atomic_fetch_sub)(void volatile*object, ORWL_AT operand) {
  return ORWL_MANG(atomic_promote)(__sync_fetch_and_sub((ORWL_AT volatile*)object, operand));
}

inline
ORWL_RT ORWL_MANG(atomic_fetch_or)(void volatile*object, ORWL_AT operand) {
  return ORWL_MANG(atomic_promote)(__sync_fetch_and_or((ORWL_AT volatile*)object, operand));
}

inline
ORWL_RT ORWL_MANG(atomic_fetch_xor)(void volatile*object, ORWL_AT operand) {
  return ORWL_MANG(atomic_promote)(__sync_fetch_and_xor((ORWL_AT volatile*)object, operand));
}

inline
ORWL_RT ORWL_MANG(atomic_fetch_and)(void volatile*object, ORWL_AT operand) {
  return ORWL_MANG(atomic_promote)(__sync_fetch_and_and((ORWL_AT volatile*)object, operand));
}

inline
ORWL_RT ORWL_MANG(atomic_load)(void volatile*object) {
  return ORWL_MANG(atomic_promote)(ORWL_MANG(atomic_fetch_xor)((ORWL_AT volatile*)object, 0));
}

#undef ORWL_RT
#undef ORWL_AT
#undef ORWL_MANG
