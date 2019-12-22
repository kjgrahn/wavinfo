/*
 * Copyright (c) 2013, 2014 Jörgen Grahn
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
#include "fmt.h"
#include "bext.h"
#include "newname.h"
#include "basename.h"
#include "mv.h"

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <cassert>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


namespace {

    time_t mtime(const char* filename)
    {
	struct stat buf;
	if(stat(filename, &buf)) {
	    return 0;
	}
	return buf.st_mtime;
    }


    std::ostream& verbose_print(std::ostream& os,
				const std::string& filename,
				const Wave& wave)
    {
	os << filename << '\n';
	wave.tabular(os);

	Bext bext;
	if(!wave.bext.empty() && parse(bext, wave.bext)) {
	    tabular(os, bext);
	}

	return os;
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-v] file ...\n"
	"       "
	+ prog + " -e file ...\n"
	"       "
	+ prog + " --version\n"
	"       "
	+ prog + " --help";
    const char optstring[] = "ve";
    const struct option long_options[] = {
	{"version", 0, 0, 'V'},
	{"help", 0, 0, 'H'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    bool verbose = false;
    bool rename = false;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring,
			    &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'v':
	    verbose = true;
	    rename = false;
	    break;
	case 'e':
	    verbose = false;
	    rename = true;
	    break;
	case 'V':
	    std::cout << prog << " 1.0\n"
		      << "Copyright (c) 2013, 2014 Jörgen Grahn\n";
	    return 0;
	    break;
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    if(argv+optind == argv+argc) {
	std::cerr << usage << '\n';
	return 1;
    }

    int rc = 0;
    const bool onefile = (argv+optind+1 == argv+argc);

    char** p = argv+optind;
    while(p != argv+argc) {
	const bool first = (p==argv+optind);
	const char* const filename = *p++;

	std::ifstream is(filename);
	if(!is) {
	    std::cerr << filename << ": "
		      << strerror(errno) << '\n';
	    rc = 1;
	    continue;
	}
	const Wave w = riff(is);

	if(!w.valid()) {
	    std::cerr << filename << ": not a WAVE file\n";
	    rc = 1;
	    continue;
	}

	if(verbose) {
	    if(!onefile && !first) {
		std::cout << '\n';
	    }
	    verbose_print(std::cout, filename, w);
	}
	else {
	    Bext bext;
	    if(!w.bext.empty()) {
		parse(bext, w.bext);
	    }

	    const string name = bext.date.empty() ?
		newname(mtime(filename), filename) :
		newname(bext.date, filename);
	    std::cout << path::basename(name) << '\n';

	    if(!bext.date.empty()) {
		std::cout << bext.date << ' '
			  << bext.time << '\n';
	    }
	    std::cout << w.duration() << " seconds\n"
		      << '\n';

	    if(rename && !mv(std::cerr, filename, name)) {
		rc = 1;
	    }
	}
    }

    return rc;
}
