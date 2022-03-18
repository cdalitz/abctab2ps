abctab2ps
=========

*abctab2ps* is a music and tablature typesetting program which translates an
input file in the abc language into postscript. It is based on Michael
Methfessel's program *abc2ps*. While abc2ps can only typeset music, abctab2ps
is an extension that can also handle lute tablature.


Installation from source
------------------------

Prerequisites:

  - C++98 compiler. Compilation has been tested
    with clang on MacOS X, gcc on Linux, and MingW on Windows
  - GNU make

To compile *abctab2ps* from the source code package, change to the subdirectory
*src* and type *make*. To install it on Linux, type ($ is the shell prompt):

	$ sudo make install

If you install the abctab2ps fonts in some other directory than
/usr/share/abctab2ps or /usr/local/share/abctab2ps, you must set the
environment variable ABCTABFONTS to the full path of the directory
containing the tablature font files.

If you ever need to uninstall *abctab2ps*, type *sudo make uninstall*.
Alternatively, you can use the ESP Package Manager *epm* (see
<https://www.msweet.org/epm/>), to create a RPM or DEB package and
install it (replace "platform" with your processor type and "x.y.z"
with the current version of abctab2ps):

    $ # on rpm based systems like Red Hat or Suse:
    $ make rpm
	$ sudo rpm -i linux-platform/abctab2ps-x.y.z.rpm

    $ # on deb based systems like Debian or Ubuntu:
    $ sudo make deb
	$ sudo dpkg -i linux-platform/abctab2ps-x.y.z.rpm

Note that abctab2ps will create a *postscript* file. This means that
you will additionally need a postscript interpreter like [ghostscript](https://www.ghostscript.com/)
to convert the PS file to PDF or to view it directly.



Binary packages
---------------

File releases and precompiled binary installers for different platforms are
available from http://www.lautengesellschaft.de/cdmm/.


Usage and Documentation
-----------------------

In order to create a music/tablature score you must perform
the following steps:

1. Start an editor, enter the music in abc language and save 
   it under a filename with the extension ".abc", eg. "bla.abc".

2. Switch to the command line and call abctab2ps on that file, 
   eg. `abctab2ps bla.abc`. This will create the score as 
   postscript graphics in the file "Out.ps".

You will find the necessary documentation for both steps in the
subdirectory *doc*:

1. The User's Guide in the subdirectory *doc/userguide*
   describes the abc language and is essential for step 1.
   There is both a PDF and a HTML version.

2. The manual page in the subdirectory *doc/manpages* describes 
   the invocation of abctab2ps and all command line parameters. 
   It is essential for step 2. There is both a html version
   and a nroff version (for online reference with "man").
   If you run Unix and the installation with "make install" was 
   successful, you should be able to read the man page with 
   `man abctab2ps`.

3. Some illustrative examples are in the subdirectory *doc/examples*.


Copyright and author
--------------------

Copyright (c) 1999-2022 by Christoph Dalitz

Official home page of abctab2ps and the integrated editior flabc:
<http://www.lautengesellschaft.de/cdmm/>

For licensing information, see the file LICENSE for details.
