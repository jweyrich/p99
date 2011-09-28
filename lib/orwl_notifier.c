/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_notifier.h"

P99_INSTANTIATE(orwl_notifier*, orwl_notifier_init, orwl_notifier*);
P99_INSTANTIATE(void, orwl_notifier_destroy, orwl_notifier*);
P99_INSTANTIATE(void, orwl_notifier_set, orwl_notifier*);
P99_INSTANTIATE(void, orwl_notifier_unset, orwl_notifier*);
P99_INSTANTIATE(void, orwl_notifier_block, orwl_notifier*);
P99_INSTANTIATE(bool, orwl_notifier_verify, orwl_notifier*);

DEFINE_NEW_DELETE(orwl_notifier);
