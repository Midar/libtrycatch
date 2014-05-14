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

#include <stdio.h>
#include <stdlib.h>

#include "trycatch.h"

#ifndef TRYCATCH_THREAD_LOCAL
# define TRYCATCH_THREAD_LOCAL
#endif

static TRYCATCH_THREAD_LOCAL struct trycatch_frame *trycatch_frames = NULL;

struct trycatch_frame*
trycatch_frame_push()
{
	struct trycatch_frame *frame;

	if ((frame = malloc(sizeof(*frame))) == NULL) {
		fputs("Not enough memory to allocate exception frame!\n",
		    stderr);
		abort();
	}

	frame->prev = trycatch_frames;
	trycatch_frames = frame;

	return frame;
}

void
trycatch_frame_pop()
{
	struct trycatch_frame *frame = trycatch_frames;
	trycatch_frames = frame->prev;
	free(frame);
}

void _Noreturn
trycatch_throw(int ex)
{
	if (trycatch_frames == NULL) {
		fputs("Exception with no handler!\n", stderr);
		abort();
	}

	longjmp(*trycatch_frames->jmpbuf, ex);
}
