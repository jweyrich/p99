#include "p99.h"
#include "p99_generic.h"

/* Use the following three macros to extend test coverage */
#ifndef MYCONST
# define MYCONST const
#endif

#ifndef VTYPE
# define VTYPE size_t
#endif

#ifndef ETYPE
# define ETYPE double
#endif

typedef struct p99_vertex p99_vertex;
typedef struct p99_arc p99_arc;


struct p99_arc {
  void* p00_weight;
  p99_vertex* p00_vertex;
};

struct p99_vertex {
  uintptr_t p00_id;
  void* p00_weight;
  uintptr_t p00_deg;
  p99_arc* p00_arcs;
};

typedef p99_vertex p99_graph[];

/* We need at least one spare vertex at the start of the graph array:
   we store n and m, there, and p00_id of vertices should never be
   0. */
#if P00_GRAPH_OFFSET <= 0
# define P00_GRAPH_OFFSET 1U
#endif

#define P00_VPOS(X) ((X)+P00_GRAPH_OFFSET)


#define P00_GRAPH(TV, TE, NAME, ...)                    \
P99_WARN_INIT_PUSH                                      \
  {                                                     \
    [0].p00_deg = P99_SEA(P00_ARCC_, __VA_ARGS__),      \
    [0].p00_id = sizeof((char const[]){                 \
        P99_SEQ(P00_VERTEXC_, __VA_ARGS__),             \
          P99_SEQ(P00_ARCSC_, __VA_ARGS__),             \
          }),                                           \
    P99_SEQ(P00_VERTEX_, __VA_ARGS__),                  \
    P99_SEQ(P00_ARC_, __VA_ARGS__),                     \
  }                                                     \
P99_WARN_INIT_POP

#define P00_VERTEX_EXPAND(NAME3, X, I) (NAME3, P00_ROBUST X)

#define P00_GRAPH_EXPAND(NAME3, ...)\
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_VERTEX_EXPAND, __VA_ARGS__)

#define P99_GRAPH_INITIALIZER(TV, TE, NAME, ...) P00_GRAPH(TV, TE, NAME, P00_GRAPH_EXPAND((TV, TE, NAME), __VA_ARGS__))

/* Many of the macros here receive a triplet for the generic
   information about the graph. These three macros access the
   individual parts. */

#define P00_GRAPH_TV(TV, TE, NAME) TV
#define P00_GRAPH_TE(TV, TE, NAME) TE
#define P00_GRAPH_NAME(TV, TE, NAME) NAME

/* Implement the P00_VERTEX_ macro that collects the per vertex
   information. */

#define P00_VERTEX_2(NAME3, POS, VAL, ...)                              \
[P00_VPOS(POS)].p00_id = P00_VPOS(POS),                                 \
[P00_VPOS(POS)].p00_weight =                                            \
P99_IF_EMPTY(VAL)                                                       \
((void*)0)                                                              \
(                                                                       \
P99_GENERIC(&(P00_GRAPH_NAME NAME3)[0], ,                               \
            (p99_vertex *, &(P00_GRAPH_TV NAME3){ VAL }),               \
            (p99_vertex const*, (void*)&(const P00_GRAPH_TV NAME3){ VAL }))  \
)

#define P00_VERTEX_0(NAME3, POS)                \
[P00_VPOS(POS)].p00_id = P00_VPOS(POS),         \
[P00_VPOS(POS)].p00_weight = (void*)(0)

#define P00_VERTEX_1(NAME3, ...)                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_VERTEX_0(NAME3, __VA_ARGS__ ))             \
(P00_VERTEX_2(NAME3, __VA_ARGS__ ))

#define P00_VERTEX_(ARGS) P00_VERTEX_1 ARGS

/* Implement the P00_VERTEXC_ macro that is similar, but only serves
   to compute the maximum vertex ID. */

#define P00_VERTEXC_0(NAME3, POS, ...) [POS] = 0
#define P00_VERTEXC_1(NAME3, ...) P00_VERTEXC_0(NAME3, __VA_ARGS__, 0)
#define P00_VERTEXC_(ARGS) P00_VERTEXC_1 ARGS

#define P00_ARC_TARGET(TARGET, ...) TARGET
#define P00_ARC_VALUE(TARGET, ...) __VA_ARGS__

/* Implement the P00_ARC_ macro that collects the information on
   arcs. This is the most complex part, here, since this has actually
   to compute a list of items. */

#define P00_ARC_VERTEX(NAME, X, I) [I].p00_vertex = (void*)&(P00_GRAPH_NAME NAME)[P00_VPOS(P00_ARC_TARGET X)]
#define P00_ARC_WEIGHT(NAME, X, I)              \
[I].p00_weight =                                \
  P99_IF_EMPTY(P00_ARC_VALUE X)                 \
  ((void*)0)                                    \
  (&(P00_GRAPH_TE NAME){ P00_ARC_VALUE X })

#define P00_ARC_WEIGHTC(NAME, X, I)                             \
[I].p00_weight =                                                \
  P99_IF_EMPTY(P00_ARC_VALUE X)                                 \
  ((void*)0)                                                    \
  ((void*)&(const P00_GRAPH_TE NAME){ P00_ARC_VALUE X })

#define P00_ARC_ENDPOINT(NAME, X, I) [P00_VPOS(P00_ARC_TARGET X)].p00_id = P00_VPOS(P00_ARC_TARGET X)

#define P00_ARC_2_(NAME3, N, ...)                               \
  P00_ARC_2_LIT(NAME3, N,                                       \
                (P00_ARC_2_LIST_W(NAME3, N, __VA_ARGS__)),      \
                (P00_ARC_2_LIST_WC(NAME3, N, __VA_ARGS__)),     \
                (P00_ARC_2_LIST_V(NAME3, N, __VA_ARGS__))       \
                )

#define P00_ARC_2_LIT(NAME3, N, V, VC, A)                              \
  P99_GENERIC(&(P00_GRAPH_NAME NAME3)[0], ,                           \
              (p99_vertex*, (p99_arc[(N)+1]){ P00_ROBUST V, P00_ROBUST A, }), \
              (p99_vertex const*, (p99_arc*)(p99_arc const[(N)+1]){ P00_ROBUST VC, P00_ROBUST A, }))

/* create a list of entries for all arc weights, the version that has
   modifiable weights */
#define P00_ARC_2_LIST_W(NAME3, N, ...)                      \
  P99_FOR(NAME3, N, P00_SEQ, P00_ARC_WEIGHT, __VA_ARGS__)

/* create a list of entries for all arc weights, the version that has
   const weights */
#define P00_ARC_2_LIST_WC(NAME3, N, ...)                      \
  P99_FOR(NAME3, N, P00_SEQ, P00_ARC_WEIGHTC, __VA_ARGS__)

/* create a list of entries for all arc endpoints */
#define P00_ARC_2_LIST_V(NAME3, N, ...)                      \
  P99_FOR(NAME3, N, P00_SEQ, P00_ARC_VERTEX, __VA_ARGS__)

/* create a list of entries for all vertices that are used as endpoints of arcs */
#define P00_ARC_2_LIST_E(NAME3, N, ...)                                 \
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARC_ENDPOINT, __VA_ARGS__)

/* For the particular arc list of a vertex, first compute entries for
   all the target vertices of these arcs. Such entries may occur
   multiple times, for each arc in which a vertex appears. These
   entries for any target vertex are always the same value. C99
   explicitly allows such multiple initializations. */
#define P00_ARC_2(NAME3, POS, VAL, ...)                                 \
P00_ARC_2_LIST_E(NAME3, P99_NARG(__VA_ARGS__), __VA_ARGS__),            \
  /* store the degree */                                                \
[P00_VPOS(POS)].p00_deg = (P99_NARG(__VA_ARGS__)),                      \
[P00_VPOS(POS)].p00_arcs = P00_ARC_2_(NAME3, P99_NARG(__VA_ARGS__), __VA_ARGS__)

/* The case of an empty arc list. Set everything to zero, such that
   this is the same as an arc list that is default initialized. */
#define P00_ARC_0(NAME3, POS, ...)              \
[P00_VPOS(POS)].p00_deg = 0,                    \
[P00_VPOS(POS)].p00_arcs = (void*)(0)



#define P00_ARC_1(...)                          \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_ARC_0(__VA_ARGS__))                        \
(P00_ARC_2(__VA_ARGS__))

/* This must be specially expanded because the arc list may be an
   empty list. */
#define P00_ARC_(ARGS) P00_ARC_1 ARGS


/* Implement the P00_ARCC_ macro that is similar, but only serves to
   compute the number of arcs. */

#define P00_ARCSC_VERTEX(NAME, X, I)            \
P99_IF_EMPTY(P00_ARC_TARGET X)                  \
([0] = 0)                                       \
([P00_ARC_TARGET X] = 0)

#define P00_ARCSC_2(NAME3, POS, VAL, ...)                                \
  P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARCSC_VERTEX, __VA_ARGS__) \

#define P00_ARCSC_0(NAME3, POS, ...)  [0] = 0

#define P00_ARCSC_1(...)                        \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_ARCSC_0(__VA_ARGS__))                      \
(P00_ARCSC_2(__VA_ARGS__))

#define P00_ARCSC_(ARGS) P00_ARCSC_1 ARGS

#define P00_ARCC_2(NAME3, POS, VAL, ...) P99_NARG(__VA_ARGS__)
#define P00_ARCC_0(NAME3, POS, ...) 0
#define P00_ARCC_1(...)                         \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_ARCC_0(__VA_ARGS__))                       \
(P00_ARCC_2(__VA_ARGS__))

#define P00_ARCC_(ARGS) P00_ARCC_1 ARGS

#define P99_GRAPH_N(NAME) ((NAME)[0].p00_id)
#define P99_GRAPH_M(NAME) ((NAME)[0].p00_deg)



p99_inline
void* p00_arc_weight(p99_arc* p00_v) {
  return p00_v ? p00_v->p00_weight : 0;
}

p99_inline
void const* p00_arc_weightc(p99_arc const* p00_v) {
  return p00_v ? p00_v->p00_weight : 0;
}

#define P99_ARC_WEIGHT(ARC)                     \
P99_GENERIC((ARC), ,                            \
            (p99_arc *, p00_arc_weight),        \
            (p99_arc const*, p00_arc_weightc))  \
(ARC)

p99_inline
p99_vertex* p00_arc_vertex(p99_arc* p00_v) {
  return p00_v ? p00_v->p00_vertex : 0;
}

p99_inline
p99_vertex const* p00_arc_vertexc(p99_arc const* p00_v) {
  return p00_v ? p00_v->p00_vertex : 0;
}

#define P99_ARC_VERTEX(ARC)                     \
P99_GENERIC((ARC), ,                            \
            (p99_arc *, p00_arc_vertex),        \
            (p99_arc const*, p00_arc_vertexc))  \
(ARC)

#define P99_VERTEX_DEGREE(VERTEX) ((VERTEX)[0].p00_deg)


p99_inline
void* p00_vertex_weight(p99_vertex* p00_v) {
  return p00_v ? p00_v->p00_weight : 0;
}

p99_inline
void const* p00_vertex_weightc(p99_vertex const* p00_v) {
  return p00_v ? p00_v->p00_weight : 0;
}

#define P99_VERTEX_WEIGHT(VERTEX)                       \
P99_GENERIC((VERTEX), ,                                 \
            (p99_vertex *, p00_vertex_weight),          \
            (p99_vertex const*, p00_vertex_weightc))    \
(VERTEX)

#define P99_VERTEX_DEGREE(VERTEX) ((VERTEX)[0].p00_deg)
#define P99_VERTEX_ARCS(VERTEX) ((VERTEX)[0].p00_arcs)

p99_inline
p99_vertex* p00_graph_vertex(p99_vertex* p00_v) {
  return (p00_v && p00_v->p00_id) ? p00_v : 0;
}

p99_inline
p99_vertex const* p00_graph_vertexc(p99_vertex const* p00_v) {
  return (p00_v && p00_v->p00_id) ? p00_v : 0;
}

#define P99_GRAPH_VERTEX(NAME, I)                       \
P99_GENERIC(&(NAME)[0], ,                               \
            (p99_vertex *, p00_graph_vertex),           \
            (p99_vertex const*, p00_graph_vertexc))     \
((NAME)+P00_VPOS(I))

#define P99_GRAPH_VWEIGHT(NAME, I) P99_VERTEX_WEIGHT(P99_GRAPH_VERTEX(NAME, I))
#define P99_GRAPH_VDEGREE(NAME, I) P99_VERTEX_DEGREE(P99_GRAPH_VERTEX(NAME, I))
#define P99_GRAPH_ARCS(NAME, I)                                         \
P99_GENERIC(&(NAME)[0], ,                                               \
            (p99_vertex *, P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))), \
            (p99_vertex const*, (p99_arc const*)P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))) \
            )
#define P99_GRAPH_ARC(NAME, I, POS) (P99_GRAPH_ARCS(NAME, I)+(POS))

#define P99_VERTEX_ID(VERTEX) ((VERTEX)[0].p00_id-P00_GRAPH_OFFSET)

MYCONST p99_graph hey
= P99_GRAPH_INITIALIZER(VTYPE, ETYPE, hey,
                        (1, 11),
                        (2, 0, (1, 221), (2, 222), (3, 223), (14, 227)),
                        (3, 33, (4, 334), (7, 337), (9, 339)),
                        (4, 33, (4, 334), (7, 337), (9, 339)),
                        (12,),
                        (8),
                        (9, 99, (10,), (11), (12, 8.9)));

int main(void) {
  printf("total number of vertices %zu\n", P99_GRAPH_N(hey));
  printf("total number of arcs %zu\n", P99_GRAPH_M(hey));
  P99_DO(size_t, i, 0, P99_GRAPH_N(hey)) {
    p99_vertex MYCONST* vert = P99_GRAPH_VERTEX(hey, i);
    if (vert) {
      VTYPE MYCONST* p = P99_GRAPH_VWEIGHT(hey, i);
      size_t deg = P99_GRAPH_VDEGREE(hey, i);
      size_t id = P99_VERTEX_ID(vert);
      assert(id == i);
      if (p) P99_PRINTF("vertex %s: weight %s", id, *p);
      else printf("vertex %zu no weight", id);
      printf(",\tdegree %zu\n", deg);
      P99_DO(size_t, j, 0, deg) {
        p99_arc MYCONST* arc = P99_GRAPH_ARC(hey, i, j);
        ETYPE MYCONST* weight = P99_ARC_WEIGHT(arc);
        p99_vertex MYCONST* vert = P99_ARC_VERTEX(arc);
        if (weight) P99_PRINTF("\t\t%s %s\n", P99_VERTEX_ID(vert), *weight);
        else P99_PRINTF("\t\t%s (no weight)\n", P99_VERTEX_ID(vert));
      }
      for (p99_arc MYCONST* arc = P99_GRAPH_ARCS(hey, i);
           arc && P99_ARC_VERTEX(arc);
           ++arc) {
        ETYPE MYCONST* weight = P99_ARC_WEIGHT(arc);
        p99_vertex MYCONST* vert = P99_ARC_VERTEX(arc);
        if (weight) P99_PRINTF("\t\t%s %s\n", P99_VERTEX_ID(vert), *weight);
        else printf("\t\t%zu (no weight)\n", P99_VERTEX_ID(vert));
      }
    }
  }
}
