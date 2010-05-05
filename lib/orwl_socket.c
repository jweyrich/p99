/*
** orwl_socket.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 22:00:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "orwl_socket.h"
#include "orwl_rand.h"
#include "orwl_posix_default.h"
#include "orwl_remote_queue.h"

static uint32_t mycode = 0;

typedef uint32_t U;
#define Code(C0, C1, C2, C3) ((((U)C0) << 0) | (((U)C1) << 8) | (((U)C2) << 16) | (((U)C3) << 24))

#define ifCode(C0, C1, C2, C3)                          \
do {                                                    \
  static uint32_t const c = Code(C0, C1, C2, C3);       \
  if (ntohl(c) == Code(0, 1, 2, 3)) {                   \
    mycode = c;                                         \
    goto END;                                           \
  }                                                     \
 } while (false)


DECLARE_ONCE_UPON(mycode);
DEFINE_ONCE_UPON(mycode) {
    ifCode(0, 1, 2, 3);
    ifCode(0, 1, 3, 2);
    ifCode(0, 2, 1, 3);
    ifCode(0, 2, 3, 1);
    ifCode(0, 3, 1, 2);
    ifCode(0, 3, 2, 1);

    ifCode(1, 0, 2, 3);
    ifCode(1, 0, 3, 2);
    ifCode(1, 2, 0, 3);
    ifCode(1, 2, 3, 0);
    ifCode(1, 3, 0, 2);
    ifCode(1, 3, 2, 0);

    ifCode(2, 0, 1, 3);
    ifCode(2, 0, 3, 1);
    ifCode(2, 1, 0, 3);
    ifCode(2, 1, 3, 0);
    ifCode(2, 3, 0, 1);
    ifCode(2, 3, 1, 0);

    ifCode(3, 0, 1, 2);
    ifCode(3, 0, 2, 1);
    ifCode(3, 1, 0, 2);
    ifCode(3, 1, 2, 0);
    ifCode(3, 2, 0, 1);
    ifCode(3, 2, 1, 0);
 END:;
}

bool same_endianess(uint32_t c) {
  INIT_ONCE_UPON(mycode, mycode);
  return c == mycode;
}

void orwl_hton(uint32_t *n, uint64_t const *h, size_t l);
DEFINE_DEFARG(orwl_hton, , , 1);
void orwl_ntoh(uint64_t* h, uint32_t const *n, size_t l);
DEFINE_DEFARG(orwl_ntoh, , , 1);

static
bool orwl_send_(int fd, uint64_t const*mess, size_t len) {
  uint32_t *buf = uint32_t_vnew(2 * len);
  orwl_hton(buf, mess, len);
  ssize_t ret = send(fd, buf, sizeof(uint64_t) * len, MSG_WAITALL);
  uint32_t_vdelete(buf);
  return ret != sizeof(uint64_t) * len;
}

static
bool orwl_recv_(int fd, uint64_t *mess, size_t len) {
  uint32_t *buf = uint32_t_vnew(2 * len);
  ssize_t ret = recv(fd, buf, sizeof(uint64_t) * len, MSG_WAITALL);
  orwl_ntoh(mess, buf , len);
  uint32_t_vdelete(buf);
  return ret != sizeof(uint64_t) * len;
}


in_addr_t _inet4_addr = INITIALIZER;

DEFINE_ONCE_UPON(inet4_addr) {
  char const* str = getenv("INET4");
  struct in_addr inaddr = INITIALIZER;
  if (inet_aton(str, &inaddr)) {
    _inet4_addr = inaddr.s_addr;
  }
}

in_addr_t inet4_addr(void);

addr_t* addr_t_init(addr_t *A, in_addr_t I);

DEFINE_DEFARG(addr_t_init, , TNULL(in_addr_t));

in_addr_t addr2net(addr_t const*A);

port_t* port_t_init(port_t *A, in_port_t P);

DEFINE_DEFARG(port_t_init, , TNULL(in_port_t));

in_port_t port2net(port_t const*A);





orwl_endpoint* orwl_endpoint_init
     (orwl_endpoint *endpoint,
      in_addr_t addr,
      in_port_t port
      );

void orwl_endpoint_destroy(orwl_endpoint *endpoint);


DEFINE_DEFARG(orwl_endpoint_init, , TNULL(in_addr_t), TNULL(in_port_t));

DEFINE_NEW_DELETE(orwl_endpoint);

enum { header_t_els = 2 };

typedef uint64_t header_t[header_t_els];

void orwl_ntoa(struct sockaddr_in *addr, char *name) {
  char *tmp = inet_ntoa(addr->sin_addr);
  strcpy(name, tmp);
  size_t len = strlen(name);
  name[len] = ':';
  sprintf(name + len + 1, "0x%X", (unsigned)addr->sin_port);
}

static
unsigned importance(in_addr_t a) {
  unsigned ret = ~0u;
  uint32_t h = ntohl(a);
  uchar high = (h >> 24);
  uchar low = (h >> 16) & 0xFF;
  switch (high) {
  case 0:
    ret = 0;
    break;
    /* The loopback network */
  case 127:
    ret = 1;
    break;
    /* Link local addresses */
  case 169:
    ret = (low == 254) ? 2 : ~0u;
    break;
    /* The private /16 networks */
  case 192:
    ret = (low == 168) ? 3 : ~0u;
    break;
    /* The private /12 network */
  case 172:
    ret = (16 <= low && low <= 31) ? 4 : ~0u;
    break;
    /* The private /8 network */
  case 10:
    ret = 5;
    break;
  default:;
    ret = ~0u;
  }
  return ret;
}

char const* orwl_inet_ntop(struct sockaddr const* addr, char* buf, size_t size);
DEFINE_DEFARG(orwl_inet_ntop, , , );

in_addr_t orwl_inet_addr(char const *name) {
  in_addr_t ret = INITIALIZER;
  struct addrinfo *res = INITIALIZER;
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_flags = AI_V4MAPPED | AI_ADDRCONFIG | AI_CANONNAME | (name ? 0 : AI_PASSIVE),
    .ai_socktype = SOCK_STREAM,
  };
  getaddrinfo(name, NULL, &hints, &res);
  report(1, "%s's canonical name is %s",
         (name ? name : "<unspecific>"),
         (res ? res->ai_canonname : "unknonwn"));
  for (struct addrinfo *p = res; p; p = p->ai_next) {
    report(1, "%s's inet address is %s",
           (name ? name : "<unspecific>"),
           orwl_inet_ntop(p->ai_addr));
    switch (p->ai_family) {
    case AF_INET: {
      struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
      assert(addr->sin_family == AF_INET);
      struct in_addr* act = &(addr->sin_addr);
      if (importance(act->s_addr) > importance(ret)) {
        ret = act->s_addr;
      }
    }
      break;
    default:;
    }
  }
  freeaddrinfo(res);
  return ret;
}

auth_sock* auth_sock_init(auth_sock *sock,
                                  int fd,
                                  orwl_server* srv,
                                  size_t len);

DEFINE_DEFARG(auth_sock_init, , -1, NULL, TNULL(size_t));

void auth_sock_close(auth_sock *sock) {
  /* Ack the termination of the call */
  header_t header = { sock->ret };
  orwl_send_(sock->fd, header, header_t_els);
  close(sock->fd);
  sock->fd = -1;
}

void auth_sock_destroy(auth_sock *sock) {
  if (sock->fd != -1) auth_sock_close(sock);
  if (sock->back) uint64_t_vdelete(sock->back);
  auth_sock_init(sock);
}

DEFINE_NEW_DELETE(auth_sock);


DEFINE_THREAD(auth_sock) {
  assert(Arg->mes);
  if (!orwl_recv_(Arg->fd, Arg->mes, Arg->len))
    if (Arg->srv && Arg->srv->cb) {
      /* do something with mess here */
      Arg->srv->cb(Arg);
      //report(1, "finished callback with %zd elements", Arg->len);
    }
  if (Arg->fd != -1) auth_sock_close(Arg);
}

orwl_server* orwl_server_init(orwl_server *serv);

void orwl_server_close(orwl_server *serv) {
  MUTUAL_EXCLUDE(serv->host.mut)
    if (serv->fd_listen != -1) {
      int fd = serv->fd_listen;
      serv->fd_listen = -1;
      close(fd);
    }
}


void orwl_server_terminate(orwl_server *serv, rand48_t *seed) {
  orwl_send(&serv->host.ep, seed, NULL, 0);
}

DEFINE_DEFARG(orwl_server_terminate, , seed_get());


void orwl_server_destroy(orwl_server *serv) {
  orwl_server_close(serv);
  orwl_host *n = NULL;
  MUTUAL_EXCLUDE(serv->host.mut)
    n = serv->host.next;
  for (orwl_host *h = n; h != &serv->host; ) {
    MUTUAL_EXCLUDE(h->mut) {
      n = h;
      h = h->next;
    }
    orwl_host_delete(n);
  }
}

DEFINE_NEW_DELETE(orwl_server);

DEFINE_THREAD(orwl_server) {
  report(1, "starting server");
  Arg->fd_listen = socket(AF_INET);
  if (Arg->fd_listen != -1) {
    report(1, "found %" PRIX32 ":%" PRIX16,
           addr2net(&Arg->host.ep.addr),
           port2net(&Arg->host.ep.port));
    rand48_t seed = RAND48_T_INITIALIZER;
    struct sockaddr_in addr = {
      .sin_addr = { .s_addr = addr2net(&Arg->host.ep.addr), },
      .sin_port = port2net(&Arg->host.ep.port),
      .sin_family = AF_INET,
    };
    socklen_t len = sizeof(addr);
    if (bind(Arg->fd_listen, (struct sockaddr*) &addr, sizeof(addr)) == -1)
      goto TERMINATE;
    report(1, "bound port 0x%" PRIX32, port2net(&Arg->host.ep.port));
    /* If the port was not yet specified find and store it. */
    if (!addr.sin_port) {
      if (getsockname(Arg->fd_listen, (struct sockaddr*)&addr, &len) == -1)
        goto TERMINATE;
      port_t_init(&Arg->host.ep.port, addr.sin_port);
      report(1, "allocated port 0x%" PRIX32, port2net(&Arg->host.ep.port));
    }
    if (listen(Arg->fd_listen, Arg->max_connections) == -1)
      goto TERMINATE;
    char info[256];
    uint64_t t = 0;
    snprintf(info, 256, "server at /0x%" PRIX32 ":0x%" PRIX16 "/",
             addr2net(&Arg->host.ep.addr),
             port2net(&Arg->host.ep.port));
    while (Arg->fd_listen != -1) {
      /* Do this work before being connected */
      uint64_t chal = orwl_rand64(&seed);
      uint64_t repl = orwl_challenge(chal);
      header_t header = INITIALIZER;

      progress(1, ++t, "%s", info);

      if (!repl) {
        report(1, "cannot serve without a secret");
        close(Arg->fd_listen);
        Arg->fd_listen = -1;
        goto TERMINATE;
      }
      int fd = -1;
      do {
        fd = accept(Arg->fd_listen);
      } while(fd == -1);

      /* Receive a challenge from the new connection */
      if (orwl_recv_(fd, header, header_t_els))
        goto FINISH;
      header[1] = orwl_challenge(header[0]);
      header[0] = chal;
      /* Send the reply and a new challenge to the new connection */
      if (orwl_send_(fd, header, header_t_els))
        goto FINISH;
      /* Receive the reply and the message length from the new connection */
      if (orwl_recv_(fd, header, header_t_els))
        goto FINISH;
      if (header[1] == repl) {
        size_t len = header[0];
        if (len) {
          auth_sock *sock = NEW_INIT(auth_sock, fd, Arg, len);
          auth_sock_create(sock, NULL);
          /* The spawned thread will close the fd. */
          continue;
        } else {
          /* The authorized empty message indicates termination */
          diagnose(fd, "Received termination message, closing fd %d", fd);
          close(fd);
          close(Arg->fd_listen);
          Arg->fd_listen = -1;
          break;
        }
      } else {
        diagnose(fd, "You are not authorized to to talk on fd %d", fd);
      }
    FINISH:
      close(fd);
    }
  }
 TERMINATE:
  if (errno) {
    perror("cannot proceed");
  }
  if (Arg->fd_listen != -1) close(Arg->fd_listen);
}

uint64_t orwl_send(orwl_endpoint const* ep, rand48_t *seed, uint64_t* mess, size_t len) {
  uint64_t ret = TONES(uint64_t);
  int fd = -1;
  /* do all this work before opening the socket */
  uint64_t chal = orwl_rand64(seed);
  uint64_t repl = orwl_challenge(chal);
  header_t header = { [0] = chal };
  struct sockaddr_in addr = {
    .sin_addr = { .s_addr = addr2net(&ep->addr) },
    .sin_port = port2net(&ep->port),
    .sin_family = AF_INET
  };
  if (!repl) {
    report(1, "cannot send without a secret");
    goto FINISH;
  }

  fd = socket(PF_INET, SOCK_STREAM, 0);
  if (fd == -1) return ret;

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    goto FINISH;
  /* Send a challenge to the other side */
  if (orwl_send_(fd, header, header_t_els))
    goto FINISH;
  /* Receive the answer and another challenge from the other side */
  if (orwl_recv_(fd, header, header_t_els))
    goto FINISH;
  if (header[1] == repl) {
    /* The other side is authorized. Send the answer and the size of
       the message to the other side. */
    header[1] = orwl_challenge(header[0]);
    header[0] = len;
    if (orwl_send_(fd, header, header_t_els))
      goto FINISH;
    /* The authorized empty message indicates termination.
       If not so, we now send the real message. */
    if (len) {
      if (orwl_send_(fd, mess, len))
        goto FINISH;
      /* Receive a final message, until the other end closes the
         connection. */
      if (!orwl_recv_(fd, header, header_t_els)) {
        ret = header[0];
        goto FINISH;
      } else {
        report(1, "terminal reception not successful");
      }
    }
    ret = 0;
  } else  {
    /* The other side is not authorized. Terminate. */
    diagnose(fd, "fd %d, you are not who you pretend to be", fd);
    header[1] = 0;
    header[0] = 0;
    if (orwl_send_(fd, header, header_t_els))
      goto FINISH;
  }
 FINISH:
  close(fd);
  if (ret == TONES(uint64_t) && len) report(1, "send request didn't succeed");
  return ret;
}

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
          if (p != q) pthread_mutex_unlock(&p->mut);
        }
        pthread_mutex_unlock(&q->mut);
      }
    }
    if (!ok) {
      sleepfor(1E-0);
      report(1,"looping");
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

orwl_host* orwl_host_init(orwl_host *th, in_addr_t addr, in_port_t port);

DEFINE_DEFARG(orwl_host_init, , TNULL(in_addr_t), TNULL(in_port_t));


void orwl_host_destroy(orwl_host *th);


DEFINE_NEW_DELETE(orwl_host);

addr_t getpeer(auth_sock *Arg) {
  struct sockaddr_in addr = INITIALIZER;
  int ret = getpeername(Arg->fd, (struct sockaddr*)&addr, &(socklen_t){sizeof(struct sockaddr_in)});
  return (ret == -1)
    ? TNULL(addr_t)
    : (addr_t)ADDR_T_INITIALIZER(addr.sin_addr.s_addr);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_peer, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_peer, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes and addr_t is already in host order */
  h->ep.addr = getpeer(Arg);
  h->ep.port.p = port;
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_insert_host, uint64_t addr, uint64_t port) {
  AUTH_SOCK_READ(Arg, auth_sock_insert_host, uint64_t addr, uint64_t port);
  orwl_host *h = NEW(orwl_host);
  /* mes is already in host order */
  h->ep.addr.a = addr;
  h->ep.port.p = port;
  orwl_host_connect(h, &Arg->srv->host);
}

DEFINE_AUTH_SOCK_FUNC(auth_sock_do_nothing, void) {
  /* empty */
}

DEFINE_ORWL_TYPE_DYNAMIC(auth_sock,
                         ORWL_REGISTER_ALIAS(auth_sock_insert_peer),
                         ORWL_REGISTER_ALIAS(auth_sock_insert_host),
                         ORWL_REGISTER_ALIAS(auth_sock_do_nothing),
                         ORWL_REGISTER_ALIAS(auth_sock_request_excl),
                         ORWL_REGISTER_ALIAS(auth_sock_request_incl),
                         ORWL_REGISTER_ALIAS(auth_sock_release)
                         );
