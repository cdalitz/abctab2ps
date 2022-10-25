/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

#include <stdlib.h>
#include <string.h>

#include "abctab2ps.h"
#include "util.h"
#include "tab.h"

#include "format.h"

/*  subroutines connected with page layout  */

/* 
 * null terminated list of international paper size norms
 * list is searched from top to bottom => put most probable values on top
 * The first entry is the fallback papersize, if no system
 * setting can be found
 */
struct PAPERSIZE papersizes[] = {
  /* name,   pagewidth, pageheight, leftmargin, staffwidth */
  {"a4",       21.0*CM,    29.7*CM,     1.8*CM,   17.4*CM},
  {"letter",   21.6*CM,    27.9*CM,     1.8*CM,   18.0*CM},
  {"a5",       14.8*CM,    21.0*CM,     1.4*CM,   12.0*CM},
  {"folio",    21.6*CM,    33.0*CM,     1.8*CM,   18.0*CM},
  {"quarto",   21.5*CM,    27.5*CM,     1.8*CM,   17.9*CM},
  {"legal",    21.6*CM,    35.6*CM,     1.8*CM,   18.0*CM},
  {"executive",19.0*CM,    25.4*CM,     1.5*CM,   16.0*CM},
  {"tabloid",  27.9*CM,    43.2*CM,     2.0*CM,   23.9*CM},
  {0,                0,          0,          0,         0}
};

 
/* ----- fontspec ----- */
void fontspec (struct FONTSPEC *f, const char *name, float size, int box)
{
  strcpy (f->name, name);
  f->size = size;
  f->box  = box;
}
 
/* ----- add_font ----- */
/* checks font list, adds font if new */
int add_font (struct FONTSPEC *f)
{
  int i,i0,fnum;
  
  i0=-1;
  for (i=0;i<nfontnames;i++) {
    if (!strcmp(f->name,fontnames[i])) i0=i; 
  }

  if (i0>=0) {
    fnum=i0;
    if (vb>=10) printf("Already at %d: font %s\n",fnum,f->name);
  }
  else {
    fnum=nfontnames;
    strcpy(fontnames[fnum],f->name);
    if (vb>=10) printf("Add new at %d: font %s\n",fnum,f->name);
    nfontnames++;
  }
  return fnum;
}


/* ----- make_font_list ----- */
void make_font_list (struct FORMAT *f)
{
  if (vb>=10) printf ("Adding fonts from format..\n");
  add_font (&f->titlefont);
  add_font (&f->subtitlefont);
  add_font (&f->composerfont);
  add_font (&f->partsfont);
  add_font (&f->vocalfont);
  add_font (&f->textfont);
  add_font (&f->wordsfont);
  add_font (&f->gchordfont);
  add_font (&f->barnumfont);
  add_font (&f->barlabelfont);
  add_font (&f->voicefont);
}

/* ----- set_standard_format ----- */
void set_standard_format (struct FORMAT *f)
{
  struct PAPERSIZE* ppsz;
  ppsz = get_system_papersize();

  strcpy (f->name,           "standard");
  f->pageheight             =   ppsz->pageheight;
  f->staffwidth             =   ppsz->staffwidth;
  f->leftmargin             =   ppsz->leftmargin;
  f->topmargin              =   1.0  * CM;
  f->botmargin              =   1.0  * CM;
  f->topspace               =   0.8  * CM;
  f->titlespace             =   0.2  * CM;
  f->subtitlespace          =   0.1  * CM;
  f->composerspace          =   0.2  * CM;
  f->musicspace             =   0.2  * CM;
  f->partsspace             =   0.3  * CM;
  f->staffsep               =  46.0  * PT;
  f->sysstaffsep            =  40.0  * PT;
  f->systemsep              =  55.0  * PT;
  f->stafflinethickness     =  0.75;
  f->vocalspace             =  23.0  * PT;
  f->textspace              =   0.5  * CM;
  f->wordsspace             =   0.0  * CM;
  f->gchordspace            =  14.0  * PT;
  f->scale                  =   0.70;
  f->maxshrink              =   0.65;
  f->landscape              =   0;
  f->titleleft              =   0;
  f->stretchstaff           =   1;
  f->stretchlast            =   1;
  f->continueall            =   0;
  f->breakall               =   0;
  f->writehistory           =   0;
  f->withxrefs              =   0;
  f->oneperpage             =   0;
  f->titlecaps              =   0;
  f->barsperstaff           =   0;
  f->barnums                =   -1;
  f->barinit                =   1;
  f->squarebrevis           =   0;
  f->endingdots             =   0;
  f->slurisligatura         =   0;
  f->historicstyle          =   0;
  f->nobeams                =   0;
  f->nogracestroke          =   0;
  f->printmetronome         =   1;
  f->nostems                =   0;
  f->lineskipfac            =   1.1;
  f->parskipfac             =   0.4;
  f->strict1                =   0.5;
  f->strict2                =   0.8;
  f->indent                 =   0.0;
  fontspec (&f->titlefont,     "Times-Roman",    15.0, 0); 
  fontspec (&f->subtitlefont,  "Times-Roman",    12.0, 0); 
  fontspec (&f->composerfont,  "Times-Italic",   11.0, 0); 
  fontspec (&f->partsfont,     "Times-Roman",    11.0, 0); 
  fontspec (&f->tempofont,     "Times-Bold",     10.0, 0); 
  fontspec (&f->vocalfont,     "Times-Roman",    14.0, 0); 
  fontspec (&f->textfont,      "Times-Roman",    12.0, 0); 
  fontspec (&f->wordsfont,     "Times-Roman",    12.0, 0); 
  fontspec (&f->gchordfont,    "Helvetica",      12.0, 0); 
  fontspec (&f->voicefont,     "Times-Roman",    12.0, 0); 
  fontspec (&f->barnumfont,    "Times-Italic",   12.0, 0); 
  fontspec (&f->barlabelfont,  "Times-Bold",     18.0, 0); 
  fontspec (&f->indexfont,     "Times-Roman",    11.0, 0); 
  if (vb>=10) printf ("Loading format \"%s\"\n",f->name);
}

/* ----- set_pretty_format ----- */
void set_pretty_format (struct FORMAT *f)
{
  set_standard_format (f);
  strcpy (f->name,    "pretty");
  f->titlespace             =   0.4  * CM;
  f->composerspace          =   0.25 * CM;
  f->musicspace             =   0.25 * CM;
  f->partsspace             =   0.3  * CM;
  f->staffsep               =  50.0  * PT;
  f->sysstaffsep            =  45.0  * PT;
  f->systemsep              =  55.0  * PT;
  f->stafflinethickness     =  0.75;
  f->scale                  =   0.75;
  f->maxshrink              =   0.55;
  f->parskipfac             =   0.1;
  fontspec (&f->titlefont,     "Times-Roman",    18.0, 0); 
  fontspec (&f->subtitlefont,  "Times-Roman",    15.0, 0); 
  fontspec (&f->composerfont,  "Times-Italic",   12.0, 0); 
  fontspec (&f->partsfont,     "Times-Roman",    12.0, 0); 
  fontspec (&f->tempofont,     "Times-Bold",     10.0, 0); 
  fontspec (&f->vocalfont,     "Times-Roman",    14.0, 0); 
  fontspec (&f->textfont,      "Times-Roman",    10.0, 0); 
  fontspec (&f->wordsfont,     "Times-Roman",    10.0, 0); 
  fontspec (&f->gchordfont,    "Helvetica",      12.0, 0); 
  fontspec (&f->voicefont,     "Times-Roman",    12.0, 0); 
}

/* ----- set_pretty2_format ----- */
void set_pretty2_format (struct FORMAT *f)
{
  set_standard_format (f);
  strcpy (f->name,    "pretty2");
  f->titlespace             =   0.4  * CM;
  f->composerspace          =   0.3  * CM;
  f->musicspace             =   0.25 * CM;
  f->partsspace             =   0.2  * CM;
  f->staffsep               =  55.0  * PT;
  f->sysstaffsep            =  45.0  * PT;
  f->systemsep              =  55.0  * PT;
  f->stafflinethickness     =  0.75;
  f->textspace              =   0.2  * CM;
  f->scale                  =   0.70;
  f->maxshrink              =   0.55;
  f->titleleft              =   1;
  f->parskipfac             =   0.1;
  fontspec (&f->titlefont,     "Helvetica-Bold", 16.0, 0); 
  fontspec (&f->subtitlefont,  "Helvetica-Bold", 13.0, 0); 
  fontspec (&f->composerfont,  "Helvetica",      10.0, 0); 
  fontspec (&f->partsfont,     "Times-Roman",    12.0, 0); 
  fontspec (&f->tempofont,     "Times-Bold",     10.0, 0); 
  fontspec (&f->vocalfont,     "Times-Roman",    14.0, 0); 
  fontspec (&f->textfont,      "Times-Roman",    10.0, 0); 
  fontspec (&f->wordsfont,     "Times-Roman",    10.0, 0); 
  fontspec (&f->gchordfont,    "Helvetica",      12.0, 0); 
  fontspec (&f->voicefont,     "Times-Roman",    12.0, 0); 
  fontspec (&f->barnumfont,    "Times-Roman",    11.0, 1); 
  fontspec (&f->barlabelfont,  "Times-Bold",     18.0, 1); 
}


/* ----- print_font ----- */
void print_font (const char *str, struct FONTSPEC fs)
{
  printf ("  %-14s %s %.1f", str, fs.name, fs.size);
  if (fs.box) printf (" box");
  printf ("\n");
}


/* ----- print_format ----- */
void print_format (struct FORMAT f)
{
  char yn[2][5]={"no","yes"};
  
  printf ("\nFormat \"%s\":\n", f.name);
  printf ("  pageheight     %.2fcm\n",    f.pageheight/CM);
  printf ("  staffwidth     %.2fcm\n",    f.staffwidth/CM);
  printf ("  topmargin      %.2fcm\n",    f.topmargin/CM);
  printf ("  botmargin      %.2fcm\n",    f.botmargin/CM);
  printf ("  leftmargin     %.2fcm\n",    f.leftmargin/CM);
  printf ("  topspace       %.2fcm\n",    f.topspace/CM);
  printf ("  titlespace     %.2fcm\n",    f.titlespace/CM);
  printf ("  subtitlespace  %.2fcm\n",    f.subtitlespace/CM);
  printf ("  composerspace  %.2fcm\n",    f.composerspace/CM);
  printf ("  musicspace     %.2fcm\n",    f.musicspace/CM);
  printf ("  partsspace     %.2fcm\n",    f.partsspace/CM);
  printf ("  wordsspace     %.2fcm\n",    f.wordsspace/CM);
  printf ("  textspace      %.2fcm\n",    f.textspace/CM);
  printf ("  vocalspace     %.1fpt\n",    f.vocalspace);
  printf ("  gchordspace    %.1fpt\n",    f.gchordspace);
  printf ("  staffsep       %.1fpt\n",    f.staffsep);
  printf ("  sysstaffsep    %.1fpt\n",    f.sysstaffsep);
  printf ("  systemsep      %.1fpt\n",    f.systemsep);
  printf ("  stafflinethickness %.2f\n",  f.stafflinethickness);
  printf ("  scale          %.2f\n",      f.scale);
  printf ("  maxshrink      %.2f\n",      f.maxshrink);
  printf ("  strictness1    %.2f\n",      f.strict1);
  printf ("  strictness2    %.2f\n",      f.strict2);
  printf ("  indent         %.1fpt\n",    f.indent);

  print_font("titlefont",    f.titlefont);
  print_font("subtitlefont", f.subtitlefont);
  print_font("composerfont", f.composerfont);
  print_font("partsfont",    f.partsfont);
  print_font("tempofont",    f.tempofont);
  print_font("vocalfont",    f.vocalfont);
  print_font("gchordfont",   f.gchordfont);
  print_font("textfont",     f.textfont);
  print_font("wordsfont",    f.wordsfont);
  print_font("voicefont",    f.voicefont);
  print_font("barnumberfont",f.barnumfont);
  print_font("barlabelfont", f.barlabelfont);
  print_font("indexfont",    f.indexfont);

  printf ("  lineskipfac    %.1f\n",    f.lineskipfac);
  printf ("  parskipfac     %.1f\n",    f.parskipfac);
  printf ("  barsperstaff   %d\n",      f.barsperstaff);
  printf ("  barnumbers     %d\n",      f.barnums);
  printf ("  barnumberfirst %d\n",      f.barinit);
  printf ("  landscape      %s\n", yn[f.landscape]);
  printf ("  titleleft      %s\n", yn[f.titleleft]);
  printf ("  titlecaps      %s\n", yn[f.titlecaps]);
  printf ("  stretchstaff   %s\n", yn[f.stretchstaff]);
  printf ("  stretchlast    %s\n", yn[f.stretchlast]);
  printf ("  writehistory   %s\n", yn[f.writehistory]);
  printf ("  continueall    %s\n", yn[f.continueall]);
  printf ("  breakall       %s\n", yn[f.breakall]);
  printf ("  oneperpage     %s\n", yn[f.oneperpage]);
  printf ("  withxrefs      %s\n", yn[f.withxrefs]);
  printf ("  squarebrevis   %s\n", yn[f.squarebrevis]);
  printf ("  endingdots     %s\n", yn[f.endingdots]);
  printf ("  slurisligatura %s\n", yn[f.slurisligatura]);
  printf ("  historicstyle  %s\n", yn[f.historicstyle]);
  printf ("  nobeams        %s\n", yn[f.nobeams]);
  printf ("  nogracestroke  %s\n", yn[f.nogracestroke]);
  printf ("  printmetronome %s\n", yn[f.printmetronome]);
  printf ("  nostems        %s\n", yn[f.nostems]);


  /* tablature specific stuff */
  print_tab_format();
}


/* ----- g_unum: read a number with a unit ----- */
void g_unum (const char *l, char *s, float *num)
{
  float a,b;
  char unit[81];

  strcpy(unit,"pt");
  sscanf(s,"%f%80s", &a, unit);

  if      (!strcmp(unit,"cm")) b=a*CM;
  else if (!strcmp(unit,"mm")) b=a*CM*0.1;
  else if (!strcmp(unit,"in")) b=a*IN;
  else if (!strcmp(unit,"pt")) b=a*PT;
  else {
    printf ("+++ Unknown unit \"%s\" in line: %s\n",unit,l); 
    exit (3);
  }
  *num = b;
}

/* ----- g_logv: read a logical variable ----- */ 
void g_logv (const char *l, char *s, int *v)
{
  int k;
  char t[31];

  strcpy(t,"1");
  sscanf (s,"%30s", t);
  if (!strcmp(t,"1") || !strcmp(t,"yes") || !strcmp(t,"true"))
    k=1;
  else if (!strcmp(t,"0") || !strcmp(t,"no") || !strcmp(t,"false"))
    k=0;
  else {
    printf ("\n+++ Unknown logical value \"%s\" near \"%s\"\n",t,l); 
    exit (3);
  }
  *v = k; 
}


/* ----- g_fltv: read a float variable, no units ----- */ 
void g_fltv (char *l, int nch, float *v)
{
  float k;

  sscanf (l+nch,"%f", &k);
  *v = k;
}

/* ----- g_intv: read an int variable, no units ----- */ 
void g_intv (char *l, int nch, int *v)
{
  int k;

  sscanf (l+nch,"%d", &k);
  *v = k;
}



/* ----- g_fspc: read a font specifier ----- */ 
void g_fspc (char *l, int nch, struct FONTSPEC *fn)
{
  char  fname[STRLFILE],ssiz[STRLFILE],sbox[STRLFILE];
  char  scanfstring[256];
  float fsize;

  fsize=fn->size;
  strcpy(sbox,"SnOt");
  strcpy(ssiz,"SnOt");

  sprintf(scanfstring, "%%%is %%%is %%%is", STRLFILE-1, STRLFILE-1, STRLFILE-1);
  sscanf (l+nch, scanfstring, fname, ssiz, sbox); 
  if (strcmp(fname,"*")) strcpy (fn->name, fname);

  if (strcmp(ssiz,"*")) sscanf(ssiz,"%f",&fsize);
  fn->size = fsize;

  if (!strcmp(sbox,"box"))       fn->box=1;
  else if (!strcmp(sbox,"SnOt")) ;
  else wng ("incorrect font spec: ", l+nch);

  if (!file_initialized) add_font (fn);
}


/* ----- interpret_format_line ----- */
/* read a line with a format directive, set in format struct f */
int interpret_format_line (char *line, struct FORMAT *f)
{
  char l[STRLFMT],w[STRLFMT],fnm[STRLFMT];
  char  scanfstring[256];
  int nch,i,fnum,pos;
  char *s;
  struct FONTSPEC tempfont;

  /* format lines must not be longer than STRLFMT characters */
  strnzcpy(l,line,STRLFMT);

  strcpy(w,"");
  sprintf(scanfstring, "%%%is%%n", STRLFMT-1);
  sscanf(l, scanfstring, w, &nch);
  if (!strcmp(w,"")) return 0;
  if (w[0]=='%') return 0;
  if (vb>=6) printf ("Interpret format line: %s\n", l);
  if (!strcmp(w,"end")) return 1;
  s=l+nch;

  if      (!strcmp(w,"pageheight"))    g_unum(l,s,&f->pageheight);
  else if (!strcmp(w,"staffwidth"))    g_unum(l,s,&f->staffwidth);
  else if (!strcmp(w,"topmargin"))     g_unum(l,s,&f->topmargin);
  else if (!strcmp(w,"botmargin"))     g_unum(l,s,&f->botmargin);
  else if (!strcmp(w,"leftmargin"))    g_unum(l,s,&f->leftmargin);
  else if (!strcmp(w,"topspace"))      g_unum(l,s,&f->topspace);
  else if (!strcmp(w,"wordsspace"))    g_unum(l,s,&f->wordsspace);
  else if (!strcmp(w,"titlespace"))    g_unum(l,s,&f->titlespace);
  else if (!strcmp(w,"subtitlespace")) g_unum(l,s,&f->subtitlespace);
  else if (!strcmp(w,"composerspace")) g_unum(l,s,&f->composerspace);
  else if (!strcmp(w,"musicspace"))    g_unum(l,s,&f->musicspace);
  else if (!strcmp(w,"partsspace"))    g_unum(l,s,&f->partsspace);
  else if (!strcmp(w,"staffsep"))      g_unum(l,s,&f->staffsep);
  else if (!strcmp(w,"sysstaffsep"))   g_unum(l,s,&f->sysstaffsep);
  else if (!strcmp(w,"systemsep"))     g_unum(l,s,&f->systemsep);
  else if (!strcmp(w,"vocalspace"))    g_unum(l,s,&f->vocalspace);
  else if (!strcmp(w,"textspace"))     g_unum(l,s,&f->textspace);
  else if (!strcmp(w,"gchordspace"))   g_unum(l,s,&f->gchordspace);
  
  else if (!strcmp(w,"scale"))         g_fltv(l,nch,&f->scale);
  else if (!strcmp(w,"stafflinethickness")) g_fltv(l,nch,&f->stafflinethickness);
  else if (!strcmp(w,"maxshrink"))     g_fltv(l,nch,&f->maxshrink);
  else if (!strcmp(w,"lineskipfac"))   g_fltv(l,nch,&f->lineskipfac);
  else if (!strcmp(w,"parskipfac"))    g_fltv(l,nch,&f->parskipfac);
  else if (!strcmp(w,"barsperstaff"))  g_intv(l,nch,&f->barsperstaff);
  else if (!strcmp(w,"barnumbers"))    g_intv(l,nch,&f->barnums);
  else if (!strcmp(w,"barnumberfirst")) g_intv(l,nch,&f->barinit);
  else if (!strcmp(w,"strictness1"))   g_fltv(l,nch,&f->strict1);
  else if (!strcmp(w,"strictness2"))   g_fltv(l,nch,&f->strict2);
  else if (!strcmp(w,"strictness")) {
    g_fltv(l,nch,&f->strict1); f->strict2=f->strict1; }
  else if (!strcmp(w,"indent"))        g_unum(l,s,&f->indent);

  else if (!strcmp(w,"titleleft"))     g_logv(l,s,&f->titleleft);
  else if (!strcmp(w,"titlecaps"))     g_logv(l,s,&f->titlecaps);
  else if (!strcmp(w,"landscape"))     g_logv(l,s,&f->landscape);
  else if (!strcmp(w,"stretchstaff"))  g_logv(l,s,&f->stretchstaff);
  else if (!strcmp(w,"stretchlast"))   g_logv(l,s,&f->stretchlast);
  else if (!strcmp(w,"continueall"))   g_logv(l,s,&f->continueall);
  else if (!strcmp(w,"breakall"))      g_logv(l,s,&f->breakall);
  else if (!strcmp(w,"writehistory"))  g_logv(l,s,&f->writehistory);
  else if (!strcmp(w,"withxrefs"))     g_logv(l,s,&f->withxrefs);
  else if (!strcmp(w,"oneperpage"))    g_logv(l,s,&f->oneperpage);
  else if (!strcmp(w,"squarebrevis"))  g_logv(l,s,&f->squarebrevis);
  else if (!strcmp(w,"endingdots"))    g_logv(l,s,&f->endingdots);
  else if (!strcmp(w,"slurisligatura")) g_logv(l,s,&f->slurisligatura);
  else if (!strcmp(w,"historicstyle")) g_logv(l,s,&f->historicstyle);
  else if (!strcmp(w,"nobeams"))       g_logv(l,s,&f->nobeams);
  else if (!strcmp(w,"nogracestroke")) g_logv(l,s,&f->nogracestroke);
  else if (!strcmp(w,"printmetronome")) g_logv(l,s,&f->printmetronome);
  else if (!strcmp(w,"nostems"))       g_logv(l,s,&f->nostems);

  else if (!strcmp(w,"titlefont"))     g_fspc(l,nch,&f->titlefont);
  else if (!strcmp(w,"subtitlefont"))  g_fspc(l,nch,&f->subtitlefont);
  else if (!strcmp(w,"vocalfont"))     g_fspc(l,nch,&f->vocalfont);
  else if (!strcmp(w,"partsfont"))     g_fspc(l,nch,&f->partsfont);
  else if (!strcmp(w,"tempofont"))     g_fspc(l,nch,&f->tempofont);
  else if (!strcmp(w,"textfont"))      g_fspc(l,nch,&f->textfont);
  else if (!strcmp(w,"composerfont"))  g_fspc(l,nch,&f->composerfont);
  else if (!strcmp(w,"wordsfont"))     g_fspc(l,nch,&f->wordsfont);
  else if (!strcmp(w,"gchordfont"))    g_fspc(l,nch,&f->gchordfont);
  else if (!strcmp(w,"voicefont"))     g_fspc(l,nch,&f->voicefont);
  else if (!strcmp(w,"barnumberfont")) g_fspc(l,nch,&f->barnumfont);
  else if (!strcmp(w,"barlabelfont"))  g_fspc(l,nch,&f->barlabelfont);
  else if (!strcmp(w,"indexfont"))     g_fspc(l,nch,&f->indexfont);

  else if (!strcmp(w,"font")) {
    sprintf(scanfstring, "%%*s %%%is", STRLFMT-1);
    sscanf(l, scanfstring, fnm);
    fnum=-1;
    for (i=0;i<nfontnames;i++) {
      if (!strcmp(fnm,fontnames[i])) fnum=i; 
    }
    if (fnum<0) {
      if (file_initialized) {
        printf ("+++ Cannot predefine when output file open: %s\n", l);
        exit (3);
      }
      tempfont.size=12.0;
      g_fspc(l,nch,&tempfont);
    }
  }
  
  else if (!strcmp(w,"meterdisplay")) {
    string str = s;
    string key, value;
    if ((pos=str.find('=')) != string::npos) {
      key = str.substr(0,pos); strip(&key);
      value = str.substr(pos+1,str.length()); strip(&value);
      f->meterdisplay[key] = value;
    } else {
      printf("missing '=' in meterdisplay:%s\n", s);
    }
  }

  else if (!read_tab_format(l)) {
    if (vb>=5) printf ("Ignore format line: %s\n", l);
    return 2;
  }
  return 0;
}

/* ----- read_fmt_file ----- */
int read_fmt_file (const char *filename, char *dirname, struct FORMAT *f)
{
  FILE *fp;
  string line;
  char* fname;
  int i,end;

  fname = (char*) alloca(sizeof(char)*(strlen(filename)+strlen(dirname)+4));
  strcpy(fname,filename);
  if ((fp = fopen (fname,"r")) == NULL) {      
    if (strlen(dirname)==0) 
      return 0;
    else {
      strcpy(fname,dirname);
      strcat(fname,"/");
      strcat(fname,filename);
      if ((fp = fopen (fname,"r")) == NULL) return 0;
    }
  }

  if (vb>=4) printf ("Reading format file %s:\n", fname);
  printf ("%s .. ", fname);
  getline(&line, fp);
  for (i=0;i<200;i++) {
    end=interpret_format_line ((char*)line.c_str(), f);
    if (end==1) break;
    if (feof(fp)) break;
    if (!getline(&line, fp)) break;
  }
  fclose (fp);
  return 1;
}

/* get papersize to given name */
/* if not found, NULL is returned */
struct PAPERSIZE* get_papersize (const char* name)
{
  int i;
  if (!name || !*name) return NULL;
  for (i=0; papersizes[i].name; i++)
    if (0==strcmpnocase(name,papersizes[i].name))
      return &(papersizes[i]);
  return NULL;
}

/* get papersize according to Debian policy, */
/* i.e. from /etc/papersize or environment variable PAPERSIZE */
/* implementation should be compliant to libpaper */
/* If no system setting is found, the first entry in papersizes is returned */
struct PAPERSIZE* get_system_papersize ()
{
  const char* papersizefile = DIRSEPCHAR "etc" DIRSEPCHAR "papersize";
  char* envvar;
  struct PAPERSIZE* retval;
  FILE* fp;

  /* papersize explicitly set in PAPERSIZE ? */
  if ((envvar=getenv("PAPERSIZE"))) {
    if ((retval=get_papersize(envvar))) {
      if (vb>3)
        printf("papersize found in PAPERSIZE: %s\n",envvar);
      return retval;
    } else {
      wng("unknown papersize in environment variable PAPERSIZE: ",envvar);
      printf("use default papersize %s\n", papersizes[0].name);
      return &(papersizes[0]);
    }
  }

  /* different file name set in PAPERCONF ? */
  if ((envvar=getenv("PAPERCONF"))) {
    papersizefile = envvar;
    if (vb>3)
      printf("papersize file read from PAPERCONF: %s\n",envvar);
    if (0!=access(papersizefile,R_OK)) {
      wng("cannot read papersize file set in environment variable PAPERCONF: ",
          papersizefile);
      printf("use default papersize %s\n", papersizes[0].name);
      return &(papersizes[0]);
    }  
  }

  /* check papersize file */
  if (!(fp = fopen(papersizefile,"r"))) {
    if (vb>3)
      printf("could not open papersize file %s\n", papersizefile);
  } else {
    string linebuf;
    int rc;
    int pos;
    while ((rc=getline(&linebuf, fp))) {
      /* stop on first nonempty or noncomment line */
      pos = linebuf.find_first_not_of(" \t\n\r");
      if (linebuf[pos] && (linebuf[pos]!='#')) break;
    }
    fclose(fp);
    if (rc) {
      if (vb>3)
        printf("papersize %s read from %s\n", linebuf.c_str(), papersizefile);
      retval=get_papersize(linebuf.c_str());
      if (retval)
        return retval;
      else
        wng("unknown papersize in papersize file: ", papersizefile);
    } else {
      wng("no entry in papersize file: ", papersizefile);
    }
  }

  /* no system preference found */
  if (vb>3)
    printf("use default papersize %s\n", papersizes[0].name);
  return &(papersizes[0]);
}
