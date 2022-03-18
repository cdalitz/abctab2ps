
/*
 *  This file is part of abctab2ps
 *  It defines Posix functions missing in MPW on MacOS
 *  and should be compiled only on MacOS 8.x and 9.x
 *
 *  It is not necessary on MacOS X
 */

#include "mpw.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <Types.h>
#include <Files.h>
#include <Timer.h>

/*
 * missing functions from <string.h>
 *-----------------------------------
 */

/* string duplication */
char* strdup(const char* s)
{
  char *res = (char*)malloc(strlen(s) + 1);
  if (!res)
	return (char*)0;
  strcpy(res, s);
  return res;
}


/*
 * missing functions from <sys/stat.h>
 *-----------------------------------
 */

/* get file statistics */
/* taken from libiberty, which is part of the GNU binutils */

/* Bits in ioFlAttrib: */
#define LOCKBIT	(1<<0)		/* File locked */
#define DIRBIT	(1<<4)		/* It's a directory */

int stat (const char *path, struct stat *buf)
{
  CInfoPBRec cipbr;
  HFileInfo *fpb = (HFileInfo*) &cipbr;
  DirInfo *dpb = (DirInfo*) &cipbr;
  Str255 pname;
  short err;
  long dirid = 0L;

  /* Make a temp copy of the name and pascalize. */
  strcpy ((char *) pname, path);
  c2pstr (pname);
  
  cipbr.dirInfo.ioDrDirID = dirid;
  cipbr.hFileInfo.ioNamePtr = pname;
  cipbr.hFileInfo.ioVRefNum = 0;
  cipbr.hFileInfo.ioFDirIndex = 0;
  cipbr.hFileInfo.ioFVersNum = 0;
  err = PBGetCatInfo (&cipbr, 0);
  if (err != noErr)
    {
      errno = ENOENT;
      return -1;
    }
  /* Mac files are readable if they can be accessed at all. */
  buf->st_mode = 0444;
  /* Mark unlocked files as writeable. */
  if (!(fpb->ioFlAttrib & LOCKBIT))
    buf->st_mode |= 0222;
  if (fpb->ioFlAttrib & DIRBIT)
    {
      /* Mark directories as "executable". */
      buf->st_mode |= 0111 | S_IFDIR;
      buf->st_size = dpb->ioDrNmFls;
      /* buf->st_rsize = 0; */
    }
  else
    {
      buf->st_mode |= S_IFREG;
      /* Mark apps as "executable". */
      if (fpb->ioFlFndrInfo.fdType == 'APPL')
	buf->st_mode |= 0111;
      /* Fill in the sizes of data and resource forks. */
      buf->st_size = fpb->ioFlLgLen;
      /* buf->st_rsize = fpb->ioFlRLgLen; */
    }
  /* Fill in various times. */
  buf->st_atime = fpb->ioFlCrDat;
  buf->st_mtime = fpb->ioFlMdDat;
  buf->st_ctime = fpb->ioFlCrDat;
  /* Set up an imitation inode number. */
  buf->st_ino = (unsigned short) fpb->ioDirID;
  /* Set up an imitation device. */
  GetVRefNum (buf->st_ino, &buf->st_dev);
  /* buf->st_uid = __uid; */
  /* buf->st_gid = __gid; */
  buf->st_uid = 0;
  buf->st_gid = 0;
  /* buf->st_FlFndrInfo = fpb->ioFlFndrInfo;  */
  return 0;

}


