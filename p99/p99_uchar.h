/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2015 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
/**
 ** @file p99_uchar.h
 ** @brief Emulate the unicode support of uchar.h if that is not provided
 */

#ifndef     P99_uchar_H_
# define    P99_uchar_H_

# include "p99_compiler.h"
# include "p99_int.h"
# include <wchar.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

_Static_assert(sizeof(L"äß")==sizeof(U"äß"), "we need to wchar_t to represend unicode for this to work");

union p00_mbstate {
  unsigned char p00_bytes[sizeof(mbstate_t)];
  mbstate_t p00_state;
};

P99_WEAK(mbrtoc16)
size_t mbrtoc16(char16_t * restrict _pc16,
                const char * restrict _s, size_t _n,
                mbstate_t * restrict _ps) {
  static union p00_mbstate p00_state = { .p00_bytes = { 0 }, };
  if (!_ps) _ps = &p00_state.p00_state;
  if (!_s) {
    _pc16 = 0;
    _s = "";
    _n = 1;
  }
  wchar_t p00_wc;
# if WCHAR_MAX > 0xFFFF
  mbstate_t p00_st = *_ps;
  size_t p00_ret = mbrtowc(&p00_wc, _s, _n, &p00_st);
  if (p00_wc > 0xFFFF) return -1;
  else *_ps = p00_st;
# else
  size_t p00_ret = mbrtowc(&p00_wc, _s, _n, _ps);
# endif
  if (_pc16 && (p00_ret <= _n)) *_pc16 = p00_wc;
  return p00_ret;
}

P99_WEAK(c16rtomb)
size_t c16rtomb(char * restrict _s, char16_t _c16,
                mbstate_t * restrict _ps) {
  static union p00_mbstate p00_state = { .p00_bytes = { 0 }, };
  if (!_ps) _ps = &p00_state.p00_state;
  return wcrtomb(_s, _c16, _ps);
}

P99_WEAK(mbrtoc32)
size_t mbrtoc32(char32_t * restrict _pc32,
                const char * restrict _s, size_t _n,
                mbstate_t * restrict _ps) {
  static union p00_mbstate p00_state = { .p00_bytes = { 0 }, };
  if (!_ps) _ps = &p00_state.p00_state;
  if (!_s) {
    _pc32 = 0;
    _s = "";
    _n = 1;
  }
  wchar_t p00_wc;
# if WCHAR_MAX > 0xFFFFFFFF
  mbstate_t p00_st = *_ps;
  size_t p00_ret = mbrtowc(&p00_wc, _s, _n, &p00_st);
  if (p00_wc > 0xFFFFFFFF) return -1;
  else *_ps = p00_st;
# else
  size_t p00_ret = mbrtowc(&p00_wc, _s, _n, _ps);
# endif
  if (_pc32 && (p00_ret <= _n)) *_pc32 = p00_wc;
  return p00_ret;
}

P99_WEAK(c32rtomb)
size_t c32rtomb(char * restrict _s, char32_t _c32,
                mbstate_t * restrict _ps) {
  if (_s && (_c32 > WCHAR_MAX)) {
    errno = EILSEQ;
    return -1;
  }
  static union p00_mbstate p00_state = { .p00_bytes = { 0 }, };
  if (!_ps) _ps = &p00_state.p00_state;
  return wcrtomb(_s, _c32, _ps);
}



#endif
