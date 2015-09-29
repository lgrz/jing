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

#include "node.h"
#include "symtab.h"

/*
 * Forward declaration
 * FIXME: move error codes to an external module.
 */
#include "y.tab.h"
extern void
yyerrorl(YYLTYPE t, const char *s, ...);


enum error_code {
    ENONE = 0, /* no error */
    E0001, /* unresolved name */
    E0002, /* unresolved action */
    E0003 /* argument count mismatch */
};

bool
semcheck_is_error(enum error_code err);

void
semcheck_errorl(YYLTYPE t, enum error_code err, struct node *n);

enum error_code
semcheck_sym_args(struct node *n);

enum error_code
semcheck_usercall_defined(struct symbol *sym);

enum error_code
semcheck_action_defined(struct symbol *sym);

#endif /* SEMCHECK_H */
