/*
 * Copyright (c) 1999--2001, 2013 Jörgen Grahn
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
#include "riff.h"

#include <iostream>
#include <cassert>
#include <stdint.h>


namespace {

    unsigned read16le(const char* s)
    {
	const uint8_t* u = reinterpret_cast<const uint8_t*>(s);
	unsigned n = u[0];
	n |= u[1] << 8;
	return n;
    }

    unsigned read32le(const char* s)
    {
	unsigned n = read16le(s);
	n |= read16le(s+2) << 16;
	return n;
    }

    struct Tag {
	explicit Tag(const char* s)
	    : val(s, s+4)
	{}
	bool operator== (const std::string& s) const {
	    return val==s;
	}
	bool operator!= (const std::string& s) const {
	    return !(*this==s);
	}
	std::string val;
    };

    std::ostream& operator<< (std::ostream& os, const Tag tag)
    {
	return os << tag.val;
    }

    struct TL {
	explicit TL(const char* s)
	    : tag(s),
	      len(read32le(s+4))
	{}
	Tag tag;
	size_t len;
    };

    unsigned pad(unsigned n)
    {
	return (n%2)? n+1 : n;
    }
}


int riff(std::ostream& os, std::istream& is)
{
    char buf[8];
    is.read(buf, 8);
    assert(is.gcount()==8);
    const TL riff(buf);

    if(riff.tag != "RIFF") return 1;
    if(riff.len < 4) return 1;
    unsigned len = riff.len;

    os << riff.tag
       << ' ' << riff.len
       << '\n';

    is.read(buf, 4);
    assert(is.gcount()==4);
    len -= 4;
    const Tag wave(buf);
    if(wave!="WAVE") return 1;
    os << wave << '\n';

    while(len >= 4+4) {
	is.read(buf, 8);
	assert(is.gcount()==8);
	len -= 4+4;
	const TL tl(buf);

	os << tl.tag
	   << ' ' << tl.len
	   << '\n';
	unsigned n = pad(tl.len);
	is.ignore(n);
	assert(is.gcount()==n);
	assert(len >= n);
	len -= n;
    }

    return 0;
}
