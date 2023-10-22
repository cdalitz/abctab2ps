#ifndef _abctabpsH
#define _abctabpsH

#include <stdio.h>

#include "oscompat.h"

#include <string>
#include <list>
#include <vector>
#include <map>

/* namespace declarations are mandatory in gcc 3.x */
using std::string;
using std::list;
using std::map;

/*
 * macros and global variables
 */

/* -------------- general macros ------------- */

#define VERSION              "1.8"      /* version */
#define REVISION             "25"        /* revison */
#define VDATE        "Oct 22 2023"      /* version date */
#define VERBOSE0           2            /* default verbosity */
#define DEBUG_LV           0            /* debug output level */
#define OUTPUTFILE      "Out.ps"        /* standard output file */
#define INDEXFILE       "Ind.ps"        /* output file for index */
#define PS_LEVEL           2            /* PS laguage level: must be 1 or 2 */

/* default directory to search for format files */
#define DEFAULT_FDIR   ""

/* key-value list taken from STL */
typedef map<string, string> StringMap;

/* array of strings taken from STL */
typedef vector<string> StringVector;

/* ----- macros controlling music typesetting ----- */

#define BASEWIDTH      0.8      /* width for lines drawn within music */
#define SLURWIDTH      0.8      /* width for lines for slurs */
#define STEM_YOFF      1.0      /* offset stem from note center */
#define STEM_XOFF      3.5
#define STEM            20      /* standard stem length */
#define STEM_MIN        16      /* min stem length under beams */
#define STEM_MIN2       12      /* ... for notes with two beams */
#define STEM_MIN3       10      /* ... for notes with three beams */
#define STEM_MIN4       10      /* ... for notes with four beams */
#define STEM_CH         16      /* standard stem length for chord */
#define STEM_CH_MIN     12      /* min stem length for chords under beams */
#define STEM_CH_MIN2     8      /* ... for notes with two beams */
#define STEM_CH_MIN3     7      /* ... for notes with three beams */
#define STEM_CH_MIN4     7      /* ... for notes with four beams */
#define BEAM_DEPTH     2.6      /* width of a beam stroke */
#define BEAM_OFFSET    0.25     /* pos of flat beam relative to staff line */
#define BEAM_SHIFT     5.3      /* shift of second and third beams */
/*  To align the 4th beam as the 1st: shift=6-(depth-2*offset)/3  */
#define BEAM_FLATFAC   0.6      /* factor to decrease slope of long beams */
#define BEAM_THRESH   0.06      /* flat beam if slope below this threshold */
#define BEAM_SLOPE     0.5      /* max slope of a beam */
#define BEAM_STUB      6.0      /* length of stub for flag under beam */ 
#define SLUR_SLOPE     1.0      /* max slope of a slur */
#define DOTSHIFT         5      /* shift dot when up flag on note */
#define GSTEM         10.0      /* grace note stem length */
#define GSTEM_XOFF     2.0      /* x offset for grace note stem */
#define GSPACE0       10.0      /* space from grace note to big note */
#define GSPACE         7.0      /* space between grace notes */
#define WIDTH_MIN      1.0      /* minimal left,right width for xp list */
#define RANFAC        0.05      /* max random shift = RANFAC * spacing */
#define RANCUT        1.50      /* cutoff for random shift */
#define BNUMHT        32.0      /* height for bar numbers */

#define BETA_C          0.1     /* max expansion for flag -c */
#define ALFA_X          1.0     /* max compression before complaining */
#define BETA_X          1.2     /* max expansion before complaining */

#define VOCPRE          0.4     /* portion of vocals word before note */
#define GCHPRE          0.4     /* portion of guitar chord before note */

#define DEFVOICE        "1"     /* default name for first voice */


/* ----- macros for program internals ----- */

#define CM             28.35    /* factor to transform cm to pt */
#define PT              1.00    /* factor to transform pt to pt */
#define IN             72.00    /* factor to transform inch to pt */

#define STRLINFO       301      /* string length in info fields */
#define STRLFILE       512      /* string length for file names and selection patterns */
#define MAXSYMST   11           /* max symbols in start piece */
#define MAXHD      10           /* max heads on one stem */
#define NTEXT     100           /* max history lines for output */
#define MAXINF    100           /* max number of input files */
#define BUFFSZ  50000           /* size of output buffer */
#define BUFFSZ1  3000           /* buffer reserved for one staff */
#define BUFFLN    100           /* max number of lines in output buffer */
#define NWLINE     16           /* max number of vocal lines per staff */
#define MAXGCHLINES 8           /* max number of lines in guitar chords */
#define MAXGRACE   30           /* max number of grace notes */

#define BASE           192      /* base for durations */   
#define LONGA          768      /* quadruple note (longa) */
#define BREVIS         384      /* double note (brevis) */
#define WHOLE          192      /* whole note */
#define HALF            96      /* half note */
#define QUARTER         48      /* quarter note */
#define EIGHTH          24      /* 1/8 note */
#define SIXTEENTH       12      /* 1/16 note */
#define THIRTYSECOND     6      /* 1/32 note */
#define SIXTYFOURTH      3      /* 1/64 note */

#define COMMENT          1      /* types of lines scanned */
#define MUSIC            2   
#define TO_BE_CONTINUED  3    
#define E_O_F            4   
#define INFO             5
#define TITLE            6
#define METER            7
#define PARTS            8
#define KEY              9
#define XREF            10
#define DLEN            11
#define HISTORY         12
#define BLANK           13
#define WORDS           14
#define MWORDS          15
#define PSCOMMENT       16
#define TEMPO           17
#define VOICE           18
#define CMDLINE         19

#define INVISIBLE    1          /* valid symbol types */
#define NOTE         2         
#define REST         3         
#define BAR          4
#define CLEF         5 
#define TIMESIG      6 
#define KEYSIG       7 
#define GCHORD       8 
#define BREST        9          
#define DECO        10          

#define SPACE      101           /* additional parsable things */
#define E_O_L      102
#define ESCSEQ     103
#define CONTINUE   104
#define NEWLINE    105
#define DUMMY      106


#define B_SNGL       1           /* codes for different types of bars */
#define B_DBL        2             /* ||   thin double bar */
#define B_LREP       3             /* |:   left repeat bar */
#define B_RREP       4             /* :|   right repeat bar */
#define B_DREP       5             /* ::   double repeat bar */
#define B_FAT1       6             /* [|   thick at section start */
#define B_FAT2       7             /* |]   thick at section end  */
#define B_INVIS      8             /* invisible; for endings without bars */

#define A_SH         1           /* codes for accidentals */
#define A_NT         2
#define A_FT         3
#define A_DS         4
#define A_DF         5


/* 
 * Important: music deco numbers should be < 50 
 * because values >= 50 are used for tablature (see tab.h)
 */
#define D_GRACE      1           /* codes for decoration */
#define D_STACC      2           
#define D_SLIDE      3           
#define D_TENUTO     4           
#define D_HOLD       5           
#define D_IHOLD      6   /* not yet used */           
#define D_UPBOW      7           
#define D_DOWNBOW    8           
#define D_TRILL      9           
#define D_HAT       10           
#define D_ATT       11           
#define D_SEGNO     12           
#define D_CODA      13           
#define D_PRALLER   14
#define D_MORDENT   15
#define D_TURN      16
#define D_PLUS      17
#define D_CROSS     18
#define D_ROLL      19           
#define D_DYN_PP    20
#define D_DYN_P     21
#define D_DYN_MP    22
#define D_DYN_MF    23
#define D_DYN_F     24
#define D_DYN_FF    25
#define D_DYN_SF    26
#define D_DYN_SFZ   27
#define D_BREATH    28
#define D_WEDGE     29
#define D_NOSTEM    30
#define D_DIAMOND   31
#define D_ARPEGGIO  32


#define H_FULL       1           /* types of heads */
#define H_EMPTY      2
#define H_OVAL       3

/* 
 * Important: music clef numbers must be < 50 
 * because values >= 50 are reserved for tablature (see tab.h)
 */
#define TREBLE        1          /* types of clefs */
#define TREBLE8       2
#define BASS          3
#define ALTO          4
#define TENOR         5
#define SOPRANO       6
#define MEZZOSOPRANO  7
#define BARITONE      8
#define VARBARITONE   9
#define SUBBASS      10
#define FRENCHVIOLIN 11
#define TREBLE8UP    12

#define G_FILL       1            /* modes for glue */
#define G_SHRINK     2 
#define G_SPACE      3 
#define G_STRETCH    4

#define S_TITLE      1            /* where to do pattern matching */
#define S_RHYTHM     2
#define S_COMPOSER   3
#define S_SOURCE     4

#define TEXT_H       1            /* type of a text line */
#define TEXT_W       2
#define TEXT_Z       3
#define TEXT_N       4
#define TEXT_D       5

#define DO_INDEX     1            /* what program does */
#define DO_OUTPUT    2

#define SWFAC        0.50         /* factor to estimate width of string */

#define DB_SW        0            /* debug switch */

#define MAXFORMATS 10             /* max number of defined page formats */
#define STRLFMT    81             /* string length in FORMAT struct */


#define MAXNTEXT   400            /* for text output */
#define MAXWLEN     51
#define ALIGN        1
#define RAGGED       2
#define OBEYLINES    3
#define OBEYCENTER   4
#define OBEYRIGHT    5
#define SKIP         6

#define E_CLOSED     1
#define E_OPEN       2 


/* ----- global variables ------- */

extern int db;             /* debug control */

extern int maxSyms,maxVc;  /* for malloc */
extern int allocSyms, allocVc;

#define NCOMP     5        /* max number of composer lines */

struct ISTRUCT {           /* information fields */
  char area   [STRLINFO];
  char book   [STRLINFO];
  char comp   [NCOMP][STRLINFO];
  int  ncomp;
  char disc   [STRLINFO];
  char eskip  [STRLINFO];
  char file   [STRLINFO];
  char group  [STRLINFO];
  char hist   [STRLINFO];
  char info   [STRLINFO];
  char key    [STRLINFO];
  char len    [STRLINFO];
  char meter  [STRLINFO];
  char notes  [STRLINFO];
  char orig   [STRLINFO];
  char rhyth  [STRLINFO];
  char src    [STRLINFO];
  char title  [STRLINFO];
  char title2 [STRLINFO];
  char title3 [STRLINFO];
  char parts  [STRLINFO];
  char xref   [STRLINFO];
  char trans  [STRLINFO];
  char tempo  [STRLINFO];
};
extern struct ISTRUCT info,default_info;

struct GRACE {     /* describes grace notes */
  int n;              /* number of grace notes */
  int len;            /* note length */
                      /* when zero (default), treated as accacciatura */
  int p[MAXGRACE];    /* pitches */
  int a[MAXGRACE];    /* accidentals */
};

struct Deco {             /* describes decorations */
  int n;                       /* number of decorations */
  float top;                   /* max height needed */
  int t[30];                   /* type of deco */
  Deco() {clear();}
  void clear() {n=0; top=0.0; for (int i=0; i<30; i++) t[i]=0;}
  void add(int dtype) { if (n<30) {t[n]=dtype; n++;} }
};

/* single guitar chord and list of guitar chords on a single note */
struct Gchord {
  string text;                 /* gchord text */
  float x;                     /* x position */
  Gchord() { text=""; x=0; }
};
typedef list<Gchord> GchordList;

struct SYMBOL {            /* struct for a drawable symbol */
  int type;                    /* type of symbol */
  int pits[MAXHD];             /* pitches for notes */
  int lens[MAXHD];             /* note lengths as multiple of BASE */
  int accs[MAXHD];             /* code for accidentals */
  int sl1 [MAXHD];             /* which slur start on this head */
  int sl2 [MAXHD];             /* which slur ends on this head */
  int ti1 [MAXHD];             /* flag to start tie here */
  int ti2 [MAXHD];             /* flag to end tie here */
  int ten1 [MAXHD];            /* flag to start tenuto here (only tab)*/
  int ten2 [MAXHD];            /* flag to end tenuto here (only tab) */
  int lig1;                    /* ligatura starts here */
  int lig2;                    /* ligatura ends here */
  float shhd[MAXHD];           /* horizontal shift for heads */
  float shac[MAXHD];           /* horizontal shift for accidentals */
  int npitch;                  /* number of note heads */
  int len;                     /* basic note length */
  int fullmes;                 /* flag for full-measure rests */
  int word_st;                 /* 1 if word starts here */
  int word_end;                /* 1 if word ends here */
  int slur_st;                 /* how many slurs starts here */
  int slur_end;                /* how many slurs ends here */
  int ten_st;                  /* how many tenuto strokes start (only tab) */
  int ten_end;                 /* how many tenuto strokes end (only tab) */
  int yadd;                    /* shift for treble/bass etc clefs */
  float x,y;                   /* position */
  int ymn,ymx,yav;             /* min,mav,avg note head height */
  float ylo,yhi;               /* bounds for this object */
  float xmn,xmx;               /* min,max h-pos of a head rel to top */
  int stem;                    /* 0,1,-1 for no stem, up, down */
  int flags;                   /* number of flags or bars */
  int dots;                    /* number of dots */
  int head;                    /* type of head */
  int eoln;                    /* flag for last symbol in line */
  int grcpit;                  /* pitch to which grace notes apply */
  struct GRACE gr;             /* grace notes */
  struct Deco  dc;             /* decoration symbols */
  float xs,ys;                 /* position of stem end */
  int u,v,w,t,q;               /* auxillary information */
  int invis;                   /* mark note as invisible */
  float wl,wr;                 /* left,right min width */
  float pl,pr;                 /* left,right preferred width */
  float xl,xr;                 /* left,right expanded width */
  int p_plet,q_plet,r_plet;    /* data for n-plets */
  float gchy;                  /* height of guitar chord */
  char text[41];               /* for guitar chords (no longer) etc. */
  GchordList* gchords;         /* guitar chords above symbol */
  int wlines;                  /* number of vocal lines (only stored in first symbol per line)*/
  char *wordp[NWLINE];         /* pointers to wpool for vocals */
  int p;                       /* pointer to entry in posit table */
  float time;                  /* time for symbol start */
  int tabdeco[MAXHD];          /* tablature decorations inside chord */
};


extern char lvoiceid[STRLINFO];       /* string from last V: line */
extern int  nvoice,mvoice;            /* number of voices defd, nonempty */
extern int  ivc;                      /* current voice */
extern int  ivc0;                     /* top nonempty voice */


struct XPOS {            /* struct for a horizontal position */
  int type;                    /* type of symbols here */
  int next,prec;               /* pointers for linked list */ 
  int eoln;                    /* flag for line break */
  int *p;                      /* pointers to associated syms */
  float time,dur;              /* start time, duration */
  float wl,wr;                 /* right and left widths */
  float space,shrink,stretch;  /* glue before this position */
  float tfac;                  /* factor to tune spacings */
  float x;                     /* final horizontal position */
};


extern int ixpfree;                   /* first free element in xp array */


struct METERSTR {        /* data to specify the meter */
  int meter1,meter2; /*numerator, denominator*/
  int mflag;         /*mflag: 1=C, 2=C|, 3=numerator only, otherwise 0*/
  char top[31];
  int dlen;
  int display;       /*0 for M:none, 1 for real meter, 2 for differing display*/
  /* here follow the paramaters for a differing display value */
  /* (these are only set, when display == 2) */
  int dismeter1, dismeter2;
  int dismflag;
  char distop[31];
};
extern struct METERSTR default_meter;

struct KEYSTR {          /* data to specify the key */
  int ktype;
  int sf;
  int add_pitch;       
  int root,root_acc;
  int add_transp,add_acc[7];   
};
extern struct KEYSTR default_key;
  

struct VCESPEC {         /* struct to characterize a voice */
  char id[33];                    /* identifier string, eg. a number */
  char name[81];                  /* full name of this voice */
  char sname[81];                 /* short name */
  struct METERSTR meter,meter0,meter1;    /* meter */
  struct KEYSTR   key,key0,key1;          /* keysig */
  int stems;                      /* +1 or -1 to force stem direction */
  int staves,brace,bracket;       /* for deco over several voices */
  int do_gch;                     /* 1 to output gchords for this voice */
  float sep;                      /* for space to next voice below */
  int nsym;                       /* number of symbols */
  int draw;                       /* flag if want to draw this voice */
  int select;                     /* flag if selected for output */
  int insert_btype,insert_num;    /* to split bars over linebreaks */
  int insert_bnum;                /* same for bar number */
  float insert_space;             /* space to insert after init syms */
  int end_slur;                   /* for a-b slurs */
  char insert_text[81];           /* string over inserted barline */
  float timeinit;                 /* carryover time between parts */
};


                              /* things to alloc: */
extern struct SYMBOL  *sym;              /* symbol list */
extern struct SYMBOL  **symv;            /* symbols for voices */
extern struct XPOS    *xp;               /* shared horizontal positions */
extern struct VCESPEC *voice;            /* characteristics of a voice */
extern struct SYMBOL  **sym_st;          /* symbols a staff start */
extern int            *nsym_st; 


extern int halftones;                /* number of halftones to transpose by */

                                          /* style parameters: */
extern float f0p,f5p,f1p,f0x,f5x,f1x;            /*   mapping fct */
extern float lnnp,bnnp,fnnp,lnnx,bnnx,fnnx;      /*   note-note spacing */
extern float lbnp,bbnp,rbnp,lbnx,bbnx,rbnx;      /*   bar-note spacing */
extern float lnbp,bnbp,rnbp,lnbx,bnbx,rnbx;      /*   note-bar spacing */


extern char* wpool;                   /* pool for vocal strings */
extern int maxNwpool, allocNwpool;    /* for malloc of wpool */
extern int nwpool,nwline;             /* globals to handle wpool */

extern struct SYMBOL zsym;            /* symbol containing zeros */

struct BEAM {                  /* packages info about one beam */
  int i1,i2;            
  float a,b;
  float x,y,t;
  int stem;
};

struct FONTSPEC {
  char name [STRLFMT];
  float size;
  int box;
};

struct FORMAT {                        /* struct for page layout */
  char     name     [STRLFMT];
  float    pageheight,staffwidth;
  float    topmargin,botmargin,leftmargin;
  float    topspace,wordsspace,titlespace,subtitlespace,partsspace;
  float    composerspace,musicspace,vocalspace,textspace,gchordspace;
  float    scale,maxshrink,lineskipfac,parskipfac,indent;
  float    staffsep,sysstaffsep,systemsep,stafflinethickness;
  float    strict1,strict2;
  int      landscape,titleleft,continueall,breakall,writehistory;
  int      stretchstaff,stretchlast,withxrefs,barsperstaff,oneperpage;
  int      titlecaps,barnums,barinit,squarebrevis,endingdots,slurisligatura;
  int      historicstyle,nobeams,nogracestroke,printmetronome,nostems;
  struct FONTSPEC titlefont,subtitlefont,vocalfont,textfont,tempofont;
  struct FONTSPEC composerfont,partsfont,gchordfont,wordsfont,voicefont;
  struct FONTSPEC barnumfont,barlabelfont,indexfont;
  StringMap meterdisplay;
};

extern struct FORMAT sfmt;                    /* format after initialization */
extern struct FORMAT dfmt;                    /* format at start of tune */
extern struct FORMAT cfmt;                    /* current format for output */

extern char fontnames[50][STRLFMT];           /* list of needed fonts */
extern int  nfontnames;

//extern char txt[MAXNTEXT][MAXWLEN];           /* for output of text */
//extern int  ntxt; 
extern StringVector words_of_text;            /* for output of text */

extern char vcselstr[STRLFILE];   /* string for voice selection */
extern char mbf[501];             /* mini-buffer for one line */
extern char buf[BUFFSZ];          /* output buffer.. should hold one tune */
extern int nbuf;                  /* number of bytes buffered */
extern float bposy;               /* current position in buffered data */
extern int   ln_num;              /* number of lines in buffer */
extern float ln_pos[BUFFLN];      /* vertical positions of buffered lines */
extern int   ln_buf[BUFFLN];      /* buffer location of buffered lines */
extern int   use_buffer;          /* 1 if lines are being accumulated */

extern char text [NTEXT][STRLINFO];   /* pool for history, words, etc. lines */
extern int text_type[NTEXT];      /* type of each text line */
extern int ntext;                 /* number of text lines */
extern char page_init[201];       /* initialization string after page break */
extern int do_mode;               /* control whether to do index or output */
extern char escseq[81];           /* escape sequence string */
extern int linenum;               /* current line number in input file */
extern int tunenum;               /* number of current tune */
extern int tnum1,tnum2;
extern int numtitle;              /* how many titles were read */
extern int mline;                 /* number music lines in current tune */
extern int nsym;                  /* number of symbols in line */
extern int nsym0;                 /* nsym at start of parsing a line */
extern int pagenum;               /* current page in output file */
extern int writenum;              /* calls to write_buffer for each one tune */
extern int xrefnum;               /* xref number of current tune */
extern int do_meter, do_indent;   /* how to start next block */

extern int index_pagenum;             /* for index file */
extern float index_posx, index_posy;
extern int index_initialized;

extern GchordList prep_gchlst;        /* guitar chords for preparsing */
extern Deco prep_deco;                /* decorations for preparsing */
extern int bagpipe;                   /* switch for HP mode */
extern int within_tune, within_block; /* where we are in the file */
extern int do_this_tune;              /* are we typesetting the current one ? */
extern float posx,posy;               /* overall scale, position on page */
extern int barinit;                   /* carryover bar number between parts */

extern char *p, *p0;                  /* global pointers for parsing music line */

extern int word,slur;                 /* variables used for parsing... */
extern int last_note,last_real_note;
extern int pplet,qplet,rplet;
extern int carryover;                 /* for interpreting > and < chars */
extern int ntinext,tinext[MAXHD];     /* for chord ties */

struct ENDINGS {                    /* where to draw endings */
  float a,b;                        /* start and end position */
  int ai,bi;                        /* symbol index of start/end position */
  int num;                          /* number of the ending */
  int type;                         /* shape: open or closed at right */
};
extern struct ENDINGS ending[20];
extern int num_ending;              /* number of endings to draw */
extern int mes1,mes2;               /* to count measures in an ending */

extern int slur1[20],slur2[20];     /* needed for drawing slurs */
extern int overfull;                /* flag if staff overfull */
extern int do_words;                /* flag if staff has words under it */

extern int vb, verbose;             /* verbosity, global and within tune */
extern int in_page;

                                 /* switches modified by flags: */
extern int gmode;                     /* switch for glue treatment */
extern int include_xrefs;             /* to include xref numbers in title */
extern int one_per_page;              /* new page for each tune ? */
extern int pagenumbers;               /* write page numbers ? */
extern int write_history;             /* write history and notes ? */
extern int help_me;                   /* need help ? */
extern int select_all;                /* select all tunes ? */
extern int epsf;                      /* for EPSF postscript output */
extern int choose_outname;            /* 1 names outfile w. title/fnam */
extern int break_continues;           /* ignore continuations ? */
extern int search_field0;             /* default search field */
extern int pretty;                    /* for pretty but sprawling layout */
extern int bars_per_line;             /* bars for auto linebreaking */
extern int continue_lines;            /* flag to continue all lines */
extern int landscape;                 /* flag for landscape output */
extern int barnums;                   /* interval for bar numbers */
extern int make_index;                /* write index file ? */
extern int notab;                     /* do not process tablature */
extern int transposegchords;          /* transpose gchords */
extern float alfa_c;                  /* max compression allowed */
extern float scalefac;                /* scale factor for symbol size */
extern float lmargin;                 /* left margin */
extern float swidth;                  /* staff width */
extern float staffsep,dstaffsep;      /* staff separation */
extern float strict1,strict2;         /* 1stave, mstave strictness */
extern char transpose[21];            /* target key for transposition */
extern struct PAPERSIZE* paper;       /* paper size */
extern int noauthor;                  /* suppress PS author tag (%%For) */

extern float alfa_last,beta_last;     /* for last short short line.. */

extern char in_file[MAXINF][STRLFILE];   /* list of input file names */
extern int  ninf;                     /* number of input file names */
extern FILE *fin;                     /* for input file */

extern char outf[STRLFILE];           /* output file name */
extern char outfnam[STRLFILE];        /* internal file name for open/close */
extern char styf[STRLFILE];           /* layout style file name */
extern char styd[STRLFILE];           /* layout style directory */
extern char infostr[STRLFILE];        /* title string in PS file */

extern int  file_open;                /* for output file */
extern int  file_initialized;         /* for output file */
extern FILE *fout,*findex;            /* for output file */
extern int nepsf;                     /* counter for epsf output files */

extern char sel_str[MAXINF][STRLFILE];/* list of selector strings */
extern int  s_field[MAXINF];          /* type of selection for each file */
extern int  psel[MAXINF];             /* pointers from files to selectors */

extern int temp_switch;

#endif // _abctabpsH



