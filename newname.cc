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
#include "newname.h"

#include "basename.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <ctime>


namespace {

    const char* find_last(const char* a, const char* b, char c)
    {
	const char* p = &c;
	return std::find_end(a, b, p, p+1);
    }

    /**
     * The start of a trailing subsequence of [a, b) which is only
     * digits.
     */
    const char* digits(const char* a, const char* b)
    {
	const char* p = b;
	while(a<p && std::isdigit(int(*(p-1)))) {
	    p--;
	}
	return p;
    }

    struct Date {
	explicit Date(time_t t);
	explicit Date(const std::string& date);
	unsigned yy;
	unsigned mm;
	unsigned dd;
	std::string str() const;
    };


    Date::Date(time_t t)
    {
	const tm* tt = std::localtime(&t);
	yy = tt->tm_year % 100;
	mm = tt->tm_mon + 1;
	dd = tt->tm_mday;
    }


    Date::Date(const std::string& date)
    {
	assert(date.size()==4+3+3);
	assert(date[4]=='-');
	assert(date[7]=='-');
	const char* const p = date.c_str();
	yy = strtoul(p+2, 0, 10);
	mm = strtoul(p+5, 0, 10);
	dd = strtoul(p+8, 0, 10);
    }


    std::string Date::str() const
    {
	char buf[2+3+3+1];
	std::sprintf(buf, "%02u%02u%02u",
		     yy, mm, dd);
	return buf;
    }


    std::string name(const Date& date,
		     const std::string& s)
    {
	std::string r = date.str();

	const char* p = s.c_str();
	const char* q = find_last(p, p + s.size(), '.');
	p = digits(p, q);
	if(p!=q) {
	    r.push_back('-');
	    r.append(p, q);
	}
	r.append(".wav");

	return r;
    }


    std::string newname(const Date& date, const std::string& path)
    {
	const std::string dir = path::dirname(path);
	return path::join(dir, name(date, path::basename(path)));
    }
}


std::string newname(time_t t, const std::string& path)
{
    return newname(Date(t), path);
}


/**
 * Assuming 'date' is an ISO date (yyyy-mm-dd) and 'path' is a valid
 * path, construct a new path where the basename is based on the date,
 * a serial number from the original path and the extension .wav,
 * e.g. 131216-0002.wav.
 */
std::string newname(const std::string& date, const std::string& path)
{
    return newname(Date(date), path);
}
