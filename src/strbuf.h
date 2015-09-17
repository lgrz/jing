/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef STRBUF_H
#define STRBUF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "util.h"

struct strbuf {
    char *data;
    int len;
    int cap;
};

struct strbuf *
strbuf_new(void);

void
strbuf_free(struct strbuf *buf);

int
strbuf_append(struct strbuf *buf, const char *fmt, ...);

#endif /* STRBUF_H */
