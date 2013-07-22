#include "p99.h"
#include "p99_generic.h"

typedef struct p00_cons p00_cons;

struct p00_cons {
  void * p00_car;
  void * p00_cdr;
};

#define P00_VERTEX_EXPAND(NAME3, X, I) (NAME3, P00_ROBUST X)


#define P00_GRAPH_EXPAND(NAME3, ...)\
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_VERTEX_EXPAND, __VA_ARGS__)

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

#define P00_VOFF 1
#define P00_VPOS(X) ((X)+P00_VOFF)


#define P00_GRAPH(TV, TE, NAME, ...)                    \
P99_WARN_INIT_PUSH                                      \
p99_graph NAME =                                        \
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

#define P99_GRAPH(TV, TE, NAME, ...) P00_GRAPH(TV, TE, NAME, P00_GRAPH_EXPAND((TV, TE, NAME), __VA_ARGS__))

#define P00_VERTEX_2(NAME3, POS, VAL, ...)                              \
[P00_VPOS(POS)].p00_id = ((POS)+1),                                     \
[P00_VPOS(POS)].p00_weight =                                            \
P99_IF_EMPTY(VAL)                                                       \
((void*)0)                                                              \
(                                                                       \
P99_GENERIC(&(P00_GRAPH_NAME NAME3)[0], ,                               \
            (p99_vertex *, &(P00_GRAPH_TV NAME3){ VAL }),               \
            (p99_vertex const*, (void*)&(const P00_GRAPH_TV NAME3){ VAL }))  \
)

#define P00_VERTEX_0(NAME3, POS)                \
[P00_VPOS(POS)].p00_id = ((POS)+1),                       \
[P00_VPOS(POS)].p00_weight = (void*)(0)

#define P00_VERTEX_1(NAME3, ...)                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_VERTEX_0(NAME3, __VA_ARGS__ ))             \
(P00_VERTEX_2(NAME3, __VA_ARGS__ ))

#define P00_VERTEX_(ARGS) P00_VERTEX_1 ARGS


#define P00_VERTEXC_0(NAME3, POS, ...) [POS] = 0
#define P00_VERTEXC_1(NAME3, ...) P00_VERTEXC_0(NAME3, __VA_ARGS__, 0)
#define P00_VERTEXC_(ARGS) P00_VERTEXC_1 ARGS


#define P00_ARC_TARGET(TARGET, ...) TARGET
#define P00_ARC_VALUE(TARGET, ...) __VA_ARGS__

#define P00_GRAPH_TV(TV, TE, NAME) TV
#define P00_GRAPH_TE(TV, TE, NAME) TE
#define P00_GRAPH_NAME(TV, TE, NAME) NAME

#define P00_ARC_VERTEX(NAME, X, I) [I].p00_vertex = (void*)&(P00_GRAPH_NAME NAME)[P00_VPOS(P00_ARC_TARGET X)]
#define P00_ARC_WEIGHT(NAME, X, I)                \
[I].p00_weight =                                \
  P99_IF_EMPTY(P00_ARC_VALUE X)                \
  ((void*)0)                                    \
  (&(P00_GRAPH_TE NAME){ P00_ARC_VALUE X })

#define P00_ARC_ENDPOINT(NAME, X, I) [P00_VPOS(P00_ARC_TARGET X)].p00_id = ((P00_ARC_TARGET X)+1)


#define P00_ARC_2(NAME3, POS, VAL, ...)                                \
  /* force entries for all vertices that are used */                    \
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARC_ENDPOINT, __VA_ARGS__), \
  /* store the degree */                                                \
[P00_VPOS(POS)].p00_deg = (P99_NARG(__VA_ARGS__)),                                \
  [P00_VPOS(POS)].p00_arcs =                                            \
  (p99_arc[P99_NARG(__VA_ARGS__)+1]){                                   \
  P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARC_WEIGHT, __VA_ARGS__), \
  P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_ARC_VERTEX, __VA_ARGS__), \
}

#define P00_ARC_0(NAME3, POS, ...)             \
[P00_VPOS(POS)].p00_deg = 0,                              \
[P00_VPOS(POS)].p00_arcs = (void*)(0)



#define P00_ARC_1(...)                         \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_ARC_0(__VA_ARGS__))                       \
(P00_ARC_2(__VA_ARGS__))

#define P00_ARC_(ARGS) P00_ARC_1 ARGS

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
#define P00_ARCC_1(...)                        \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_ARCC_0(__VA_ARGS__))                      \
(P00_ARCC_2(__VA_ARGS__))

#define P00_ARCC_(ARGS) P00_ARCC_1 ARGS

#define P99_GRAPH_N(NAME) ((NAME)[0].p00_id)
#define P99_GRAPH_M(NAME) ((NAME)[0].p00_deg)



#define P99_ARC_VERTEX(ARC) ((ARC)->p00_vertex)
#define P99_ARC_WEIGHT(ARC) ((ARC)->p00_weight)

#define P99_VERTEX_WEIGHT(VERTEX) ((VERTEX)[0].p00_weight)
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

#define P99_VERTEX_ID(VERTEX) ((VERTEX)[0].p00_id-1)

const
P99_GRAPH(size_t, double, hey,
          (1, 11),
          (2, 22, (1, 221), (2, 222), (3, 223), (14, 227)),
          (3, 33, (4, 334), (7, 337), (9, 339)),
          (12,),
          (8),
          (9, 99, (10,), (11), (12, 8.9)));

int main(void) {
  printf("total number of vertices %zu\n", P99_GRAPH_N(hey));
  printf("total number of arcs %zu\n", P99_GRAPH_M(hey));
  P99_DO(size_t, i, 0, P99_GRAPH_N(hey)) {
    p99_vertex const* vert = P99_GRAPH_VERTEX(hey, i);
    if (vert) {
      size_t * p = P99_GRAPH_VWEIGHT(hey, i);
      size_t deg = P99_GRAPH_VDEGREE(hey, i);
      size_t id = P99_VERTEX_ID(vert);
      if (p) printf("vertex %zu: weight %zu", id, *p);
      else printf("vertex %zu no weight", id);
      printf(",\tdegree %zu\n", deg);
      P99_DO(size_t, j, 0, deg) {
        p99_arc const* arc = P99_GRAPH_ARC(hey, i, j);
        double * weight = P99_ARC_WEIGHT(arc);
        p99_vertex* vert = P99_ARC_VERTEX(arc);
        if (weight) printf("\t\t%zu %g\n", P99_VERTEX_ID(vert), *weight);
        else printf("\t\t%zu (no weight)\n", P99_VERTEX_ID(vert));
      }
      for (p99_arc const* arc = P99_GRAPH_ARCS(hey, i);
           arc && P99_ARC_VERTEX(arc);
           ++arc) {
        double * weight = P99_ARC_WEIGHT(arc);
        p99_vertex* vert = P99_ARC_VERTEX(arc);
        if (weight) printf("\t\t%zu %g\n", P99_VERTEX_ID(vert), *weight);
        else printf("\t\t%zu (no weight)\n", P99_VERTEX_ID(vert));
      }
    }
  }
}
