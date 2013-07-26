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
    P99_SEQ(P00_ARC_, __VA_ARGS__),                     \
    P99_SEQ(P00_VERTEX_, __VA_ARGS__),                  \
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

#define P00_VERTEX_2(NAME3, POS, VAL, ...)              \
[P00_VPOS(POS)].p00_id = P00_VPOS(POS),                 \
[P00_VPOS(POS)].p00_weight =                            \
P99_IF_EMPTY(VAL)                                       \
((void*)P99_GENERIC_TCONST(P00_GRAPH_TV NAME3, (P00_GRAPH_TV NAME3[])P99_INIT, 0)) \
((void*)(P00_GRAPH_TV NAME3[]){ VAL })

#define P00_VERTEX_0(NAME3, POS)                \
[P00_VPOS(POS)].p00_id = P00_VPOS(POS),         \
[P00_VPOS(POS)].p00_weight = (void*)P99_GENERIC_TCONST(P00_GRAPH_TV NAME3, (P00_GRAPH_TV NAME3[])P99_INIT, 0)

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
#define P00_ARC_WEIGHT(NAME, X, I)                      \
[I].p00_weight =                                        \
  P99_IF_EMPTY(P00_ARC_VALUE X)                         \
  ((void*)(P00_GRAPH_TE NAME[])P99_INIT)               \
  ((void*)(P00_GRAPH_TE NAME[]){ P00_ARC_VALUE X })

#define P00_ARC_WEIGHTC(NAME, X, I)                             \
[I].p00_weight =                                                \
  P99_IF_EMPTY(P00_ARC_VALUE X)                                 \
  ((void*)0)                                                    \
  ((void*)(P00_GRAPH_TE NAME[]){ P00_ARC_VALUE X })

#define P00_ARC_ENDPOINT(WEIGHT, X, I)                                   \
[P00_VPOS(P00_ARC_TARGET X)].p00_id = P00_VPOS(P00_ARC_TARGET X),       \
[P00_VPOS(P00_ARC_TARGET X)].p00_weight = (void*)WEIGHT

#define P00_ARC_2_(NAME3, N, ...)                               \
  P00_ARC_2_LIT(NAME3, N,                                       \
                (P00_ARC_2_LIST_W(NAME3, N, __VA_ARGS__)),      \
                (P00_ARC_2_LIST_WC(NAME3, N, __VA_ARGS__)),     \
                (P00_ARC_2_LIST_V(NAME3, N, __VA_ARGS__))       \
                )

#define P00_ARC_2_LIT_(NAME3, N, A, AC)                                 \
  P99_GENERIC_PCONST(&(P00_GRAPH_NAME NAME3)[0], A, AC)

#define P00_ARC_2_LIT(NAME3, N, V, VC, A)                               \
P00_ARC_2_LIT_(NAME3, N,                                                \
               P99_GENERIC_TCONST(P00_GRAPH_TE NAME3,                   \
                                  ((p99_arc[(N)+1]){ P00_ROBUST V, P00_ROBUST A, }), \
                                  ((p99_arc[(N)+1]){ P00_ROBUST VC, P00_ROBUST A, })), \
               P99_GENERIC_TCONST(P00_GRAPH_TE NAME3,                   \
                                  ((p99_arc*)(p99_arc const[(N)+1]){ P00_ROBUST V, P00_ROBUST A, }), \
                                  ((p99_arc*)(p99_arc const[(N)+1]){ P00_ROBUST VC, P00_ROBUST A, })))

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
P99_FOR(                                                                \
        P99_GENERIC_TCONST(P00_GRAPH_TV NAME3, (P00_GRAPH_TV NAME3[])P99_INIT, 0), \
        P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARC_ENDPOINT, __VA_ARGS__)

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

#define P99_ARC_WEIGHT(ARC)                                     \
P99_GENERIC_PCONST((ARC), p00_arc_weight, p00_arc_weightc)      \
(ARC)

p99_inline
p99_vertex* p00_arc_vertex(p99_arc* p00_v) {
  return p00_v ? p00_v->p00_vertex : 0;
}

p99_inline
p99_vertex const* p00_arc_vertexc(p99_arc const* p00_v) {
  return p00_v ? p00_v->p00_vertex : 0;
}

#define P99_ARC_VERTEX(ARC)                                     \
P99_GENERIC_PCONST((ARC), p00_arc_vertex, p00_arc_vertexc)      \
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

#define P99_VERTEX_WEIGHT(VERTEX)                                       \
P99_GENERIC_PCONST((VERTEX), p00_vertex_weight, p00_vertex_weightc)     \
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

#define p99_graph_vertex(VERTEX)                                        \
P99_GENERIC_PCONST((VERTEX), p00_graph_vertex, p00_graph_vertexc)       \
(VERTEX)

#define P99_GRAPH_VERTEX(NAME, I) p99_graph_vertex((NAME)+P00_VPOS(I))

#define P99_GRAPH_VWEIGHT(NAME, I) P99_VERTEX_WEIGHT(P99_GRAPH_VERTEX(NAME, I))
#define P99_GRAPH_VDEGREE(NAME, I) P99_VERTEX_DEGREE(P99_GRAPH_VERTEX(NAME, I))
#define P99_GRAPH_ARCS(NAME, I)                                         \
P99_GENERIC_PQUALIFIED(&(NAME)[0],                                             \
                       (, P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))), \
                       (const, (p99_arc const*)P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))), \
                       (volatile, (p99_arc volatile*)P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))), \
                       (const volatile, (p99_arc const volatile*)P99_VERTEX_ARCS(P99_GRAPH_VERTEX(NAME, I))) \
                       )

#define P99_GRAPH_ARC(NAME, I, POS) (P99_GRAPH_ARCS(NAME, I)+(POS))

#define P99_VERTEX_ID(VERTEX) ((VERTEX)[0].p00_id-P00_GRAPH_OFFSET)

MYCONST p99_graph ggraph
= P99_GRAPH_INITIALIZER(VTYPE, ETYPE, ggraph,
                        (1, 11),
                        (2, 0, (1, 221), (2, 222), (3, 223), (14, 227)),
                        (3, 33, (4, 334), (7, 337), (9, 339)),
                        (4, 33, (4, 334), (7, 337), (9, 339)),
                        (12,),
                        (8),
                        (9, 99, (10,), (11), (12, 8.9)));

const p99_graph bgraph
= P99_GRAPH_INITIALIZER(char const*, char const*, bgraph,
                        (0, "a", (1, "b"), (4, "c")),
                        (1, "b", (2, "bb"), (3, "bc")),
                        (2, "bb"),
                        (3, "bc"),
                        (4, "c", (5, "cc"), (6, "cd")),
                        (5, "cc"),
                        (6, "cd"));


#define P00_GRAPH_PRINTP(TV, TE, G, F)                                  \
do {                                                                    \
  fputs("---------------------------------\n", (F));                    \
  fprintf((F), " %zu vertices (" P99_STRINGIFY(TV) "), %zu arcs (" P99_STRINGIFY(TE) ")\n", \
         P99_GRAPH_N(G), P99_GRAPH_M(G));                               \
  P99_DO(size_t, i, 0, P99_GRAPH_N(G)) {                                \
    p99_vertex const* vert = P99_GRAPH_VERTEX(G, i);                    \
    if (vert) {                                                         \
      TV const* p = P99_GRAPH_VWEIGHT(G, i);                            \
      size_t deg = P99_GRAPH_VDEGREE(G, i);                             \
      size_t id = P99_VERTEX_ID(vert);                                  \
      assert(id == i);                                                  \
      if (p) P99_FPRINTF((F), "%s (%s)\t:\t", id, *p);                  \
      else fprintf((F), "%zu\t:\t", id);                                \
      P99_DO(size_t, j, 0, deg) {                                       \
        p99_arc const* arc = P99_GRAPH_ARC(G, i, j);                    \
        TE const* weight = P99_ARC_WEIGHT(arc);                         \
        p99_vertex const* vert = P99_ARC_VERTEX(arc);                   \
        if (weight) P99_FPRINTF((F), "%s (%s)\t", P99_VERTEX_ID(vert), *weight); \
        else fprintf((F), "%zu\t", P99_VERTEX_ID(vert));                \
      }                                                                 \
      fputs("\n", (F));                                                 \
    }                                                                   \
  }                                                                     \
  fputs("---------------------------------\n", (F));                    \
} while (0)

#define P99_GRAPH_PRINTP(TV, TE, ...)           \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_GRAPH_PRINTP(TV, TE, __VA_ARGS__, stdout)) \
(P00_GRAPH_PRINTP(TV, TE, __VA_ARGS__))

#define P00_GRAPH_PRINT(TV, TE, G, F)                                   \
do {                                                                    \
  fputs("---------------------------------\n", (F));                    \
  fprintf((F), " %zu vertices, %zu arcs\n",                             \
         P99_GRAPH_N(G), P99_GRAPH_M(G));                               \
  P99_DO(size_t, i, 0, P99_GRAPH_N(G)) {                                \
    p99_vertex const* vert = P99_GRAPH_VERTEX(G, i);                    \
    if (vert) {                                                         \
      TV const p = P99_GRAPH_VWEIGHT(G, i);                             \
      size_t deg = P99_GRAPH_VDEGREE(G, i);                             \
      size_t id = P99_VERTEX_ID(vert);                                  \
      assert(id == i);                                                  \
      if (p) P99_FPRINTF((F), "%s [%s]\t:\t", id, p);                 \
      else fprintf((F), "%zu\t:\t", id);                                \
      P99_DO(size_t, j, 0, deg) {                                       \
        p99_arc const* arc = P99_GRAPH_ARC(G, i, j);                    \
        TE const weight = P99_ARC_WEIGHT(arc);                          \
        p99_vertex const* vert = P99_ARC_VERTEX(arc);                   \
        if (weight) P99_FPRINTF((F), "%s [%s]\t", P99_VERTEX_ID(vert), weight); \
        else fprintf((F), "%zu\t", P99_VERTEX_ID(vert));                \
      }                                                                 \
      fputs("\n", (F));                                                 \
    }                                                                   \
  }                                                                     \
  fputs("---------------------------------\n", (F));                    \
} while (0)

#define P99_GRAPH_PRINT(TV, TE, ...)            \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_GRAPH_PRINT(TV, TE, __VA_ARGS__, stdout))  \
(P00_GRAPH_PRINT(TV, TE, __VA_ARGS__))

#define BI_NUM_1(I, ...) I
#define BI_NUM_0(...) BI_NUM_1 __VA_ARGS__
#define BI_NUM(X, ...) BI_NUM_0(P99_CHS(X, __VA_ARGS__))

#define BI_VERT_0(...) P00_ROBUST __VA_ARGS__
#define BI_VERT(X, ...) BI_VERT_0(P99_CHS(X, __VA_ARGS__))

#define BI0(...)
#define BI1(...) (BI_VERT(0, __VA_ARGS__))
#define BI2(...) (BI_VERT(1, __VA_ARGS__), (BI_NUM(0, __VA_ARGS__))), \
    BI1(__VA_ARGS__)

#define BI3(...) (BI_VERT(1, __VA_ARGS__), (BI_NUM(0, __VA_ARGS__)), (BI_NUM(2, __VA_ARGS__))), \
    BI1(__VA_ARGS__),                                                   \
    BI1(P99_SKP(2, __VA_ARGS__))

#define BI4(...) (BI_VERT(2, __VA_ARGS__), (BI_NUM(1, __VA_ARGS__)), (BI_NUM(3, __VA_ARGS__))), \
    BI2(__VA_ARGS__),                                      \
    BI1(P99_SKP(3, __VA_ARGS__))

#define BI5(...) (BI_VERT(2, __VA_ARGS__), (BI_NUM(1, __VA_ARGS__)), (BI_NUM(4, __VA_ARGS__))), \
    BI2(__VA_ARGS__),                                      \
    BI2(P99_SKP(3, __VA_ARGS__))

#define BI6(...) (BI_VERT(3, __VA_ARGS__), (BI_NUM(1, __VA_ARGS__)), (BI_NUM(5, __VA_ARGS__))), \
    BI3(__VA_ARGS__),                                      \
    BI2(P99_SKP(4, __VA_ARGS__))

#define BI7(...) (BI_VERT(3, __VA_ARGS__), (BI_NUM(1, __VA_ARGS__)), (BI_NUM(5, __VA_ARGS__))), \
    BI3(__VA_ARGS__),                                      \
    BI3(P99_SKP(4, __VA_ARGS__))

#define BI8(...) (BI_VERT(4, __VA_ARGS__), (BI_NUM(2, __VA_ARGS__)), (BI_NUM(6, __VA_ARGS__))), \
    BI4(__VA_ARGS__),                                      \
    BI3(P99_SKP(5, __VA_ARGS__))

#define BI9(...) (BI_VERT(4, __VA_ARGS__), (BI_NUM(2, __VA_ARGS__)), (BI_NUM(7, __VA_ARGS__))), \
    BI4(__VA_ARGS__),                                      \
    BI4(P99_SKP(5, __VA_ARGS__))

#define BI10(...) (BI_VERT(5, __VA_ARGS__), (BI_NUM(2, __VA_ARGS__)), (BI_NUM(8, __VA_ARGS__))), \
    BI5(__VA_ARGS__),                                      \
    BI4(P99_SKP(6, __VA_ARGS__))

#define BI11(...) (BI_VERT(5, __VA_ARGS__), (BI_NUM(2, __VA_ARGS__)), (BI_NUM(8, __VA_ARGS__))), \
    BI5(__VA_ARGS__),                                      \
    BI5(P99_SKP(6, __VA_ARGS__))

#define BI12(...) (BI_VERT(6, __VA_ARGS__), (BI_NUM(3, __VA_ARGS__)), (BI_NUM(9, __VA_ARGS__))), \
    BI6(__VA_ARGS__),                                      \
    BI5(P99_SKP(7, __VA_ARGS__))

#define BI13(...) (BI_VERT(6, __VA_ARGS__), (BI_NUM(3, __VA_ARGS__)), (BI_NUM(10, __VA_ARGS__))), \
    BI6(__VA_ARGS__),                                      \
    BI6(P99_SKP(7, __VA_ARGS__))

#define BI14(...) (BI_VERT(7, __VA_ARGS__), (BI_NUM(3, __VA_ARGS__)), (BI_NUM(11, __VA_ARGS__))), \
    BI7(__VA_ARGS__),                                      \
    BI6(P99_SKP(8, __VA_ARGS__))

#define BI15(...) (BI_VERT(7, __VA_ARGS__), (BI_NUM(3, __VA_ARGS__)), (BI_NUM(11, __VA_ARGS__))), \
    BI7(__VA_ARGS__),                                      \
    BI7(P99_SKP(8, __VA_ARGS__))

#define BICOUNT_(_0, X, I) (I, X)

#define BICOUNT(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, BICOUNT_, __VA_ARGS__)


#define BI_(N, ...) P99_PASTE2(BI, N)(__VA_ARGS__)
#define BI(...) BI_(P99_NARG(__VA_ARGS__), BICOUNT(__VA_ARGS__))


const p99_graph graph1
= P99_GRAPH_INITIALIZER(char const, void const, graph1,
                        BI("A1"));

const p99_graph graph2
= P99_GRAPH_INITIALIZER(char const, char const, graph2,
                        BI("A2", "B2"));

const p99_graph graph3
= P99_GRAPH_INITIALIZER(char const, char const, graph3,
                        BI("A3", "B3", "C3"));

const p99_graph graph4
= P99_GRAPH_INITIALIZER(char const, char const, graph4,
                        BI("A4", "B4", "C4", "D4"));

const p99_graph graph5
= P99_GRAPH_INITIALIZER(char const, char const, graph5,
                        BI("A5", "B5", "C5", "D5", "E5"));

const p99_graph graph6
= P99_GRAPH_INITIALIZER(char const, char const, graph5,
                        BI("A6", "B6", "C6", "D6", "E6", "F6"));

const p99_graph graph7
= P99_GRAPH_INITIALIZER(char const, char const, graph6,
                        BI("A7", "B7", "C7", "D7", "E7", "F7", "G7"));

const p99_graph graph8
= P99_GRAPH_INITIALIZER(char const, char const, graph8,
                        BI("A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"));

const p99_graph graph9
= P99_GRAPH_INITIALIZER(char const, char const, graph9,
                        BI("A9", "B9", "C9", "D9", "E9", "F9", "G9", "H9", "I9"));

const p99_graph graphA
= P99_GRAPH_INITIALIZER(char const, char const, graphA,
                        BI("AA", "BA", "CA", "DA", "EA", "FA", "GA", "HA", "IA", "JA"));

const p99_graph graphB
= P99_GRAPH_INITIALIZER(char const, char const, graphB,
                        BI("AB", "BB", "CB", "DB", "EB", "FB", "GB", "HB", "IB", "JB", "KB"));

const p99_graph graphC
= P99_GRAPH_INITIALIZER(char const, char const, graphC,
                        BI("AC", "BC", "CC", "DC", "EC", "FC", "GC", "HC", "IC", "JC", "KC", "LC"));

const p99_graph graphD
= P99_GRAPH_INITIALIZER(char const, char const, graphD,
                        BI("AD", "BD", "CD", "DD", "ED", "FD", "GD", "HD", "ID", "JD", "KD", "LD", "MD"));

const p99_graph graphE
= P99_GRAPH_INITIALIZER(char const, char const, graphE,
                        BI("AE", "BE", "CE", "DE", "EE", "FE", "GE", "HE", "IE", "JE", "KE", "LE", "ME", "NE"));

const p99_graph graphF
= P99_GRAPH_INITIALIZER(char const, char const, graphF,
                        BI("AF", "BF", "CF", "DF", "EF", "FF", "GF", "HF", "IF", "JF", "KF", "LF", "MF", "NF", "OF"));

/*
BI("A", "B", "C", "D", "E", "F", "G", "H", "II", "J", "K", "L", "M", "N", "O", "P")
*/

int main(void) {
  for (size_t pos = P00_GRAPH_OFFSET; pos < P99_ALEN(bgraph); ++pos) {
    p99_vertex const* vert = p99_graph_vertex(&bgraph[pos]);
    if (!vert) continue;

    char const*const* p = P99_VERTEX_WEIGHT(vert);
    size_t id = P99_VERTEX_ID(vert);
    if (p) printf("%zu (%s)\t:\t", id, *p);
    else printf("%zu\t:\t", id);
    for (p99_arc MYCONST* arc = P99_VERTEX_ARCS(vert);
         arc && P99_ARC_VERTEX(arc);
         ++arc) {
      char const*const* weight = P99_ARC_WEIGHT(arc);
      p99_vertex MYCONST* vert = P99_ARC_VERTEX(arc);
      if (weight) printf("%zu (%s)\t", P99_VERTEX_ID(vert), *weight);
      else printf("%zu\t", P99_VERTEX_ID(vert));
    }
    fputs("\n", stdout);
  }
  P99_GRAPH_PRINTP(VTYPE, ETYPE, ggraph);
  P99_GRAPH_PRINTP(char const*, char const*, bgraph, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph1, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph2, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph3, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph4, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph5, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph6, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph7, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph8, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graph9, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphA, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphB, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphC, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphD, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphE, stderr);
  P99_GRAPH_PRINT(char const*, void const*, graphF, stderr);
}
