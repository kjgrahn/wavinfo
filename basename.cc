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
#include "basename.h"

#include <algorithm>
#include <cstring>


namespace {

    const char* find_last(const char* a, const char* b, char c)
    {
	const char* p = &c;
	return std::find_end(a, b, p, p+1);
    }

    /**
     * Trim 'c' from the end of [a, b) but don't make the range
     * empty.  Returns the new 'b'.
     */
    const char* trim_end(const char* a, const char* b, const char c)
    {
	while(b-a > 2) {
	    const char* e = b-1;
	    if(*e!=c) {
		break;
	    }
	    b = e;
	}
	return b;
    }
}


namespace path {

    static const std::string dot(".");

    std::string dirname(const char* a, const char* b)
    {
	if(a==b) return dot;

	b = trim_end(a, b, '/');
	const char* c = find_last(a, b, '/');
	if(c==b) return dot;
	if(c==a) c++;
	b = trim_end(a, c, '/');

	return std::string(a, b);
    }

    std::string basename(const char* a, const char* b)
    {
	if(a==b) return dot;

	b = trim_end(a, b, '/');
	const char* c = find_last(a, b, '/');
	if(c!=b) {
	    a = c+1;
	}
	return std::string(a, b);
    }

    std::string dirname(const std::string& path)
    {
	const char* p = path.c_str();
	return dirname(p, p + path.size());
    }

    std::string dirname(const char* path)
    {
	return dirname(path, path + std::strlen(path));
    }

    std::string basename(const std::string& path)
    {
	const char* p = path.c_str();
	return basename(p, p + path.size());
    }

    std::string basename(const char* path)
    {
	return basename(path, path + std::strlen(path));
    }

    std::string join(const std::string& a,
		     const std::string& b)
    {
	const char* p = a.c_str();
	const char* q = trim_end(p, p+a.size(), '/');
	std::string s(p, q);
	s.push_back('/');
	s += b;
	return s;
    }
}
