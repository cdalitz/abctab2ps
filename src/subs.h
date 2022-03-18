#ifndef _subsH
#define _subsH

/*  
 *  This file is part of abctab2ps, 
 *  see the file abctab2ps.cpp for details
 */

/*  miscellaneous subroutines  */

/* ----- write_help ----- */
void write_help (void);

/* ----- write_version ----- */
void write_version (void);

/* ----- is_xrefstr: check if string ok for xref selection ---- */
int is_xrefstr (char str[]);

/* ----- make_arglist: splits one string into list or arguments ---- */
int make_arglist (char str[], char *av[]);

/* ----- init_ops ----- */
void init_ops (int job);

/* ----- ops_into_fmt ----- */
void ops_into_fmt (struct FORMAT *fmt);

/* ----- parse_args: parse list of arguments, interpret flags ----- */
int parse_args (int ac, char *av[]);

/* ----- process_cmdline: parse '%!'-line from input file ----- */
void process_cmdline (char *line);

/* ----- memory allocation for structures ----- */
void *zrealloc(void* addr, size_t nold, size_t nnew, size_t size);
void alloc_structs (void);
void realloc_structs (int newmaxSyms, int newmaxVc);

/* ----- set_page_format ----- */
int set_page_format (void);

/* ----- tex_str: change string to take care of some tex-style codes --- */
int tex_str (const char *str, string *s, float *wid);

/* ----- put_str: output a string in postscript ----- */
void put_str (char *str);

/* ----- set_font ----- */
void set_font (FILE *fp, struct FONTSPEC font, int add_bracket);

/* ----- set_font_str ----- */
void set_font_str (char str[], struct FONTSPEC font);

/* ----- check_margin: do horizontal shift if needed ---- */
void check_margin (float new_posx);

/* ----- epsf_title ------ */
void epsf_title (char title[], char fnm[]);

/* ----- close_output_file ------ */
int close_output_file (void);

/* ----- open_output_file ------ */
void open_output_file (char fnam[], char tstr[]);

/* ----- open_index_file ------- */
void open_index_file (char fnam[]);

/* ----- close_index_file ------- */
void close_index_file (void);

/* ----- add_to_text_block ----- */
void add_to_text_block (char ln[], int add_final_nl);

/* ----- write_text_block ----- */
void write_text_block (FILE *fp, int job);

/* ----- put_words ------- */
void put_words (FILE *fp);

/* ----- put_text ------- */
void put_text (FILE *fp, int type, char str[]);

/* ----- put_history ------- */
void put_history (FILE *fp);

/* ----- write_inside_title  ----- */
void write_inside_title (FILE *fp);

/* ----- write_tunetop ----- */
void write_tunetop(FILE *fp);

/* ----- tempo_is_metronomemark ----- */
int tempo_is_metronomemark(char* tempostr);

/* ----- write_tempo ----- */
void write_tempo(FILE *fp, char tempo[], struct METERSTR meter);

/* ----- write_inside_tempo  ----- */
void write_inside_tempo (FILE *fp);

/* ----- write_heading  ----- */
void write_heading (FILE *fp);

/* ----- write_parts  ----- */
void write_parts (FILE *fp);

#endif // _subsH
