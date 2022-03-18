#ifndef _oscompatH
#define _oscompatH

/*
 *  This file is part of abctab2ps
 *  It is necessary for compatibility between 
 *  different operating systems
 */

#include <string>
using namespace std;

/* directory separator */
/* '/' on Unix, Win32 and MacOS X, ':' on MacOs 8/9 */
#ifdef MACINTOSH
  #define DIRSEPCHAR ":"
#else
  #define DIRSEPCHAR "/"
#endif

/* default tabfont search path */
#ifdef MACINTOSH
  #define TABFONTDIRS ":fonts"
#else
  #define TABFONTDIRS "/usr/share/abctab2ps;/usr/local/share/abctab2ps;fonts"
#endif


/* MPW on MacOs 8/9 lacks some Posix stuff */
#ifndef MACINTOSH
  #include <sys/stat.h>
#endif

/* implementation of missing functions in MPW on MacOs 9 */
#ifdef MACINTOSH
  #include "mpw.h"
#endif


/* Borland has io.h instead of unistd.h */
#ifdef __BORLANDC__	
  #include <io.h>
  /* missing macros for access() checks */
  #define	F_OK	0	/* file existence */
  #define	X_OK	1	/* execute permission. */
  #define	W_OK	2	/* write permission */
  #define	R_OK	4	/* read permission */
#else
  #ifndef MACINTOSH
    #include <unistd.h>
  #endif
#endif

/* MingW32 has alloca declared in malloc.h instead of stdlib.h */
#ifdef __MINGW32__
  #include <malloc.h>
#endif

/* get real user name info */
string get_real_user_name();

#endif // _oscompatH


