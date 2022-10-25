#ifndef _utilH
#define _utilH

#include <cstdarg>

/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

/*  low-level utilities  */


/* ----- error warning ----- */
void wng (const char* msg, const char* str);

/* ----- error exit ----- */
void rx (const char* msg, const char* str);

void rx1 (const char* msg, char c);

void rxi (const char* msg, int i);

/* ----- bug: print message for internal error and maybe stop -----  */
void bug (const char* msg, int fatal);

/* ----- ranf(x1,x2): return random float between x1 and x2 --- */
float ranf(float x1, float x2);

/* ----- chartoi: convert a single character to an integer ----- */
int chartoi(char c);

/* ----- getline ----- */
int getline(string *s, FILE *fp);

/* ----- strip: remove leading and trailing whitespace ----- */
void strip (char str1[], char str[]);
void strip (string* str);

/* ----- nwords: count words in string ----- */
int nwords (char *str);

/* ----- getword: return n-th word from string ---- */
int getword (int iw, char *str, char *str1);

/* ----- abbrev: check for valid abbreviation ----- */
int abbrev (char str[], const char ab[], int nchar);

/* ----- strext: set extension on a file identifier ----- */
void strext (char fid1[], char fid[], const char ext[], int force);

/* ----- cutext: cut off extension on a file identifier ----- */
void cutext (char fid[]);

/* ----- getext: get extension on a file identifier ----- */
void getext (char fid[], char ext[]);

/* ----- sscanu ----- */
float scan_u(char str[]);

/* ----- match ------- */
int match (char str[], char pat[]);

/* check for empty string */
int isblankstr(const char str[]);

/* ----- cap_str: capitalize a string ----- */
void cap_str(char str[]);

/* ----- cwid ----- */
/*  These are char widths for Times-Roman */
float cwid(char c);

/* ----- get_file_size ------- */
int get_file_size (char fname[]);
/* version which counts bytes by hand */
int get_file_size1 (char fname[]);

/* case insensitive string comparison 
 *
 * most systems have strcasecmp(), but some have stricmp() or strcmpi()
 * implementing an own version of this trivial function avoids messing
 * with #ifdefs for various environments
 */
int strcmpnocase (const char* s1, const char* s2);
int strncmpnocase (const char* s1, const char* s2, int n);

/* save version of strncpy which adds trailing zero */
int strnzcpy (char* dst, const char* src, int len);

/*
 * fgets() variant that works with \r and \n and \r\n and \n\r
 */
int fgets_rn (string* s, FILE* fp);


/*
 * key-value list ("kl") implementation in plain C
 */
struct kl_entry {
  char* key;
  char* value;
  struct kl_entry* next;
};
/* manipulation functions assume NULL entry as end indicator */
void kl_setkey (struct kl_entry** list, char* key, char* value);
struct kl_entry* kl_getkey (struct kl_entry* list, char* key);
void kl_clear (struct kl_entry** list);

/* maximum and minimum of two arguments */
template<class T> const T& maximum(const T& a, const T& b) {
  return (a<b) ? b : a;
}
template<class T> const T& minimum(const T& a, const T& b) {
  return (a<b) ? a : b;
}

#endif // _utilH
