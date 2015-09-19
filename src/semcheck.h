/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef SEMCHECK_H
#define SEMCHECK_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "symtab.h"

bool
semcheck_action_defined(struct symbol *sym);

bool
semcheck_proc_defined(struct symbol *sym);

#endif /* SEMCHECK_H */
