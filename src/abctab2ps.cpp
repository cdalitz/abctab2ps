/*
 *  abctab2ps: a program to typeset tunes in abc format using PostScript
 *  Copyright (C) 1996 - 1998  Michael Methfessel
 *  Copyright (C) 1999 - 2019  Christoph Dalitz
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  The author can be contacted as follows:
 *  
 *  Christoph Dalitz
 *  <christoph (dot) dalitz (at) hs-niederrhein (dot) de>
 *
 */

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "abctab2ps.h"
#include "subs.h"
#include "format.h"
#include "util.h"
#include "parse.h"
#include "music.h"
#include "oscompat.h"


/* ----- definitions of global variables ----- */

int db=DEBUG_LV;                  /* debug control */
int maxSyms,maxVc;                /* for malloc */
int allocSyms,allocVc;
struct ISTRUCT info,default_info; /* information fields */
char lvoiceid[STRLINFO];          /* string from last V: line */
int  nvoice,mvoice;               /* number of voices defd, nonempty */
int  ivc;                         /* current voice */
int  ivc0;                        /* top nonempty voice */
int ixpfree;                      /* first free element in xp array */
struct METERSTR default_meter;    /* data to specify the meter */
struct KEYSTR default_key;        /* data to specify the key */
                          /* things to alloc: */
struct SYMBOL  *sym;              /* symbol list */
struct SYMBOL  **symv;            /* symbols for voices */
struct XPOS    *xp;               /* shared horizontal positions */
struct VCESPEC *voice;            /* characteristics of a voice */
struct SYMBOL  **sym_st;          /* symbols a staff start */
int            *nsym_st; 

int halftones;                    /* number of halftones to transpose by */

float f0p,f5p,f1p,f0x,f5x,f1x;            /*   mapping fct */
float lnnp,bnnp,fnnp,lnnx,bnnx,fnnx;      /*   note-note spacing */
float lbnp,bbnp,rbnp,lbnx,bbnx,rbnx;      /*   bar-note spacing */
float lnbp,bnbp,rnbp,lnbx,bnbx,rnbx;      /*   note-bar spacing */


char* wpool = NULL;            /* pool for vocal strings */
int maxNwpool = 0;             /* alloced size of wpool */
int allocNwpool = 4000;        /* incrase size for malloc of wpool */
int nwpool,nwline;             /* globals to handle wpool */

struct SYMBOL zsym;            /* symbol containing zeros */

struct FORMAT sfmt;                    /* format after initialization */
struct FORMAT dfmt;                    /* format at start of tune */
struct FORMAT cfmt;                    /* current format for output */

char fontnames[50][STRLFMT];           /* list of needed fonts */
int  nfontnames;

//char txt[MAXNTEXT][MAXWLEN];           /* for output of text */
//int  ntxt; 
StringVector words_of_text;            /* for output of text */


char vcselstr[STRLFILE];          /* string for voice selection */
char mbf[501];                 /* mini-buffer for one line */
char buf[BUFFSZ];              /* output buffer.. should hold one tune */
int nbuf;                      /* number of bytes buffered */
float bposy;                   /* current position in buffered data */
int   ln_num;                  /* number of lines in buffer */
float ln_pos[BUFFLN];          /* vertical positions of buffered lines */
int   ln_buf[BUFFLN];          /* buffer location of buffered lines */
int   use_buffer;              /* 1 if lines are being accumulated */

char text [NTEXT][STRLINFO];   /* pool for history, words, etc. lines */
int text_type[NTEXT];          /* type of each text line */
int ntext;                     /* number of text lines */
char page_init[201];           /* initialization string after page break */
int do_mode;                   /* control whether to do index or output */
char escseq[81];               /* escape sequence string */
int linenum;                   /* current line number in input file */
int tunenum;                   /* number of current tune */
int tnum1,tnum2;
int numtitle;                  /* how many titles were read */
int mline;                     /* number music lines in current tune */
int nsym;                      /* number of symbols in line */
int nsym0;                     /* nsym at start of parsing a line */
int pagenum;                   /* current page in output file */
int writenum;                  /* calls to write_buffer for each one tune */
int xrefnum;                   /* xref number of current tune */
int do_meter, do_indent;       /* how to start next block */

int index_pagenum;             /* for index file */
float index_posx, index_posy;
int index_initialized;

GchordList prep_gchlst;          /* guitar chords for preparsing */
Deco prep_deco;                  /* decorations for preparsing */
int bagpipe;                     /* switch for HP mode */
int within_tune, within_block;   /* where we are in the file */
int do_this_tune;                /* are we typesetting the current one ? */
float posx,posy;                 /* overall scale, position on page */
int barinit;                     /* carryover bar number between parts */

char *p, *p0;                    /* global pointers for parsing music line */

int word,slur;                   /* variables used for parsing... */
int last_note,last_real_note;
int pplet,qplet,rplet;
int carryover;                   /* for interpreting > and < chars */
int ntinext,tinext[MAXHD];       /* for chord ties */

struct ENDINGS ending[20];       /* where to draw endings */           
int num_ending;                  /* number of endings to draw */
int mes1,mes2;                   /* to count measures in an ending */

int slur1[20],slur2[20];         /* needed for drawing slurs */
int overfull;                    /* flag if staff overfull */
int do_words;                    /* flag if staff has words under it */

int vb, verbose;                 /* verbosity, global and within tune */
int in_page=0;

                                 /* switches modified by flags: */
int gmode;                         /* switch for glue treatment */
int include_xrefs;                 /* to include xref numbers in title */
int one_per_page;                  /* new page for each tune ? */
int pagenumbers;                   /* write page numbers ? */
int write_history;                 /* write history and notes ? */
int help_me;                       /* need help ? */
int select_all;                    /* select all tunes ? */
int epsf;                          /* for EPSF postscript output */
int choose_outname;                /* 1 names outfile w. title/fnam */
int break_continues;               /* ignore continuations ? */
int search_field0;                 /* default search field */
int pretty;                        /* for pretty but sprawling layout */
int bars_per_line;                 /* bars for auto linebreaking */
int continue_lines;                /* flag to continue all lines */
int landscape;                     /* flag for landscape output */
int barnums;                       /* interval for bar numbers */
int make_index;                    /* write index file ? */
int notab;                         /* do not process tablature */
int transposegchords;              /* transpose gchords */
float alfa_c;                      /* max compression allowed */
float scalefac;                    /* scale factor for symbol size */
float lmargin;                     /* left margin */
float swidth;                      /* staff width */
float staffsep,dstaffsep;          /* staff separation */
float strict1,strict2;             /* 1stave, mstave strictness */
char transpose[21];                /* target key for transposition */
struct PAPERSIZE* paper;           /* paper size (a4, letter) */
int noauthor;                      /* suppress PS author tag (%%For) */


float alfa_last,beta_last;         /* for last short short line.. */

char in_file[MAXINF][STRLFILE];  /* list of input file names */
int  ninf;                       /* number of input file names */
FILE *fin;                       /* for input file */

char outf[STRLFILE];             /* output file name */
char outfnam[STRLFILE];          /* internal file name for open/close */
char styf[STRLFILE];             /* layout style file name */
char styd[STRLFILE];             /* layout style directory */
char infostr[STRLFILE];          /* title string in PS file */

int  file_open;                  /* for output file */
int  file_initialized;           /* for output file */
FILE *fout,*findex;              /* for output file */
int nepsf;                       /* counter for epsf output files */

char sel_str[MAXINF][STRLFILE];  /* list of selector strings */
int  s_field[MAXINF];            /* type of selection for each file */
int  psel[MAXINF];               /* pointers from files to selectors */

int temp_switch;

/* signal handler for premature termination */
void sighnd_term(int sig)
{
  close_output_file();
  exit(130);
}


/* ----- start of main ------ */
int main(int argc, char **argv)
{
  char ext[41];
  char xref_str[STRLFILE], pat[30][STRLFILE];
  int isel,j,npat,search_field,retcode,rc;

  /* cleanup on premature termination */
  if ((signal(SIGINT,sighnd_term)==SIG_ERR) ||
      (signal(SIGTERM,sighnd_term)==SIG_ERR)) {
    wng("could not install signal handler for SIGTERM and SIGINT","");
  }

  /* ----- set default options and parse arguments ----- */

  maxSyms = allocSyms = 800;
  maxVc   = allocVc = 3;

  init_ops (1);
  retcode=parse_args (argc, argv);
  if (retcode) exit (1);
  if (do_mode==DO_OUTPUT) {
    printf("This is abctab2ps, version %s.%s (%s)\n", VERSION, REVISION, VDATE);
  }

  alloc_structs ();

  maxNwpool = allocNwpool;
  wpool = (char *)zrealloc(NULL, 0, maxNwpool, sizeof(char));

  /* ----- set the page format ----- */
  nfontnames=0;
  if (!set_page_format()) exit (3);
  if (help_me==2) {
    print_format(cfmt);
    exit (0);
  }
  
  /* ----- help printout  ----- */
  if (help_me==1) {
    write_help ();
    exit (0);
  }
  
  if (ninf>0) isel=psel[ninf-1];
  else isel=psel[0];
  search_field0=s_field[isel];   /* default for interactive mode */
  if (epsf) cutext(outf);
  
  /* ----- initialize ----- */
  zero_sym();
  pagenum=0;
  tunenum=tnum1=tnum2=0;
  verbose=0;
  file_open=file_initialized=0;
  nepsf=0;
  bposy=0;
  posx=cfmt.leftmargin;
  posy=cfmt.pageheight-cfmt.topmargin; 

  strcpy(page_init, "");

  if ((do_mode == DO_OUTPUT) && make_index) open_index_file (INDEXFILE);

  /* ----- loop over files in list ---- */
  if (ninf==0) printf ("++++ No input files, read from stdin\n");
  for (j=0;j<=ninf;j++) {
    if (ninf==0) {
      /* no input file specified: open stdin */
      fin = stdin;
      strcpy(in_file[0],"stdin");
    } else {
      /* process list of input files */
      if (j==ninf) {
        break;
      } else {
        getext (in_file[j],ext);
        /*  skip .ps and .eps files */
        if ((!strcmp(ext,"ps"))||(!strcmp(ext,"eps"))) continue;

        if ((fin = fopen (in_file[j],"r")) == NULL) {      
          if (!strcmp(ext,"")) strext (in_file[j],in_file[j],"abc",1);
          if ((fin = fopen (in_file[j],"r")) == NULL) { 
            printf ("++++ Cannot open input file: %s\n", in_file[j]);
            continue;
          }
        }
      }
    }

    isel=psel[j];
    search_field=s_field[isel];
    npat=rehash_selectors (sel_str[isel], xref_str, pat);
    dfmt=sfmt;
    strnzcpy(infostr, in_file[j], STRLFILE);

    if (do_mode==DO_INDEX) {
      printf ("%s:\n", in_file[j]);
      do_index (fin,xref_str,npat,pat,select_all,search_field);
    }
      
    else {
      if (!epsf) {
        strext (outf, outf, "ps", 1);
        if (choose_outname) strext (outf, in_file[j], "ps", 1);
        open_output_file(outf,in_file[j]);
      }
      printf ("%s: ", in_file[j]); 
      if (vb>=3) printf ("\n"); 
      process_file (fin,fout,xref_str,npat,pat,select_all,search_field);
      printf ("\n");
    }
      
  }
  
  if (do_mode==DO_INDEX)
    printf ("Selected %d title%s of %d\n", tnum1, tnum1==1?"":"s", tnum2);
  
  if ((do_mode == DO_OUTPUT) && make_index) close_index_file ();
  rc = close_output_file ();

  if ((do_mode == DO_OUTPUT) && rc)
    return 1;
  else
    return 0;
}


