#ifndef _parseH
#define _parseH

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.c for details.
 */

#include "abctab2ps.h"


/*  subroutines connected with parsing the input file  */

/* ----- syntax: print message for syntax errror ----- */
void syntax (const char* msg, char *q);

/* ----- isnote: checks char for valid note symbol ----- */
int isnote (char c);

/* ----- zero_sym: init global zero SYMBOL struct ----- */
void zero_sym (void);

/* ----- add_sym: returns index for new symbol at end of list ----- */
int add_sym (int type);

/* ----- insert_sym: returns index for new symbol inserted at k ----- */
int insert_sym (int type, int k);

/* ----- get_xref: get xref from string ----- */
int get_xref (char str[]);

/* ----- parse_meter_token: interpret pure meter string ---- */
int parse_meter_token(char* str, int* meter1, int* meter2, int* mflag, char* meter_top, int* dlen);

/* ----- set_meter: interpret meter string, store in struct ----- */
void set_meter (char str[], struct METERSTR *meter);

/* ----- set_dlen: set default length for parsed notes ----- */
void set_dlen (char str[], struct METERSTR *meter);

/* ----- set_keysig/set_clef: interpret keysig string, store in struct ----- */
int set_keysig(char s[], struct KEYSTR *ks, int init);
int set_clef(char* s, struct KEYSTR *ks);

/* ----- get_halftones: figure out how by many halftones to transpose ----- */
int get_halftones (struct KEYSTR key, char transpose[]);

/* ----- shift_key: make new key by shifting nht halftones ----- */
void shift_key (int sf_old, int nht, int *sfnew, int *addt);

/* ----- set_transtab: setup for transposition by nht halftones ----- */
void set_transtab (int nht, struct KEYSTR *key);

/* ----- do_transpose: transpose numeric pitch and accidental ----- */
void do_transpose (struct KEYSTR key, int *pitch, int *acc);

/* ----- gch_transpose: transpose guitar chord string in gch ----- */
void gch_transpose (string* gch, struct KEYSTR key);

/* ----- init_parse_params: initialize variables for parsing ----- */
void init_parse_params (void);

/* ----- add_text ----- */
void add_text (char str[], int type);

/* ----- reset_info ----- */
void reset_info (struct ISTRUCT *inf);

/* ----- get_default_info: set info to default, except xref field ----- */
void get_default_info (void);

/* ----- is_info_field: identify any type of info field ---- */
int is_info_field (char str[]);

/* ----- is_end_line: identify eof ----- */
int is_end_line (const char str[]);

/* ----- is_pseudocomment ----- */
int is_pseudocomment (const char str[]);

/* ----- is_comment ----- */
int is_comment (const char str[]);

/* ----- is_cmdline ----- */
int is_cmdline (const char str[]);

/* ----- find_voice ----- */
int find_voice (char vid[], int *newv);

/* ----- switch_voice: read spec for a voice, return voice number ----- */
int switch_voice (const char *str);

/* ----- info_field: identify info line, store in proper place  ---- */
int info_field (char str[]);

/* ----- append_meter: add meter to list of symbols -------- */
void append_meter (const struct METERSTR* meter);

/* ----- append_key_change: append change of key to sym list ------ */
void append_key_change(struct KEYSTR oldkey, struct KEYSTR newkey);

/* ----- numeric_pitch ------ */
int numeric_pitch(char note);

/* ----- symbolic_pitch: translate numeric pitch back to symbol ------ */
int symbolic_pitch(int pit, char str[]);

/* ----- handle_inside_field: act on info field inside body of tune --- */
void handle_inside_field(int type);

/* ----- parse_uint: parse for unsigned integer ----- */
int parse_uint (void);
  
/* ----- parse_bar: parse for some kind of bar ---- */
int parse_bar (void);
  
/* ----- parse_space: parse for whitespace ---- */
int parse_space (void);

/* ----- parse_esc: parse for escape sequence ----- */
int parse_esc (void);

/* ----- parse_nl: parse for newline ----- */
int parse_nl (void);

/* ----- parse_gchord: parse guitar chord, add to global gchlst ----- */
int parse_gchord ();

/* ----- parse_deco: parse for decoration on note ----- */
int parse_deco ();

/* ----- parse_length: parse length specifer for note or rest --- */
int parse_length (void);

/* ----- parse_brestnum: parse number of bars for multimeasure rest --- */
int parse_brestnum (void);

/* ----- parse_grace_sequence --------- */
int parse_grace_sequence (int pgr[], int agr[], int* len);

/* ----- identify_note: set head type, dots, flags for note --- */
void identify_note (struct SYMBOL *s, char *q);

/* ----- double_note: change note length for > or < char --- */
void double_note (int i, int num, int sign, char *q);

/* ----- parse_basic_note: parse note or rest with pitch and length --*/
int parse_basic_note (int *pitch, int *length, int *accidental);

/* ----- parse_note: parse for one note or rest with all trimmings --- */
int parse_note (void);

/* ----- parse_sym: parse a symbol and return its type -------- */
int parse_sym (void);

/* ----- add_wd ----- */
char *add_wd(char str[]);

/* ----- parse_vocals: parse words below a line of music ----- */
int parse_vocals (char line[]);

/* ----- parse_music_line: parse a music line into symbols ----- */
int parse_music_line (char line[]);

/* ----- is_selected: check selection for current info fields ---- */
int is_selected (char xref_str[], int npat, char pat[][STRLFILE], int select_all, int search_field);

/* ----- rehash_selectors: split selectors into patterns and xrefs -- */
int rehash_selectors (char sel_str[], char xref_str[], char pat[][STRLFILE]);

/* ----- decomment_line: cut off after % ----- */
void decomment_line (char ln[]);

/* ----- get_line: read line, do first operations on it ----- */
int get_line (FILE *fp, string *ln);

/* ----- read_line: returns type of line scanned --- */
int read_line (FILE *fp, int do_music, string* line);

/* ----- do_index: print index of abc file ------ */
void do_index(FILE *fp, char xref_str[], int npat, char pat[][STRLFILE], int select_all, int search_field);

#endif // _parseH
