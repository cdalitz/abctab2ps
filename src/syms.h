#ifndef _symsH
#define _symsH

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

#include <stdio.h>
 
/*  subroutines to define postscript macros which draw symbols  */


/* ----- def_misc ------- */
void def_misc (FILE *fp);

/* ----- def_typset ------- */
void def_typeset(FILE *fp);

/* ----- define_font ------- */
void define_font (FILE *fp, char name[], int num);

/* ----- def_tsig ------- */
void def_tsig (FILE *fp);

/* ----- add_cv ------- */
void add_curve (FILE *fp, float f1, float f2, float (*p)[2], int i0, int ncv);

/* ----- add_sg ------- */
void add_sg (FILE *fp, float f1, float f2, float p[][2], int i0, int ncv);

/* ----- add_mv ------- */
void add_mv (FILE *fp, float f1, float f2, float p[][2], int i0);

/* ----- def_stems ------- */
void def_stems (FILE *fp);

/* ----- def_dot ------- */
void def_dot (FILE *fp);

/* ----- def_deco ------- */
void def_deco (FILE *fp);

/* ----- def_deco1 ------- */
void def_deco1 (FILE *fp);

/* ----- def_hl ------- */
void def_hl (FILE *fp);

/* ----- def_beam ------- */
void def_beam (FILE *fp);

/* ----- def_flags1 ------- */
void def_flags1 (FILE *fp);

/* ----- def_flags2 ------- */
void def_flags2 (FILE *fp);

/* ----- def_xflags ------- */
void def_xflags (FILE *fp);

/* ----- def_acc ------- */
void def_acc (FILE *fp);

/* ----- def_rests ------- */
void def_rests (FILE *fp);

/* ----- def_bars ------ */
void def_bars (FILE *fp);

/* ----- def_ends ------ */
void def_ends (FILE *fp);
       
/* ----- def_gchord ------ */
void def_gchord (FILE *fp);

/* ----- def_sl ------ */
void def_sl (FILE *fp);

/* ----- def_hd1 ------- */
void def_hd1 (FILE *fp);

/* ----- def_hd2 ------- */
void def_hd2 (FILE *fp);

/* ----- def_hd3 ------- */
void def_hd3 (FILE *fp);

/* ----- def_gnote ------- */
void def_gnote (FILE *fp);

/* ----- def_csig ------- */
void def_csg (FILE *fp);

/* ----- def_gclef ------- */
void def_gclef (FILE *fp);

/* ----- def_t8clef ------- */
void def_t8clef (FILE *fp);

/* ----- def_fclef ------- */
void def_fclef (FILE *fp);

/* ----- def_cclef ------- */
void def_cclef (FILE *fp);

/* ----- def_brace ------- */
void def_brace (FILE *fp);

/* ----- def_staff ------- */
void def_staff (FILE *fp);

/* ----- def_sep ------- */
void def_sep (FILE *fp);

/* ----- define_symbols: write postscript macros to file ------ */
void define_symbols (FILE *fp);



#endif // _symsH
