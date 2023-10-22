/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

#include <string.h>
#include <math.h>

#include "abctab2ps.h"
#include "syms.h"
#include "tab.h"


/*  subroutines to define postscript macros which draw symbols  */

/* ----- def_misc ------- */
void def_misc (FILE *fp)
{
  fprintf (fp,
           "\n/cshow { %% string cshow - center at current pt\n"
           "   dup stringwidth pop 2 div neg 0 rmoveto\n"
           "   bx {box} if show\n"
           "} bind def\n"
           "\n/lshow { %% string lshow - show left-aligned\n"
           "   dup stringwidth pop neg 0 rmoveto bx {box} if show\n"
           "} bind def\n"
           "\n/rshow { %% string rshow - show right-aligned\n"
           "   bx {box} if show\n"
           "} bind def\n"
           );
  
  fprintf (fp,
           "\n/box { %% str box - draw box around string\n"
           "  gsave 0.5 setlinewidth dup stringwidth pop\n"
           "  -2 -2 rmoveto 4 add fh 4 add 2 copy\n"
           "  0 exch rlineto 0 rlineto neg 0 exch rlineto neg 0 rlineto\n"
           "  stroke grestore\n"
           "} bind def\n");

  fprintf (fp, 
           "\n/wd { moveto bx {box} if show } bind def\n"
           "/wln {\n"
           "dup 3 1 roll moveto gsave 0.6 setlinewidth lineto stroke grestore\n"
           "} bind def\n");

  fprintf (fp,
           "/whf {moveto gsave 0.5 1.2 scale (-) show grestore} bind def\n");

}

/* ----- def_typset ------- */
void def_typeset(FILE *fp)
{

  fprintf (fp,
           "\n/WS { %% w nspaces str WS\n"
           "   dup stringwidth pop 4 -1 roll\n"
           "   sub neg 3 -1 roll div 0 8#040 4 -1 roll\n"
           "   widthshow\n"
           "} bind def\n");


  fprintf (fp, 
           "\n/W1 { show pop pop } bind def\n");


  fprintf (fp, 
           "\n/str 50 string def\n"
           "/W0 {\n"
           "   dup stringwidth pop str cvs exch show (  ) show show pop pop\n"
           "} bind def\n");

  fprintf (fp,
           "\n/WC { counttomark 1 sub dup 0 eq { 0 }\n"
           "  {  ( ) stringwidth pop neg 0 3 -1 roll\n"
           "  {  dup 3 add index stringwidth pop ( ) stringwidth pop add\n"
           "  dup 3 index add 4 index lt 2 index 1 lt or\n"
           "  {3 -1 roll add exch 1 add} {pop exit} ifelse\n"
           "  } repeat } ifelse\n"
           "} bind def\n");

  fprintf (fp,
           "\n/P1 {\n"
           "  {  WC dup 0 le {exit} if\n"
           "      exch pop gsave { exch show ( ) show } repeat grestore LF\n"
           "   } loop pop pop pop pop\n"
           "} bind def\n");

  fprintf (fp,
           "\n/P2 {\n"
           "   {  WC dup 0 le {exit} if\n"
           "      dup 1 sub dup 0 eq\n"
           "      { pop exch pop 0.0 }\n"
           "      { 3 2 roll 3 index exch sub exch div } ifelse\n"
           "      counttomark 3 sub 2 index eq { pop 0 } if exch gsave\n"
           "      {  3 2 roll show ( ) show dup 0 rmoveto } repeat\n"
           "      grestore LF pop\n"
           "   } loop pop pop pop pop\n"
           "} bind def\n");

}

/* ----- define_font ------- */
void define_font (FILE *fp, char *name, int num)
{

  if (!strcmp(name,"Symbol")) {
    fprintf (fp, 
             "/F%d { 1 eq {/bx true def} { /bx false def} ifelse\n"
             "  dup 0.72 mul  /fh exch def\n"
             "  /%s exch selectfont } bind def\n", 
             num, name); 
    return;
  }

  fprintf (fp,
           "\n/%s findfont\n"
           "dup length dict begin\n"
           "   {1 index /FID ne {def} {pop pop} ifelse} forall\n"
           "   /Encoding ISOLatin1Encoding def\n"
           "   currentdict\n"
           "end\n"
           "/%s-ISO exch definefont pop\n",
           name, name);
  
   fprintf (fp, 
            "/F%d { 1 eq {/bx true def} { /bx false def} ifelse\n"
            "  dup 0.72 mul  /fh exch def\n"
            "  /%s-ISO exch selectfont } bind def\n", 
            num, name); 

}


/* ----- def_tsig ------- */
void def_tsig (FILE *fp)
{
  fprintf (fp,
           "\n/tsig { %% x (top) (bot) tsig -- draw time signature\n"
           "   3 -1 roll 0 moveto /bx false def\n"
           "   gsave /NewCenturySchlbk-Bold 16 selectfont\n"
           "   0 0.8 rmoveto currentpoint 3 -1 roll cshow\n"
           "   moveto 0 12 rmoveto cshow grestore\n"
           "} bind def\n"
           );
  fprintf (fp,
           "\n/t1sig { %% x (top) t1sig - timesig without denominator\n"
           "   exch 4.5 moveto /bx false def\n"
           "   /NewCenturySchlbk-Bold 19 selectfont cshow\n"
           "} bind def\n"
           );
}

/* ----- add_curve ------- */
void add_curve (FILE *fp, float f1, float f2, float (*p)[2], int i0, int ncv)
{
  int i,i1,m;
  
  i1=i0;
  for (m=0; m<ncv; m++) {
    fprintf (fp, " ");
    for (i=0; i<3; i++) 
      fprintf (fp, " %.2f %.2f", 
               f1*(p[i1+i][0]-p[i1-1][0]), 
               f2*(p[i1+i][1]-p[i1-1][1]));
    fprintf (fp, " rcurveto\n");
    i1=i1+3;
  }
}

/* ----- add_sg ------- */
void add_sg (FILE *fp, float f1, float f2, float (*p)[2], int i0, int nseg)
{
  int i;
  for (i=0; i<nseg; i++)
    fprintf (fp, "  %.2f %.2f rlineto\n",
             f1*(p[i0+i][0]-p[i0+i-1][0]),
             f2*(p[i0+i][1]-p[i0+i-1][1]));
}

/* ----- add_mv ------- */
void add_mv (FILE *fp, float f1, float f2, float (*p)[2], int i0)
{
  if (i0==0) 
    fprintf (fp, "  %.2f %.2f rmoveto\n", 
             f1*p[i0][0], f2*p[i0][1]);
  else
    fprintf (fp, "  %.2f %.2f rmoveto\n", 
             f1*(p[i0][0]-p[i0-1][0]), 
             f2*(p[i0][1]-p[i0-1][1]));
}


/* ----- def_stems ------- */
void def_stems (FILE *fp)
{
  fprintf (fp, 
           "\n/su { %% len su - up stem\n"
           "  x y moveto %.1f %.1f rmoveto %.1f sub 0 exch rlineto stroke\n"
           "} bind def\n", 
           STEM_XOFF, STEM_YOFF, STEM_YOFF );
  
  fprintf(fp, 
          "\n/sd { %% len td - down stem\n"
          "  x y moveto %.1f %.1f rmoveto neg %.1f add 0 exch rlineto stroke\n"
          "} bind def\n", 
          -STEM_XOFF, -STEM_YOFF, STEM_YOFF);

  fprintf (fp, 
           "\n/sud { %% len su - up stem diamond\n"
           "  x y moveto %.1f %.1f rmoveto %.1f sub 0 exch rlineto stroke\n"
           "} bind def\n", 
           STEM_XOFF, 0.0, STEM_YOFF);
  
  fprintf(fp, 
          "\n/sdd { %% len td - down stem diamond\n"
          "  x y moveto %.1f %.1f rmoveto neg %.1f add 0 exch rlineto stroke\n"
          "} bind def\n", 
          -STEM_XOFF, 0.0, STEM_YOFF);
}

/* ----- def_dot ------- */
void def_dot (FILE *fp)
{
  fprintf(fp, 
          "\n/dt { %% dx dy dt - dot shifted by dx,dy\n"
          "  y add exch x add exch 1.2 0 360 arc fill\n"
          "} bind def\n");
}

/* ----- def_deco ------- */
void def_deco (FILE *fp)
{

  float p[7][2] = {
    {-10,-2},{0,15},{1,-11},{10,2},{0,-15},{-1,11},{-10,-2} };

/*  float q[7][2] = {
    {-13,0},{-2,9},{2,9},{13,0},{3,5},{-3,5},{-13,-0} }; */

/*  float q[7][2] = {
    {-11,0},{-9,10},{9,10},{11,0},{5,7},{-5,7},{-11,-0} }; */

  /* Walsh suggestion, scale 1.8 in y */ 
  float q[7][2] = {
    {-13,0},{-12,9},{12,9},{13,0},{10,7.4},{-10,7.4},{-13,-0} };

  float s[7][2] = {
    {-8,-4.8},{-6,-5.5},{-3,-4.6},{0,0},{-2.3,-5},{-6,-6.8},{-8.5,-6} };

  float f1,f2;
  int i;

  f1=0.5;
  f2=0.5;

  fprintf (fp, "\n/grm { %% x y grm - gracing mark\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "   fill\n} bind def\n");
  fprintf (fp, "\n/stc { %% x y stc - staccato mark\n"
           "  1.2 0 360 arc fill } bind def\n");

  fprintf (fp, "\n/hat { %% x y hat\n"
           "  moveto\n"
           "  -4 -2 rmoveto 4 6 rlineto currentpoint stroke moveto\n"
           "  4 -6 rlineto -2 0 rlineto -3 4.5 rlineto fill\n"
           " } bind def\n");

  fprintf (fp, "\n/att { %% x y att\n"
           "  moveto\n"
           "  -4 -3 rmoveto 8 3 rlineto -8 3 rlineto stroke\n"
           " } bind def\n");

  f2=f2*1.8;

  if (temp_switch==3) { f1=0.8*f1; f2=0.8*f2; }
  else                { f1=0.9*f1; f2=0.9*f2; }
  
  fprintf (fp, "\n/cpu { %% x y cpu - roll sign above head\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,q,0);
  add_curve (fp,f1,f2,q,1,2);
  fprintf (fp, "   fill\n} bind def\n");

  for (i=0;i<7;i++) q[i][1]=-q[i][1];

  fprintf (fp, "\n/cpd { %% x y cpd - roll sign below head\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,q,0);
  add_curve (fp,f1,f2,q,1,2);
  fprintf (fp, "   fill\n} bind def\n");

  f1=0.9;
  f2=1.0;
  fprintf (fp, "\n/sld { %% y dx sld - slide\n"
           "  x exch sub exch moveto\n"); 
  add_mv (fp,f1,f2,s,0);
  add_curve (fp,f1,f2,s,1,2);
  fprintf (fp, "   fill\n} bind def\n");
  
  fprintf (fp, "\n/emb { %% x y emb - tenuto sign\n"
           "  gsave 1.4 setlinewidth 1 setlinecap\n"
           "  moveto -3.5 0 rmoveto 7 0 rlineto stroke grestore\n"
           "} bind def\n");

  fprintf (fp, "\n/trl { %% x y trl - trill sign\n"
           "  gsave /Times-BoldItalic 14 selectfont\n"
           "  moveto (tr) cshow grestore\n"
           "} bind def\n");

  fprintf (fp, "\n/sgno { %% x y sgno - segno\n"
		   "  moveto gsave\n"
		   "  0 3 rmoveto currentpoint currentpoint currentpoint\n"
		   "  2.8 -0.54 2.89 1.18 1.39 1.63 rcurveto\n"
		   "  -2.2 -0.9 -1.4 -3.15 2.76 -2.48 rcurveto\n"
		   "  3.1 2.4 2.54 6.26 -7.71 13.5 rcurveto\n"
		   "  0.5 3.6 3.6 3.24 5.4 2.5 rcurveto\n"
		   "  -2.8 0.54 -2.89 -1.18 -1.39 -1.63 rcurveto\n"
		   "  2.2 0.9 1.4 3.15 -2.76 2.48 rcurveto\n"
		   "  -3.1 -2.4 -2.54 -6.26 7.71 -13.5 rcurveto\n"
		   "  -0.5 -3.6 -3.6 -3.24 -5.4 -2.5 rcurveto\n"
		   "  fill\n"
		   "  moveto 0.6 setlinewidth -5.6 1.6 rmoveto 12.5 12.5 rlineto stroke\n"
		   "  7.2 add exch -5.6 add exch 1 0 360 arc fill\n"
		   "  8.4 add exch 7 add exch 1 0 360 arc fill grestore\n"
		   "} bind def\n");
  
  fprintf (fp, "\n/coda { %% x y coda - coda \n"
                   "  gsave 1.2 setlinewidth 2 copy moveto 0 20 rlineto\n"
		   "  2 copy 10 add exch -10 add exch moveto 20 0 rlineto stroke\n"
		   "  10 add 6 0 360 arc 1.8 setlinewidth stroke grestore\n"
		   "} bind def\n");

  fprintf (fp, "\n/umrd { %% x y umrd - upper mordent\n"
           "  4 add moveto\n"
           "  2.2 2.2 rlineto 2.1 -2.9 rlineto 0.7 0.7 rlineto\n"
           "  -2.2 -2.2 rlineto -2.1 2.9 rlineto -0.7 -0.7 rlineto\n"
           "  -2.2 -2.2 rlineto -2.1 2.9 rlineto -0.7 -0.7 rlineto\n"
           "  2.2 2.2 rlineto 2.1 -2.9 rlineto 0.7 0.7 rlineto fill\n"
           "} bind def\n");

  fprintf (fp, "\n/lmrd { %% x y lmrd - lower mordent\n"
           "  2 copy umrd 8 add moveto\n"
           "  0.6 setlinewidth 0 -8 rlineto stroke\n"
           "} bind def\n");

  fprintf (fp, "\n/turn { %% x y turn - doppelschlag\n"
           "  moveto 5.2 8 rmoveto\n"
           "  1.4 -0.5 0.9 -4.8 -2.2 -2.8 rcurveto\n"
           "  -4.8 3.5 rlineto\n"
           "  -3.0 2.0 -5.8 -1.8 -3.6 -4.4 rcurveto\n"
           "  1.0 -1.1 2.0 -0.8 2.1 0.1 rcurveto\n"
           "  0.1 0.9 -0.7 1.2 -1.9 0.6 rcurveto\n"
           "  -1.4 0.5 -0.9 4.8 2.2 2.8 rcurveto\n"
           "  4.8 -3.5 rlineto\n"
           "  3.0 -2.0 5.8 1.8 3.6 4.4 rcurveto\n"
           "  -1.0 1.1 -2 0.8 -2.1 -0.1 rcurveto\n"
           "  -0.1 -0.9 0.7 -1.2 1.9 -0.6 rcurveto\n"
           "  fill\n"
           "} bind def\n");

  fprintf(fp, "\n/wedge { %% x y wedge \n"
          "  moveto 1.4 7 rlineto\n"
          "  -0.8 2 -1.2 2 -2.8 0 rcurveto\n"
          "  1.4 -7 rlineto fill\n"
          "} bind def\n");

  fprintf (fp, "\n/plus { %% x y plus - plus sign\n"
           "  1 add moveto gsave 1 setlinewidth\n"
           "  0 10 rlineto currentpoint stroke moveto\n"
           "  -5 -5 rmoveto 10 0 rlineto stroke grestore\n"
           "} bind def\n");

  fprintf (fp, "\n/cross { %% x y cross - x shaped cross\n"
           "  gsave 1.2 setlinewidth 1 setlinecap\n"
           "  moveto -3 1 rmoveto 6 8 rlineto currentpoint stroke moveto\n"
           "  0 -8 rmoveto -6 8 rlineto stroke grestore\n"
           "} bind def\n");

  fprintf (fp, "\n/dyn { %% (s) x y dyn - dynamic mark s\n"
           "  gsave /Times-BoldItalic 16 selectfont 5 add moveto cshow grestore\n"
           "} bind def\n");
  
  fprintf (fp, "\n/arp { %% h x ylow arp - arpeggio\n"
           "  gsave 90 rotate exch neg\n"
           "  4 add translate\n"
           "  0 6 3 -1 roll{\n"
           "  2 {\n"
           "    0 0.4 M\n"
           "    2 1.9 3.4 2.3 3.9 0 curveto\n"
           "    2.1 0 lineto\n"
           "    1.9 0.8 1.4 0.7 0 -0.4 curveto\n"
           "    fill\n"
           "    180 rotate -6 0 translate\n"
           "  } repeat\n"
           "  pop 6 0 translate\n"
           "  } for\n"
           "  grestore\n"
           "} bind def\n");
}



/* ----- def_deco1 ------- */
void def_deco1 (FILE *fp)
{

  float p[8][2] = {     /* for hold sign */
    {-15,0},{-15,23},{15,23},{15,0},
    {14.5,0},{12,18},{-12,18},{-14.5,0} };

  float q[8][2] = {    /* for down bow sign */
    {-4,0},{-4,9},{4,9},{4,0},
    {-4,6},{-4,9},{4,9},{4,6} };

  float r[3][2] = {    /* for up bow sign */
    {-3.2,11},{0,0},{3.2,11} };

  float f1,f2;

  f1=f2=0.5;
  fprintf (fp, "\n/hld { %% x y hld - hold sign\n"
           "  2 copy moveto 2 copy 1.5 add 1.3 0 360 arc moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,1);
  add_sg (fp,f1,f2,p,4,1);
  add_curve (fp,f1,f2,p,5,1);
  fprintf (fp, "   fill\n} bind def\n");

  f1=f2=0.8;
  fprintf (fp, "\n/dnb { %% x y dnb - down bow\n"
           "  moveto\n");
  add_mv (fp,f1,f2,q,0);
  add_sg (fp,f1,f2,q,1,3);
  fprintf (fp, "   currentpoint stroke moveto\n");
  add_mv (fp,f1,f2,q,4);
  add_sg (fp,f1,f2,q,5,3);
  fprintf (fp, "   fill\n} bind def\n");

  fprintf (fp, "\n/upb { %% x y upb - up bow\n"
           "  moveto\n");
  add_mv (fp,f1,f2,r,0);
  add_sg (fp,f1,f2,r,1,2);
  fprintf (fp, "   stroke\n} bind def\n");

  fprintf (fp, "\n/brth { %% x y brth - breath mark\n"
           "  gsave 0.8 setlinewidth\n"
           "  moveto -1 -3 rmoveto 2 7 rlineto stroke grestore\n"
           "} bind def\n");

}

/* ----- def_hl ------- */
void def_hl (FILE *fp)
{
  fprintf(fp, 
          "\n/hl { %% y hl - ledger line at height y\n"
          "   gsave 1 setlinewidth x exch moveto \n"
          "   -5.5 0 rmoveto 11 0 rlineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, 
          "\n/hl1 { %% y hl1 - longer ledger line for whole notes\n"
          "   gsave 1 setlinewidth x exch moveto \n"
          "   -7 0 rmoveto 14 0 rlineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, 
          "\n/hl2 { %% y hl2 - longer ledger line for brevis\n"
          "   gsave 1 setlinewidth x exch moveto \n"
          "   -8 0 rmoveto 16 0 rlineto stroke grestore\n"
          "} bind def\n");
}

/* ----- def_beam ------- */
void def_beam (FILE *fp)
{
  fprintf(fp, 
          "\n/bm { %% x1 y1 x2 y2 t bm - beam, depth t\n"
          "  3 1 roll moveto dup 0 exch neg rlineto\n"
          "  dup 4 1 roll sub lineto 0 exch rlineto fill\n"
          "} bind def\n");

  fprintf(fp, 
          "\n/bnum { %% x y (str) bnum - number on beam\n"
          "  3 1 roll moveto gsave /Times-Italic 12 selectfont\n"
          "  /bx false def cshow grestore\n"
          "} bind def\n");
  
  fprintf(fp, 
          "\n/hbr { %% x1 y1 x2 y2 hbr - half bracket\n"
          "  moveto lineto 0 -3 rlineto stroke\n"
          "} bind def\n");

  fprintf(fp, 
          "\n/fbr { %% x1 y1 x2 y2 fbr - full bracket\n"
          "  moveto currentpoint 4 2 roll lineto 0 -3 rlineto\n"
          "  moveto 0 -3 rlineto stroke\n"
          "} bind def\n");
}


/* ----- def_flags1 ------- */
void def_flags1 (FILE *fp)
{
  float p[13][2] = {
    {0.0, 0.0},  {1.5, -3.0},  {1.0, -2.5},  {4.0, -6.0},  {9.0, -10.0},  
    {9.0, -16.0},  {8.0, -20.0},  {7.0, -24.0},  {4.0, -26.0},  
    {6.5, -21.5},  {9.0, -15.0},  {4.0, -9.0},  {0.0, -8.0} } ;

  float f1,f2;
  int i;

  f1=f2=6.0/9.0;
  fprintf (fp, "\n/f1u { %% len f1u - single flag up\n"
           "  y add x %.1f add exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  fprintf (fp, "   fill\n} bind def\n");

  f1=1.2*f1;
  for (i=0;i<13;i++) p[i][1]=-p[i][1];
  fprintf (fp, "\n/f1d { %% len f1d - single flag down\n"
           "  neg y add x %.1f sub exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  fprintf (fp, "   fill\n} bind def\n");
  
}

/* ----- def_flags2 ------- */
void def_flags2 (FILE *fp)
{

  float p[13][2] = {
    {0.0, 0.0},  
    {2.0, -5.0},  {9.0, -6.0},  {7.5, -18.0},  
    {7.5, -9.0},  {1.5, -6.5},  {0.0, -6.5},  
    {2.0, -14.0},  {9.0, -14.0}, {7.5, -26.0}, 
    {7.5, -17.0},  {1.5, -14.5},  {0.0, -14.0},
  };

  float f1,f2;
  int i;

  f1=f2=6.0/9.0;                       /* up flags */
  fprintf (fp, "\n/f2u { %% len f2u - double flag up\n"
           "  y add x %.1f add exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  fprintf (fp, "   fill\n} bind def\n");

  f1=1.2*f1;                           /* down flags */
  for (i=0;i<13;i++) p[i][1]=-p[i][1];
  fprintf (fp, "\n/f2d { %% len f2d - double flag down\n"
           "  neg y add x %.1f sub exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  fprintf (fp, "   fill\n} bind def\n");

}  



/* ----- def_xflags ------- */
void def_xflags (FILE *fp)
{

  float p[7][2] = {
    {0.0, 0.0},  
    {2.0, -7.5},  {9.0, -7.5}, {7.5, -19.5}, 
    {7.5, -10.5},  {1.5, -8.0},  {0.0, -7.5}
  };

  float f1,f2;
  int i;

  f1=f2=6.0/9.0;                       /* extra up flag */
  fprintf (fp, "\n/xfu { %% len xfu - extra flag up\n"
           "  y add x %.1f add exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "   fill\n} bind def\n");

  f1=1.2*f1;                           /* extra down flag */
  for (i=0;i<7;i++) p[i][1]=-p[i][1];
  fprintf (fp, "\n/xfd { %% len xfd - extra flag down\n"
           "  neg y add x %.1f sub exch moveto\n", STEM_XOFF); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "   fill\n} bind def\n");

  fprintf (fp, 
           "\n/f3d {dup f2d 9.5 sub xfd} bind def\n");

  fprintf (fp, 
           "\n/f4d {dup dup f2d 9.5 sub xfd 14.7 sub xfd} bind def\n");

  fprintf (fp, 
           "\n/f3u {dup f2u 9.5 sub xfu} bind def\n");

  fprintf (fp, 
           "\n/f4u {dup dup f2u 9.5 sub xfu 14.7 sub xfu} bind def\n");

}  

/* ----- def_acc ------- */
void def_acc (FILE *fp)
{
  float p[12][2]={  
    {-2,3},{6,6.5},{6,-1},{-2,-4.5},{4,0},{4,4},{-2,2},{-2,10},{-2,-4}};  
  float q[14][2]={  
    {4,4},{4,7},{-4,5},{-4,2},{4,4},{4,-5},{4,-2},{-4,-4},{-4,-7},{4,-5}, 
    {2,-10},{2,11.5},{-2,-11.5},{-2,10} };
  float r[14][2]={  
    {-2.5,-6}, {2.5,-5}, {2.5,-2}, {-2.5,-3}, {-2.5,6},  
    {-2.5,2}, {2.5,3}, {2.5,6}, {-2.5,5}, {-2.5,2},
    {-2.5,11}, {-2.5,-5.5},  
    {2.5,5.5}, {2.5,-11} };
  float s[25][2]={
    {0.7,0},{3.9,3},{6,3},{6.2,6.2},{3,6},{3,3.9},
    {0,0.7},{-3,3.9},{-3,6},{-6.2,6.2},{-6,3},{-3.9,3},
    {-0.7,0},{-3.9,-3},{-6,-3},{-6.2,-6.2},{-3,-6},{-3,-3.9},
    {0,-0.7},{3,-3.9},{3,-6},{6.2,-6.2},{6,-3},{3.9,-3},
    {0.7,0} };


  float f1,f2;
  
  f2=8.0/9.0;
  f1=f2*0.9;
  fprintf (fp, "\n/ft0 { %% x y ft0 - flat sign\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  currentpoint fill moveto\n");
  add_mv (fp,f1,f2,p,7);
  add_sg (fp,f1,f2,p,8,1);
  fprintf (fp, "  stroke\n } bind def\n");
  fprintf (fp, "/ft { %% dx ft - flat relative to head\n"
           " neg x add y ft0 } bind def\n");
  
  f2=8.0/9.0;    /* more narrow flat sign for double flat */
  f1=f2*0.8;
  fprintf (fp, "\n/ftx { %% x y ftx - narrow flat sign\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  currentpoint fill moveto\n");
  add_mv (fp,f1,f2,p,7);
  add_sg (fp,f1,f2,p,8,1);
  fprintf (fp, "  stroke\n } bind def\n");

  fprintf (fp, "/dft0 { %% x y dft0 ft - double flat sign\n"
           "  2 copy exch 2.5 sub exch ftx exch 1.5 add exch ftx } bind def\n"
           "/dft { %% dx dft - double flat relative to head\n"
           "  neg x add y dft0 } bind def\n");


  f2=6.5/9.0;
  f1=f2*0.9;
  fprintf (fp, "\n/sh0 { %% x y sh0 - sharp sign\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,q,0);
  add_sg (fp,f1,f2,q,1,4);
  add_mv (fp,f1,f2,q,5);
  add_sg (fp,f1,f2,q,6,4);
  fprintf (fp, "  currentpoint fill moveto\n");
  add_mv (fp,f1,f2,q,10);
  add_sg (fp,f1,f2,q,11,1);
  fprintf (fp, "  currentpoint stroke moveto\n");
  add_mv (fp,f1,f2,q,12);
  add_sg (fp,f1,f2,q,13,1);
  fprintf (fp, "  stroke\n } bind def\n");
  fprintf (fp, "/sh { %% dx sh - sharp relative to head\n"
           " neg x add y sh0 } bind def\n");
  fprintf (fp, "/sh1 { %% sh1 - sharp inline at currentpos\n"
           "  currentpoint\n"
           "  currentpoint moveto 3.5 4.6 rmoveto gsave 0.75 0.75 scale currentpoint sh0 grestore\n"
           "  moveto 7 0 rmoveto\n"
           "} bind def\n"
           "/ft1 { %% ft1 - flat inline at currentpos\n"
           "  currentpoint\n"
           "  currentpoint moveto 3.7 3.1 rmoveto gsave 0.75 0.75 scale currentpoint ft0 grestore\n"
           "  moveto 7 0 rmoveto\n"
           "} bind def\n"
           "/nt1 { %% nt1 - natural inline at currentpos\n"
           "  currentpoint\n"
           "  currentpoint moveto 4 4 rmoveto gsave 0.75 0.75 scale currentpoint nt0 grestore\n"
           "  moveto 7 0 rmoveto\n"
           "} bind def\n"
           );


  f2=6.5/9.0;
  f1=f2*0.9;
  fprintf (fp, "\n/nt0 { %% x y nt0 - neutral sign\n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,r,0);
  add_sg (fp,f1,f2,r,1,4);
  add_mv (fp,f1,f2,r,5);
  add_sg (fp,f1,f2,r,6,4);
  fprintf (fp, "  currentpoint fill moveto\n");
  add_mv (fp,f1,f2,r,10);
  add_sg (fp,f1,f2,r,11,1);
  fprintf (fp, "  currentpoint stroke moveto\n");
  add_mv (fp,f1,f2,r,12);
  add_sg (fp,f1,f2,r,13,1);
  fprintf (fp, "  stroke\n } bind def\n");
  fprintf (fp, "/nt { %% dx nt - neutral relative to head\n"
           " neg x add y nt0 } bind def\n");

  f1=5.0/9.0;
  f2=f1;
  fprintf (fp, "\n/dsh0 { %% x y dsh0 - double sharp \n"
           "  moveto\n"); 
  add_mv (fp,f1,f2,s,0);
  add_sg (fp,f1,f2,s,1,24);
  fprintf (fp, "  fill\n } bind def\n");
  fprintf (fp, "/dsh { %% dx dsh - double sharp relative to head\n"
           " neg x add y dsh0 } bind def\n");
}

/* ----- def_rests ------- */
void def_rests (FILE *fp)
{
  float p[14][2]={  
    {-1,17}, {15,4}, {-6,8}, {6.5,-5}, {-2,-2}, {-5,-11}, {1,-15},
    {-9,-11}, {-6,0}, {1,-1},   {-9,7}, {7,5}, {-1,17} };
  float q[16][2]={  
    {8,14}, {5,9}, {3,5}, {-1.5,4},
    {4,11}, {-9,14}, {-9,7},
    {-9,4}, {-6,2}, {-3,2},
    {4,2}, {5,7}, {7,11},
    {-1.8,-20},  {-0.5,-20}, {8.5,14}};
  float r[29][2]={  
    {8,14}, {5,9}, {3,5}, {-1.5,4},
    {4,11}, {-9,14}, {-9,7},
    {-9,4}, {-6,2}, {-3,2},
    {4,2}, {5,7}, {7,11},
    {8,14}, {5,9}, {3,5}, {-1.5,4},
    {4,11}, {-9,14}, {-9,7},
    {-9,4}, {-6,2}, {-3,2},
    {4,2}, {5,7}, {7.3,11},
    {-1.8,-21},  {-0.5,-21}, {8.5,14} };
  float f1,f2;
  int i;

  fprintf (fp, "\n/r4 { %% x y r4 - quarter rest\n"
           "   dup /y exch def exch dup /x exch def exch moveto\n");
  f1=f2=6.0/11.5;
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  fprintf (fp, "  fill\n } bind def\n");

  fprintf (fp, "\n/r8 { %% x y r8 - eighth rest\n"
           "   dup /y exch def exch dup /x exch def exch moveto\n");
  f1=f2=7/18.0;
  add_mv (fp,f1,f2,q,0);
  add_curve (fp,f1,f2,q,1,4);
  add_sg (fp,f1,f2,q,13,3);
  fprintf (fp, "  fill\n } bind def\n");

  for (i=13;i<26;i++) { r[i][0]-=4.2; r[i][1]-=14; }
  fprintf (fp, "\n/r16 { %% x y r16 - 16th rest\n"
           "   dup /y exch def exch dup /x exch def exch moveto\n");
  f1=f2=7/18.0;
  add_mv (fp,f1,f2,r,0);
  add_curve (fp,f1,f2,r,1,4);
  add_sg (fp,f1,f2,r,13,1);
  add_curve (fp,f1,f2,r,14,4);
  add_sg (fp,f1,f2,r,26,3);
  fprintf (fp, "  fill\n } bind def\n");

  fprintf (fp, 
           "\n/r00 { %% x y r00 - longa rest\n"
           "  2 copy /y exch def /x exch def moveto\n"
           "  -1 6 rmoveto 0 -12 rlineto 3 0 rlineto 0 12 rlineto fill\n"
           "} bind def\n");

  fprintf (fp, 
           "\n/r0 { %%	x y r0	-  brevis rest\n"
           "  6 add dup /y exch def exch dup /x exch def exch moveto\n"
           "  -1 0 rmoveto 0 -6 rlineto 3 0 rlineto 0 6 rlineto fill\n"
           "} bind def\n");

  fprintf (fp, 
           "\n/r1 { %% x y r1 - whole rest\n"
           "  dup /y exch def exch dup /x exch def exch moveto\n"
           "  -3 6 rmoveto 0 -3 rlineto 6 0 rlineto 0 3 rlineto fill\n"
           "} bind def\n");

  fprintf (fp, 
           "\n/r2 { %% x y r2 - half rest\n"
           "  dup /y exch def exch dup /x exch def exch moveto\n"
           "  -3 0 rmoveto 0 3 rlineto 6 0 rlineto 0 -3 rlineto fill\n"
           "} bind def\n"
           );
  
  /* get 32nd, 64th rest by overwriting 8th and 16th rests */
  fprintf (fp,
           "\n/r32 {\n"
           "2 copy r16 5.5 add exch 1.6 add exch r8\n"
           "} bind def\n");
  fprintf (fp,
           "\n/r64 {\n"
           "2 copy 5.5 add exch 1.6 add exch r16\n"
           "5.5 sub exch 1.5 sub exch r16\n"
           "} bind def\n");

  fprintf (fp,
	   "\n/brest { %% nb_measures x y brest\n"
	   "   gsave T 0.8 setlinewidth\n"
	   "   0 -6 moveto 0 12 rlineto 40 -6 moveto 0 12 rlineto stroke\n"
	   "   3 setlinewidth 0 0 moveto 40 0 rlineto stroke\n"
	   "   /Times-Bold 13 selectfont 20 16 moveto cshow grestore\n"
	   "} bind def \n");  

}


/* ----- def_bars ------ */
void def_bars (FILE *fp)
{

  fprintf(fp, "\n/bar { %% x bar - single bar\n"
          "  0 moveto  0 24 rlineto stroke\n"
          "} bind def\n"
          
          "\n/dbar { %% x dbar - thin double bar\n"
          "   0 moveto 0 24 rlineto -3 -24 rmoveto\n"
          "   0 24 rlineto stroke\n"
          "} bind def\n"

          "\n/fbar1 { %% x fbar1 - fat double bar at start\n"
          "  0 moveto  0 24 rlineto 3 0 rlineto 0 -24 rlineto \n"
          "  currentpoint fill moveto\n"
          "  3 0 rmoveto 0 24 rlineto stroke\n"
          "} bind def\n"

          "\n/fbar2 { %% x fbar2 - fat double bar at end\n"
          "  0 moveto  0 24 rlineto -3 0 rlineto 0 -24 rlineto \n"
          "  currentpoint fill moveto\n"
          "  -3 0 rmoveto 0 24 rlineto stroke\n"
          "} bind def\n"

          "\n/rdots { %% x rdots - repeat dots \n"
          "  0 moveto 0 9 rmoveto currentpoint 2 copy 1.2 0 360 arc \n"
          "  moveto 0 6 rmoveto  currentpoint 1.2 0 360 arc fill\n"
          "} bind def\n");
}

/* ----- def_ends ------ */
void def_ends (FILE *fp)
{
  /* use dy=6 for smaller boxes */
  int dy=12;

  fprintf(fp, "\n/end1 { %% x1 x2 h (str) end1 - mark first ending\n"
          "  4 1 roll -%d add dup 3 1 roll moveto 0 %d rlineto 2 copy %d add lineto 0 -%d rlineto stroke\n"
          "  moveto 4 %d rmoveto gsave /Times-Roman 13 selectfont 1.2 0.95 scale\n"
          "  show grestore\n"
          "} bind def\n",
          dy, dy, dy, dy, dy-10);

  fprintf(fp, "\n/end2 { %% x1 x2 h (str) end2 - mark second ending\n"
          "  4 1 roll dup 3 1 roll moveto 2 copy lineto 0 -%d rlineto stroke\n"
          "  moveto 4 -10 rmoveto gsave /Times-Roman 13 selectfont 1.2 0.95 scale\n"
          "  show grestore\n"
          "} bind def\n",
          dy);
}  
       
/* ----- def_gchord ------ */
void def_gchord (FILE *fp)
{
  /*  fprintf(fp,"\n/gc { %% x y (str) gc  -- draw guitar chord string\n"
          "  3 1 roll moveto rshow\n"
          "} bind def\n"); */
}

/* ----- def_sl ------ */
void def_sl (FILE *fp)
{
/*  fprintf(fp, "\n/sl { %% x1 y2 x2 y2 x3 y3 x0 y0 sl\n"
          "  gsave %.1f setlinewidth moveto curveto stroke grestore\n"
          "} bind def\n", SLURWIDTH); */

  fprintf(fp, "\n/SL { %% pp2x pp1x p1 pp1 pp2 p2 p1 sl\n"
          "  moveto curveto rlineto curveto fill\n"
          "} bind def\n");

}

/* ----- def_hd1 ------- */
void def_hd1 (FILE *fp)
{
  float p[7][2] = {
    {8.0, 0.0},  {8.0, 8.0},  {-8.0, 8.0},  {-8.0, 0.0},  {-8.0, -8.0},  
    {8.0, -8.0},  {8.0, 0.0} };

  float c,s,xx,yy,f1,f2;
  int i;
/*float phi; */
  
/*phi=0.6;
  c=cos(phi);
  s=sin(phi); */
  
  c=0.825; s=0.565;

  for (i=0;i<7;i++) {
    xx = c*p[i][0] - s*p[i][1];
    yy = s*p[i][0] + c*p[i][1];
    p[i][0]=xx;
    p[i][1]=yy;
  }
  
  f1=f2=6.0/12.0;
  fprintf (fp, "\n/hd { %% x y hd - full head\n"
           "  dup /y exch def exch dup /x exch def exch moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "   fill\n} bind def\n");
}

/* ----- def_hd2 ------- */
void def_hd2 (FILE *fp)
{

  float p[14][2] = {
    {8.0, 0.0},  {8.0, 8.5},  {-8.0, 8.5},  {-8.0, 0.0},  {-8.0, -8.5},  
    {8.0, -8.5},  {8.0, 0.0},  {7.0, 0.0},  {7.0, -4.0},  {-7.0, -4.0}, 
    {-7.0, 0.0},  {-7.0, 4.0},  {7.0, 4.0},  {7.0, 0.0} };

/*  float phi; */
  float c,s,xx,yy,f1,f2;
  int i;

/*phi=0.5;
  c=cos(phi);
  s=sin(phi); */

  c=0.878; s=0.479;

  for (i=0;i<14;i++) {
    xx = c*p[i][0] - s*p[i][1];
    yy = s*p[i][0] + c*p[i][1];
    p[i][0]=xx;
    p[i][1]=yy;
  }
  
  f1=f2=6.0/12.0;
  fprintf (fp, "\n/Hd { %% x y Hd - open head for half\n"
           "  dup /y exch def exch dup /x exch def exch moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  add_mv (fp,f1,f2,p,7);
  add_curve (fp,f1,f2,p,8,2);
  fprintf (fp, "   fill\n} bind def\n");
}

/* ----- def_hd3 ------- */
void def_hd3 (FILE *fp)
{

  float p[13][2] = {
    {11.0, 0.0}, {11.0, 2.0},  {6.0, 6.5},  {0.0, 6.5},  {-6.0, 6.5}, 
    {-11.0, 2.0},  {-11.0, 0.0},  {-11.0, -2.0},  {-6.0, -6.5}, 
    {0.0, -6.5},  {6.0, -6.5},  {11.0, -2.0},  {11.0, 0.0}  };

  float q[8][2] = {
    {11.0, 0.0},  {5.0, 0.0},  {5.0, -5.0},  {-5.0, -5.0},  {-5.0, 0.0},  
    {-5.0, 5.0},  {5.0, 5.0},  {5.0, 0.0}};
           
/*  float phi; */
  float c,s,xx,yy,f1,f2;
  int i;

/*phi=2.5;
  c=cos(phi);
  s=sin(phi); */
  
  c=-0.801; s=0.598;

  for (i=1;i<8;i++) {
    xx = c*q[i][0] - s*q[i][1];
    yy = s*q[i][0] + c*q[i][1];
    q[i][0]=xx;
    q[i][1]=yy;
  }
  
  f1=f2=6.5/12.0;
  fprintf (fp, "\n/HD { %% x y HD - open head for whole\n"
           "  dup /y exch def exch dup /x exch def exch moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,4);
  add_mv (fp,f1,f2,q,1);
  add_curve (fp,f1,f2,q,2,2);
  fprintf (fp, "   fill\n} bind def\n");

  /* brevis (round and square) and longa */
  fprintf (fp,
           "\n/HDD { %% x y HDD - double note\n"
           "  HD\n"
           "  x y moveto -6 -4 rmoveto 0 8 rlineto stroke\n"
           "  x y moveto 6 -4 rmoveto 0 8 rlineto stroke\n"
           "} bind def\n");
  fprintf (fp,
           "\n/brevis { %% x y brevis - square brevis\n"
           "  2 copy /y exch def /x exch def moveto\n"
           "  2.5 setlinewidth -6 -2.7 rmoveto 12 0 rlineto\n"
           "  0 5.4 rmoveto -12 0 rlineto stroke\n"
           "  0.8 setlinewidth x y moveto -6 -5 rmoveto 0 10 rlineto\n"
           "  x y moveto 6 -5 rmoveto 0 10 rlineto stroke\n"
           "} bind def\n");
  fprintf (fp,
           "\n/longa { %% x y longa\n"
           "  brevis x y moveto 6 -10 rmoveto 0 15 rlineto stroke\n"
           "} bind def\n");
}

/* ----- def_historic ------- */
void def_historic (FILE *fp)
{
  /* diamond shaped heads */
  fprintf(fp, "\n/hdh { %% x y hdh - full head (historic)\n"
          "  dup /y exch def exch dup /x exch def exch\n"
          "  moveto 0 -3.5 rmoveto\n"
          "  3.5 3.5 rlineto -3.5 3.5 rlineto -3.5 -3.5 rlineto 3.5 -3.5 rlineto fill\n"
          "} bind def\n");

  fprintf(fp, "\n/Hdh { %% x y Hdh - open head (historic)\n"
          "  dup /y exch def exch dup /x exch def exch\n"
          "  gsave 1 setlinewidth\n"
          "  moveto 0 -4.0 rmoveto\n"
          "  4.0 4.0 rlineto currentpoint\n"
          "  -0.4 0.4 rlineto -4.0 -4.0 rlineto 0.4 -0.4 rlineto fill moveto\n"
          "  -4.0 4.0 rlineto currentpoint\n"
          "  -1.1 -1.1 rlineto 4.0 -4.0 rlineto 1.1 1.1 rlineto fill moveto\n"
          "  -4.0 -4.0 rlineto currentpoint\n"
          "  0.4 -0.4 rlineto 4.0 4.0 rlineto -0.4 0.4 rlineto fill moveto\n"
          "  4.0 -4.0 rlineto currentpoint\n"
          "  1.1 1.1 rlineto -4.0 4.0 rlineto -1.1 -1.1 rlineto fill moveto\n"
          "  grestore\n"
          "} bind def\n");

  /* stems and flags */
  fprintf(fp, "\n/suh { %% len su - up stem (historic)\n"
          "  gsave 1 setlinewidth\n"
          "  x y moveto 0 3.5 rmoveto 1.0 sub 0 exch rlineto stroke\n"
          "  grestore\n"
          "} bind def\n"
          "\n"
          "/sdh { %% len sd - down stem (historic)\n"
          "  gsave 1 setlinewidth\n"
          "  x y moveto 0 -3.5 rmoveto neg 1.0 add 0 exch rlineto stroke\n"
          "  grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/f1uh { %% len f1uh - single flag up (historic)\n"
          "  2.5 add y add x exch moveto\n"
          "  7.0 1.0 4.5 -5.0 0.0 -9.0 rcurveto 0 1 rlineto\n"
          "  3.0 3.5 5.0 6.0 0.0 6.0 rcurveto closepath\n"
          "  fill\n"
          "} bind def\n"
          "\n"
          "/f1dh { %% len f1dh - single flag down (historic)\n"
          "  2.5 add neg y add x exch moveto\n"
          "  -7.0 -1.0 -4.5 5.0 0.0 9.0 rcurveto 0 -1 rlineto\n"
          "  -3.0 -3.5 -5.0 -6.0 0.0 -6.0 rcurveto closepath\n"
          "  fill\n"
          "} bind def\n");

  fprintf(fp, "\n/f2uh { %% len f2uh - double flag up (historic)\n"
          "  2.5 add y add x exch moveto\n"
          "  7.0 1.0 4.5 -5.0 0.0 -9.0 rcurveto 0 1 rlineto\n"
          "  3.0 3.5 5.0 6.0 0.0 6.0 rcurveto closepath\n"
          "  currentpoint fill gsave moveto\n"
          "  1 setlinewidth 0 -7 rmoveto 5.0 -6.5 rlineto stroke grestore\n"
          "} bind def\n"
          "\n"
          "/f2dh { %% len f2d - double flag down (historic)\n"
          "  2.5 add neg y add x exch moveto\n"
          "  -7.0 -1.0 -4.5 5.0 0.0 9.0 rcurveto 0 -1 rlineto\n"
          "  -3.0 -3.5 -5.0 -6.0 0.0 -6.0 rcurveto closepath\n"
          "  currentpoint fill gsave moveto\n"
          "  1 setlinewidth 0 7 rmoveto -5.0 6.5 rlineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/f4dh {f3dh} bind def %% unsupported\n");

  /* brevis and longa */
  fprintf(fp, "\n/brevish { %% x y brevish - brevis (historic)\n"
          "  2 copy /y exch def /x exch def moveto\n"
          "  1.8 setlinewidth -4 -2.5 rmoveto 8 0 rlineto\n"
          "  0 5.0 rmoveto -8 0 rlineto stroke\n"
          "  0.8 setlinewidth x y moveto -4 -4.8 rmoveto 0 9.6 rlineto\n"
          "  x y moveto 4 -4.8 rmoveto 0 9.6 rlineto stroke\n"
          "} bind def\n");

  fprintf(fp, "\n/longah { %% x y longah - longa (historic)\n"
          "  brevish x y moveto 4 -2.5 rmoveto 0 -10 rlineto stroke\n"
          "} bind def\n");

  /* time signatures */
  double r2 = 5.0;    /*outer radius*/
  double arc1 = 35.0; /*inner border angle*/
  double thick = 1.2; /*line thickness*/
  double r1 = r2 - thick; /*inner radius*/
  double arc2 = M_1_PI * 180.0 * acos(cos(arc1*M_PI/180.0) * r1 / r2);
  fprintf(fp, "\n/csigh { %% x csigh - C timesig (historic)\n"
          "  newpath dup 12 %.1f %.1f -%.1f arc 12 %.1f -%.1f %.1f arcn closepath fill\n"
          "} bind def\n", r2, arc2, arc2, r1, arc1, arc1);

  fprintf(fp, "\n/ctsigh { %% x ctsigh - C| timesig historic\n"
          "  dup csigh 3 moveto 0 18 rlineto stroke\n"
          "} bind def\n");

}

/* ----- def_gnote ------- */
void def_gnote (FILE *fp)
{
  /* parameter for filled head */
  float p[7][2] = {
    {0,10}, {16,10}, {16,-10}, {0,-10}, {-16,-10}, {-16,10}, {0,10} };

  /* parameter for half note head */
  float ph[14][2] = {
    {8.0, 0.0},  {8.0, 8.5},  {-8.0, 8.5},  {-8.0, 0.0},  {-8.0, -8.5},  
    {8.0, -8.5},  {8.0, 0.0},  {7.0, 0.0},  {7.0, -4.0},  {-7.0, -4.0}, 
    {-7.0, 0.0},  {-7.0, 4.0},  {7.0, 4.0},  {7.0, 0.0} };

  /* parameter for whole note head */
  float pw[13][2] = {
    {11.0, 0.0}, {11.0, 2.0},  {6.0, 6.5},  {0.0, 6.5},  {-6.0, 6.5}, 
    {-11.0, 2.0},  {-11.0, 0.0},  {-11.0, -2.0},  {-6.0, -6.5}, 
    {0.0, -6.5},  {6.0, -6.5},  {11.0, -2.0},  {11.0, 0.0}  };
  float qw[8][2] = {
    {11.0, 0.0},  {5.0, 0.0},  {5.0, -5.0},  {-5.0, -5.0},  {-5.0, 0.0},  
    {-5.0, 5.0},  {5.0, 5.0},  {5.0, 0.0}};

/*  float phi; */
  float c,s,xx,yy,f1,f2,fh,fw;
  int i;
  
/*phi=0.7;
  c=cos(phi);
  s=sin(phi); */
  
  c=0.765; s=0.644;
  for (i=0;i<7;i++) {
    xx = c*p[i][0] - s*p[i][1];
    yy = s*p[i][0] + c*p[i][1];
    p[i][0]=xx;
    p[i][1]=yy;
  }
  for (i=0;i<14;i++) {
    xx = c*ph[i][0] - s*ph[i][1];
    yy = s*ph[i][0] + c*ph[i][1];
    ph[i][0]=xx;
    ph[i][1]=yy;
  }

  c=-0.801; s=0.598;
  for (i=1;i<8;i++) {
    xx = c*qw[i][0] - s*qw[i][1];
    yy = s*qw[i][0] + c*qw[i][1];
    qw[i][0]=xx;
    qw[i][1]=yy;
  }
  
  
  f1=f2=2./10.0;

  fprintf (fp, "\n/gn16 { %% x y l gn16 - single sixteenth grace note\n"
           "  3 1 roll 2 copy moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  fill moveto %.2f 0 rmoveto 0 exch rlineto currentpoint\n"
           "  3 -4 4 -5 3 -8 rcurveto -5 2 rmoveto stroke\n"
           "  3.4 sub moveto\n"
           "  3 -4 4 -5 2 -8 rcurveto -5 2 rmoveto stroke\n",
           GSTEM_XOFF);
  fprintf (fp, "} bind def\n");

  fprintf (fp, "\n/gn8 { %% x y l gn8 - single eighth grace note\n"
           "  3 1 roll 2 copy moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  fill moveto %.2f 0 rmoveto 0 exch rlineto\n"
           "3 -4 4 -5 2 -8 rcurveto -5 2 rmoveto\n"
           "stroke\n",
           GSTEM_XOFF);
  fprintf (fp, "} bind def\n");

  fprintf (fp, "\n/gn8s { %% x y l gn8s - single eighth grace note with stroke\n"
           "  3 1 roll 2 copy moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  fill moveto %.2f 0 rmoveto 0 exch rlineto\n"
           "3 -4 4 -5 2 -8 rcurveto -5 2 rmoveto 7 4 rlineto\n"
           "stroke\n",
           GSTEM_XOFF);
  fprintf (fp, "} bind def\n");

  fprintf (fp, "\n/gnt { %% x y l gnt - grace note without flag\n"
           "  3 1 roll 2 copy moveto\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,2);
  fprintf (fp, "  fill moveto %.2f 0 rmoveto 0 exch rlineto stroke\n",
           GSTEM_XOFF);
  fprintf (fp, "} bind def\n");

  fprintf (fp, "\n/gn2 { %% x y l gn2 - grace half note\n"
           "  3 1 roll 2 copy moveto\n");
  fh=8.0/25.0;
  add_mv (fp,fh,fh,ph,0);
  add_curve (fp,fh,fh,ph,1,2);
  add_mv (fp,fh,fh,ph,7);
  add_curve (fp,fh,fh,ph,8,2);
  fprintf (fp, "  fill moveto %.2f 0 rmoveto 0 exch rlineto stroke\n",
           GSTEM_XOFF);
  fprintf (fp, "} bind def\n");

  fprintf (fp, "\n/gn1 { %% x y gn1 - whole grace note\n"
           "  2 copy moveto\n");
  fw=9.0/25.0;
  add_mv (fp,fw,fw,pw,0);
  add_curve (fp,fw,fw,pw,1,4);
  add_mv (fp,fw,fw,qw,1);
  add_curve (fp,fw,fw,qw,2,2);
  fprintf (fp, "  fill\n");
  fprintf (fp, "} bind def\n");

  fprintf(fp, "\n/gbm1 { %% x1 y1 x2 y2 gbm1 - single note beam\n"
          "  gsave 1.4 setlinewidth\n"
          "  0.5 sub moveto 0.5 sub lineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gbm2 { %% x1 y1 x2 y2 gbm2 - double note beam\n"
          "  gsave 1.4 setlinewidth\n"
          "  4 copy 0.5 sub moveto 0.5 sub lineto stroke\n"
          "  3.4 sub moveto 3.4 sub lineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gbm3 { %% x1 y1 x2 y2 gbm3 - triple gnote beam\n"
          "  gsave 1.2 setlinewidth\n"
          "  4 copy 0.3 sub moveto 0.3 sub lineto stroke\n"
          "  4 copy 2.5 sub moveto 2.5 sub lineto stroke\n"
          "  4.7 sub moveto 4.7 sub lineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/ghl { %% x y ghl - grace note ledger line\n"
          "   gsave 0.7 setlinewidth moveto \n"
          "   -3 0 rmoveto 6 0 rlineto stroke grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gsl { %% x1 y2 x2 y2 x3 y3 x0 y0 gsl\n"
          "  moveto curveto stroke\n"
          "} bind def\n");

  fprintf(fp, "\n/gsh0 { %% x y gsh0\n"
          "gsave translate 0.7 0.7 scale 0 0 sh0 grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gft0 { %% x y gft0\n"
          "gsave translate 0.7 0.7 scale 0 0 ft0 grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gnt0 { %% x y gnt0\n"
          "gsave translate 0.7 0.7 scale 0 0 nt0 grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gdf0 { %% x y gdf0\n"
          "gsave translate 0.7 0.6 scale 0 0 dft0 grestore\n"
          "} bind def\n");

  fprintf(fp, "\n/gds0 { %% x y gds0\n"
          "gsave translate 0.7 0.7 scale 0 0 dsh0 grestore\n"
          "} bind def\n");
}


/* ----- def_csig ------- */
void def_csg (FILE *fp)
{
  float p[25][2]={
  {0,26},  
  {4,26}, {11,23},  {11,14}, 
  {11,20},  {5,19}, {5,14}, 
  {5,9}, {12,9}, {12,15}, 
  {12,25}, {6,28},  {0,28}, 
  {-15,28}, {-25,17}, {-25,2}, 
  {-25,-10}, {-10,-28}, {11,-8}, 
  {-6,-20}, {-18,-11}, {-18,2}, 
  {-18,14}, {-14,26}, {0,26} };

  float f1,f2;
  int i;
  
  for (i=0;i<25;i++) {
    p[i][0]=p[i][0]+4;
    p[i][1]=p[i][1]+43;
  }
  f1 = f2 = 0.25;
  fprintf (fp, "\n/csig { %% x csig - C timesig \n"
           "  0 moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,8);
  fprintf (fp, "   fill\n} bind def\n");

  fprintf (fp, "\n/ctsig { %% x ctsig - C| timesig \n"
           "  dup csig 4 moveto 0 16 rlineto stroke\n"
           "} bind def\n");
}


/* ----- def_gclef ------- */
void def_gclef (FILE *fp)
{
  float p[71][2]={
    {-6, 16},  {-8, 13},  {-14, 19},  {-10, 35},  {2, 35},  {8, 37},  
    {21, 30},  {21, 17},  {21, 5},  {10, -1},  {0, -1},  {-12, -1},  
    {-23, 5},  {-23, 22},  {-23, 29},  {-22, 37},  {-7, 49},  {10, 61},  
    {10, 68},  {10, 73},  {10, 78},  {9, 82},  {7, 82},  {2, 78},  
    {-2, 68},  {-2, 62},  {-2, 25},  {10, 18},  {11, -8},  {11, -18}, 
    {5, -23},  {-4, -23},  {-10, -23},  {-15, -18},  {-15, -13},  
    {-15, -8},  {-12, -4},  {-7, -4},  {3, -4},  {3, -20},  {-6, -17},
    {-5, -23},  {9, -20},  {9, -9},  {7, 24},  {-5, 30},  {-5, 67}, 
    {-5, 78},  {-2, 87},  {7, 91},  {13, 87},  {18, 80},  {17, 73},  
    {17, 62},  {10, 54},  {1, 45},  {-5, 38},  {-15, 33},  {-15, 19}, 
    {-15, 7},  {-8, 1},  {0, 1},  {8, 1},  {15, 6},  {15, 14},  {15, 23},
    {7, 26},  {2, 26},  {-5, 26},  {-9, 21},  {-6, 16} };

  float f1,f2;
  
  f1 = f2 = 24.0/65.0;
  fprintf (fp, "\n/gclef { %% x n gclef - G clef on line n\n"
           "  2 sub 6 mul moveto\n"); /*shift to line n*/
  add_mv (fp,f1,f2,p,0);
  add_sg (fp,f1,f2,p,1,1);
  add_curve (fp,f1,f2,p,2,23);
  fprintf (fp, "   fill\n} bind def\n");
  fprintf (fp, "\n/sgclef {\n"
           "  0.85 div exch 0.85 div exch" /*compensate scale*/
           " gsave 0.85 0.85 scale gclef grestore\n"
           "} bind def\n");
}

/* ----- def_t8clef ------- */
void def_t8clef (FILE *fp)
{
  fprintf (fp, "\n/t8clef { %% x t8clef - treble 8va clef\n"
           "  dup 2 gclef -20 moveto\n"
           "  gsave /Times-Roman 12 selectfont\n"
           "  /bx false def (8) cshow grestore\n"
           "} bind def\n"
           "\n/st8clef {\n"
           "  0.85 div gsave 0.85 0.85 scale t8clef grestore\n"
           "} bind def\n");
  fprintf (fp, "\n/t8upclef { %% x t8upclef - treble 8va clef\n"
           "  dup 2 gclef 33 moveto\n"
           "  gsave /Times-Italic 12 selectfont\n"
           "  /bx false def (8) cshow grestore\n"
           "} bind def\n"
           "\n/st8upclef {\n"
           "  0.85 div gsave 0.85 0.85 scale t8upclef grestore\n"
           "} bind def\n");
}

/* ----- def_fclef ------- */
void def_fclef (FILE *fp)
{
  float p[42][2]={
    {-2.3,3}, {6,7}, {10.5,12}, {10.5,16},
    {10.5,20.5}, {8.5,23.5}, {6.2,23.3},
    {5.2,23.5}, {2,23.5}, {0.5,19.5},
    {2,20}, {4,19.5}, {4,18},
    {4,17}, {3.5,16}, {2,16},
    {1,16}, {0,16.9}, {0,18.5},
    {0,21}, {2.1,24}, {6,24}, 
    {10,24}, {13.5,21.5}, {13.5,16.5},
    {13.5,11}, {7,5.5}, {-2.0,2.8},
    {14.9,21}, 
    {14.9,22.5}, {16.9,22.5}, {16.9,21},
    {16.9,19.5}, {14.9,19.5}, {14.9,21},
    {14.9,15},
    {14.9,16.5}, {16.9,16.5}, {16.9,15},
    {16.9,13.5}, {14.9,13.5}, {14.9,15} };
  
  int i;
  float f1,f2;

  for (i=0;i<42;i++) {p[i][0]-=7.5; p[i][1]-=0.5; }
  f1 = f2 = 1.0;
  fprintf (fp, "\n/fclef { %% x n fclef - F clef on line n\n"
           "  4 sub 6 mul moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,9);
  add_curve (fp,f1,f2,p,1,9);

  add_mv (fp,f1,f2,p,28);
  add_curve (fp,f1,f2,p,29,2);

  add_mv (fp,f1,f2,p,25);
  add_curve (fp,f1,f2,p,36,2);

  fprintf (fp, "fill\n} bind def\n");

  fprintf (fp, "\n/sfclef {\n"
           "  0.85 div exch 0.85 div exch" /*compensate scale*/
           " gsave 0.85 0.85 scale fclef grestore\n"
           "} bind def\n");
}

/* ----- def_cclef ------- */
void def_cclef (FILE *fp)
{
  float p[30][2]={
    {0,0}, {2,5.5}, 
    {9,4.5}, {12,10}, {12,15.5},
    {12,19.5}, {11,23.3}, {6.5,23.5},
    {5.2,23.5}, {2,23.5}, {0.5,19.5},
    {2,20}, {4,19.5}, {4,18},
    {4,17}, {3.5,16}, {2,16},
    {1,16}, {0,16.9}, {0,18.5},
    {0,21}, {2.1,24}, {6,24},
    {12,24}, {15,21.5}, {15,16.5},
    {15,10}, {10,4.5}, {4,5},
    {3,0} };
  int i;
  float f1,f2;

  for (i=0;i<30;i++) p[i][1]+=24;

  f1 = 0.6;
  f2 = 0.5;
  fprintf (fp, "\n/cchalf {\n"
           "  0 moveto\n"); 
  add_mv (fp,f1,f2,p,0);
  add_sg (fp,f1,f2,p,1,1);
  add_curve (fp,f1,f2,p,2,9);
  add_sg (fp,f1,f2,p,29,1);
  fprintf (fp, "fill\n} bind def\n");

  fprintf (fp,
           "\n/cclef {   %% x n cclef - C clef on line n\n"
           "   gsave 0 exch 3 sub 6 mul translate\n"
           "   dup dup dup\n"
           "   cchalf 0 24 translate 1 -1 scale cchalf\n"
           "   6.5 sub 0 moveto 0 24 rlineto 3 0 rlineto 0 -24 rlineto fill\n"
           "   1.8 sub 0 moveto 0 24 rlineto 0.8 setlinewidth stroke grestore \n"
           "} bind def\n");
           
  fprintf (fp, "\n/scclef { cclef } bind def\n");
}

/* ----- def_brace ------- */
void def_brace (FILE *fp)
{
  float p[8][2]={
    {7.2,60}, {-7,39}, {17,17}, {-1,0}, 
    {-1.4,0}, {13,13}, {-11,39}, {7,60} };

  float q[8][2]={
    {-3,0}, {2,0}, {4,1}, {5.5,5}, 
    {5.9,4.7}, {4.7,1.2}, {3.2,-.4}, {-1,-1.2} };

  float f1,f2;

  f1 = 0.9;
  f2 = 1.0;
  fprintf (fp, "\n/bracehalf {\n");
  add_mv (fp,f1,f2,p,0);
  add_curve (fp,f1,f2,p,1,1);
  add_sg (fp,f1,f2,p,4,1);
  add_curve (fp,f1,f2,p,5,1);
  fprintf (fp, "  fill\n} bind def\n");

  fprintf (fp,
           "\n/brace {   %% scale x0 y0 brace\n"
           "   3 copy moveto gsave 1 exch scale bracehalf grestore\n"
           "   moveto gsave neg 1 exch scale bracehalf grestore\n"
           "} bind def\n");

  f1 = 1.0;
  f2 = 1.0;
  fprintf (fp, "\n/brackhead {\n");
  add_mv (fp,f1,f2,q,0);
  add_curve (fp,f1,f2,q,1,1);
  add_sg (fp,f1,f2,q,4,1);
  add_curve (fp,f1,f2,q,5,1);
  fprintf (fp, "  fill\n} bind def\n");

  fprintf (fp,
           "\n/bracket {   %% h x0 y0 bracket\n"
           "   3 copy moveto 0 exch rmoveto brackhead\n"
           "   3 copy moveto pop gsave 1 -1 scale brackhead grestore \n"
           "   moveto -3 0 rlineto 0 exch rlineto 3 0 rlineto fill\n"
           "} bind def \n");

}


/* ----- def_staff ------- */
void def_staff (FILE *fp)
{
  fprintf (fp,
           "\n/staff { %% l staff - draw staff\n"
           "  gsave %3.2f setlinewidth 0 0 moveto\n"
           "  dup 0 rlineto dup neg 6 rmoveto\n"
           "  dup 0 rlineto dup neg 6 rmoveto\n"
           "  dup 0 rlineto dup neg 6 rmoveto\n"
           "  dup 0 rlineto dup neg 6 rmoveto\n"
           "  dup 0 rlineto dup neg 6 rmoveto\n"
           "  pop stroke grestore\n"
           "} bind def\n",
           cfmt.stafflinethickness);
}

/* ----- def_sep ------- */
void def_sep (FILE *fp)
{
  fprintf (fp,
           "\n/sep0 { %% x1 x2 sep0 - hline separator \n"
           "   0 moveto 0 lineto stroke\n"
           "} bind def\n");
}

/* ----- define_symbols: write postscript macros to file ------ */
void define_symbols (FILE *fp)
{
  /* load tablature fonts */
  def_tabfonts (fp);

  /* general music stuff */
  def_misc (fp);
  def_gclef (fp);
  def_t8clef (fp);
  def_fclef (fp);
  def_cclef (fp);
  def_hd1 (fp);
  def_hd2 (fp); 
  def_hd3 (fp); 
  def_historic (fp); 
  def_stems (fp);
  def_beam (fp);
  def_sl (fp);
  def_dot (fp);
  def_deco (fp);
  def_deco1 (fp);
  def_hl (fp);
  def_flags1 (fp);
  def_flags2 (fp);
  def_xflags (fp);
  def_acc (fp);
  def_gchord (fp);
  def_rests (fp);
  def_bars (fp);
  def_ends (fp);
  def_gnote (fp);
  def_csg (fp);
  def_sep (fp);
  def_tsig (fp);
  def_staff (fp);
  def_brace (fp);
  def_typeset(fp);

  /* tablature stuff */
  def_tabsyms (fp);
}
