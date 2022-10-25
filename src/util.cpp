/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "abctab2ps.h"

#include "util.h"

/*  low-level utilities  */


/* ----- error warning ----- */
void wng (const char *msg, const char *str)
{
  printf ("+++ %s%s\n", msg, str);
}

/* ----- error exit ----- */
void rx (const char *msg, const char *str)
{
  printf ("\n+++ %s%s\n", msg, str);
  exit (1);
}

void rx1 (const char *msg, char c)
{
  printf ("\n+++ %s%c\n", msg, c);
  exit (1);
}

void rxi (const char *msg, int i)
{
  printf ("\n+++ %s%d\n", msg, i);
  exit (1);
}

/* ----- bug: print message for internal error and maybe stop -----  */
void bug(const char *msg, int fatal)
{
  printf ("\n\nThis cannot happen!");
  if (strlen(msg)>0) printf ("\nInternal error: %s.\n", msg);
  if (fatal) {
    printf ("Emergency stop.\n\n");
    exit (1);
  }
  else {
    printf ("Trying to continue...\n\n");
  }
}

/* ----- ranf(x1,x2): return random float between x1 and x2 --- */
float ranf(float x1, float x2)
{
  static int m=259200;   /* generator constants */
  static int a=421;
  static int c=54773;
  static int j=1;        /* seed */
  float r;
  
  j=(j*a+c)%m;
  r=x1+(x2-x1)*(double)j/(double)m;
  return r;
}

/* ----- chartoi: convert a single character to an integer ----- */
int chartoi(char c)
{
  switch (c) {
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  default:  return 0;
  }
}

/* ----- getline ----- */
/*
 * Reads a line from fp into string s, and trims away any trailing whitespace
 * This function works with \r and \n and \r\n and even \n\r as EOL
 * (the latter occurs on MacOs 8/9, because MPW swaps \r and \n)
 * RC: 1 = line successfully read; 0 = EOF or error
 */
int getline(string* s, FILE *fp)
{
  *s = "";
  char c1,c2;

  while ((c1 = getc(fp)) != EOF) {
    if (c1 == '\n') {
      *s += '\n';
      c2 = getc(fp);
      if (c2 != '\r')
        ungetc(c2, fp);
      break;
    }
    else if (c1 == '\r') {
      *s += '\n';
      c2 = getc(fp);
      if (c2 != '\n')
        ungetc(c2, fp);
      break;
    }
    else {
      *s += c1;
    }
  }

  if (ferror(fp) || ((0==s->length()) && feof(fp))) {
    return 0;
  }

  /* remove trailing whitespace */
  s->erase(s->find_last_not_of(" \t\n\r")+1);

  return 1;
}


/* ----- strip: remove leading and trailing whitespace ----- */
void strip (char *str1, char *str)
{
  int l,i,i1,i2;
  const char* blank = " \t\r\n";
  l=strlen(str);

  i1=0;
  for (i=0; i<l; i++) 
    //if ((str[i]!=' ') && (str[i]!='\n')) { i1=i; break; }
    if (!strchr(blank,str[i])) { i1=i; break; }
  i2=0;
  for (i=l-1; i>=0; i--) 
    //if ((str[i]!=' ') && (str[i]!='\n')) { i2=i+1; break; }
    if (!strchr(blank,str[i])) { i2=i+1; break; }
  for (i=i1;i<i2;i++) str1[i-i1]=str[i];
  str1[i2-i1]=0;
/*  printf (" l=%d i1=%d i2=%d <%s> <%s>\n", l, i1, i2, str, str1);*/
}
void strip (string* str)
{
  const char* blank = " \t\r\n";
  int b,e;

  if(str->length() == 0) return;
  b = str->find_first_not_of(blank);
  e = str->find_last_not_of(blank);
  if(b == -1) {
    *str="";
  } else {
  *str = str->substr(b, e - b + 1);
  }
}


/* ----- nwords: count words in string ----- */
int nwords (char *str)
{
  int w,k;
  char *c;
  c=str;
  w=0;
  for(k=0;k<=50;k++) {
    while (*c==' ') c++;
    if (*c=='\0') break;
    w++;
    while ((*c!=' ') && (*c!='\0')) c++;
    if (*c=='\0') break;
  }
  return w;
}    



/* ----- getword: return n-th word from string ---- */
int getword (int iw, char *str, char *str1)
{
  int w,k;
  char *c,*cc;
  if (iw<0) { *str1='\0'; return 0;}
  c=str;
  w=0;
  for(k=0;k<=50;k++) {
    while (*c==' ') c++;
    if (*c=='\0') break;
    if (w==iw) {
      cc=str1;
      while ((*c!=' ')&&(*c!='\0')) { *cc=*c; c++; cc++; }
      *cc='\0';
      return 1;
    }
    w++;
    while ((*c!=' ') && (*c!='\0')) c++;
    if (*c=='\0') break;
  }
  *str1='\0';
  return 0;
}    


/* ----- abbrev: check for valid abbreviation ----- */
int abbrev (char *str, const char *ab, int nchar)
{
  int i,nc;
  if (strlen(str) > strlen(ab)) return 0;
  nc=strlen(str);
  if (nc<nchar) nc=nchar;
  for (i=0;i<nc;i++) if (str[i] != ab[i]) return 0;
  return 1;
}

/* ----- strext: set extension on a file identifier ----- */
/*  force=1 forces change even if fid already has an extension */
/*  force=0 does not change the extension if there already is one */
void strext (char *fid1, char *fid, const char *ext, int force)
{
  int i,l;
  char *p,*q;

  if (fid1 != fid) {
    strcpy (fid1, fid);
  }
  l=strlen(fid1);
  p=fid1;
  for (i=0;i<l;i++) 
    if (fid1[i]=='/') p=fid1+i;

  if (!force) {
    q=strchr(p,'.');
    if (q && (q!=fid1+strlen(fid1)-1)) return;
  }
  if (!strchr(p,'.')) strcat (fid1,".");
  q=strchr(p,'.'); 
  if (strlen(ext)>0) q++;
  *q = 0;
  strcat(fid1,ext);

}

/* ----- cutext: cut off extension on a file identifier ----- */
void cutext (char *fid)
{
  int i,l;

  l=strlen(fid);
  for (i=0;i<l;i++) 
    if (fid[i]=='.') fid[i]='\0';
}

/* ----- getext: get extension on a file identifier ----- */
void getext (char *fid, char *ext)
{
  int i,l,k;
  
  l=strlen(fid);
  k=l-1;
  for (i=0;i<l;i++) 
    if (fid[i]=='.') k=i;
  
  for (i=k+1;i<l;i++)
    ext[i-k-1]=fid[i];
  ext[l-k-1]='\0';

}


/* ----- sscanu ----- */
float scan_u(char *str)
{
  char unit[81];
  float a,b;
  
  strcpy(unit,"pt");
  sscanf(str,"%f%s", &a, unit);
  
  if      (!strcmp(unit,"cm")) b=a*CM;
  else if (!strcmp(unit,"in")) b=a*IN;
  else if (!strcmp(unit,"pt")) b=a*PT;
  else {
    printf ("+++ Unknown unit \"%s\" in: %s\n",unit,str); 
    exit (3);
  }
  return b;
}


/* ----- match ------- */
int match (char *str, char *pat)
{
  char *p,*s;
  p=pat;
  s=str; 

  if (strlen(pat)==0) return 1;

  while (*p != 0) {

    if (*p == '*') {           /* found wildcard '*' in pattern */
      p++;
      while (*p == '*') p++;
      if (*p == 0) return 1;   /* trailing '*' matches all */
      for (;;) {               /* find match to char after '*' */
        if (*s == 0) return 0;
        if ((*s == *p) || (*p == '+')) 
          if (match(s+1,p+1)) return 1;   /* ok if rest matches */
        s++;
      }
    }
    
    else {                     /* no wildcard -- char must match */   
      if (*s == 0) return 0;
      if ((*p != *s) && (*p != '+')) return 0;
      s++;
    }
    p++;
  }

  if (*s != 0) return 0;       /* pattern but not string exhausted */
  return 1;
}

/* ----- isblankstr: check for blank string ---- */
int isblankstr (const char *str)
{
  int i;
  for (i=0;i<strlen(str);i++) if (str[i] != ' ') return 0;
  return 1;
}

/* ----- cap_str: capitalize a string ----- */
void cap_str(char *str)
{
  char *c;
  for (c=str; *c!='\0'; c++)
	*c = toupper(*c);
}
    

/* ----- cwid ----- */
/*  These are char widths for Times-Roman */
float cwid(char c)
{
  float w;
  if      (c=='a') w=44.4;
  else if (c=='b') w=50.0;
  else if (c=='c') w=44.4;
  else if (c=='d') w=50.0;
  else if (c=='e') w=44.4;
  else if (c=='f') w=33.3;
  else if (c=='g') w=50.0;
  else if (c=='h') w=50.0;
  else if (c=='i') w=27.8;
  else if (c=='j') w=27.8;
  else if (c=='k') w=50.0;
  else if (c=='l') w=27.8;
  else if (c=='m') w=77.8;
  else if (c=='n') w=50.0;
  else if (c=='o') w=50.0;
  else if (c=='p') w=50.0;
  else if (c=='q') w=50.0;
  else if (c=='r') w=33.3;
  else if (c=='s') w=38.9;
  else if (c=='t') w=27.8;
  else if (c=='u') w=50.0;
  else if (c=='v') w=50.0;
  else if (c=='w') w=72.2;
  else if (c=='x') w=50.0;
  else if (c=='y') w=50.0;
  else if (c=='z') w=44.4;

  else if (c=='A') w=72.2;
  else if (c=='B') w=66.7;
  else if (c=='C') w=66.7;
  else if (c=='D') w=72.2;
  else if (c=='E') w=61.1;
  else if (c=='F') w=55.6;
  else if (c=='G') w=72.2;
  else if (c=='H') w=72.2;
  else if (c=='I') w=33.3;
  else if (c=='J') w=38.9;
  else if (c=='K') w=72.2;
  else if (c=='L') w=61.1;
  else if (c=='M') w=88.9;
  else if (c=='N') w=72.2;
  else if (c=='O') w=72.2;
  else if (c=='P') w=55.6;
  else if (c=='Q') w=72.2;
  else if (c=='R') w=66.7;
  else if (c=='S') w=55.6;
  else if (c=='T') w=61.1;
  else if (c=='U') w=72.2;
  else if (c=='V') w=72.2;
  else if (c=='W') w=94.4;
  else if (c=='X') w=72.2;
  else if (c=='Y') w=72.2;
  else if (c=='Z') w=61.1;

  else if (c=='0') w=50.0;
  else if (c=='1') w=50.0;
  else if (c=='2') w=50.0;
  else if (c=='3') w=50.0;
  else if (c=='4') w=50.0;
  else if (c=='5') w=50.0;
  else if (c=='6') w=50.0;
  else if (c=='7') w=50.0;
  else if (c=='8') w=50.0;
  else if (c=='9') w=50.0;

  else if (c=='~') w=54.1;
  else if (c=='!') w=33.3;
  else if (c=='@') w=92.1;
  else if (c=='#') w=50.0;
  else if (c=='$') w=50.0;
  else if (c=='%') w=83.3;
  else if (c=='^') w=46.9;
  else if (c=='&') w=77.8;
  else if (c=='*') w=50.0;
  else if (c=='(') w=33.3;
  else if (c==')') w=33.3;
/*|   else if (c=='-') w=33.3; |*/
  else if (c=='-') w=40.0;
  else if (c=='_') w=50.0;
  else if (c=='+') w=56.4;
  else if (c=='=') w=55.0;
  else if (c=='[') w=33.3;
  else if (c==']') w=33.3;
  else if (c=='{') w=48.0;
  else if (c=='}') w=48.0;
  else if (c=='|') w=20.0;
  else if (c==':') w=27.8;
  else if (c==';') w=27.8;
  else if (c=='.') w=27.8;
  else if (c==',') w=27.8;
  else if (c=='\\') w=27.8;
  else if (c=='\'') w=33.3;
  else if (c=='\"') w=40.8;
  else if (c=='<') w=56.4;
  else if (c=='>') w=56.4;
  else if (c=='?') w=44.4;
  else if (c=='/') w=27.8;
  else if (c=='`') w=33.3;
  else if (c==' ') w=25.0;
  else if ((c=='\201')||(c=='\202')||(c=='\203')) w=50.0; /*sharp,flat,nat*/
  else             w=50.0;
  return w/100.0;
}


/* ----- get_file_size ------- */
/* version using standard function stat */
int get_file_size (char *fname)
{
  int m,rc;
  struct stat statbuf; 
  rc = stat(fname,&statbuf); 
  if (rc == -1) {
    printf ("Unsuccessful call to stat for file %s\n", fname);
    return -1;
  } 
  m=statbuf.st_size; 
  return m;
} 
	
/* version which counts bytes by hand */
int get_file_size1 (char *fname)
{
  int m,i;
  FILE *fp;
	  
  if ((fp = fopen (fname,"r")) == NULL) {
    printf ("Cannot open file to determine size: %s", fname);
    return -1;
  }
	
  m=0;
  i=getc(fp);
  while (i != EOF) {
    m++;
    i=getc(fp);
  } 
  fclose (fp);
  return m;
}	

/* cross platform implementation of strcasecmp */
int strcmpnocase (const char* s1, const char* s2)
{
  while (*s1 && *s2) {
    if (tolower(*s1) != tolower(*s2)) break;
    s1++; s2++;
  }
  if (!*s1 && !*s2)
    return 0;
  else if (tolower(*s1) > tolower(*s2))
    return 1;
  else
    return -1;
}

/* cross platform implementation of strncasecmp */
int strncmpnocase (const char* s1, const char* s2, int n)
{
  int i=0;
  while (*s1 && *s2 && (i<n)) {
    if (tolower(*s1) != tolower(*s2)) break;
    s1++; s2++; i++;
  }
  if ((!*s1 && !*s2) || (i==n))
    return 0;
  else if (tolower(*s1) > tolower(*s2))
    return 1;
  else
    return -1;
}

/*
 * save version of strncpy which adds trailing zero
 *  RC: number of copied bytes
 */
int strnzcpy (char* dst, const char* src, int len)
{
  int i = 0;
  char* s = (char*)src;
  char* d = dst;
  while (*s && (i<len-1)) {
    *d = *s;
    i++; s++; d++;
  }
  *d = 0;
  return i;
}


#if 0
/*
 * fgets() variant that works with \r and \n and \r\n and even \n\r
 * (the latter occurs on MacOs 8/9, because MPW swaps \r and \n)
 * returned string contains single \n as EOL marker
 * RC: 1 = line successfully read; 0 = EOF or error
 */
int fgets_rn (string* s, FILE* fp)
{
}
#endif

/* key-value list: 
 *   add or replace new entry
 *   memory for key and value is allocated with strdup()
 */
void kl_setkey (struct kl_entry** list, char* key, char* value)
{
  struct kl_entry* ent;

  if ((ent = kl_getkey(*list, key))) {
    /* known key: replace its value */
    free(ent->value);
    ent->value = strdup(value);
  } else {
    /* unknown key: add it */
    ent = (struct kl_entry*) malloc(sizeof(struct kl_entry));
    ent->key = strdup(key);
    ent->value = strdup(value);
    /* add new entry at head of list */
    ent->next = *list;
    *list = ent;
  }
}

/* key-value list: 
 *   get entry to given key
 *   returns NULL if not found
 */
struct kl_entry* kl_getkey (struct kl_entry* list, char* key)
{
  struct kl_entry* ent;

  for (ent=list; ent; ent=ent->next) {
    if (0 == strcmp(ent->key, key))
      return ent;
  }
  return NULL;
}

/* key-value list: 
 *   remove all entries and free memory 
 *   all key and value strings are freed too
 */
void kl_clear (struct kl_entry** list)
{
  struct kl_entry* ent = (*list);
  struct kl_entry* next;

  while (ent) {
    next = ent->next;
    free(ent->key); free(ent->value); free(ent);
    ent = next;
  }
  *list = NULL;
}
