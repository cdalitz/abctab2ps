#ifndef _formatH
#define _formatH

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

/*  subroutines connected with page layout  */

 
/* ----- fontspec ----- */
void fontspec (struct FONTSPEC *f, const char name[], float size, int box);
 
/* ----- add_font - checks font list, adds font if new ----- */
int add_font (struct FONTSPEC *f);

/* ----- make_font_list ----- */
void make_font_list (struct FORMAT *f);

/* ----- set_standard_format ----- */
void set_standard_format (struct FORMAT *f);

/* ----- set_pretty_format ----- */
void set_pretty_format (struct FORMAT *f);

/* ----- set_pretty2_format ----- */
void set_pretty2_format (struct FORMAT *f);

/* ----- print_font ----- */
void print_font (const char *str, struct FONTSPEC fs);

/* ----- print_format ----- */
void print_format (struct FORMAT f);

/* ----- g_unum: read a number with a unit ----- */
void g_unum (const char *l, char *s, float *num);

/* ----- g_logv: read a logical variable ----- */ 
void g_logv (const char *l, char *s, int *v);

/* ----- g_fltv: read a float variable, no units ----- */ 
void g_fltv (char *l, int nch, float *v);

/* ----- g_intv: read an int variable, no units ----- */ 
void g_intv (char *l, int nch, int *v);

/* ----- g_fspc: read a font specifier ----- */ 
void g_fspc (char *l, int nch, struct FONTSPEC *fn);

/* ----- interpret_format_line ----- */
/* read a line with a format directive, set in format struct f */
int interpret_format_line (char l[], struct FORMAT *f);

/* ----- read_fmt_file ----- */
int read_fmt_file (const char filename[], char dirname[], struct FORMAT *f);

/* ----- handling international papersizes ----- */
struct PAPERSIZE {        /* dimension of a paper norm */
  const char* name;
  float pagewidth;
  float pageheight;
  float leftmargin;
  float staffwidth;
};
/* null terminated list of papersize norms and lookup function */
extern struct PAPERSIZE papersizes[];
struct PAPERSIZE* get_papersize (const char* name);
/* get system papersize according to Debian policy/libpaper */
struct PAPERSIZE* get_system_papersize ();

#endif // _formatH
