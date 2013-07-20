#include "p99.h"

typedef struct p00_cons p00_cons;

struct p00_cons {
  void * p00_car;
  void * p00_cdr;
};

#define P00_VERTEX_EXPAND(NAME3, X, I) (NAME3, P00_ROBUST X)


#define P00_GRAPH_EXPAND(NAME3, ...)\
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_VERTEX_EXPAND, __VA_ARGS__)

typedef p00_cons p99_vertex[2];
typedef p00_cons p99_edge;
typedef p99_vertex p99_graph[];

#define P00_GRAPH(TV, TE, NAME, ...)            \
P99_WARN_INIT_PUSH                              \
TV * P99_PASTE(p00_tv_, NAME) = 0;              \
TE * P99_PASTE(p00_te_, NAME) = 0;              \
p99_graph NAME =                                \
  {                                             \
    P99_SEQ(P00_VERTEX_, __VA_ARGS__),          \
    P99_SEQ(P00_EDGE_, __VA_ARGS__),            \
  }                                             \
P99_WARN_INIT_POP

#define P99_GRAPH(TV, TE, NAME, ...) P00_GRAPH(TV, TE, NAME, P00_GRAPH_EXPAND((TV, TE, NAME), __VA_ARGS__))

#define P00_VERTEX_2(NAME3, POS, VAL, ...)      \
[POS][0].p00_car = (void*)(POS),                \
[POS][0].p00_cdr =                              \
P99_IF_EMPTY(VAL)                               \
((void*)0)                                      \
(&(P00_GRAPH_TV NAME3){ VAL })

#define P00_VERTEX_0(NAME3, POS)                \
[POS][0].p00_car = (void*)((POS)+1),            \
[POS][0].p00_cdr = (void*)(0)

#define P00_VERTEX_1(NAME3, ...)                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)             \
(P00_VERTEX_0(NAME3, __VA_ARGS__ ))             \
(P00_VERTEX_2(NAME3, __VA_ARGS__ ))

#define P00_VERTEX_(ARGS) P00_VERTEX_1 ARGS


#define P00_EDGE_TARGET(TARGET, ...) TARGET
#define P00_EDGE_VALUE(TARGET, ...) __VA_ARGS__

#define P00_GRAPH_TV(TV, TE, NAME) TV
#define P00_GRAPH_TE(TV, TE, NAME) TE
#define P00_GRAPH_NAME(TV, TE, NAME) NAME

#define P00_EDGE_CDR(NAME, X, I) [I].p00_cdr = &(P00_GRAPH_NAME NAME)[P00_EDGE_TARGET X]
#define P00_EDGE_CAR(NAME, X, I)                \
[I].p00_car =                                   \
  P99_IF_EMPTY(P00_EDGE_VALUE X)                \
  ((void*)0)                                    \
  (&(P00_GRAPH_TE NAME){ P00_EDGE_VALUE X })

#define P00_EDGE_ENDPOINT(NAME, X, I) [P00_EDGE_TARGET X][0].p00_car = (void*)((P00_EDGE_TARGET X)+1)


#define P00_EDGE_2(NAME3, POS, VAL, ...)                                \
  /* force entries for all vertices that are used */                    \
P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_EDGE_ENDPOINT, __VA_ARGS__), \
  /* store the degree */                                                \
[POS][1].p00_car = (void*)(P99_NARG(__VA_ARGS__)),                      \
[POS][1].p00_cdr = (p99_edge[P99_NARG(__VA_ARGS__)+1]){                 \
  P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_EDGE_CAR, __VA_ARGS__), \
  P99_FOR(NAME3, P99_NARG(__VA_ARGS__), P00_SEQ, P00_EDGE_CDR, __VA_ARGS__), \
}

#define P00_EDGE_0(NAME3, POS, ...)             \
[POS][1].p00_car = (void*)(0),                  \
[POS][1].p00_cdr = (void*)(0)



#define P00_EDGE_1(...)                         \
P99_IF_LT(P99_NARG(__VA_ARGS__), 4)             \
(P00_EDGE_0(__VA_ARGS__))                       \
(P00_EDGE_2(__VA_ARGS__))

#define P00_EDGE_(ARGS) P00_EDGE_1 ARGS

#define P99_GRAPH_N(NAME) P99_ALEN(NAME)

#define P99_GRAPH_V(NAME, I) ((NAME)[I][0].p00_car)


#define P99_EDGE_VERTEX(EDGE) ((EDGE)->p00_cdr)
#define P99_EDGE_WEIGHT(EDGE) ((EDGE)->p00_car)

#define P99_VERTEX_ID(VERTEX) ((size_t)(uintptr_t)((VERTEX)[0][0].p00_car)-1)
#define P99_VERTEX_WEIGHT(VERTEX) ((VERTEX)[0][0].p00_cdr)
#define P99_VERTEX_DEGREE(VERTEX) ((size_t)(uintptr_t)(VERTEX)[0][1].p00_car)
#define P99_VERTEX_EDGES(VERTEX) ((p99_edge*)((VERTEX)[0][1].p00_cdr))

#define P99_GRAPH_VWEIGHT(NAME, I) P99_VERTEX_WEIGHT(&((NAME)[I]))
#define P99_GRAPH_VDEGREE(NAME, I) (P99_VERTEX_DEGREE(&((NAME)[I])))
#define P99_GRAPH_EDGES(NAME, I) (P99_VERTEX_EDGES((NAME)+(I)))
#define P99_GRAPH_EDGE(NAME, I, POS) (P99_GRAPH_EDGES(NAME, I)+(POS))

P99_GRAPH(size_t, double, hey,
          (1, 11),
          (2, 22, (1, 221), (2, 222), (3, 223), (14, 227)),
          (3, 33, (4, 334), (7, 337), (9, 339)),
          (12,),
          (8),
          (9, 99, (10,), (11), (12, 8.9)));

int main(void) {
  printf("number of vertices %zu\n", P99_GRAPH_N(hey));
  P99_DO(size_t, i, 0, P99_GRAPH_N(hey)) {
    size_t * p = P99_GRAPH_VWEIGHT(hey, i);
    size_t deg = P99_GRAPH_VDEGREE(hey, i);
    size_t id = P99_VERTEX_ID(hey+i);
    if (P99_GRAPH_V(hey, i)) {
      if (p) printf("vertex %zu: weight %zu", id, *p);
      else printf("vertex %zu no weight", id);
      printf(",\tdegree %zu\n", deg);
      P99_DO(size_t, j, 0, deg) {
        p99_edge* edge = P99_GRAPH_EDGE(hey, i, j);
        double * weight = P99_EDGE_WEIGHT(edge);
        p99_vertex* vert = P99_EDGE_VERTEX(edge);
        if (weight) printf("\t\t%zu %g\n", P99_VERTEX_ID(vert), *weight);
        else printf("\t\t%zu (no weight)\n", P99_VERTEX_ID(vert));
      }
    }
  }
}
