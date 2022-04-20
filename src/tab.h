#ifndef _tabH
#define _tabH

/*  
 * This file is part of abctab2ps, 
 * See the file abctab2ps.cpp for details
 */

#include <stdio.h>

/*-------------------------------------------------------------------------
 macros and type definitions
---------------------------------------------------------------------------*/
#define STAFFHEIGHT 24     /* height of music staves in pt */

/* 
 * Important: tablature clef numbers must be >= 50 
 * because values < 50 are reserved for music (see abctab2ps.h)
 */
#define FRENCHTAB   50     /* types of tablature "clefs" */
#define SPANISHTAB  51
#define ITALIANTAB  52
#define ITALIAN7TAB 53
#define SPANISH5TAB 54
#define SPANISH4TAB 55
#define FRENCH5TAB  56
#define FRENCH4TAB  57
#define ITALIAN5TAB 58
#define ITALIAN4TAB 59
#define ITALIAN8TAB 60
#define GERMANTAB   61
#define NEAPOLTAB   62

/* 
 * Important: tablature deco numbers should be >= 50 
 * because values < 50 are used for music (see abctab2ps.h)
 */
#define D_INDEX       50    /* codes for decoration */
#define D_MEDIUS      51
#define D_ANNULARIUS  52
#define D_POLLIX      53
#define D_TABACC      54
#define D_TABX        55
#define D_TABU        56
#define D_TABV        57
#define D_TABTRILL    58
#define D_TABSTAR     59
#define D_TABCROSS    60
#define D_TABOLINE    61
#define D_STRUMUP     62
#define D_STRUMDOWN   63

struct TABFONT {           /* tablature font settings */
  int size;                /* font size and line distance */
  float scale;             /* scale factor for font */
  char frfont[STRLFMT];    /* font for frenchtab */
  char itfont[STRLFMT];    /* font for italiantab */
  char defont[STRLFMT];    /* font for germantab */
  TABFONT() {
    size = 14;
    scale = 1.0;
    strcpy(frfont, "frFrancisque");
    strcpy(itfont, "itTimes");
    strcpy(defont, "deCourier");
  }
};

#define RHSIMPLE      1    /* possible rhythm styles */
#define RHMODERN      2
#define RHDIAMOND     3
#define RHNONE        4
#define RHGRID        5
#define RHMODERNBEAMS 6

#define BRUMMER_ABC   1    /* possible styles for Brummer in germantab */
#define BRUMMER_1AB   2
#define BRUMMER_123   3

struct TABFORMAT {         /* tablature format parameters */
  int addflags;            /* how many more flags in tabrhythm */
  int rhstyle;             /* rhythm flag style */
  int allflags;            /* whether all flags are to be printed */
  int firstflag;           /* whether only changing flags are to be printed */
  int ledgeabove;          /* bourdon ledger lines above symbol? */
  float flagspace;         /* additional space between flag and tab system */
  float gchordspace;       /* distance between gchord and tablature */
  int brummer;             /* sytle for brumemr in german tab */
  int germansepline;       /* draw separator line in German tab */
  int underline8;          /* draw 8th course with underline instead of slash */
  int underscoreis8;       /* interpret {_a} as 8th course */
  TABFORMAT() {
    addflags = 2;
    rhstyle = RHSIMPLE;
    allflags = 0;
    firstflag = 0;
    ledgeabove = 0;
    flagspace = 0.0;
    gchordspace = 10;
    brummer = BRUMMER_ABC;
    germansepline = 1;
    underline8 = 0;
    underscoreis8 = 0;
  }
};


/*-------------------------------------------------------------------------
 global variables for tablature
---------------------------------------------------------------------------*/
/* previous notelength */
extern int TAB_prevlen;

/* for making next rhythmflag visible after > and < */
extern int TAB_makenextvisible;

/* tablature font setting */
extern TABFONT tabfont;

/* tablature format paramter setting */
extern TABFORMAT tabfmt;


/*-------------------------------------------------------------------------
 function prototypes
---------------------------------------------------------------------------*/
/* subroutines for writing postscript macros in outfile */
FILE* open_tabfontfile (char* basename);
void def_tabfonts (FILE *fp);
void def_tabsyms (FILE *fp);

/* parse command line arguments starting with -tab */
void parse_tab_args(char** av, int* i);

/* parse "string" for tablature key and store result in "ktype" */
int parse_tab_key (char* string, int* ktype);

/* decide whether the clef number in "key" means tablature */
int is_tab_key (struct KEYSTR *key);

/* return number of lines per tablature system */
int tab_numlines (struct KEYSTR *key);

/* decide whether an input line is tablature */
int is_tab_line (char* line);

/* reads a tab format line */
int read_tab_format (char* line);

/* print tablature format parameters */
void print_tab_format (void);

/* check whether all selected voices are tablature */
int only_tabvoices(void);

/* parse a tablature line into symbols */
int parse_tab_line (char* line);

/* parse a tablature symbol and return its type */
int parse_tab_sym(void);

/* parse one chord of tablature and adds it to symbols */
int parse_tab_chord(void);

/* parse length specifer for note or rest */
int parse_tab_length (void);

/* parse decorations for entire chord */
int parse_tabdeco ();

/* draw tablature symbols at proper positions on tablature staff */
void draw_tab_symbols (FILE *fp, float bspace, float *bpos, int is_top);

/* draw_tabnote - draws notes of chord at position x */
void draw_tabnote (float x, struct SYMBOL *s, float* gchy);

/* compute number of flags to given length */
int tab_flagnumber (int len);

/* return space between tablature system and rhythm flags */
float tab_flagspace (struct KEYSTR *key);

/* draw flag at position x */
void draw_tabflag (float x, struct SYMBOL *s);

/* draw (multibar-) rest at position x */
void draw_tabrest (float x, struct SYMBOL *s);
void draw_tabbrest (float x, struct SYMBOL *s);

/* functions for drawing bars */
void draw_tabbar (float x, struct SYMBOL *s);
void draw_tabbarnums (FILE *fp);
void draw_tabendings (void);

/* functions for drawing beams (grids) */
int calculate_tabbeam (int i0, struct BEAM *bm);
void draw_tabbeams (struct BEAM *bm, int rhstyle);

/* draw time signature at position x */
void draw_tabtimesig (float x, struct SYMBOL *s);

/* functions for drawing slurs */
void draw_tabslurs (void);
void draw_tabnormalslur (int from, int to);
void draw_tabchordslurs (int from, int to);
float tab_slurshift(void);

/* draw tenuto strokes in current line */
void draw_tabtenutos (void);

/* draw nplets in current line */
void draw_tabnplets (void);

/* draw chord decoration at position x */
void draw_tabdeco (float x, struct SYMBOL *s, float* gchy);

/* returns tablature line corresponding to given course */
int tabline (int course);

/* return lowest/(next)highest course in chord */
int lowest_course (struct SYMBOL *s);
int highest_course (struct SYMBOL *s);
int next_line (struct SYMBOL *s, int hc);

/* computes code number from german font */
int german_tabcode(int course, int fret);

#endif // _tabH




