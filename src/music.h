#ifndef _musicH
#define _musicH

/*  
 *  This file is part of abctab2ps, 
 *  See file abc2ps.cpp for details.
 */

#include <stdio.h>
 
#include "abctab2ps.h"

/*  subroutines connected with output of music  */

#define XP_START   0
#define XP_END     (maxSyms-1)

#define ABS(a)  ((a)>0) ? (a) : (-a)
#define AT_LEAST(a,b)  if((a)<(b)) a=b;

/* parameter values for get_staffheight() */
enum nettobrutto {
  NETTO,
  BRUTTO,
  ALMOSTBRUTTO
};

/* ----- nwid ----- */
float nwid (float dur);

/* ----- xwid -- --- */
float xwid (float dur);

/* ----- next_note, prec_note ------ */
int next_note (int k, int n, struct SYMBOL symb[]);
int prec_note (int k, int n, struct SYMBOL symb[]);

/* ----- followed_by_note ------ */
int followed_by_note (int k, int n, struct SYMBOL symb[]);

/* ----- preceded_by_note ------ */
int preceded_by_note (int k, int n, struct SYMBOL symb[]);

/* ----- sub to exchange two integers ----- */
void xch (int *i, int *j);

/* ----- print_linetype ----------- */
void print_linetype (int t);

/* ----- print_syms: show sym properties set by parser ------ */
void print_syms(int num1, int num2, struct SYMBOL symb[]);

/* ----- print_vsyms: print symbols for all voices ----- */
void print_vsyms (void);

/* ----- set_head_directions ----------- */
void set_head_directions (struct SYMBOL *s);

/* ----- set_minsyms: want at least one symbol in each voice --- */
void set_minsyms(int ivc);

/* ----- set_sym_chars: set symbol characteristics --- */
void set_sym_chars (int n1, int n2, struct SYMBOL symb[]);

/* ----- set_beams: break beams on flagless notes ---- */
void set_beams (int n1, int n2, struct SYMBOL symb[], int istab);

/* ----- set_stems: decide on stem directions and lengths ---- */
void set_stems (int n1, int n2, struct SYMBOL symb[]);

/* ----- set_sym_times: set time axis; also count through bars ----- */
int set_sym_times (int n1, int n2, struct SYMBOL symb[], struct METERSTR meter0, float* timeinit);

/* ----- get_staffheight: returns staffheight of given voice index --- */
float get_staffheight (int iv, enum nettobrutto nb);

/* ----- set_sym_widths: set widths and prefered space --- */
void set_sym_widths (int ns1, int ns2, struct SYMBOL symb[], int ivc);

/* ----- contract_keysigs: delete duplicate keysigs at staff start ---- */
int contract_keysigs (int ip1);

/* ----- set_initsyms: set symbols at start of staff ----- */
int set_initsyms (int v, float *wid0);

/* ----- print_poslist ----- */
void print_poslist (void);

/* ----- insert_poslist: insert new element after element nins ----- */
int insert_poslist (int nins);

/* ----- set_poslist: make list of horizontal posits to align voices --- */
void set_poslist (void);

/* ----- set_xpwid: set symbol widths and tfac in xp list ----- */
void set_xpwid(void);

/* ----- set_spaces: set the shrink,space,stretch distances ----- */
void set_spaces (void);

/* ----- check_overflow: returns upper limit which fits on staff ------ */
int check_overflow (int ip1, int ip2, float width);

/* ----- set_glue --------- */
float set_glue (int ip1, int ip2, float width);

/* ----- adjust_group: even out spacings for one group of notes --- */
void adjust_group(int i1, int i2);

/* ----- adjust_spacings: even out triplet spacings etc --- */
void adjust_spacings (int n);

/* ----- adjust_rests: position single rests in bar center */
void adjust_rests (int n, int v);

/* ----- copy_vsyms: copy selected syms for voice to v sym --- */
int copy_vsyms (int v, int ip1, int ip2, float wid0);

/* ----- draw_timesig ------- */
void draw_timesig (float x, struct SYMBOL s);

/* ----- draw_keysig: return sf for this key ----- */
int draw_keysig (float x, struct SYMBOL s);

/* ----- draw_bar ------- */
void draw_bar (float x, struct SYMBOL *s);

/* ----- draw_barnums ------- */
void draw_barnums (FILE *fp);

/* ----- update_endings: remember where to draw endings ------- */
void update_endings (float x, struct SYMBOL s);

/* ----- set_ending: determine limits of ending box ------- */
void set_ending (int i);

/* ----- draw_endings ------- */
void draw_endings (void);

/* ----- draw_rest ----- */
void draw_rest (float x, float yy, struct SYMBOL *s, float *gchy);

/* ----- draw_brest ----- */
void draw_brest (float x, float yy, struct SYMBOL *s, float *gchy);

/* ----- draw_gracenotes ----- */
void draw_gracenotes (float x, float w, float d, struct SYMBOL *s);

/* ----- draw_basic_note: draw m-th head with accidentals and dots -- */
void draw_basic_note (float x, float w, float d, struct SYMBOL *s, int m);

/* ----- draw_decorations ----- */
float draw_decorations (float x, struct SYMBOL *s, float *tp);

/* ----- draw_note ----- */
float draw_note (float x, float w, float d, struct SYMBOL *s, int fl, float *gchy);

/* ----- vsh: up/down shift needed to get k*6  ----- */
float vsh (float x, int dir);

/* ----- rnd3: up/down shift needed to get k*3  ----- */
float rnd3(float x);

/* ----- rnd6: up/down shift needed to get k*6  ----- */
float rnd6(float x);

/* ----- b_pos ----- */
float b_pos (int stem, int flags, float b);

/* ----- calculate_beam ----- */
int calculate_beam (int i0, struct BEAM *bm);
  
/* ----- rest_under_beam ----- */
float rest_under_beam (float x, int head, struct BEAM *bm);

/* ----- draw_beam_num: draw number on a beam ----- */
void draw_beam_num (struct BEAM *bm, int num, float xn);
  
/* ----- draw_beam: draw a single beam ----- */
void draw_beam (float x1, float x2, float dy, struct BEAM *bm);

/* ----- draw_beams: draw the beams for one word ----- */
void draw_beams (struct BEAM *bm);

/* ----- extreme: return min or max, depending on s ----- */
float extreme (float s, float a, float b);

/* ----- draw_bracket  ----- */
void draw_bracket (int p, int j1, int j2);

/* ----- draw_nplet_brackets  ----- */
void draw_nplet_brackets (void);

/* ----- slur_direction: decide whether slur goes up or down --- */
float slur_direction (int k1, int k2);

/* ----- output_slur: output slur -- --- */
void output_slur (float x1, float y1, float x2, float y2, float s, float height, float shift);

/* ----- draw_slur (not a pretty routine, this) ----- */
void draw_slur (int k1, int k2, int nn, int level);

/* ----- prev_scut, next_scut: find place to terminate/start slur --- */
int next_scut (int i);
int prev_scut(int i);

/* ----- draw_chord_slurs ----- */
void draw_chord_slurs(int k1, int k2, int nh1, int nh2, int nslur, int mhead1[MAXHD], int mhead2[MAXHD], int job);

/* ----- draw_slurs: draw slurs/ties between neighboring notes/chords */
void draw_slurs (int k1, int k2, int job);

/* ----- draw_phrasing: draw phrasing slur between two symbols --- */
void draw_phrasing (int k1, int k2, int level);

/* ----- draw_all_slurs: draw all slurs/ties between neighboring notes  */
void draw_all_slurs (void);

/* ----- draw_all_phrasings: draw all phrasing slurs for one staff ----- */
void draw_all_phrasings (void);

/* ----- draw_all_ligaturae: draw all ligatura brackets for one staff ----- */
void draw_all_ligaturae (void);

/* ----- check_bars1 ---------- */
void check_bars1 (int ip1, int ip2);

/* ----- check_bars2 ---------- */
void check_bars2 (int ip1, int ip2);

/* ----- draw_vocals ----- */
void draw_vocals (FILE *fp, int nwl, float botnote, float bspace, float *botpos);

/* ----- draw_gchords: draws all gchords in current line ----- */
void draw_gchords (int istablature);

/* ----- draw_gchordline: draw line of gchord at x,y ----- */
void draw_gchordline (char* text, float x, float y);

/* ----- draw_symbols: draw symbols at proper positions on staff ----- */
void draw_symbols (FILE *fp, float bspace, float *bpos, int is_top);

/* ----- draw_sysbars: draw bars extending over staves----- */
void draw_sysbars (FILE *fp, int ip1, int ip2, float wid0, float h1, float dh);

/* ----- count_symbols: count number of "real" symbols ---- */
int count_symbols (void);

/* ----- select_piece: choose limits for piece to put on one staff ---- */
int select_piece (int ip1);

/* ----- is_topvc: check for top voice of set staved together --- */
int is_topvc (int jv);

/* ----- is_highestvc: check for highest voice in output --- */
int is_highestvc (int jv);

/* ----- vc_select: set flags for voice selection from -V option --- */
int vc_select (void);

/* ----- voice_label: label voice, or just return length if job==0 -- */
float voice_label (FILE *fp, char *label, float h, float xc, float dx0, int job);

/* ----- mstave_deco: draw decorations over multiple staves ----- */
void mstave_deco (FILE *fp, int ip1, int ip2, float wid0, float hsys, float htab[], float botheight);

/* ----- output_music: output for parsed symbol list ----- */
void output_music (FILE *fp);

/* ----- process_textblock ----- */
void process_textblock(FILE *fpin, FILE *fp, int job);

/* ----- process_pscomment  ----- */
void process_pscomment (FILE *fpin, FILE *fp, char line[]);

/* ----- check_selected ----- */
void check_selected(FILE *fp, char xref_str[], int npat, char pat[][STRLFILE], int sel_all,int search_field);

/* ----- process_line ----- */
void process_line (FILE *fp, int type, char xref_str[], int npat, char pat[][STRLFILE], int sel_all, int search_field);

/* ----- process_file ----- */
void process_file (FILE *fpin, FILE *fpout, char xref_str[], int npat, char pat[][STRLFILE], int sel_all, int search_field);

#endif // _musicH
