/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "strbuf.h"

#define STRBUF_SIZE 16
#define STRBUF_MAX_SIZE 0x40000000

/*
 * Create an strbuf.
 */
struct strbuf *
strbuf_new(void)
{
    struct strbuf *buf = bmalloc(sizeof(*buf));

    buf->len = 0;
    buf->cap = STRBUF_SIZE;
    buf->data = bmalloc(STRBUF_SIZE);

    return buf;
}

/*
 * Destroy strbuf.
 */
void
strbuf_free(struct strbuf *buf)
{
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

/*
 * Append formatted output to an `strbuf`.
 */
int
strbuf_append(struct strbuf *buf, const char *fmt, ...)
{
    int left;
    char *ptr;
    va_list ap;

    ptr = buf->data + buf->len;
    do {
        left = buf->cap - (buf->len + 1);

        assert(0 == strlen(ptr));
        assert(left > 0);

        va_start(ap, fmt);
        left -= vsnprintf(ptr, left, fmt, ap);
        va_end(ap);

        if (buf->cap == STRBUF_MAX_SIZE) {
            fprintf(stderr, "strbuf max size reached, characters may be "
                    "discarded\n");
            break;
        }

        if (left <= 0) {
            /* realloc */
            buf->cap *= 2;
            buf->data = brealloc(buf->data, buf->cap);
            ptr = buf->data + buf->len;
            *ptr = '\0';
        } else {
            break;
        }
    } while (1);

    buf->len += strlen(ptr);

    return buf->len;
}
