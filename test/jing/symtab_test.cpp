/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <CppUTest/TestHarness.h>

extern "C" {
#include "symtab.h"
}

TEST_GROUP(symtab)
{
};

TEST(symtab, can_empty_and_reuse_bucket)
{
    struct symbol *sym;

    sym = symtab_lookup((char *)"one");
    symtab_free(sym);
    sym = symtab_lookup((char *)"one");

    // when the bucket is emptied it should be set back to NULL
    CHECK(NULL == sym->next);

    symtab_free(sym);
}
