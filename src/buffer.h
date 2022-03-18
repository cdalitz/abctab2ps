#ifndef _bufferH
#define _bufferH

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

/*  subroutines to handle output buffer  */

/* PUTn: add to buffer with n arguments */

#define PUT0(f) do {sprintf(mbf,f); a2b(mbf); } while (0)
#define PUT1(f,a) do {sprintf(mbf,f,a); a2b(mbf); } while (0)
#define PUT2(f,a,b) do {sprintf(mbf,f,a,b); a2b(mbf); } while (0)
#define PUT3(f,a,b,c) do {sprintf(mbf,f,a,b,c); a2b(mbf); } while (0)
#define PUT4(f,a,b,c,d) do {sprintf(mbf,f,a,b,c,d); a2b(mbf); } while (0)
#define PUT5(f,a,b,c,d,e) do {sprintf(mbf,f,a,b,c,d,e); a2b(mbf); } while (0)


/* ----- a2b: appends string to output buffer ----- */
void a2b (char *t);

/* ----- bskip(h): translate down by h points in output buffer ---- */
void bskip(float h);

/* ----- init_pdims: initialize page dimensions ----- */
void init_pdims (void);

/* ----- clear_buffer ------- */
void clear_buffer (void);

/* ----- write_index_entry ------- */
void write_index_entry (void);

/* ----- write_buffer: write buffer contents, break at full pages ---- */
void write_buffer (FILE *fp);

/* ----- buffer_eob: handle completed block in buffer ------- */
void buffer_eob (FILE *fp);

/* ----- check_buffer: dump buffer if less than nb bytes avilable --- */
void check_buffer (FILE *fp, int nb);

#endif // _bufferH

