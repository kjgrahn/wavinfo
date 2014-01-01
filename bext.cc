/*
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "bext.h"
#include "littleendian.h"

#include <iostream>
#include <cassert>


namespace {

    std::string string(int len, const char* a, const char* b)
    {
	assert(b-a >= len);
	b = a + len;
	const char* p = a;
	while(*p && p!=b) p++;
	return std::string(a, p);
    }


    uint64_t read64(const char* s)
    {
	unsigned lo = le::read32(s);
	uint64_t n = le::read32(s+4);
	n = (n << 32) | lo;
	return n;
    }



    bool parse(Bext& val, const char* a, const char* const b)
    {
	using namespace le;

	val.description = string(256, a, b);
	a += 256;
	val.originator = string(32, a, b);
	a += 32;
	val.ref = string(32, a, b);
	a += 32;
	val.date = string(10, a, b);
	a += 10;
	val.time = string(8, a, b);
	a += 8;
	val.time_reference = read64(a);
	a += 8;
	val.version = read16(a);
	a += 2;
	assert(b-a >= 64);
	val.umid = std::vector<uint8_t>(a, a+64);
	a += 64;
	val.loudness.value = read16(a)/100.0;
	a+=2;
	val.loudness.range = read16(a)/100.0;
	a+=2;
	val.loudness.max_true_peak = read16(a)/100.0;
	a+=2;
	val.loudness.max_momentary_loudness = read16(a)/100.0;
	a+=2;
	val.loudness.max_short_term_loudness = read16(a)/100.0;
	a+=2;
	assert(b-a >= 180);
	val.history = string(b-a, a, b);
	return true;
    }
}


bool parse(Bext& val, const std::vector<char>& v)
{
    const char* s = &v[0];
    return parse(val, s, s + v.size());
}


std::ostream& operator<< (std::ostream& os, const Bext& val)
{
    return os <<   "description " << val.description
	      << "\noriginator  " << val.originator
	      << "\nref         " << val.ref
	      << "\ndate        " << val.date
	      << "\ntime        " << val.time
	      << "\ntime ref    " << val.time_reference
	      << "\nversion     " << val.version
	      << "\nloudness   ["
	      << val.loudness.value << ", "
	      << val.loudness.range << ", "
	      << val.loudness.max_true_peak << ", "
	      << val.loudness.max_momentary_loudness << ", "
	      << val.loudness.max_short_term_loudness << "]"
	      << "\nhistory     " << val.history
	      << '\n';
}


std::ostream& tabular(std::ostream& os, const Bext& val)
{
    return os << val;
}
