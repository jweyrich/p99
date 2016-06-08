/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* (C) copyright  2016 Jens Gustedt, INRIA, France                            */
/*                                                                            */
/* This file is free software; it is part of the P99 project.                 */
/* You can redistribute it and/or modify it under the terms of the QPL as     */
/* given in the file LICENSE. It is distributed without any warranty;         */
/* without even the implied warranty of merchantability or fitness for a      */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_UF_H
#define P99_UF_H

#include "p99_compiler.h"
#include "p99_type.h"
#include "p99_c99.h"

/**
 ** @brief A simple Union-Find data structure.
 **
 ** Each position @c x in the table is either a link to another
 ** element @c y of the same set, or the number of elements in the set
 ** if @c x is the root of its set.
 **
 ** In the first case, @c y is just encoded as is, by the contents @c
 ** tab[x].  In the second case, the size is the negative of the
 ** encoded value that is @c -tab[x].
 **
 ** To decide if @x is an inner node or the root we just have to
 ** compare it against @c 0.
 **
 ** @warning This must be allocated only through ::p99_uf_alloc, and
 ** never on the stack.
 **
 ** @see p99_uf_alloc
 ** @see p99_uf_free
 ** @see p99_uf_find
 ** @see p99_uf_findCompress
 ** @see p99_uf_union
 ** @see p99_uf_flaten
 ** @see p99_uf_size
 **/
P99_DECLARE_STRUCT(p99_uf);

struct p99_uf {
  uint64_t size;
  int64_t tab[];
};

/**
 ** @brief Initialize the UF data structure to all singletons, that is
 ** everybody is root and the size of the set is @c 1.
 **/
p99_inline
p99_uf* p99_uf_init(p99_uf* uf, uint64_t size) {
  if (uf) {
    *uf = (p99_uf){ .size = size, };
    memset(uf->tab, UCHAR_MAX, sizeof(int64_t[size]));
  }
  return uf;
}

p99_inline
void p99_uf_destroy(p99_uf volatile* uf) {
  // empty
}

/**
 ** @brief Allocate an new UF data structure
 **
 ** Initially the UF is a family of singletons.  Elements in the set
 ** are numbered from @c 0 upto @c size-1.
 **/
p99_inline
p99_uf* p99_uf_alloc(uint64_t size) {
  return p99_uf_init(malloc(sizeof(int64_t[size])+offsetof(p99_uf, tab)), size);
}

/**
 ** @brief Free a UF data structure
 **/
p99_inline
void p99_uf_free(p99_uf volatile* uf) {
  p99_uf_destroy(uf);
  free((void*)uf);
}

p99_inline
bool p00_uf_root(int64_t tab[static 1], int64_t pos) {
  return tab[pos] < 0;
}

p99_inline
uint64_t p00_uf_size(int64_t tab[static 1], int64_t pos) {
  return p00_uf_root(tab, pos) ? -tab[pos] : 0;
}

p99_inline
int64_t p00_uf_find(int64_t tab[static 1], uint64_t pos) {
  while (!p00_uf_root(tab, pos)) pos = tab[pos];
  return pos;
}

p99_inline
int64_t p99_uf_find(p99_uf* uf, uint64_t pos) {
  if (!uf || (uf->size <= pos)) return -1;
  else return p00_uf_find(uf->tab, pos);
}

p99_inline
int64_t p00_uf_exchange(int64_t tab[static 1], int64_t pos, int64_t val) {
  int64_t ret = tab[pos];
  tab[pos] = val;
  return ret;
}

p99_inline
void p00_uf_compress(int64_t tab[static 1], uint64_t pos, uint64_t root) {
  while (!p00_uf_root(tab, pos)) {
    pos = p00_uf_exchange(tab, pos, root);
  }
}

p99_inline
int64_t p00_uf_findCompress(int64_t tab[static 1], int64_t pos) {
  int64_t root = p00_uf_find(tab, pos);
  p00_uf_compress(tab, pos, root);
  return root;
}

p99_inline
int64_t p99_uf_findCompress(p99_uf* uf, uint64_t pos) {
  if (!uf || (uf->size <= pos)) return -1;
  return p00_uf_findCompress(uf->tab, pos);
}

p99_inline
uint64_t p99_uf_size(p99_uf* uf, uint64_t pos) {
  if (uf || (uf->size > pos)) {
    int64_t root = p00_uf_findCompress(uf->tab, pos);
    if (root >= 0 && p00_uf_root(uf->tab, pos))
      return -uf->tab[pos];
  }
  return 0;
}

p99_inline
void p00_uf_union(int64_t* tab, int64_t left, int64_t right) {
  tab[left] += p00_uf_exchange(tab, right, left);
}

p99_inline
int64_t p99_uf_union(p99_uf* uf, uint64_t left, uint64_t right) {
  int64_t root = -1;
  if (uf && (uf->size > left) && (uf->size > right)) {
    root = p00_uf_findCompress(uf->tab, left);
    if (root >= 0) {
      int64_t rright = p00_uf_find(uf->tab, right);
      if (rright >= 0) {
        // use the new root to mark all elements of the right
        p00_uf_compress(uf->tab, right, root);
        // now also link the old root on the right to the new one
        p00_uf_union(uf->tab, root, rright);
      } else
        root = rright;
    }
  }
  return root;
}

p99_inline
void p99_uf_flatten(p99_uf* uf, uint64_t pos, uint64_t length) {
  if (uf && (uf->size > pos) && ((uf->size-pos) >= length)) {
    for (uint64_t stop = pos+length; pos < stop; ++pos) {
      p00_uf_findCompress(uf->tab, pos);
    }
  }
}

#endif
