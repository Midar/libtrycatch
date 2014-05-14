/*
 * Copyright (c) 2014, Jonathan Schleifer <js@webkeks.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TRYCATCH_H__
#define __TRYCATCH_H__

#include <setjmp.h>
#include <stdnoreturn.h>

#define TRYCATCH_CONCAT2(a, b) a ## b
#define TRYCATCH_CONCAT(a, b) TRYCATCH_CONCAT2(a, b)
#ifndef TRYCATCH_NORETURN
# define TRYCATCH_NORETURN
#endif
#define try								   \
	struct trycatch_frame *TRYCATCH_CONCAT(trycatch_frame, __LINE__);  \
	jmp_buf TRYCATCH_CONCAT(trycatch_jmpbuf, __LINE__);		   \
	TRYCATCH_CONCAT(trycatch_frame, __LINE__) = trycatch_frame_push(); \
	trycatch_ex = setjmp(TRYCATCH_CONCAT(trycatch_jmpbuf, __LINE__));  \
	if (trycatch_ex == 0)						   \
		TRYCATCH_CONCAT(trycatch_frame, __LINE__)->jmpbuf =	   \
		    &TRYCATCH_CONCAT(trycatch_jmpbuf, __LINE__);	   \
	if (trycatch_ex == 0)
#define catch(ex)							\
	trycatch_frame_pop();						\
	for (int ex = trycatch_ex; trycatch_ex != 0; trycatch_ex = 0)
#define throw(ex) trycatch_throw(ex)
#define rethrow trycatch_throw(trycatch_ex)

struct trycatch_frame {
	jmp_buf *jmpbuf;
	struct trycatch_frame *prev;
};

static int trycatch_ex;

struct trycatch_frame *trycatch_frame_push(void);
void trycatch_frame_pop(void);
void TRYCATCH_NORETURN trycatch_throw(int ex);

#endif
