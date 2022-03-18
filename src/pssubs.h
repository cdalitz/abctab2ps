#ifndef _pssubsH
#define _pssubsH

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.c for details.
 */

/*  subroutines for postscript output  */


/* ----- level1_fix: special defs for level 1 Postscript ------- */
void level1_fix (FILE *fp);

/* ----- init_ps ------- */
void init_ps (FILE *fp, char str[], int is_epsf, float bx1, float by1, float bx2, float by2);

/* ----- close_ps ------- */
void close_ps (FILE *fp);

/* ----- init_page: initialize postscript page ----- */
void init_page (FILE *fp);

/* ----- init_index_page ----- */
void init_index_page (FILE *fp);

/* ----- init_index_file ------- */
void init_index_file (void);

/* ----- close_index_page-------- */
void close_index_page (FILE *fp);

/* ----- close_page-------- */
void close_page (FILE *fp);

/* ----- init_epsf: initialize epsf file ----- */
void init_epsf (FILE *fp);

/* ----- close_epsf: close epsf file ----- */
void close_epsf (FILE *fp);

/* ----- write_pagebreak ----- */
void write_pagebreak (FILE *fp);

#endif // _pssubsH
