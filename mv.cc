/*
 * Copyright (c) 2014 Jörgen Grahn
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
#include "mv.h"
#include "basename.h"

#include <iostream>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


namespace {

    bool eqpath(const std::string& a, const std::string& b)
    {
	using path::dirname;
	using path::basename;

	return dirname(a)==dirname(b)
	    && basename(a)==dirname(b);
    }
}


/**
 * Similar to mv -i (except doesn't work across file systems).
 * Rename a file without overwriting anything which exists (modulo
 * race conditions).
 *
 * Writes error messages to 'err', amd returns false if the result
 * isn't satisfactory.  oldpath == newpath is not an error.
 */
bool mv(std::ostream& err,
	const std::string& src,
	const std::string& dest)
{
    if(eqpath(src, dest)) {
	/* XXX Should probably return false if src
	 * doesn't exist.
	 */
	return true;
    }

    struct stat st;
    if(lstat(dest.c_str(), &st) == 0) {
	err << src << ": not renaming: \"" << dest << "\" exists\n";
	return false;
    }

    if(rename(src.c_str(), dest.c_str()) != 0) {
	err << src << ": not renaming: " << strerror(errno) << '\n';
	return false;
    }

    return true;
}
