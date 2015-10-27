/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef TYPE_H
#define TYPE_H

enum type {
    /* undefined */
    TNONE,
    /* action */
    TACTION,
    /* procedure */
    TPROC,
    /* dummy type for procedure arguments */
    TPROCVAR,
    /* relational fluent */
    TFLUENTREL,
    /* functional fluent */
    TFLUENTFUN,
    /* prolog predicate (callable) */
    TPROLOG
};

#endif /* TYPE_H */
