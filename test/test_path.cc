/**
 *
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <basename.h>

#include <testicle.h>

namespace split {

    void assert_split(const std::string& a,
		      const char* const b, const char* const c)
    {
	const char* const a0 = a.c_str();
	testicle::assert_eq(path::dirname(a),  b);
	testicle::assert_eq(path::dirname(a0), b);
	testicle::assert_eq(path::basename(a),  c);
	testicle::assert_eq(path::basename(a0), c);
    }

    using testicle::TC;

    void empty(TC)
    {
	assert_split("", ".", ".");
    }

    void simple(TC)
    {
	assert_split("/usr/lib", "/usr", "lib");
    }

    void susv2(TC)
    {
	assert_split("/usr/lib", "/usr", "lib");
	assert_split("/var/",    "/",    "var");
	assert_split("etc",      ".",    "etc");
#if 0
	assert_split("/",        "/",    "/");
#endif
	assert_split(".",        ".",    ".");
	assert_split("..",       ".",    "..");
    }

    void extra(TC)
    {
	assert_split("usr/",     ".",    "usr");
    }

    void triple(TC)
    {
	assert_split("foo/bar/baz",  "foo/bar", "baz");
	assert_split("foo/bar/baz/", "foo/bar", "baz");
    }

    void doubleslash(TC)
    {
	assert_split("foo/bar//",  "foo", "bar");
	assert_split("foo/bar///", "foo", "bar");
	assert_split("foo/bar",    "foo", "bar");
	assert_split("foo//bar",   "foo", "bar");
	assert_split("foo///bar",  "foo", "bar");
    }
}


namespace join {

    using testicle::TC;

    void assert_joins(const std::string& a,
		      const char* const b, const char* const c)
    {
	testicle::assert_eq(a, path::join(b, c));
    }

    void test(TC)
    {
	assert_joins("foo/bar",  "foo", "bar");
	assert_joins("foo/bar",  "foo/", "bar");
	assert_joins("foo/bar/", "foo", "bar/");
    }
}
