/* This may look like nonsense, but it really is -*- mode: C -*-              */
/*                                                                            */
/* Except for parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                           */
/* all rights reserved,  2011-2012 Jens Gustedt, INRIA, France                */
/*                                                                            */
/* This file is part of the P99 project. You received this file as as         */
/* part of a confidential agreement and you may generally not                 */
/* redistribute it and/or modify it, unless under the terms as given in       */
/* the file LICENSE.  It is distributed without any warranty; without         */
/* even the implied warranty of merchantability or fitness for a              */
/* particular purpose.                                                        */
/*                                                                            */
#ifndef P99_FUTEX_H
#define P99_FUTEX_H

#include "p99_type.h"
#include "p99_threads.h"
#include "p99_posix_default.h"

P99_DECLARE_STRUCT(p99_futex_c11);

#if (defined(__linux__) && !defined(NO_FUTEX)) || defined(DOXYGEN)
typedef _Atomic(unsigned) p99_futex;
#include "p99_futex_linux.h"
#else
typedef p99_futex_c11 p99_futex;
#include "p99_futex_c11.h"
#endif

/**
 ** @addtogroup library_support Compiler and system support features
 ** @{
 **/

/**
 ** @typedef p99_futex
 **
 ** @brief A counter similar to a conditional variable that allows
 ** atomic increment and decrement and to wait for the atomic compare
 ** and exchange for a specific value.
 **
 ** This data structure is inspired by the linux system call @c futex,
 ** thus the name. Linux futex (Fast User muTEX) allows to have a full
 ** fledged control structure on just one @c int. The contents of the
 ** @c int is taken to reflect the state of the structure and its
 ** address and value can be used to block the calling thread until a
 ** certain value is reached. That feature transparently uses the
 ** kernel address of the @c int, so the waiting can be organized even
 ** between different processes that don't share their address space.
 **
 ** Most of the operations operate as their atomic counterparts, with
 ** one exception. They wake up waiters if the value reaches a certain
 ** interval. Therefore they have additional arguments @c p00_cstart,
 ** @c p00_clen and @c p00_wmin @c p00_wmax that give two ranges. One of the
 ** conditional values and one of a range of waiters that should be
 ** woken up.
 **
 ** This is implemented more or less efficiently according to the
 ** features that the platform provides:
 ** - one, for Linux, that uses atomic operations and futexes for signaling
 ** - a fall back that mutexes all access and uses a condition variable for
 **   signaling.
 **
 ** @see P99_FUTEX_COMPARE_EXCHANGE for a generic tool to operate
 ** with ::p99_futex.
 **/




/**
 ** @dfn P99_FUTEX_INITIALIZER(INITIAL)
 ** @brief Initialize an ::p99_futex object.
 ** @related p99_futex
 **/

/**
 ** @brief the maximum number of waiters that an ::p99_futex may have
 ** @related p99_futex
 **/
# define P99_FUTEX_MAX_WAITERS (INT_MAX+0U)


/**
 ** @brief Initialize an ::p99_futex object.
 ** @related p99_futex
 **/
inline p99_futex* p99_futex_init(p99_futex* p00_c, unsigned p00_ini);

/**
 ** @brief Destroy an ::p99_futex object.
 ** @related p99_futex
 **/
inline void p99_futex_destroy(p99_futex* p00_c);

/**
 ** @brief increment the counter of @a p00_fut atomically by @a p00_hmuch.
 **
 ** @return the previous value of the counter
 **
 ** After effecting the operation atomically, some waiters on this
 ** ::p99_futex will be woken up if the new value of the futex is in
 ** the range that is specified by the arguments @a p00_cstart and @a
 ** p00_clen. See ::P99_FUTEX_COMPARE_EXCHANGE for more details on the
 ** rules for the @a p00_wmin and @a p00_wmax counts.
 **
 ** If @a p00_hmuch is @c 0, nothing is added so ::p99_futex_add is
 ** equivalent to ::p99_futex_load but with the additional effect
 ** that waiters might be woken up.
 **
 ** @remark to decrement the counter, just pass a negative argument to
 ** @a p00_hmuch. The modulo arithmetic for @c unsigned will do the rest
 ** for you.
 ** @remark Waiters that wait for a value in the @a p00_clen element
 ** range starting at @a p00_cstart will be woken up if the counter
 ** reaches that value with this operation.
 ** @remark @a p00_clen defaults to @c 1u
 ** @remark @a p00_wmin defaults to @c 0u
 ** @remark @a p00_wmax defaults to ::P99_FUTEX_MAX_WAITERS
 ** @related p99_futex
 **/
inline unsigned p99_futex_add(p99_futex volatile* p00_fut, unsigned p00_hmuch,
                               unsigned p00_cstart, unsigned p00_clen,
                               unsigned p00_wmin, unsigned p00_wmax);

#ifndef DOXYGEN
#define p99_futex_add(...) P99_CALL_DEFARG(p99_futex_add, 6, __VA_ARGS__)
#define p99_futex_add_defarg_3() 1u
#define p99_futex_add_defarg_4() 0u
#define p99_futex_add_defarg_5() P99_FUTEX_MAX_WAITERS
#endif


/**
 ** @brief Obtain the value of futex @a p00_fut atomically.
 ** @related p99_futex
 **/
inline unsigned p99_futex_load(p99_futex volatile* p00_fut);

/**
 ** @brief Unconditionally and atomically set the futex @a p00_fut to
 ** value @a p00_desired.
 **
 ** After effecting the operation atomically, some waiters on this
 ** ::p99_futex will be woken up if the new value of the futex is in
 ** the range that is specified by the arguments @a p00_cstart and @a
 ** p00_clen. See ::P99_FUTEX_COMPARE_EXCHANGE for more details on the
 ** rules for the @a p00_wmin and @a p00_wmax counts.
 **
 ** Also signals eventual waiters if the futex falls within the given range.
 ** @return the previous value of the futex
 ** @remark Waiters that wait for a value in the @a p00_clen element
 ** range starting at @a p00_cstart will be woken up if the counter
 ** reaches that value with this operation.
 ** @remark This signaling of waiters might not be performed if the
 ** counter reached @a p00_cstart because it wrapped around.
 ** @remark @a p00_clen defaults to @c 1u
 ** @remark @a p00_wmin defaults to @c 0u
 ** @remark @a p00_wmax defaults to ::P99_FUTEX_MAX_WAITERS
 ** @related p99_futex
 **/
inline unsigned p99_futex_fetch_and_store(p99_futex volatile* p00_fut, unsigned p00_desired,
    unsigned p00_cstart, unsigned p00_clen,
    unsigned p00_wmin, unsigned p00_wmax);

#ifndef DOXYGEN
#define p99_futex_fetch_and_store(...) P99_CALL_DEFARG(p99_futex_fetch_and_store, 6, __VA_ARGS__)
#define p99_futex_fetch_and_store_defarg_3() 1u
#define p99_futex_fetch_and_store_defarg_4() 0u
#define p99_futex_fetch_and_store_defarg_5() P99_FUTEX_MAX_WAITERS
#endif

/**
 ** @brief wait until the futex @a p00_fut has value @a p00_expected and
 ** change it to @a p00_desired
 **
 ** After effecting the operation atomically, some waiters on this
 ** ::p99_futex will be woken up if the new value of the futex is in
 ** the range that is specified by the arguments @a p00_cstart and @a
 ** p00_clen. See ::P99_FUTEX_COMPARE_EXCHANGE for more details on the
 ** rules for the @a p00_wmin and @a p00_wmax counts.
 **
 ** As long as the futex has not the value @a p00_expected this will block
 ** the calling thread.
 **
 ** @remark Waiters that wait for a value in the @a p00_clen element
 ** range starting at @a p00_cstart will be woken up if the counter
 ** reaches that value with this operation.
 ** @remark @a p00_clen defaults to @c 1u
 ** @remark @a p00_wmin defaults to @c 0u
 ** @remark @a p00_wmax defaults to ::P99_FUTEX_MAX_WAITERS
 **
 ** @see P99_FUTEX_COMPARE_EXCHANGE for a macro with more flexibility
 ** concerning the expect value, the desired value or the p00_wmax
 ** conditions.
 **
 ** @related p99_futex
 **/
inline void p99_futex_compare_exchange(p99_futex volatile* p00_fut, unsigned p00_expected, unsigned p00_desired,
                                        unsigned p00_cstart, unsigned p00_clen,
                                        unsigned p00_wmin, unsigned p00_wmax);

#ifndef DOXYGEN
#define p99_futex_compare_exchange(...) P99_CALL_DEFARG(p99_futex_compare_exchange, 7, __VA_ARGS__)
#define p99_futex_compare_exchange_defarg_4() 1u
#define p99_futex_compare_exchange_defarg_5() 0u
#define p99_futex_compare_exchange_defarg_6() P99_FUTEX_MAX_WAITERS
#endif

/**
 ** @brief Wake up threads that are waiting for a futex
 ** @related p99_futex
 **
 ** Most other ::p99_futex functions or macros also fulfill this
 ** functionality conditionally as a side effect when they change the
 ** value of the futex.
 **
 ** @see P99_FUTEX_COMPARE_EXCHANGE for more detailed explanations.
 **/
inline
void p99_futex_wakeup(p99_futex volatile* p00_fut,
                       unsigned p00_wmin, unsigned p00_wmax);


/**
 ** @brief Unconditionally wait for futex @a p00_fut
 ** @related p99_futex
 **
 ** This deschedules the calling thread until it is woken up by
 ** another thread through ::p99_futex_wakeup or equivalent.
 **
 ** @remark Such a waiting thread will not be subject to spurious wake
 ** ups when receiving signals.
 **/
inline
void p99_futex_wait(p99_futex volatile* p00_fut);



#ifdef DOXYGEN
/**
 ** @brief a catch all macro to operate on ::p99_futex
 ** @related p99_futex
 **
 ** @param FUTEX a pointer that is compatible to <code>p99_futex
 ** volatile*</code>
 **
 ** @param ACT an identifier that can be used in the three following
 ** parameters. If used there it will correspond to local @c register
 ** variable of type <code>unsigned const</code>, that is the variable
 ** can't be modified it and its address can't be taken.
 **
 ** The other parameters are expressions that may contain arbitrary
 ** code that is valid at the point of calling this macro.  The
 ** evaluation may include the local variable @a ACT.
 **
 ** @param EXPECTED an expression that is interpreted as a Boolean
 ** condition. The calling thread will be blocked until this
 ** expression evaluates to @c true.
 **
 ** @param DESIRED an expression that is interpreted as an @c
 ** unsigned. Once the futex fulfills the condition @a EXPECTED the
 ** futex is atomically set to that value if necessary.
 **
 ** @param WAKEMIN an expression that is interpreted as an @c unsigned
 ** that should not exceed ::P99_FUTEX_MAX_WAITERS. The calling
 ** thread will block until it has woken up at least that number of
 ** waiters.
 **
 ** @param WAKEMAX an expression that is interpreted as an @c unsigned
 ** that should not exceed ::P99_FUTEX_MAX_WAITERS. @a WAKEMAX is
 ** adjusted to be at least as large as @a WAKEMIN.
 **
 ** After the futex value has been set atomically to the desired
 ** value, waiters may be woken up. There are three different
 ** situations: If @a WAKEMAX (after adjustment) evaluates to @c 0, no
 ** waiter is woken up. If it is @c 1, and if there are any waiters
 ** exactly one of them is awoken.
 **
 ** For other values, if there are N waiters, at least <code>min(N,
 ** WAKEMAX)</code> are woken, up. If N is less than @a WAKEMIN, the
 ** calling threads blocks until it has been able to wake up the
 ** requested amount.
 **
 ** @warning blocking on the @a WAKEMIN condition can be an active
 ** wait and should be avoided whenever possible.
 **
 ** @warning @a EXPECTED and @a DESIRED may be evaluated several times
 ** if the underlying atomic compare exchange operations fails
 ** temporarily. So you should not have destructive side, here.
 **
 ** To see how to use this, let us look into an implementation of a
 ** semaphore, starting with a "post" operation.
 **
 **  - Such a post operation on a semaphore should not place any
 **    condition to perform the post action so @a EXPECTED should
 **    always be true.
 **
 ** - The operation should increment the value by one, so @a DESIRED
 **   should be <code>ACT + 1</code>.
 **
 ** - It should always wake up potential waiters, so @a WAKEMAX
 **   shouldn't be @c 0.
 **
 ** - It should not wake up more than one waiters to avoid the waiters
 **   racing for one little token that had been placed. So @a WAKEMAX
 **   should be 1.
 **
 ** @code
 ** inline
 ** void my_sem_post(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val, true, val + 1u, 0u, 1u);
 ** }
 ** @endcode
 **
 ** So effectively, such a post operation is just incrementing the
 ** value by one and then it up wakes some waiters. In a real world
 ** implementation this would better be done by using
 ** ::p99_futex_add.
 **
 ** A semaphore wait operation should block if the value is @c 0,
 ** decrement the value by 1 and never wake up any other waiters:
 **
 ** @code
 ** inline
 ** void my_sem_wait(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val, val, val - 1u, 0u);
 ** }
 ** @endcode
 **
 ** A semaphore trywait operation should never block, but only
 ** decrement the value if it is not @c 0, and never wake up any
 ** waiters. A trywait operation must be able to return a value that
 ** indicates success or failure of the operation. We set the return
 ** value by side effect in the @a WAKEMIN expression, where we know
 ** that it is evaluated only once.
 **
 ** @code
 ** inline
 ** int my_sem_trywait(p99_futex volatile* f) {
 **   int ret;
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val, true,
 **                               (val ? 0u : val - 1u),
 **                               (ret = val, 0u),
 **                               0u);
 **   if (ret) {
 **     errno = EAGAIN;
 **     ret = -1;
 **   }
 **   return ret;
 ** }
 ** @endcode
 **
 ** Another example of the use of an ::p99_futex could be a reference
 ** counter. Such a counter can e.g be useful to launch a number of
 ** threads, and then wait for all the threads to have finished.
 **
 ** @code
 ** int launch_my_threads_detached(void *arg) {
 **  p99_futex* fut = arg;
 **  ...
 **  my_counter_unlock(fut);
 **  return 0;
 ** }
 **
 ** p99_futex fut;
 ** p99_futex_init(&fut);
 ** for (size_t i = 0; i < large_number; ++i) {
 **   my_counter_lock(&fut);
 **   thrd_t id;
 **   thrd_create(&id, launch_my_threads_detached, &fut);
 **   thrd_detach(id);
 ** }
 ** ...
 ** my_counter_wait(&fut);
 ** @endcode
 **
 ** For that scheme to work we just need three "counter" functions.
 ** The first just silently piles up references:
 **
 ** @code
 ** inline
 ** void my_counter_lock(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val, true, val + 1u, 0u, 0u);
 ** }
 ** @endcode
 **
 ** So effectively, such an operation is just incrementing the value
 ** by one. In a real world implementation this would better be done
 ** by using ::atomic_fetch_add.
 **
 ** An unlock operation should decrement the value and, if the value
 ** falls to @c 0, wake up all waiters:
 **
 ** @code
 ** inline
 ** void my_counter_lock_unsafe(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val,
 **                               true,
 **                               val - 1u,
 **                               0u,
 **                               ((val == 1u) ? P99_FUTEX_MAX_WAITERS : 0u));
 ** }
 ** @endcode
 **
 ** This unsafe version makes no provision for underflow of the
 ** counter in case these functions are used erroneously. A safer
 ** variant would look like this:
 **
 ** @code
 ** inline
 ** void my_counter_lock(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val,
 **                               true,
 **                               (val ? val - 1u : 0u),
 **                               0u,
 **                               ((val <= 1u) ? P99_FUTEX_MAX_WAITERS : 0u));
 ** }
 ** @endcode
 **
 ** A wait operation should just expect the counter to fall to @c 0
 ** and do not much else.
 **
 ** @code
 ** inline
 ** void my_counter_wait(p99_futex volatile* f) {
 **   P99_FUTEX_COMPARE_EXCHANGE(f, val, 0u, 0u, 0u);
 ** }
 ** @endcode
 **/
#define P99_FUTEX_COMPARE_EXCHANGE(FUTEX, ACT, EXPECTED, DESIRED, WAKEMIN, WAKEMAX)
#endif

inline
void p99_futex_compare_exchange(p99_futex volatile* p00_fut,
                                 unsigned p00_expected, unsigned p00_desired,
                                 unsigned p00_cstart, unsigned p00_clen,
                                 unsigned p00_wmin, unsigned p00_wmax) {
  if (!P99_IN_RANGE(p00_desired, p00_cstart, p00_clen)) {
    p00_wmin = 0;
    p00_wmax = 0;
  }
  P99_FUTEX_COMPARE_EXCHANGE(p00_fut, x,
                              (x == p00_expected),
                              p00_desired,
                              p00_wmin,
                              p00_wmax);
}

/**
 ** @}
 **/

#endif
