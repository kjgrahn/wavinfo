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
#include "fmt.h"
#include "littleendian.h"

#include <iostream>

namespace {

    bool parse(Fmt& fmt, const char* v)
    {
	using namespace le;

	fmt.wFormatTag = read16(v); v+=2;
	fmt.nChannels = read16(v); v+=2;
	fmt.nSamplesPerSec = read32(v); v+=4;
	fmt.nAvgBytesPerSec = read32(v); v+=4;
	fmt.nBlockAlign = read16(v); v+=2;
	fmt.wBitsPerSample = read16(v); v+=2;
	return true;
    }
}


bool parse(Fmt& fmt, const std::vector<char>& v)
{
    return v.size()<16 ? false : parse(fmt, &v[0]);
}


std::ostream& operator<< (std::ostream& os, const Fmt& val)
{
    return os << '[' << val.wFormatTag
	      << ' ' << val.nChannels
	      << ' ' << val.nSamplesPerSec
	      << ' ' << val.nAvgBytesPerSec
	      << ' ' << val.nBlockAlign
	      << ' ' << val.wBitsPerSample
	      << ']';
}
