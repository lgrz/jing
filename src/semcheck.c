/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "semcheck.h"

/*
 * Check `action` is defined.
 */
bool
semcheck_action_defined(struct symbol *sym)
{
    assert(sym);

    if (TACTION == sym->type) {
        return true;
    }

    return false;
}

/*
 * Check `procedure` is defined.
 */
bool
semcheck_proc_defined(struct symbol *sym)
{
    assert(sym);

    if (TPROC == sym->type) {
        return true;
    }

    return false;
}
