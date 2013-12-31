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
#include "riff.h"
#include "littleendian.h"
#include "fmt.h"

#include <iostream>
#include <cassert>



namespace {

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
	      len(le::read32(s+4))
	{}
	Tag tag;
	size_t len;
    };

    unsigned pad(unsigned n)
    {
	return (n%2)? n+1 : n;
    }

    std::istream& read(std::istream& is, unsigned n,
		       std::vector<char>& v, size_t vlen)
    {
	v.resize(n);
	is.read(&v[0], n);
	if(vlen < n) {
	    v.resize(vlen);
	}
	return is;
    }
}


Wave riff(std::istream& is)
{
    Wave w;

    char buf[8];
    is.read(buf, 8);
    assert(is.gcount()==8);
    const TL riff(buf);

    if(riff.tag != "RIFF") return w;
    if(riff.len < 4) return w;
    unsigned len = riff.len;

    is.read(buf, 4);
    assert(is.gcount()==4);
    len -= 4;
    const Tag wave(buf);
    if(wave!="WAVE") return w;

    while(len >= 4+4) {
	is.read(buf, 8);
	assert(is.gcount()==8);
	len -= 4+4;
	const TL tl(buf);
	unsigned n = pad(tl.len);

	if(tl.tag=="fmt ") {
	    read(is, n, w.fmt, tl.len);
	}
	else if(tl.tag=="bext") {
	    read(is, n, w.bext, tl.len);
	}
	else if(tl.tag=="data") {
	    w.datasize += tl.len;
	    is.ignore(n);
	}
	else {
	    is.ignore(n);
	}
	assert(is.gcount()==std::streamsize(n));
	assert(len >= n);
	len -= n;
    }

    return w;
}


/**
 * The duration of the sample, rounded to whole seconds.
 */
unsigned Wave::duration() const
{
    Fmt f;
    if(!parse(f, fmt)) return 0;
    const unsigned bytes_per_sec = f.nSamplesPerSec * f.nChannels * f.wBitsPerSample / 8;

    double s = datasize;
    s = s / bytes_per_sec;

    return s+.5;
}
