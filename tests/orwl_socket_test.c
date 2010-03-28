/*
** orwl_socket_test.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Mar 24 21:50:12 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <signal.h>
#include <stdio.h>
#include "orwl_socket.h"
#include "orwl_register.h"
#include "orwl_wait_queue.h"

#define pthread_mutex_init(...) pthread_mutex_init _call_with(2, __VA_ARGS__)(pthread_mutex_init, __VA_ARGS__)
declare_default_arg(pthread_mutex_init, 1, pthread_mutexattr_t*, NULL);
define_default_arg(pthread_mutex_init, 1, pthread_mutexattr_t*);

#define strtoul(...) strtoul _call_with(3, __VA_ARGS__)(strtoul, __VA_ARGS__)
declare_default_arg(strtoul, 2, int, 0);
declare_default_arg(strtoul, 1, char **, NULL);

define_default_arg(strtoul, 2, int);
define_default_arg(strtoul, 1, char **);

struct orwl_host;

typedef struct orwl_host orwl_host;

struct orwl_host {
  pthread_mutex_t mut;
  orwl_endpoint ep;
  orwl_host *prev;
  orwl_host *next;
  size_t refs;
};

#define ORWL_HOST_STATIC_INITIALIZER(NAME) {    \
  .mut = PTHREAD_MUTEX_INITIALIZER,             \
  .ep = INITIALIZER,                            \
  .prev = &NAME,                                \
  .next = &NAME,                                \
  .refs = 2                                     \
}

orwl_host here = ORWL_HOST_STATIC_INITIALIZER(here);



void orwl_host_connect(orwl_host *th, orwl_host *q) {
  if (!th || !q) return;
  if (th->prev != th || th->next != th) return;
  for (bool ok = false; !ok; ) {
    MUTUAL_EXCLUDE(th->mut) {
      if (!pthread_mutex_trylock(&q->mut)) {
        orwl_host *p = q->prev;
        if (p == q || !pthread_mutex_trylock(&p->mut)) {
          ok = true;
          /* splice us between q and p */
          th->next = q;
          th-> prev = p;
          th->refs += 2;
          q->prev = th;
          p->next = th;
          report(stderr, "%p points to %p and %p", (void*)th, (void*)th->prev, (void*)th->next);
          report(stderr, "%p points to %p and %p", (void*)q, (void*)q->prev, (void*)q->next);
          if (p != q) pthread_mutex_unlock(&p->mut);
        }
        pthread_mutex_unlock(&q->mut);
      }
    }
    if (!ok) {
      sleepfor(1E-0);
      report(stderr,"looping");
    }
  }
}

void orwl_host_disconnect(orwl_host *th) {
  if (!th) return;
  if (th->prev || th->next) return;
  for (bool ok = false; !ok; ) {
    MUTUAL_EXCLUDE(th->mut) {
      orwl_host *q = th->next;
      orwl_host *p = q->prev;
      if (!pthread_mutex_trylock(&q->mut)) {
        if (p == q || !pthread_mutex_trylock(&p->mut)) {
          ok = true;
          th->next = th;
          th->prev = th;
          th->refs -= 2;
          q->prev = p;
          p->next = q;
          if (p != q) pthread_mutex_unlock(&p->mut);
        }
        pthread_mutex_unlock(&q->mut);
      }
    }
    if (!ok) sleepfor(1E-6);
  }
}

void orwl_host_init(orwl_host *th) {
  th->next = th;
  th->prev = th;
  th->refs = 0;
  pthread_mutex_init(&th->mut);
}

void orwl_host_destroy(orwl_host *th) {
  orwl_host_disconnect(th);
}

DECLARE_NEW_DELETE(orwl_host);
DEFINE_NEW_DELETE(orwl_host);

void insert_peer(auth_sock *Arg) {
  struct sockaddr_in addr = INITIALIZER;
  if (getpeername(Arg->fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)}) != -1) {
    report(stderr, "insertion of /%X:0x%X/ ", addr.sin_addr.s_addr, Arg->mes[1]);
  }
  orwl_host *h = orwl_host_new();
  h->ep.addr = addr.sin_addr.s_addr;
  h->ep.port = Arg->mes[1];
  orwl_host_connect(h, &here);
}

void insert_host(auth_sock *Arg) {
  report(stderr, "insertion of /%X:0x%X/ ", Arg->mes[1], Arg->mes[2]);
  orwl_host *h = orwl_host_new();
  h->ep.addr = Arg->mes[1];
  h->ep.port = Arg->mes[2];
  orwl_host_connect(h, &here);
}

void do_nothing(auth_sock *Arg) {
  /* empty */
}

void test_callback(auth_sock *Arg);

DECLARE_ORWL_TYPE_DYNAMIC(auth_sock);

DECLARE_ORWL_REGISTER(insert_peer);
DECLARE_ORWL_REGISTER(insert_host);
DECLARE_ORWL_REGISTER(do_nothing);
DECLARE_ORWL_REGISTER(test_callback);


DEFINE_ORWL_REGISTER_ALIAS(insert_peer, auth_sock);
DEFINE_ORWL_REGISTER_ALIAS(insert_host, auth_sock);
DEFINE_ORWL_REGISTER_ALIAS(do_nothing, auth_sock);
DEFINE_ORWL_REGISTER_ALIAS(test_callback, auth_sock);

DEFINE_ORWL_TYPE_DYNAMIC(auth_sock,
                         ORWL_REGISTER_ALIAS(insert_peer),
                         ORWL_REGISTER_ALIAS(insert_host),
                         ORWL_REGISTER_ALIAS(do_nothing),
                         ORWL_REGISTER_ALIAS(test_callback)
                         );


DEFINE_ORWL_TYPES(ORWL_REGISTER_TYPE(orwl_wh), ORWL_REGISTER_TYPE(auth_sock));


void test_callback(auth_sock *Arg) {
  diagnose(Arg->fd, "message of size %d", Arg->len);
  for (size_t i = 0; i < Arg->len; ++i)
    report(stdout, "%jX", (uintmax_t)Arg->mes[i]);
  orwl_domain_call(ORWL_FTAB(auth_sock), Arg->mes[0], Arg);
}

int main(int argc, char **argv) {
  report(stderr, "starting");
  /* ORWL_TYPE_DYNAMIC_INIT(auth_sock); */
  orwl_types_init();
  orwl_server srv = {
    .ep = { .addr =  orwl_inet_addr(argv[1]), .port = 0 },
    .max_connections = 4,
    .cb = test_callback,
  };
  report(stderr, "starting %jX:0x%jX", srv.ep.addr, srv.ep.port);
  pthread_t id;
  orwl_server_create(&srv, &id);
  report(stderr, "started %jX:0x%jX, got id 0x%jX", srv.ep.addr, srv.ep.port, id);

  rand48_t seed = { srv.ep.addr, srv.ep.port };

  if (argc > 2) {
    in_addr_t addr = orwl_inet_addr(argv[2]);
    uint16_t port = strtoul(argv[3]);


    orwl_endpoint other = { .addr = addr, .port = port };
    uint64_t mess[2] = {  ORWL_OBJID(insert_peer), srv.ep.port };
    /* wait until the other side is up. */
    while (orwl_send(&other, seed, mess, 2)) {
      int ret = pthread_kill(id, 0);
      if (ret) {
        char mesg[256] = INITIALIZER;
        strerror_r(ret, mesg, 256);
        report(stderr, "Server already terminated: %s", mesg);
        break;
      }
      sleepfor(0.2);
    }
    report(stderr, "server %jX:0x%jX is set up", srv.ep.addr, srv.ep.port, id);
  } else {
    report(stderr, "initial server %jX:0x%jX is set up", srv.ep.addr, srv.ep.port, id);
    for (size_t t = 0; t < 1000; ++t) {
      sleepfor(1.0);
      report(stderr, "looping %jd", t);
      orwl_host *n = NULL;
      MUTUAL_EXCLUDE(here.mut)
        n = here.next;
      report(stderr, "%p -- %p", (void*)&here, (void*)n);
      for (orwl_host *h = n; h != &here; ) {
        orwl_endpoint other = INITIALIZER;
        MUTUAL_EXCLUDE(h->mut) {
          other.addr = h->ep.addr;
          other.port = h->ep.port;
          h = h->next;
        }
        uint64_t mess[4] = { ORWL_OBJID(do_nothing), srv.ep.addr, srv.ep.port, t };
        report(stderr, "sending to /%X:0x%X/ ", other.addr, other.port);
        orwl_send(&other, seed, mess, 4);
      }
    }
    //orwl_send(&other, seed, NULL, 0);
  }

  orwl_server_join(id);
  orwl_pthread_wait_detached();
}
