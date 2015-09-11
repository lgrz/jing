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

TEST(symtab, new_symbol_has_undefined_type)
{
    struct symbol *sym;

    sym = symtab_lookup((char *)"dummy");

    CHECK(TNONE == sym->type);

    symtab_free();
}
