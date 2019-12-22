/**
 *
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <newname.h>
#include <basename.h>

#include <orchis.h>

using orchis::TC;

namespace name_date {

    void assert2014(const char* orig, const char* s)
    {
	const std::string s2 = newname("2014-01-01", orig);
	orchis::assert_eq(path::basename(s2), s);
    }

    void simple(TC)
    {
	assert2014("zoom0007.wav", "140101-0007.wav");
    }

    void ucase(TC)
    {
	assert2014("zoom0007.WAV", "140101-0007.wav");
    }

    void extension(TC)
    {
	assert2014("zoom0007.foo", "140101-0007.wav");
	assert2014("zoom0007.",    "140101-0007.wav");
	assert2014("zoom0007",     "140101-0007.wav");
	assert2014("zoom.0007.fo", "140101-0007.wav");
	assert2014("zoo.m0007.fo", "140101-0007.wav");
	assert2014(".zoom0007.fo", "140101-0007.wav");
    }

    void digits(TC)
    {
	assert2014("zoom0007.wav", "140101-0007.wav");
	assert2014("zoom007.wav", "140101-007.wav");
	assert2014("zoom1987007.wav", "140101-1987007.wav");
	assert2014("zoom000.wav", "140101-000.wav");
	assert2014("zoom0.wav", "140101-0.wav");
    }

    void no_digits(TC)
    {
	assert2014("zoom.wav", "140101.wav");
    }
}


namespace name_path {

    void assert2014(const char* orig, const char* s)
    {
	const std::string s2 = newname("2014-01-01", orig);
	orchis::assert_eq(path::dirname(s2),
			    path::dirname(s));
	orchis::assert_eq(path::basename(s2),
			    path::basename(s));
    }

    void simple(TC)
    {
	assert2014("zoom0007.wav", "./140101-0007.wav");
	assert2014("/zoom0007.wav", "/140101-0007.wav");
	assert2014("foo/zoom0007.wav", "foo/140101-0007.wav");
	assert2014("../zoom0007.wav", "../140101-0007.wav");
    }
}
