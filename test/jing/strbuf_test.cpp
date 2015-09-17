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
#include "strbuf.h"
}

TEST_GROUP(strbuf)
{
    struct strbuf *buf;

    void setup()
    {
        buf = strbuf_new();
    }

    void teardown()
    {
        strbuf_free(buf);
        buf = NULL;
    }
};

TEST(strbuf, new_strbuf_is_empty)
{
    STRCMP_EQUAL("", buf->data);
}

TEST(strbuf, can_append_to_strbuf)
{
    strbuf_append(buf, "hello");
    STRCMP_EQUAL("hello", buf->data);
    LONGS_EQUAL(5, buf->len);
    LONGS_EQUAL(16, buf->cap);
}

TEST(strbuf, can_grow_strbuf)
{
    const char *eleven_bytes = "1234567890";
    const char *seven_bytes = "123456";

    strbuf_append(buf, eleven_bytes);
    strbuf_append(buf, seven_bytes);

    STRCMP_EQUAL("1234567890123456", buf->data);
    LONGS_EQUAL(16, buf->len);
    LONGS_EQUAL(32, buf->cap);
}

TEST(strbuf, can_grow_strbuf_fmt)
{
    const char *name = "some_procedure_name";

    strbuf_append(buf, "proc(%s, ", name);
    strbuf_append(buf, "[%s, %s, %s, %s]", "up", "down", "left", "right");
    strbuf_append(buf, ").");

    STRCMP_EQUAL("proc(some_procedure_name, [up, down, left, right]).",
            buf->data);
    LONGS_EQUAL(51, buf->len);
    LONGS_EQUAL(64, buf->cap);
}
