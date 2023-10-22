/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "abctab2ps.h"
#include "util.h"
#include "format.h"
#include "subs.h"
#include "tab.h"

#include "parse.h"


/*  subroutines connected with parsing the input file  */

/* ----- sytax: print message for syntax error -------- */
void syntax (const char *msg, char *q)
{
  int i,n,len,m1,m2,pp,qq,maxcol=65;
  
  if (verbose<=2) printf ("\n");
  if (!q) {
    printf ("+++ %s in line %d\n", msg, linenum);
    return;
  }
  qq=q-p0+1;
  if (qq<0) qq=0;
  printf ("+++ %s in line %d.%d \n", msg, linenum, qq);
  m1=0;
  m2=len=strlen(p0);
  n=q-p0;
  if (m2>maxcol) {
    if (n<maxcol) 
      m2=maxcol;
    else {
      m1=n-10;
      m2=m1+maxcol;
      if (m2>len) m2=len;
    }
  }

  printf ("%4d ", linenum);
  pp=5;
  if (m1>0) { printf ("..."); pp+=3; }
  for (i=m1;i<m2;i++) printf ("%c", p0[i]);
  if (m2<len) printf ("...");
  printf ("\n"); 

  if (n>=0 && n<200) {
    for (i=0;i<n+pp-m1;i++) printf(" ");
    printf ("^\n"); 
  }
}

/* ----- isnote: checks char for valid note symbol ----- */
int isnote (char c)
{
  if (c == '\0') return 0;
  if (strchr("CDEFGABcdefgab^=_",c)) return 1;
  return 0;
}

/* ----- zero_sym: init global zero SYMBOL struct ----- */
void zero_sym (void)
{
  int j;
  zsym.type      = 0;
  for (j=0;j<MAXHD;j++) {
    zsym.pits[j]=zsym.lens[j]=zsym.accs[j]=0;
    zsym.sl1[j]=zsym.sl2[j]=0;
    zsym.ti1[j]=zsym.ti2[j]=0;
    zsym.ten1[j]=zsym.ten2[j]=0;
    zsym.shhd[j]=zsym.shac[j]=0;
  }
  zsym.lig1 = zsym.lig2 = 0;
  zsym.npitch    = 0;
  zsym.len       = 0;
  zsym.fullmes   = 0;
  zsym.word_st   = 0;
  zsym.word_end  = 0;
  zsym.slur_st   = 0;
  zsym.slur_end  = 0;
  zsym.ten_st    = 0;
  zsym.ten_end   = 0;
  zsym.yadd      = 0;
  zsym.x=zsym.y  = 0.0;
  zsym.ymn       = 0;
  zsym.ymx       = 0;
  zsym.yav       = 0;
  zsym.ylo       = 12;
  zsym.yhi       = 12;
  zsym.xmn=zsym.xmx = 0;
  zsym.stem      = 0;
  zsym.flags     = 0;
  zsym.dots      = 0;
  zsym.head      = 0;
  zsym.eoln      = 0;
  zsym.grcpit    = 0;
  zsym.gr.n      = 0;
  zsym.dc.clear();
  zsym.xs        = 0;
  zsym.ys        = 0;
  zsym.u         = 0;
  zsym.v         = 0;
  zsym.w         = 0;
  zsym.t         = 0;
  zsym.q         = 0;
  zsym.invis     = 0;
  zsym.wl=zsym.wr= 0;
  zsym.pl=zsym.pr = 0;
  zsym.xl=zsym.xr = 0;
  zsym.p_plet=zsym.q_plet=zsym.r_plet = 0;
  zsym.gchy      = 0;
  strcpy (zsym.text, "");
  zsym.gchords = NULL;
  zsym.wlines = 0;
  for (j=0;j<NWLINE;j++) zsym.wordp[j] = 0;
  zsym.p         = -1;
  zsym.time=0;
  for (j=0;j<MAXHD;j++) {
    zsym.tabdeco[j]=0;
  }
}

/* ----- add_sym: returns index for new symbol at end of list ---- */
int add_sym (int type)
{
  int k;

  k=voice[ivc].nsym;
  if (k>=maxSyms) {
    /*rxi("Too many symbols; increase maxSyms, now ",maxSyms);*/
    realloc_structs(maxSyms+allocSyms,maxVc);
  }
  voice[ivc].nsym++;
  symv[ivc][k]=zsym;
  symv[ivc][k].gchords= new GchordList();
  symv[ivc][k].type = type;
  return k;

}

/* ----- insert_sym: returns index for new symbol inserted at k --- */
int insert_sym (int type, int k)
{
  int i,n;

  n=voice[ivc].nsym;
  if (n>=maxSyms) {
    /*rxi("insert_sym: maxSyms exceeded: ",maxSyms);*/
    realloc_structs(maxSyms+allocSyms,maxVc);
  }
  for (i=n;i>k;i--) symv[ivc][i]=symv[ivc][i-1];
  n++;
  symv[ivc][k]=zsym;
  symv[ivc][k].gchords= new GchordList();
  symv[ivc][k].type = type;
  voice[ivc].nsym=n;
  return k;
}

/* ----- get_xref: get xref from string ----- */
int get_xref (char *str)
{
  
  int a,ok;
  char *q;

  if (strlen(str)==0) {
    wng ("xref string is empty", "");
    return 0;
  }
  q=str;
  ok=1;
  while (*q != '\0') {
    if (!isdigit(*q)) ok=0;
    q++;
  }
  if (!ok) {
    wng ("xref string has invalid symbols: ", str);
    return 0;
  }

  sscanf (str, "%d", &a);

  return a;
}

/* ----- parse_meter_token: interpret pure meter string ---- */
int parse_meter_token(char* str, int* meter1, int* meter2, int* mflag, char* meter_top, int* dlen)
{
  int m1,m2,m1a,m1b,m1c,d;
  char* q;

  /* parse meter specification */
  if (str[0]=='C') {
    if (str[1]=='|') {
      *meter1=4;
      *meter2=4;
      *dlen=EIGHTH;
      *mflag=2;
      strcpy(meter_top,"C");
    }
    else {
      *meter1=4;
      *meter2=4;
      *dlen=EIGHTH;
      *mflag=1;
      strcpy(meter_top,"C");
    }
  }
  else if (!strchr(str,'/')) {
    *meter1=atoi(str);
    *meter2=4;  /*just to put something therein*/
    *dlen=EIGHTH;
    *mflag=3;
    strcpy(meter_top,str);
  }
  else {
    m1=m2=m1a=m1b=m1c=0;
    strcpy (meter_top, str);
    q=strchr(meter_top,'/');
    if (!q) {
      wng("Cannot identify meter, missing /: ", str); 
      m1=m2=1;
      return 1;
    }
    *q='\0';
    if (strchr(meter_top,'+')) {
      sscanf(str,"%d+%d+%d/", &m1a, &m1b, &m1c);
      m1=m1a+m1b+m1c;
    }
    else {
      sscanf(str,"%d %d %d/", &m1a, &m1b, &m1c);
      m1=m1a; if (m1b>m1) m1=m1b; if (m1c>m1) m1=m1c;
      if (m1>30) {            /* handle things like 78/8 */
        m1a=m1/100;
        m1c=m1-100*m1a;
        m1b=m1c/10;
        m1c=m1c-10*m1b;
        m1=m1a; if (m1b>m1) m1=m1b; if (m1c>m1) m1=m1c;
      }
    }

    q++;
    sscanf (q, "%d", &m2);
    if (m1*m2 == 0) wng("Cannot identify meter: ", str); 
    d=BASE/m2;
    if (d*m2 != BASE) wng("Meter not recognized: ", str);
    *meter1=m1;
    *meter2=m2;
    *dlen=EIGHTH;
    if (4*(*meter1) < 3*(*meter2)) *dlen=SIXTEENTH;
    *mflag=0;
  }

  return 0;
}

/* ----- set_meter: interpret meter string, store in struct ---- */
void set_meter (char *mtrstr, struct METERSTR *meter)
{
  int meter1,meter2,dlen,mflag;
  int dismeter1,dismeter2,dismflag;
  char meter_top[31], meter_distop[31], *mtrstrcopy;
  string mtrstring;
  int display;

  if (strlen(mtrstr)==0) { wng("Empty meter string", ""); return; }

  /* if no meter, set invisible 4/4 (for default length) */
  if (!strcmp(mtrstr,"none")) {
    mtrstring = "4/4";
    display = 0;
  } else {
    mtrstring = mtrstr;
    display = 1;
  }

  /* if global meterdisplay option, add "display=..." string accordingly */
  /* (this is ugly and not extensible for more keywords, but works for now) */
  if (!cfmt.meterdisplay.empty() && (string::npos==mtrstring.find("display="))) {
    StringMap::iterator it = cfmt.meterdisplay.find(mtrstring);
    if (it != cfmt.meterdisplay.end())
      mtrstring += " display=" + it->second;
  }

  /* now we move to char* because there is no C++ equivalent to strtok */
  mtrstrcopy = (char*) alloca(sizeof(char)*(mtrstring.length()+1));
  strcpy(mtrstrcopy,mtrstring.c_str());

  /* loop over blank delimited fields */
  for (char* s=strtok(mtrstrcopy," \t"); s; s=strtok(NULL," \t")) {
    int rc, dummy;
    rc = 0;
    if (0==strcmp("display=none", s)) {
      display = 0;
    } else if (0==strncmp("display=", s, strlen("display="))) {
      rc = parse_meter_token(s + strlen("display="), &dismeter1, &dismeter2,
                             &dismflag, meter_distop, &dummy);
      display = 2;
    } else {
      rc = parse_meter_token(s, &meter1, &meter2,
                             &mflag, meter_top, &dlen);
    }
    if (rc) return;
  }
  
  if (verbose>=4) {
    printf ("Meter <%s> is %d over %d with default length 1/%d\n", 
            mtrstr, meter1, meter2, BASE/dlen);
    if (display==2)
      printf ("Meter <%s> will display as %d over %d\n", 
            mtrstr, dismeter1, dismeter2);
    else if (display==0)
      printf ("Meter <%s> will display as <none>\n", mtrstr);
  }

  /* store parsed data in struct */
  meter->meter1 = meter1;
  meter->meter2 = meter2;
  meter->mflag  = mflag;
  if (!meter->dlen)
    meter->dlen = dlen;
  strcpy(meter->top, meter_top);
  meter->display = display;
  if (display==2) {
    meter->dismeter1 = dismeter1;
    meter->dismeter2 = dismeter2;
    meter->dismflag  = dismflag;
    strcpy(meter->distop, meter_distop);
  } else {
    meter->dismeter1 = 0;
    meter->dismeter2 = 0;
    meter->dismflag  = 0;
    strcpy(meter->distop, "");
  }
    
}


/* ----- set_dlen: set default length for parsed notes ---- */
void set_dlen (char *str, struct METERSTR *meter)
{
  int l1,l2,d,dlen;

  l1=0;
  l2=1;
  sscanf(str,"%d/%d ", &l1, &l2);
  if (l1 == 0) return;       /* empty string.. don't change default length */
  else {
    d=BASE/l2;
    if (d*l2 != BASE) {
      wng("Length incompatible with BASE, using 1/8: ",str);
      dlen=BASE/8;
    }
    else 
      dlen = d*l1;
  }
  if (verbose>=4)
    printf ("Dlen  <%s> sets default note length to %d/%d = 1/%d\n", 
            str, dlen, BASE, BASE/dlen); 

  meter->dlen=dlen;

}

/* ----- set_keysig: interpret keysig string, store in struct ---- */
/* This part was adapted from abc2mtex by Chris Walshaw */
/* updated 03 Oct 1997 Wil Macaulay - support all modes */
/* Returns 1 if key was actually specified, because then we want
   to transpose. Returns zero if this is just a clef change. */
int set_keysig(char *s, struct KEYSTR *ks, int init)
{
  int c,sf,j,ok;
  char w[81];
  int root,root_acc;

  /* maybe initialize with key C (used for K field in header) */
  if (init) {
    ks->sf         = 0;
    ks->ktype      = TREBLE;
    ks->add_pitch  = 0;
    ks->root       = 2;
    ks->root_acc   = A_NT;
  }

  /* check for clef specification with no other information */
  if (!strncmp(s,"clef=",5) && set_clef(s+5,ks)) return 0;
  if (set_clef(s,ks)) return 0;

  /* check for key specifier */
  c=0;
  bagpipe=0;
  switch (s[c]) {
  case 'F':
    sf = -1; root=5;
    break;
  case 'C':
    sf = 0; root=2;
    break;
  case 'G':
    sf = 1; root=6;
    break;
  case 'D':
    sf = 2; root=3;
    break;
  case 'A':
    sf = 3; root=0;
    break;
  case 'E': 
    sf = 4; root=4;
    break;
  case 'B':
    sf = 5; root=1;
    break;
  case 'H':
    bagpipe=1;
    c++;
    if      (s[c] == 'P') { sf=0; root=2; }
    else if (s[c] == 'p') { sf=2; root=3; }
    else { sf=0; root=2; wng("unknown bagpipe-like key: ",s); }
    break;
  default:
    wng ("Using C because key not recognised: ", s);
    sf = 0; root=2;
  }
  c++;

  root_acc=A_NT;
  if (s[c] == '#') {
    sf += 7;
    c += 1;
    root_acc=A_SH;
  } else if (s[c] == 'b') {
    sf -= 7;
    c += 1;
    root_acc=A_FT;
  }

  /* loop over blank-delimited words: get the next token in lower case */
  for (;;) { 
    while (isspace(s[c])) c++;
    if (s[c]=='\0') break;

    j=0;
    while (!isspace(s[c]) && (s[c]!='\0')) { w[j]=tolower(s[c]); c++; j++; }
    w[j]='\0';
    
    /* now identify this word */
    
    /* first check for mode specifier */
    if ((strncmp(w,"mix",3)) == 0) {
      sf -= 1;
      ok = 1;
      /* dorian mode on the second note (D in C scale) */
    } else if ((strncmp(w,"dor",3)) == 0) {
      sf -= 2;
      ok = 1;
      /* phrygian mode on the third note (E in C scale) */
    } else if ((strncmp(w,"phr",3)) == 0) {
      sf -= 4;
      ok = 1;
      /* lydian mode on the fourth note (F in C scale) */
    } else if ((strncmp(w,"lyd",3)) == 0) {
      sf += 1;
      ok = 1;
      /* locrian mode on the seventh note (B in C scale) */
    } else if ((strncmp(w,"loc",3)) == 0) {
      sf -= 5;
      ok = 1;
      /* major and ionian are the same ks */
    } else if ((strncmp(w,"maj",3)) == 0) {
      ok = 1;
    } else if ((strncmp(w,"ion",3)) == 0) {
      ok = 1;
      /* aeolian, m, minor are the same ks - sixth note (A in C scale) */
    } else if ((strncmp(w,"aeo",3)) == 0) {
      sf -= 3;
      ok = 1;
    } else if ((strncmp(w,"min",3)) == 0) {
      sf -= 3;
      ok = 1;
    } else if ((strcmp(w,"m")) == 0) {
      sf -= 3;
      ok = 1;
    }

    /* check for trailing clef specifier */
    else if (!strncmp(w,"clef=",5)) {
      if (!set_clef(w+5,ks))
        wng("unknown clef specifier: ",w);
    }
    else if (set_clef(w,ks)) {
      /* (clef specification without "clef=" prefix) */
    }

    /* nothing found */
    else wng("Unknown token in key specifier: ",w);

  }  /* end of loop over blank-delimted words */

  if (verbose>=4) printf ("Key   <%s> gives sharpsflats %d, type %d\n", 
                          s, sf, ks->ktype);

  /* copy to struct */
  ks->sf         = sf;
  ks->root       = root;
  ks->root_acc   = root_acc;

  return 1;

}

/* ----- set_clef: parse clef specifier and store in struct --- */
/* RC: 0 if no clef specifier; 1 if clef identified */
int set_clef(char* s, struct KEYSTR *ks)
{
  struct {  /* zero terminated list of music clefs */
    const char* name; int value;
  } cleflist[] = {
    {"treble", TREBLE},
    {"treble8", TREBLE8},
    {"treble8up", TREBLE8UP},
    {"bass", BASS},
    {"alto", ALTO},
    {"tenor", TENOR},
    {"soprano", SOPRANO},
    {"mezzosoprano", MEZZOSOPRANO},
    {"baritone", BARITONE},
    {"varbaritone", VARBARITONE},
    {"subbass", SUBBASS},
    {"frenchviolin", FRENCHVIOLIN},
    {0, 0}
  };
  int i, len;

  /* get clef name length without octave modifier (+8 etc) */
  len = strcspn(s,"+-");

  /* loop over possible music clefs */
  for (i=0; cleflist[i].name; i++) {
    if (!strncmp(s,cleflist[i].name,len)) {
      ks->ktype=cleflist[i].value;
      /* check for octave modifier */
      if (s[len]) {
        char* mod=&(s[len]);
        if (!strcmp(mod,"+8"))
          ks->add_pitch=7;
        else if (!strcmp(mod,"-8"))
          ks->add_pitch=-7;
        else if (!strcmp(mod,"+0") || !strcmp(mod,"-0"))
          ks->add_pitch=0;
        else if (!strcmp(mod,"+16"))
          ks->add_pitch=+14;
        else if (!strcmp(mod,"-16"))
          ks->add_pitch=-14;
        else
          wng("unknown octave modifier in clef: ",s);
      }
      return 1;
    }
  }

  /* check for tablature clef */
  if (parse_tab_key(s,&ks->ktype)) return 1;

  return 0;
}

/* ----- get_halftones: figure out how by many halftones to transpose --- */
/*  In the transposing routines: pitches A..G are coded as with 0..7 */
int get_halftones (struct KEYSTR key, char *transpose)
{
  int pit_old,pit_new,direction,stype,root_new,racc_new,nht;
  int root_old, racc_old;
  char *q;
  /* pit_tab associates true pitches 0-11 with letters A-G */ 
  int pit_tab[] = {0,2,3,5,7,8,10};

  if (strlen(transpose)==0) return 0;
  root_new=root_old=key.root;
  racc_old=key.root_acc;

  /* parse specification for target key */
  q=transpose;
  direction=0;

  if (*q=='^') { 
    direction=1; q++;
  } else if (*q=='_') {
    direction=-1; q++; 
  }
  stype=1;
  if (strchr("ABCDEFG",*q)) {
    root_new=*q-'A'; q++; stype=2;
  } else if (strchr("abcdefg",*q)) {
    root_new=*q-'a'; q++;  stype=2;
  }
  
  /* first case: offset was given directly as numeric argument */
  if (stype==1) {
    sscanf(q,"%d", &nht);
    if (direction<0) nht=-nht;
    if (nht==0) {
      if (direction<0) nht=-12;
      if (direction>0) nht=+12;
    }
    return nht;
  }

  /* second case: root of target key was specified explicitly */
  racc_new=0;
  if (*q=='b') {
    racc_new=A_FT; q++;
  } else if (*q=='#') {
    racc_new=A_SH; q++;
  } else if (*q!='\0')
    wng ("expecting accidental in transpose spec: ", transpose);
  
  /* get pitch as number from 0-11 for root of old key */
  pit_old=pit_tab[root_old];
  if (racc_old==A_FT) pit_old--;
  if (racc_old==A_SH) pit_old++;
  if (pit_old<0)  pit_old+=12;
  if (pit_old>11) pit_old-=12;

  /* get pitch as number from 0-11 for root of new key */
  pit_new=pit_tab[root_new];
  if (racc_new==A_FT) pit_new--;
  if (racc_new==A_SH) pit_new++;
  if (pit_new<0)  pit_new+=12;
  if (pit_new>11) pit_new-=12;

  /* number of halftones is difference */
  nht=pit_new-pit_old;
  if (direction==0) {
    if (nht>6)  nht-=12;
    if (nht<-5) nht+=12;
  } 
  if (direction>0 && nht<=0) nht+=12;
  if (direction<0 && nht>=0) nht-=12;
    
  return nht;

}



/* ----- shift_key: make new key by shifting nht halftones --- */
void shift_key (int sf_old, int nht, int *sfnew, int *addt)
{
  int sf_new,r_old,r_new,add_t,  dh,dr;
  int skey_tab[] = {2,6,3,0,4,1,5,2};
  int fkey_tab[] = {2,5,1,4,0,3,6,2};
  char root_tab[]={'A','B','C','D','E','F','G'};

  /* get sf_new by adding 7 for each halftone, then reduce mod 12 */
  sf_new=sf_old+nht*7;
  sf_new=(sf_new+240)%12;
  if (sf_new>=6) sf_new=sf_new-12;
  
  /* get old and new root in ionian mode, shift is difference */
  r_old=2;
  if (sf_old>0) r_old=skey_tab[sf_old];
  if (sf_old<0) r_old=fkey_tab[-sf_old];
  r_new=2;
  if (sf_new>0) r_new=skey_tab[sf_new];
  if (sf_new<0) r_new=fkey_tab[-sf_new];
  add_t=r_new-r_old;

  /* fix up add_t to get same "decade" as nht */
  dh=(nht+120)/12; dh=dh-10;
  dr=(add_t+70)/7; dr=dr-10;
  add_t=add_t+7*(dh-dr);

  if (verbose>=8) 
    printf ("shift_key: sf_old=%d new %d   root: old %c new %c  shift by %d\n",  
            sf_old, sf_new, root_tab[r_old], root_tab[r_new], add_t); 
  
  *sfnew=sf_new;
  *addt=add_t;
 
}


/* ----- set_transtab: setup for transposition by nht halftones --- */
void set_transtab (int nht, struct KEYSTR *key)
{
  int a,b,sf_old,sf_new,add_t,i,j,acc_old,acc_new,root_old,root_acc;
  /* for each note A..G, these tables tell how many sharps (resp. flats)
     the keysig must have to get the accidental on this note. Phew. */
  int sh_tab[] = {5,7,2,4,6,1,3};
  int fl_tab[] = {3,1,6,4,2,7,5};
  /* tables for pretty printout only */
  char root_tab[]={'A','B','C','D','E','F','G'};
  char acc_tab[][3] ={"bb","b ","  ","# ","x "};
  char c1[6],c2[6],c3[6];

  /* nop if no transposition is wanted */
  if (nht==0) {
    key->add_transp=0;
    for (i=0;i<7;i++) key->add_acc[i]=0;
    return;
  }

  /* get new sharps_flats and shift of numeric pitch; copy to key */
  sf_old   = key->sf;
  root_old = key->root;
  root_acc = key->root_acc;
  shift_key (sf_old, nht, &sf_new, &add_t);
  key->sf = sf_new;
  key->add_transp = add_t;

  /* set up table for conversion of accidentals */
  for (i=0;i<7;i++) {
    j=i+add_t;
    j=(j+70)%7;
    acc_old=0;
    if ( sf_old >= sh_tab[i]) acc_old=1;
    if (-sf_old >= fl_tab[i]) acc_old=-1;
    acc_new=0;
    if ( sf_new >= sh_tab[j]) acc_new=1;
    if (-sf_new >= fl_tab[j]) acc_new=-1;
    key->add_acc[i]=acc_new-acc_old;
  }

  /* printout keysig change */
  if (verbose>=3) {
    i=root_old;
    j=i+add_t;
    j=(j+70)%7;
    acc_old=0;
    if ( sf_old >= sh_tab[i]) acc_old=1;
    if (-sf_old >= fl_tab[i]) acc_old=-1;
    acc_new=0;
    if ( sf_new >= sh_tab[j]) acc_new=1;
    if (-sf_new >= fl_tab[j]) acc_new=-1;
    strcpy(c3,"s"); if (nht==1 || nht==-1) strcpy(c3,"");
    strcpy(c1,""); strcpy(c2,""); 
    if (acc_old==-1) strcpy(c1,"b"); if (acc_old==1) strcpy(c1,"#");
    if (acc_new==-1) strcpy(c2,"b"); if (acc_new==1) strcpy(c2,"#");
    printf ("Transpose root from %c%s to %c%s (shift by %d halftone%s)\n",
            root_tab[i],c1,root_tab[j],c2,nht,c3);
  }
  
  /* printout full table of transformations */
  if (verbose>=4) {
    printf ("old & new keysig    conversions\n");
    for (i=0;i<7;i++) {
      j=i+add_t;
      j=(j+70)%7;
      acc_old=0;
      if ( sf_old >= sh_tab[i]) acc_old=1;
      if (-sf_old >= fl_tab[i]) acc_old=-1;
      acc_new=0;
      if ( sf_new >= sh_tab[j]) acc_new=1;
      if (-sf_new >= fl_tab[j]) acc_new=-1;
      printf("%c%s-> %c%s           ", root_tab[i],acc_tab[acc_old+2],
             root_tab[j],acc_tab[acc_new+2]);
      for (a=-1;a<=1;a++) {
        b=a+key->add_acc[i];
        printf ("%c%s-> %c%s  ", root_tab[i],acc_tab[a+2],
                root_tab[j],acc_tab[b+2]);
      }
      printf ("\n");
    }
  }

}   

/* ----- do_transpose: transpose numeric pitch and accidental --- */
void do_transpose (struct KEYSTR key, int *pitch, int *acc)
{
  int pitch_old,pitch_new,sf_old,sf_new,acc_old,acc_new,i,j;

  pitch_old = *pitch;
  acc_old   = *acc;
  acc_new   = acc_old;
  sf_old    = key.sf;
  pitch_new=pitch_old+key.add_transp;
  i=(pitch_old+70)%7;
  j=(pitch_new+70)%7;

  if (acc_old) {
    if (acc_old==A_DF) sf_old=-2;
    if (acc_old==A_FT) sf_old=-1;
    if (acc_old==A_NT) sf_old=0 ;
    if (acc_old==A_SH) sf_old=1;
    if (acc_old==A_DS) sf_old=2;
    sf_new=sf_old+key.add_acc[i];
    if (sf_new==-2) acc_new=A_DF;
    if (sf_new==-1) acc_new=A_FT;
    if (sf_new== 0) acc_new=A_NT;
    if (sf_new== 1) acc_new=A_SH;
    if (sf_new== 2) acc_new=A_DS;
  }
  else {
    acc_new=0;
  }
  *pitch = pitch_new;
  *acc   = acc_new;
}


/* ----- gch_transpose: transpose guitar chord string in gch --- */
void gch_transpose (string* gch, struct KEYSTR key)
{
  char *q,*r;
  char* gchtrans;
  int root_old,root_new,sf_old,sf_new,ok;
  char root_tab[]={'A','B','C','D','E','F','G'};
  char root_tub[]={'a','b','c','d','e','f','g'};

  if (!transposegchords || (halftones==0)) return;

  q = (char*)gch->c_str();
  gchtrans = (char*)alloca(sizeof(char)*gch->length());
  r = gchtrans;

  for (;;) {
    while (*q==' ' || *q=='(') { *r=*q; q++; r++; }
    if (*q=='\0') break;
    ok=0;
    if (strchr("ABCDEFG",*q)) {
      root_old=*q-'A'; q++; ok=1;
    } else if (strchr("abcdefg",*q)) {
      root_old=*q-'a'; q++; ok=2;
    } else {
      root_old=key.root;
    }
    
    if (ok) {
      sf_old=0;
      if (*q=='b') { sf_old=-1; q++; }
      if (*q=='#') { sf_old= 1; q++; }
      root_new=root_old+key.add_transp;
      root_new=(root_new+28)%7;
      sf_new=sf_old+key.add_acc[root_old];
      if (ok==1) { *r=root_tab[root_new]; r++; }
      if (ok==2) { *r=root_tub[root_new]; r++; }
      if (sf_new==-1) { *r='b'; r++; }
      if (sf_new== 1) { *r='#'; r++; }
    }
    
    while (*q!=' ' && *q!='/' && *q!='\0') {*r=*q; q++; r++; }
    if (*q=='/') {*r=*q; q++; r++; }
    
  }  
  
  *r='\0';
  /*|   printf("tr_ch: <%s>  <%s>\n", gch, str);   |*/

  *gch = gchtrans;
}


/* ----- init_parse_params: initialize variables for parsing --- */
void init_parse_params (void)
{
  int i;

  slur=0;
  nwpool=nwline=0;
  ntinext=0;

  /* for continuation after output: reset nsym, switch to first voice */
  for (i=0;i<nvoice;i++) {
    voice[i].nsym=0;
    /* this would suppress tie/repeat carryover from previous page: */
    /* voice[i].insert_btype=0; */
    /* voice[i].end_slur=0; */
  }

  ivc=0;

  word=0;
  carryover=0;
  last_note=last_real_note=-1;
  pplet=qplet=rplet=0;
  num_ending=0;
  mes1=mes2=0;
  prep_gchlst.clear();
  prep_deco.clear();
}

/* ----- add_text ---- */
void add_text (char *str, int type)
{
  if (do_mode!=DO_OUTPUT) return;
  if (ntext>=NTEXT) {
    wng ("No more room for text line <%s>", str);
    return;
  }
  strcpy (text[ntext], str);
  text_type[ntext]=type;
  ntext++;
}

/* ----- reset_info ---- */
void reset_info (struct ISTRUCT *inf)
{

  /* reset all info fields except info.xref */

  strcpy(inf->parts,  "");   
  strcpy(inf->area,   "");   
  strcpy(inf->book,   "");   
  inf->ncomp=0;
  strcpy(inf->disc,   "");   
  strcpy(inf->file,   "");  
  strcpy(inf->group,  "");  
  strcpy(inf->hist,   "");   
  strcpy(inf->info,   "");   
  strcpy(inf->key,    "C");    
  strcpy(inf->meter,  "none");
  strcpy(inf->notes,  "");  
  strcpy(inf->orig,   "");   
  strcpy(inf->rhyth,  "");  
  strcpy(inf->src,    "");    
  strcpy(inf->title,  "");  
  strcpy(inf->title2, "");  
  strcpy(inf->title3, "");  
  strcpy(inf->trans,  "");  
  strcpy(inf->tempo,  "");  
} 

/* ----- get_default_info: set info to default, except xref field --- */
void get_default_info (void)
{
  char savestr[STRLINFO];

  strcpy (savestr, info.xref);
  info=default_info;
  strcpy (info.xref, savestr);
  
}

/* ----- is_info_field: identify any type of info field ---- */
int is_info_field (char *str)
{
  if (strlen(str)<2) return 0;
  if (str[1]!=':')   return 0;
  if (str[0]=='|')   return 0;   /* |: at start of music line */
  return 1;
}

/* ----- is_end_line: identify eof ----- */
int is_end_line (const char *str)
{
  if (strlen(str)<3) return 0;
  if (str[0]=='E' && str[1]=='N' && str[2]=='D') return 1;
  return 0;
}

/* ----- is_pseudocomment ----- */
int is_pseudocomment (const char *str)
{
  if (strlen(str)<2) return 0;
  if ((str[0]=='%')&&(str[1]=='%'))  return 1;
  return 0;
}

/* ----- is_comment ----- */
int is_comment (const char *str)
{
  if (strlen(str)<1) return 0;
  if (str[0]=='%')  return 1;
  if (str[0]=='\\') return 1;
  return 0;
}

/* ----- is_cmdline: command line ----- */
int is_cmdline (const char *str)
{
  if (strlen(str)<2) return 0;
  if (str[0]=='%' && str[1]=='!')  return 1;
  return 0;
}

/* ----- find_voice ----- */
int find_voice (char *vid, int *newv)
{
  int i;
  
  for (i=0;i<nvoice;i++) 
    if (!strcmp(vid,voice[i].id)) {
      *newv=0;
      return i;
    }

  i=nvoice;
  if (i>=maxVc) {
    /*rxi("Too many voices; use -maxv to increase limit, now ",maxVc);*/
    realloc_structs(maxSyms,maxVc+allocVc);
  }

  strcpy(voice[i].id,    vid);
  strcpy(voice[i].name,  "");
  strcpy(voice[i].sname, "");
  voice[i].meter = default_meter;
  voice[i].key   = default_key;
  voice[i].stems    = 0;
  voice[i].staves = voice[i].brace = voice[i].bracket = 0;
  voice[i].do_gch   = 1;
  voice[i].sep      = 0.0;
  voice[i].nsym     = 0;
  voice[i].select   = 1;
  // new systems must start with invisible bar as anchor for barnumbers
  //voice[i].insert_btype = voice[i].insert_num = 0;
  //voice[i].insert_bnum = 0;
  voice[i].insert_btype = B_INVIS;
  voice[i].insert_num = 0;
  voice[i].insert_bnum = barinit;
  voice[i].insert_space = 0.0;
  voice[i].end_slur = 0;
  voice[i].timeinit = 0.0;
  strcpy(voice[i].insert_text, "");
  nvoice++;
  if (verbose>5) 
    printf ("Make new voice %d with id \"%s\"\n", i,voice[i].id); 
  *newv=1;
  return i;
  
}

/* ----- switch_voice: read spec for a voice, return voice number ----- */
int switch_voice (const char *str)
{
  int j,np,newv;
  const char *r;
  char *q;
  char t1[STRLINFO],t2[STRLINFO];

  if (!do_this_tune) return 0;

  j=-1;

  /* start loop over voice options: parse t1=t2 */
  r=str;
  np=newv=0;
  for (;;) {
    while (isspace(*r)) r++;
    if (*r=='\0') break;
    strcpy(t1,"");
    strcpy(t2,"");
    q=t1;
    while (!isspace(*r) && *r!='\0' && *r!='=') { *q=*r; r++; q++; }
    *q='\0';
    if (*r=='=') {
      r++;
      q=t2;
      if (*r=='"') {
        r++;
        while (*r!='"' && *r!='\0') { *q=*r; r++; q++; }
        if (*r=='"') r++;
      }
      else {
        while (!isspace(*r) && *r!='\0') { *q=*r; r++; q++; }
      }
      *q='\0';
    }
    np++;

    /* interpret the parsed option. First case is identifier. */
    if (np==1) {
      j=find_voice (t1,&newv);
    }

    else {                         /* interpret option */
      if (j<0) bug("j invalid in switch_voice",1);
      if      (!strcmp(t1,"name")    || !strcmp(t1,"nm"))   
        strcpy(voice[j].name,  t2);

      else if (!strcmp(t1,"sname")   || !strcmp(t1,"snm"))  
        strcpy(voice[j].sname, t2);

      else if (!strcmp(t1,"staves")  || !strcmp(t1,"stv"))  
        voice[j].staves  = atoi(t2);

      else if (!strcmp(t1,"brace")   || !strcmp(t1,"brc")) 
        voice[j].brace   = atoi(t2);

      else if (!strcmp(t1,"bracket") || !strcmp(t1,"brk")) 
        voice[j].bracket = atoi(t2);

      else if (!strcmp(t1,"gchords") || !strcmp(t1,"gch"))
        g_logv (str,t2,&voice[j].do_gch);

      /* for sspace: add 2000 as flag if not incremental */ 
      else if (!strcmp(t1,"space")   || !strcmp(t1,"spc")) {  
        g_unum (str,t2,&voice[j].sep);
        if (t2[0]!='+' && t2[0]!='-') voice[j].sep += 2000.0;
      }
      
      else if (!strcmp(t1,"clef")    || !strcmp(t1,"cl")) {
        if (!set_clef(t2,&voice[j].key))
          wng("Unknown clef in voice spec: ",t2);
      }
      else if (!strcmp(t1,"stems") || !strcmp(t1,"stm")) {
        if      (!strcmp(t2,"up"))    voice[j].stems=1;
        else if (!strcmp(t2,"down"))  voice[j].stems=-1;
        else if (!strcmp(t2,"free"))  voice[j].stems=0;
        else wng("Unknown stem setting in voice spec: ",t2);
      }
      else if (!strcmp(t1,"octave")) {
        ; /* ignore abc2midi parameter for compatibility */
      }
      else wng("Unknown option in voice spec: ",t1);
    }
    
  }

  /* if new voice was initialized, save settings im meter0, key0 */
  if (newv) {
    voice[j].meter0 = voice[j].meter;
    voice[j].key0   = voice[j].key;
  }

  if (verbose>7) 
    printf ("Switch to voice %d  <%s> <%s> <%s>  clef=%d\n", 
            j,voice[j].id,voice[j].name,voice[j].sname, 
            voice[j].key.ktype); 

  nsym0=voice[j].nsym;  /* set nsym0 to decide about eoln later.. ugly */
  return j;

} 


/* ----- info_field: identify info line, store in proper place  ---- */
/* switch within_block: either goes to default_info or info.
   Only xref ALWAYS goes to info. */
int info_field (char *str)
{
  char s[STRLINFO];
  struct ISTRUCT *inf;
  int i;

  if (within_block) {
    inf=&info;
  }
  else {
    inf=&default_info;
  }

  if (strlen(str)<2) return 0;
  if (str[1]!=':')   return 0;
  if (str[0]=='|')   return 0;   /* |: at start of music line */

  for (i=0;i<strlen(str);i++) if (str[i]=='%') str[i]='\0';

  /* info fields must not be longer than STRLINFO characters */
  strnzcpy(s,str,STRLINFO);
  
  if (s[0]=='X') {
    strip (info.xref,   &s[2]);
    xrefnum=get_xref(info.xref);
    return XREF;
  }

  else if (s[0]=='A') strip (inf->area,   &s[2]);
  else if (s[0]=='B') strip (inf->book,   &s[2]);
  else if (s[0]=='C') {
    if (inf->ncomp>=NCOMP) 
      wng("Too many composer lines","");
    else {
      strip (inf->comp[inf->ncomp],&s[2]);
      inf->ncomp++;
    }
  }
  else if (s[0]=='D') {
    strip (inf->disc,   &s[2]);
    add_text (&s[2], TEXT_D);
  }

  else if (s[0]=='F') strip (inf->file,   &s[2]);
  else if (s[0]=='G') strip (inf->group,  &s[2]);
  else if (s[0]=='H') {
    strip (inf->hist,   &s[2]);
    add_text (&s[2], TEXT_H);
    return HISTORY;
  }
  else if (s[0]=='W') {
    add_text (&s[2], TEXT_W);
    return WORDS;
  }
  else if (s[0]=='I') strip (inf->info,   &s[2]);
  else if (s[0]=='K') {
    strip (inf->key,    &s[2]);
    return KEY;
  }
  else if (s[0]=='L') {
    strip (inf->len,    &s[2]);
    return DLEN;
  }
  else if (s[0]=='M') {
    strip (inf->meter,  &s[2]);
    return METER;
  }
  else if (s[0]=='N') {
    strip (inf->notes,  &s[2]);
    add_text (&s[2], TEXT_N);
  }
  else if (s[0]=='O') strip (inf->orig,   &s[2]);
  else if (s[0]=='R') strip (inf->rhyth,  &s[2]);
  else if (s[0]=='P') {
    strip (inf->parts,  &s[2]);
    return PARTS;
  }
  else if (s[0]=='S') strip (inf->src,    &s[2]);
  else if (s[0]=='T') {
    //strip (t, &s[2]);
    numtitle++;
    if (numtitle>3) numtitle=3;
    if (numtitle==1)      strip (inf->title,  &s[2]);
    else if (numtitle==2) strip (inf->title2, &s[2]);
    else if (numtitle==3) strip (inf->title3, &s[2]);
    return TITLE;
  }
  else if (s[0]=='V') {
    strip (lvoiceid,  &s[2]);
    if (!*lvoiceid) {
      syntax("missing voice specifier",p);
      return 0;
    }
    return VOICE;
  }
  else if (s[0]=='Z') {
    strip (inf->trans,  &s[2]);
    add_text (&s[2], TEXT_Z);
  }
  else if (s[0]=='Q') {
    strip (inf->tempo,  &s[2]);
    return TEMPO;
  }

  else if (s[0]=='E') ;
  
  else {
    return 0;
  }

  return INFO;
}

/* ----- append_meter: add meter to list of symbols -------- */
/*
 * Warning: only called for inline fields
 * normal meter symbols are added in set_initsyms
 */
void append_meter (const struct METERSTR* meter)
{
  int kk;

  //must not be ignored because we need meter for counting bars!
  //if (meter->display==0) return;

  kk=add_sym(TIMESIG);
  symv[ivc][kk]=zsym;
  symv[ivc][kk].gchords= new GchordList();
  symv[ivc][kk].type = TIMESIG;
  if (meter->display==2) {
    symv[ivc][kk].u    = meter->dismeter1;
    symv[ivc][kk].v    = meter->dismeter2;
    symv[ivc][kk].w    = meter->dismflag;
    strcpy(symv[ivc][kk].text,meter->distop);
  } else {
    symv[ivc][kk].u    = meter->meter1;
    symv[ivc][kk].v    = meter->meter2;
    symv[ivc][kk].w    = meter->mflag;
    strcpy(symv[ivc][kk].text,meter->top);
  }
  if (meter->display==0) symv[ivc][kk].invis=1;
}

/* ----- append_key_change: append change of key to sym list ------ */
void append_key_change(struct KEYSTR oldkey, struct KEYSTR newkey)
{
  int n1,n2,t1,t2,kk;

  n1=oldkey.sf;    
  t1=A_SH;
  if (n1<0) { n1=-n1; t1=A_FT; }
  n2=newkey.sf;    
  t2=A_SH;                    

  if (newkey.ktype != oldkey.ktype) {      /* clef change */
    kk=add_sym(CLEF);
    symv[ivc][kk].u=newkey.ktype;
    symv[ivc][kk].v=1;
  }

  if (n2<0) { n2=-n2; t2=A_FT; }
  if (t1==t2) {              /* here if old and new have same type */
    if (n2>n1) {                 /* more new symbols ..*/
      kk=add_sym(KEYSIG);        /* draw all of them */
      symv[ivc][kk].u=1;
      symv[ivc][kk].v=n2;
      symv[ivc][kk].w=100;
      symv[ivc][kk].t=t1;
    }
    else if (n2<n1) {            /* less new symbols .. */
      kk=add_sym(KEYSIG);          /* draw all new symbols and neutrals */
      symv[ivc][kk].u=1;
      symv[ivc][kk].v=n1;
      symv[ivc][kk].w=n2+1;
      symv[ivc][kk].t=t2;
    }
    else return;
  }
  else {                     /* here for change s->f or f->s */
    kk=add_sym(KEYSIG);          /* neutralize all old symbols */
    symv[ivc][kk].u=1;
    symv[ivc][kk].v=n1;
    symv[ivc][kk].w=1;
    symv[ivc][kk].t=t1;
    kk=add_sym(KEYSIG);          /* add all new symbols */
    symv[ivc][kk].u=1;
    symv[ivc][kk].v=n2;
    symv[ivc][kk].w=100;
    symv[ivc][kk].t=t2;
  }

}



/* ----- numeric_pitch ------ */
/* adapted from abc2mtex by Chris Walshaw */
int numeric_pitch(char note)
{

  if (note=='z') 
    return 14;
  if (note >= 'C' && note <= 'G')
    return(note-'C'+16+voice[ivc].key.add_pitch);
  else if (note >= 'A' && note <= 'B')
    return(note-'A'+21+voice[ivc].key.add_pitch);
  else if (note >= 'c' && note <= 'g')
    return(note-'c'+23+voice[ivc].key.add_pitch);
  else if (note >= 'a' && note <= 'b')
    return(note-'a'+28+voice[ivc].key.add_pitch);
  printf ("numeric_pitch: cannot identify <%c>\n", note);
  return(0);
}

/* ----- symbolic_pitch: translate numeric pitch back to symbol ------ */
int symbolic_pitch(int pit, char *str)
{
  int  p,r,s;
  char ltab1[7] = {'C','D','E','F','G','A','B'};
  char ltab2[7] = {'c','d','e','f','g','a','b'};

  p=pit-16;
  r=(p+700)%7;
  s=(p-r)/7;

  if (p<7) {
    sprintf (str,"%c,,,,,",ltab1[r]);
    str[1-s]='\0';
  }
  else {
    sprintf (str,"%c'''''",ltab2[r]);
    str[s]='\0';
  }
  return 0;
}

/* ----- handle_inside_field: act on info field inside body of tune --- */
void handle_inside_field(int type)
{
  struct KEYSTR oldkey;
  int rc;

  if (type==METER) {
    if (nvoice==0) ivc=switch_voice (DEFVOICE);
    set_meter (info.meter,&voice[ivc].meter);
    append_meter (&(voice[ivc].meter));
  }
  
  else if (type==DLEN) {
    if (nvoice==0) ivc=switch_voice (DEFVOICE);
    set_dlen (info.len,  &voice[ivc].meter);
  }
  
  else if (type==KEY) {
    if (nvoice==0) ivc=switch_voice (DEFVOICE);
    oldkey=voice[ivc].key;
    rc=set_keysig(info.key,&voice[ivc].key,0);
    if (rc) set_transtab (halftones,&voice[ivc].key);
    append_key_change(oldkey,voice[ivc].key);
  }
  
  else if (type==VOICE) {
    ivc=switch_voice (lvoiceid);
  }

}



/* ----- parse_uint: parse for unsigned integer ----- */
int parse_uint (void)
{
  int number,ndig;
  char num[21];
  
  if (!isdigit(*p)) return 0;
  ndig=0;
  while (isdigit(*p)) {                
    num[ndig]=*p; 
    ndig++; 
    num[ndig]=0;
    p++;
  }
  sscanf (num, "%d", &number);
  if (db>3) printf ("  parsed unsigned int %d\n", number);
  return number;

}
  
/* ----- parse_bar: parse for some kind of bar ---- */
int parse_bar (void)
{
  int k;
  GchordList::iterator ii;

  /* special cases: [1 or [2 without a preceeding bar, [| */
  if (*p=='[') {
    if (strchr("123456789",*(p+1))) {
      k=add_sym (BAR);
      symv[ivc][k].u=B_INVIS;
      symv[ivc][k].v=chartoi(*(p+1));
      p=p+2;
      return 1;
    }
  }
  
  /* identify valid standard bar types */
  if (*p == '|') {
    p++;
    if (*p == '|') { 
      k=add_sym (BAR);
      symv[ivc][k].u=B_DBL;
      p++;
    }
    else if (*p == ':') { 
      k=add_sym(BAR);
      symv[ivc][k].u=B_LREP;
      p++; 
    }
    else if (*p==']') {                  /* code |] for fat end bar */
      k=add_sym(BAR);
      symv[ivc][k].u=B_FAT2;
      p=p+1;
    }
    else {
      k=add_sym(BAR);
      symv[ivc][k].u=B_SNGL;
    }
  }
  else if (*p == ':') {
    if (*(p+1) == '|') { 
      k=add_sym(BAR);
      symv[ivc][k].u=B_RREP;
      p+=2;
    }
    else if (*(p+1) == ':') {
      k=add_sym(BAR);
      symv[ivc][k].u=B_DREP;
      p+=2; }
    else {
      /* ':' is decoration in tablature
	   *syntax ("Syntax error parsing bar", p-1);
	   */
      return 0;
    }
  }

  else if ((*p=='[') && (*(p+1)=='|') && (*(p+2)==']')) {  /* code [|] invis */
    k=add_sym(BAR);
    symv[ivc][k].u=B_INVIS;
    p=p+3;
  }

  else if ((*p=='[') && (*(p+1)=='|')) {    /* code [| for thick-thin bar */
    k=add_sym(BAR);
    symv[ivc][k].u=B_FAT1;
    p=p+2;
  }

  else return 0;

  /* copy over preparsed stuff (gchords, decos) */
  strcpy(symv[ivc][k].text,"");
  if (!prep_gchlst.empty()) {
    for (ii=prep_gchlst.begin(); ii!=prep_gchlst.end(); ii++) {
      symv[ivc][k].gchords->push_back(*ii);
    }
    prep_gchlst.clear();
  }
  if (prep_deco.n) {
    for (int i=0; i<prep_deco.n; i++) {
      int deco=prep_deco.t[i];
      if ((deco!=D_STACC) && (deco!=D_SLIDE))
        symv[ivc][k].dc.add(deco);
    }
    prep_deco.clear();
  }

  /* see if valid bar is followed by specifier for first or second ending */
  if (strchr("123456789",*p)) {
    symv[ivc][k].v=chartoi(*p); p++;
  } else if ((*p=='[') && (strchr("123456789",*(p+1)))) {
    symv[ivc][k].v=chartoi(*(p+1)); p=p+2;
  } else if ((*p==' ') && (*(p+1)=='[') && (strchr("123456789",*(p+2)))) {
    symv[ivc][k].v=chartoi(*(p+2)); p=p+3;
  }

  return 1;
}
  
/* ----- parse_space: parse for whitespace ---- */
int parse_space (void)
{
  int rc;

  rc=0;
  while ((*p==' ')||(*p=='\t')) {
    rc=1;
    p++;
  }
  if (db>3) if (rc) printf ("  parsed whitespace\n"); 
  return rc;
}

/* ----- parse_esc: parse for escape sequence ----- */
int parse_esc (void)
{
  
  int nseq;
  char *pp;
  
  if (*p == '\\') {                     /* try for \...\ sequence */
    p++;
    nseq=0;
    while ((*p!='\\') && (*p!=0)) {
      escseq[nseq]=*p;
      nseq++;
      p++;
    }
    if (*p == '\\') {
      p++;
      escseq[nseq]=0;
      if (db>3) printf ("  parsed esc sequence <%s>\n", escseq);  
      return ESCSEQ;
    }
    else {
      if (cfmt.breakall) return DUMMY;
      if (db>3) printf ("  parsed esc to EOL.. continuation\n");  
    }
    return CONTINUE;
  }

  /* next, try for [..] sequence */
  if ((*p=='[') && (*(p+1)>='A') && (*(p+1)<='Z') && (*(p+2)==':')) {  
    pp=p;
    p++;
    nseq=0;
    while ((*p!=']') && (*p!=0)) {
      escseq[nseq]=*p;
      nseq++;
      p++;
    }           
    if (*p == ']') {
      p++;
      escseq[nseq]=0;
      if (db>3) printf ("  parsed esc sequence <%s>\n", escseq);  
      return ESCSEQ;
    }
    syntax ("Escape sequence [..] not closed", pp);
    return ESCSEQ;
  }
  return 0;
}


/* ----- parse_nl: parse for newline ----- */
int parse_nl (void)
{
  
  if ((*p == '\\')&&(*(p+1)=='\\')) {
    p+=2;
    return 1;
  }
  else
    return 0;
}

/* ----- parse_gchord: parse guitar chord, add to global prep_gchlst ----- */
int parse_gchord ()
{
  char *q;
  int n=0;
  Gchord gchnew;

  if (*p != '"') return 0;
  
  q=p;
  p++;
  //n=strlen(gch);
  //if (n > 0) syntax ("Overwrite unused guitar chord", q);
  
  while ((*p != '"') && (*p != 0)) {
    gchnew.text += *p;
    n++;
    if (n >= 200) {
      syntax ("String for guitar chord too long", q);
      return 1;
    }
    p++;
  }
  if (*p == 0) {
    syntax ("EOL reached while parsing guitar chord", q);
    return 1;
  }
  p++;
  if (db>3) printf("  parse guitar chord <%s>\n", gchnew.text.c_str());
  gch_transpose(&gchnew.text, voice[ivc].key);
  prep_gchlst.push_back(gchnew);

/*|   gch_transpose (voice[ivc].key); |*/

  return 1;
}


/* ----- parse_deco: parse for decoration, add to global prep_deco ----- */
int parse_deco ()
{
  int deco,n;
  /* mapping abc code to decorations */
  /* for no abbreviation, set abbrev=0; for no !..! set fullname="" */
  struct s_deconame { int index; char abbrev; const char* fullname; };
  static struct s_deconame deconame[] = {
    {D_GRACE,   '~', "!grace!"},
    {D_STACC,   '.', "!staccato!"},
    {D_SLIDE,   'J', "!slide!"},
    {D_TENUTO,  'N', "!tenuto!"},
    {D_HOLD,    'H', "!fermata!"},
    {D_ROLL,    'R', "!roll!"},
    {D_TRILL,   'T', "!trill!"},
    {D_UPBOW,   'u', "!upbow!"},
    {D_DOWNBOW, 'v', "!downbow!"},
    {D_HAT,     'K', "!sforzando!"},
    {D_ATT,     'k', "!accent!"},
    {D_ATT,     'L', "!emphasis!"}, /*for abc standard 1.7.6 compliance*/
    {D_SEGNO,   'S', "!segno!"},
    {D_CODA,    'O', "!coda!"},
    {D_PRALLER, 'P', "!pralltriller!"},
    {D_PRALLER, 'P', "!uppermordent!"}, /*for abc standard 1.7.6 compliance*/
    {D_MORDENT, 'M', "!mordent!"},
    {D_MORDENT, 'M', "!lowermordent!"}, /*for abc standard 1.7.6 compliance*/
    {D_TURN,     0,  "!turn!"},
    {D_PLUS,     0,  "!plus!"},
    {D_PLUS,     0,  "!+!"}, /*for abc standard 1.7.6 compliance*/
    {D_CROSS,   'X', "!x!"},
    {D_DYN_PP,   0,  "!pp!"},
    {D_DYN_P,    0,  "!p!"},
    {D_DYN_MP,   0,  "!mp!"},
    {D_DYN_MF,   0,  "!mf!"},
    {D_DYN_F,    0,  "!f!"},
    {D_DYN_FF,   0,  "!ff!"},
    {D_DYN_SF,   0,  "!sf!"},
    {D_DYN_SFZ,  0,  "!sfz!"},
    {D_BREATH,   0,  "!breath!"},
    {D_WEDGE,    0,  "!wedge!" },
    {D_NOSTEM,   0,  "!nostem!" },
    {D_DIAMOND,  0,  "!diamond!" },
    {D_ARPEGGIO, 0,  "!arpeggio!" },
    {0, 0, ""} /*end marker*/
  };
  struct s_deconame* dn;

  n=0;

  for (;;) {
    deco=0;

    /*check for fullname deco*/
    if (*p == '!') {
      int slen;
      char* q;
      q = strchr(p+1,'!');
      if (!q) {
        syntax ("Deco sign '!' not closed",p);
        p++;
        return n;
      } else {
        slen = q+1-p;
        /*lookup in table*/
        for (dn=deconame; dn->index; dn++) {
          if (0 == strncmp(p,dn->fullname,slen)) {
            deco = dn->index;
            break;
          }
        }
        if (!deco) syntax("Unknown decoration",p+1);
        p += slen;
      }
    }
    /*check for abbrev deco*/
    else {
      for (dn=deconame; dn->index; dn++) {
        if (dn->abbrev && (*p == dn->abbrev)) {
          deco = dn->index;
          p++;
          break;
        }
      }
    }

    if (deco) {
      prep_deco.add(deco);
      n++;
    }
    else 
      break;
  }

  return n;
}


/* ----- parse_length: parse length specifer for note or rest --- */
int parse_length (void)
{
  int len,fac;

  len=voice[ivc].meter.dlen;          /* start with default length */

  if (len<=0) {
    syntax("got len<=0 from current voice", p);
    printf("Cannot proceed without default length. Emergency stop.\n");
    exit(1);
  }

  if (isdigit(*p)) {                 /* multiply note length */
    fac=parse_uint ();
    if (fac==0) fac=1;
    len *= fac;
  }

  if (*p=='/') {                   /* divide note length */
    while (*p=='/') {
      p++;
      if (isdigit(*p)) 
        fac=parse_uint();
      else 
        fac=2;
      if (len%fac) {
        syntax ("Bad length divisor", p-1);
        return len; 
      }
      len=len/fac;
    }
  }
  
  return len;
}

/* ----- parse_brestnum parses the number of measures on a brest */
int parse_brestnum (void)
{
  int fac,len;
  len=1;
  if (isdigit(*p)) {                 /* multiply note length */
    fac=parse_uint ();
    if (fac==0) fac=1;
    len *= fac;
  }
  return len;
}

/* ----- parse_grace_sequence --------- */
/*
 * result is stored in arguments
 * when no grace sequence => arguments are not altered
 */
int parse_grace_sequence (int *pgr, int *agr, int* len)
{
  char *p0;
  int n;
  
  p0=p;
  if (*p != '{') return 0;
  p++;
  
  *len = 0;   /* default is no length => accacciatura */
  n=0;
  while (*p != '}') {
    if (*p == '\0') {
      syntax ("Unbalanced grace note sequence", p0);
      return 0;
    }
    if (!isnote(*p)) {
      syntax ("Unexpected symbol in grace note sequence", p);
      p++;
    }
    if (n >= MAXGRACE) {
      p++; continue;
    }
    agr[n]=0;  
    if (*p == '=') agr[n]=A_NT;
    if (*p == '^') {
      if (*(p+1)=='^') { agr[n]=A_DS; p++; }
      else agr[n]=A_SH;
    }
    if (*p == '_') {
      if (*(p+1)=='_') { agr[n]=A_DF; p++; }
      else agr[n]=A_FT;
    }
    if (agr[n]) p++;

    pgr[n] = numeric_pitch(*p);
    p++;
    while (*p == '\'') { pgr[n] += 7; p++; }
    while (*p == ',') {  pgr[n] -= 7; p++; }

    do_transpose (voice[ivc].key, &pgr[n], &agr[n]);

    /* parse_length() returns default length when no length specified */
    /* => we may only call it when explicit length specified */
    if (*p == '/' || isdigit(*p))
      *len=parse_length ();
    n++;
  }
  
  p++;
  return n;
}

/* ----- identify_note: set head type, dots, flags for note --- */
void identify_note (struct SYMBOL *s, char *q)
{
  int head,base,len,flags,dots;

  if (s->len==0) s->len=s->lens[0];
  len=s->len;

  /* set flag if duration equals (or gretaer) length of one measure */
  if (nvoice>0) {
    if (len>=(WHOLE*voice[ivc].meter.meter1)/voice[ivc].meter.meter2)
      s->fullmes=1;
  }

  base=LONGA;
  if (len>=LONGA)              base=LONGA;
  else if (len>=BREVIS)        base=BREVIS;
  else if (len>=WHOLE)         base=WHOLE;
  else if (len>=HALF)          base=HALF;
  else if (len>=QUARTER)       base=QUARTER;
  else if (len>=EIGHTH)        base=EIGHTH;
  else if (len>=SIXTEENTH)     base=SIXTEENTH;
  else if (len>=THIRTYSECOND)  base=THIRTYSECOND;
  else if (len>=SIXTYFOURTH)   base=SIXTYFOURTH;
  else syntax("Cannot identify head for note",q);
      
  if (base>=WHOLE)     head=H_OVAL;
  else if (base==HALF) head=H_EMPTY;
  else                 head=H_FULL;
      
  if (base==SIXTYFOURTH)        flags=4;
  else if (base==THIRTYSECOND)  flags=3;
  else if (base==SIXTEENTH)     flags=2;
  else if (base==EIGHTH)        flags=1;
  else                          flags=0;
  
  dots=0;
  if (len==base)            dots=0;
  else if (2*len==3*base)   dots=1;
  else if (4*len==7*base)   dots=2;
  else if (8*len==15*base)  dots=3;
  else syntax("Cannot handle note length for note",q);
  
/*  printf ("identify_note: length %d gives head %d, dots %d, flags %d\n",
          len,head,dots,flags);  */

  s->head=head;
  s->dots=dots;
  s->flags=flags;
}


/* ----- double_note: change note length for > or < char --- */
/* Note: if symv[ivc][i] is a chord, the length shifted to the following
   note is taken from the first note head. Problem: the crazy syntax 
   permits different lengths within a chord. */
void double_note (int i, int num, int sign, char *q)
{
  int m,shift,j,len;

  if ((symv[ivc][i].type!=NOTE) && (symv[ivc][i].type!=REST)) 
    bug("sym is not NOTE or REST in double_note", 1);
  
  shift=0;
  len=symv[ivc][i].lens[0];
  for (j=0;j<num;j++) {
    len=len/2;
    shift -= sign*len;
    symv[ivc][i].len += sign*len;
    for (m=0;m<symv[ivc][i].npitch;m++) symv[ivc][i].lens[m] += sign*len;
  }
  identify_note (&symv[ivc][i],q);
  carryover += shift;
}

/* ----- parse_basic_note: parse note or rest with pitch and length --*/
int parse_basic_note (int *pitch, int *length, int *accidental)
{
  int pit,len,acc;
  
  acc=pit=0;                       /* look for accidental sign */
  if (*p == '=') acc=A_NT;
  if (*p == '^') {
    if (*(p+1)=='^') { acc=A_DS; p++; }
    else acc=A_SH;
  }
  if (*p == '_') {
    if (*(p+1)=='_') { acc=A_DF; p++; }
    else acc=A_FT;
  }

  if (acc) {
    p++;
    if (!strchr("CDEFGABcdefgab",*p)) {
      syntax("Missing note after accidental", p-1);
      return 0;
    }
  }
  if (!isnote(*p)) {
    syntax ("Expecting note", p);
    p++;
    return 0;
  }
  
  pit= numeric_pitch(*p);             /* basic pitch */
  p++;
  
  while (*p == '\'') {                /* eat up following ' chars */
    pit += 7;
    p++;
  }
  
  while (*p == ',') {                 /* eat up following , chars */
    pit -= 7;
    p++;
  }
  
  len=parse_length();

  do_transpose (voice[ivc].key, &pit, &acc);

  *pitch=pit;
  *length=len;
  *accidental=acc;

  if (db>3) printf ("  parsed basic note,"
                    "length %d/%d = 1/%d, pitch %d\n", 
                    len,BASE,BASE/len,pit);
  
  return 1;
  
}


/* ----- parse_note: parse for one note or rest with all trimmings --- */
int parse_note (void)
{
  int k,deco,i,chord,m,type,rc,sl1,sl2,j,n;
  int pitch,length,accidental,invis;
  char *q,*q0;
  GchordList::iterator ii;
  /*grace sequence must be remembered in static variables,*/
  /*because otherwise it is lost after slurs*/
  static int ngr = 0;
  static int pgr[MAXGRACE],agr[MAXGRACE],lgr;
  
  n=parse_grace_sequence(pgr,agr,&lgr);   /* grace notes */
  if (n) ngr = n;

  parse_gchord();               /* permit chord after graces */
  
  deco=parse_deco();              /* decorations */

  parse_gchord();               /* permit chord after deco */

  chord=0;                             /* determine if chord */
  q=p;
  if ((*p=='+') || (*p=='[')) { chord=1; p++; }

  type=invis=0;
  parse_deco(); /* allow for decos within chord */
  if (isnote(*p)) type=NOTE;
  if (chord && (*p=='(')) type=NOTE;
  if (chord && (*p==')')) type=NOTE;   /* this just for better error msg */
  if (*p=='z') type=REST;
  if (*p=='Z') type=BREST;
  if ((*p=='x')||(*p=='X')) {type=REST; invis=1; }
  if (!type) return 0;

  k=add_sym(type);                     /* add new symbol to list */


  symv[ivc][k].dc.n=prep_deco.n;       /* copy over pre-parsed stuff */
  for (i=0;i<prep_deco.n;i++) 
    symv[ivc][k].dc.t[i]=prep_deco.t[i];
  prep_deco.clear();
  if (ngr) {
    symv[ivc][k].gr.n=ngr;
    symv[ivc][k].gr.len=lgr;
    for (i=0;i<ngr;i++) {
      symv[ivc][k].gr.p[i]=pgr[i];
      symv[ivc][k].gr.a[i]=agr[i];
    }
    ngr = 0;
  } else {
    symv[ivc][k].gr.n=0;
    symv[ivc][k].gr.len=0;
  }

  if (!prep_gchlst.empty()) {
    /*gch_transpose (voice[ivc].key);*/
    for (ii=prep_gchlst.begin(); ii!=prep_gchlst.end(); ii++) {
      symv[ivc][k].gchords->push_back(*ii);
    }
    prep_gchlst.clear();
  }

  q0=p;
  if (type==REST) {
    p++;
    symv[ivc][k].lens[0] = parse_length();
    symv[ivc][k].npitch=1;
    symv[ivc][k].invis=invis;
    if (db>3) printf ("  parsed rest, length %d/%d = 1/%d\n", 
                      symv[ivc][k].lens[0],BASE,BASE/symv[ivc][k].lens[0]); 
  }
  else if (type==BREST) {
    p++;
    symv[ivc][k].lens[0] = (WHOLE*voice[ivc].meter.meter1)/voice[ivc].meter.meter2;
    symv[ivc][k].len = symv[ivc][k].lens[0];
    symv[ivc][k].fullmes = parse_brestnum();
    symv[ivc][k].npitch=1;
  }
  else {
    m=0;                                 /* get pitch and length */
    sl1=sl2=0;
    for (;;) {
      if (chord && (*p=='(')) {
        sl1++;
        symv[ivc][k].sl1[m]=sl1;
        p++;
      }
      if ((deco=parse_deco())) {     /* for extra decorations within chord */
        for (i=0;i<deco;i++) symv[ivc][k].dc.add(prep_deco.t[i]);
        prep_deco.clear();
      }

      rc=parse_basic_note (&pitch,&length,&accidental);
      if (rc==0) { voice[ivc].nsym--; return 0; }
      symv[ivc][k].pits[m] = pitch;
      symv[ivc][k].lens[m] = length;
      symv[ivc][k].accs[m] = accidental;
      symv[ivc][k].ti1[m]  = symv[ivc][k].ti2[m] = 0;
      for (j=0;j<ntinext;j++) 
        if (tinext[j]==symv[ivc][k].pits[m]) symv[ivc][k].ti2[m]=1;

      if (chord && (*p=='-')) {symv[ivc][k].ti1[m]=1; p++;}

      if (chord && (*p==')')) {
        sl2++;
        symv[ivc][k].sl2[m]=sl2;
        p++;
      }

      if (chord && (*p=='-')) {symv[ivc][k].ti1[m]=1; p++;}

      m++;

      if (!chord) break;
      if ((*p=='+')||(*p==']')) {
        p++;
        break;
      }
      if (*p=='\0') {
        if (chord) syntax ("Chord not closed", q); 
        return type;
      }
    }
    ntinext=0;
    for (j=0;j<m;j++)
      if (symv[ivc][k].ti1[j]) {
        tinext[ntinext]=symv[ivc][k].pits[j];
        ntinext++;
      }
    symv[ivc][k].npitch=m;
    if (m>0) {
      symv[ivc][k].grcpit = symv[ivc][k].pits[0];
    }
  }

  for (m=0;m<symv[ivc][k].npitch;m++) {   /* add carryover from > or < */
    if (symv[ivc][k].lens[m]+carryover<=0) {
      syntax("> leads to zero or negative note length",q0);
    }
    else
      symv[ivc][k].lens[m] += carryover;
  }
  carryover=0;

  if (db>3) printf ("  parsed note, decos %d, text <%s>\n",
                    symv[ivc][k].dc.n, symv[ivc][k].text);


  symv[ivc][k].yadd=0;
  if (voice[ivc].key.ktype==BASS)         symv[ivc][k].yadd=-6;
  if (voice[ivc].key.ktype==ALTO)         symv[ivc][k].yadd=-3;
  if (voice[ivc].key.ktype==TENOR)        symv[ivc][k].yadd=+3;
  if (voice[ivc].key.ktype==SOPRANO)      symv[ivc][k].yadd=+6;
  if (voice[ivc].key.ktype==MEZZOSOPRANO) symv[ivc][k].yadd=-9;
  if (voice[ivc].key.ktype==BARITONE)     symv[ivc][k].yadd=-12;
  if (voice[ivc].key.ktype==VARBARITONE)  symv[ivc][k].yadd=-12;
  if (voice[ivc].key.ktype==SUBBASS)      symv[ivc][k].yadd=0;
  if (voice[ivc].key.ktype==FRENCHVIOLIN) symv[ivc][k].yadd=-6;

  if (type!=BREST)
    identify_note (&symv[ivc][k],q0); 
  return type;
}


/* ----- parse_sym: parse a symbol and return its type -------- */
int parse_sym (void)
{
  int i;

  if (parse_gchord())   return GCHORD;
  if (parse_deco())     return DECO;
  if (parse_bar())      return BAR;
  if (parse_space())    return SPACE;
  if (parse_nl())       return NEWLINE;
  if ((i=parse_esc()))  return i;
  if ((i=parse_note())) return i;

  return 0;
}

/* ----- add_wd ----- */
char *add_wd(char *str)
{
  char *rp;
  int l;

  l=strlen(str);
  if (l==0) return 0;
  if (nwpool+l+1>maxNwpool) {
    //rx ("Overflow while parsing vocals; increase maxNwpool and recompile.","");
    wpool = (char *)zrealloc(wpool,maxNwpool,maxNwpool+allocNwpool,sizeof(char));
    if (wpool==NULL) rx("Out of memory","");
    maxNwpool += allocNwpool;
  }
  
  strcpy(wpool+nwpool, str);
  rp=wpool+nwpool;
  nwpool=nwpool+l+1;
  return rp;
}

/* ----- parse_vocals: parse words below a line of music ----- */
/* Use '^' to mark a '-' between syllables - hope nobody needs '^' ! */
int parse_vocals (char *line)
{
  int isym;
  char *c,*c1,*w;
  char word[81];

  if ((line[0]!='w') || (line[1]!=':')) return 0;
  p0=line;

  /* increase vocal line counter in first symbol of current line */
  symv[ivc][nsym0].wlines++;

  isym=nsym0-1;
  c=line+2;
  for (;;) {
    while(*c==' ') c++;
    if (*c=='\0') break;
    c1=c;
    if ((*c=='_') || (*c=='*') || (*c=='|') || (*c=='-')) {
      word[0]=*c; 
      if (*c=='-') word[0]='^';
      word[1]='\0';
      c++;
    }
    else {
      w=word;
      *w='\0';
      while ((*c!=' ') && (*c!='\0')) { 
        if ((*c=='_') || (*c=='*') || (*c=='|')) break;
        if (*c=='-') {
          if (*(c-1) != '\\') break;
          w--;
          *w='-';
        }
        *w=*c; w++; c++; 
      }
      if (*c=='-') { *w='^' ; w++; c++; }
      *w='\0';
    }

    /* now word contains a word, possibly with trailing '^',
       or one of the special characters * | _ -               */

    if (!strcmp(word,"|")) {               /* skip forward to next bar */
      isym++;
      while ((symv[ivc][isym].type!=BAR) && (isym<voice[ivc].nsym)) isym++; 
      if (isym>=voice[ivc].nsym) 
        { syntax("Not enough bar lines for |",c1); break; }
    } 
    
    else {                                 /* store word in next note */
      w=word;
      while (*w!='\0') {                   /* replace * and ~ by space */
		/* cd: escaping with backslash possible */
        /* (does however not yet work for '*') */
        if ( ((*w=='*') || (*w=='~')) && !(w>word && *(w-1)=='\\') ) *w=' ';
        w++;
      }
      isym++;
      while ((symv[ivc][isym].type!=NOTE) && (isym<voice[ivc].nsym)) isym++; 
      if (isym>=voice[ivc].nsym) 
        { syntax ("Not enough notes for words",c1); break; }
      symv[ivc][isym].wordp[nwline]=add_wd(word);
    }

    if (*c=='\0') break;
  }

  nwline++;
  return 1;
}


/* ----- parse_music_line: parse a music line into symbols ----- */
int parse_music_line (char *line)
{
  int type,num,nbr,n,itype,i;
  char msg[81];
  char *p1,*pmx;

  if (ivc>=nvoice) bug ("Trying to parse undefined voice",1);

  nwline=0;
  type=0;
  nsym0=voice[ivc].nsym;

  nbr=0;
  p1=p=p0=line;
  pmx=p+strlen(p);

  while ((p <= pmx) && (*p != 0)) {
    type=parse_sym();
    n=voice[ivc].nsym;
    i=n-1;
    if ((db>4) && type)  
      printf ("   sym[%d] code (%d,%d)\n",
              n-1,symv[ivc][n-1].type,symv[ivc][n-1].u);
    
    if (type==NEWLINE) {
      if ((n>0) && !cfmt.continueall && !cfmt.barsperstaff) {
        symv[ivc][i].eoln=1;
        if (word) {
          symv[ivc][last_note].word_end=1;
          word=0;
        }
      }
    }
    
    if (type==ESCSEQ) {
      if (db>3)  
        printf ("Handle escape sequence <%s>\n", escseq); 
      itype=info_field (escseq);
      handle_inside_field (itype);
    }
    
    if (type==REST) {
      if (pplet) {                   /* n-plet can start on rest */
        symv[ivc][i].p_plet=pplet;
        symv[ivc][i].q_plet=qplet;
        symv[ivc][i].r_plet=rplet;
        pplet=0;
      }
      last_note=i;                 /* need this so > and < work */
      p1=p;
    }

    if (type==NOTE) {
      if (!word) {
        symv[ivc][i].word_st=1;
        word=1;
      }
      if (nbr && cfmt.slurisligatura)
        symv[ivc][i].lig1 += nbr;
      else
        symv[ivc][i].slur_st += nbr;
      nbr=0;
      if (voice[ivc].end_slur) symv[ivc][i].slur_end++;
      voice[ivc].end_slur=0;

      if (pplet) {                   /* start of n-plet */
        symv[ivc][i].p_plet=pplet;
        symv[ivc][i].q_plet=qplet;
        symv[ivc][i].r_plet=rplet;
        pplet=0;
      }
      last_note=last_real_note=i;
      p1=p;
    }
    
    if (word && ((type==BAR)||(type==SPACE))) {
      if (last_real_note>=0) symv[ivc][last_real_note].word_end=1;
      word=0;
    }

    if (!type) {

      if (*p == '-') {                  /* a-b tie */
        symv[ivc][last_note].slur_st++;
        voice[ivc].end_slur=1;
        p++;
      }                              

      else if (*p == '(') {
        p++;
        if (isdigit(*p)) {
          pplet=*p-'0'; qplet=0; rplet=pplet;
          p++;
          if (*p == ':') {
            p++;
            if (isdigit(*p)) { qplet=*p-'0';  p++; }
            if (*p == ':') {
              p++;
              if (isdigit(*p)) { rplet=*p-'0';  p++; }
            }
          }
        }
        else {
          nbr++;
        }
      }
      else if (*p == ')') {
        if (last_note>0)
          if (cfmt.slurisligatura)
            symv[ivc][last_note].lig2++;
          else
            symv[ivc][last_note].slur_end++;
        else
          syntax ("Unexpected symbol",p);
        p++;
      }
      else if (*p == '>') {
        num=1;
        p++;
        while (*p == '>') { num++; p++; }
        if (last_note<0) 
          syntax ("No note before > sign", p);
        else 
          double_note (last_note, num, 1, p1);
      }
      else if (*p == '<') {
        num=1;
        p++;
        while (*p == '<') { num++; p++; }
        if (last_note<0) 
          syntax ("No note before < sign", p);
        else 
          double_note (last_note, num, -1, p1);
      }
      else if (*p == '*')     /* ignore stars for now  */
        p++;
      else if (*p == '!')     /* ditto for '!' */
        p++;
      else {
        if (*p != '\0') 
          sprintf (msg, "Unexpected symbol \'%c\'", *p);
        else 
          sprintf (msg, "Unexpected end of line");
        syntax (msg, p);
        p++;
      }
    }
  }

  /* maybe set end-of-line marker, if symbols were added */
  n=voice[ivc].nsym;

  if (n>nsym0) {        
    if (type==CONTINUE || cfmt.barsperstaff || cfmt.continueall) {
      symv[ivc][n-1].eoln=0;
    } else {
      /* add invisible bar, if last symbol no bar */
      if (symv[ivc][n-1].type != BAR) {
        i=add_sym(BAR);
        symv[ivc][i].u=B_INVIS;
        n=i+1;
      }
      symv[ivc][n-1].eoln=1;
    }
  }



  /* break words at end of line */
  if (word && (symv[ivc][n-1].eoln==1)) {
    symv[ivc][last_note].word_end=1;
    word=0;
  }

  return TO_BE_CONTINUED;

}

/* ----- is_selected: check selection for current info fields ---- */
int is_selected (char *xref_str, int npat, char (*pat)[STRLFILE], int select_all, int search_field)
{
  int i,j,a,b,m;

  /* true if select_all or if no selectors given */
  if (select_all) return 1;
  if (isblankstr(xref_str) && (npat==0)) return 1;

  m=0;
  for (i=0;i<npat;i++) {             /*patterns */
    if (search_field==S_COMPOSER) {
      for (j=0;j<info.ncomp;j++) {
        if (!m) m=match(info.comp[j],pat[i]);
      }
    }
    else if (search_field==S_SOURCE)
      m=match(info.src,pat[i]);
    else if (search_field==S_RHYTHM)
      m=match(info.rhyth,pat[i]);
    else {
      m=match(info.title,pat[i]);
      if ((!m) && (numtitle>=2)) m=match(info.title2,pat[i]);
      if ((!m) && (numtitle>=3)) m=match(info.title3,pat[i]);
    }
    if (m) return 1;
  }

  /* check xref against string of numbers */
  p=xref_str;
  while (*p != 0) {
    parse_space();
    a=parse_uint();
    if (!a) return 0;          /* can happen if invalid chars in string */
    parse_space();
    if (*p == '-') {
      p++;
      parse_space();
      b=parse_uint();
      if (!b) {
        if (xrefnum>=a) return 1;
      }
      else
        for (i=a;i<=b;i++) if (xrefnum==i) return 1;
    }
    else {
      if (xrefnum==a) return 1;
    }
    if (*p == ',') p++;
  }

  return 0;

}

/* ----- rehash_selectors: split selectors into patterns and xrefs -- */
int rehash_selectors (char *sel_str, char *xref_str, char (*pat)[STRLFILE])
{
  char *q;
  char arg[501];
  int i,npat;

  npat=0;
  strcpy (xref_str, "");
  q=sel_str;
  
  i=0;
  while (1) {
    if ((*q==' ') || (*q=='\0')) {
      arg[i]='\0';
      i=0;
      if (!isblankstr(arg)) {
        if (arg[0]=='-')               /* skip any flags */
          ;
        else if (is_xrefstr(arg)) {
          strcat(xref_str, arg);
          strcat(xref_str, " ");
        }
        else {                         /* pattern with * or + */
          if ((strchr(arg,'*')) || (strchr(arg,'+'))) {
            strcpy(pat[npat],arg);
          }
          else {                       /* simple pattern */
            strcpy(pat[npat],"*");
            strcat(pat[npat],arg);
            strcat(pat[npat],"*");
          }
          npat++;
        }
      }
    }
    else {
      arg[i]=*q;
      i++;
    }
    if (*q=='\0') break;
    q++;
  }
  return npat;
}


/* ----- decomment_line: cut off after % ----- */
void decomment_line (char *ln)
{
  char* p;
  int inquotes = 0; /* do not remove inside double quotes */
  for (p=ln; *p; p++) {
    if (*p=='"') {if (inquotes) inquotes=0; else inquotes=1;}
    if ((*p=='%') && !inquotes) *p='\0';
  }
}


/* ----- get_line: read line, do first operations on it ----- */
int get_line (FILE *fp, string *ln)
{
  *ln = "";
  if (feof(fp)) return 0;

  getline(ln, fp);
  linenum++;
  if (is_end_line(ln->c_str()))  return 0;

  if ((verbose>=7) || (vb>=10) ) printf ("%3d  %s \n", linenum, ln->c_str());

  return 1;
}


/* ----- read_line: returns type of line scanned --- */
int read_line (FILE *fp, int do_music, string* linestr)
{
  int type,nsym0;
  static char* line = NULL;

  if (!get_line(fp,linestr)) return E_O_F;
  if (line) free(line);
  line = strdup(linestr->c_str());

  if ((linenum==1) && is_cmdline(line)) return CMDLINE;
  if (isblankstr(line))                 return BLANK;
  if (is_pseudocomment(line))           return PSCOMMENT;
  if (is_comment(line))                 return COMMENT;

  decomment_line (line);

  if ((type=info_field(line))) {
    /* skip after history field. Nightmarish syntax, that. */
    if (type != HISTORY) 
      return type;
    else {          
      for (;;) {
        if (! get_line(fp,linestr)) return E_O_F;
        if (isblankstr(linestr->c_str())) return BLANK;
        if (line) free(line);
        line = strdup(linestr->c_str());
        if (is_info_field(line)) break;
        add_text (line, TEXT_H);
      }
      type=info_field (line);
      return type;
    }
  }
  
  if (!do_music) return COMMENT;
  if (parse_vocals(line)) return MWORDS;

  /* now parse a real line of music */
  if (nvoice==0) ivc=switch_voice (DEFVOICE);

  nsym0=voice[ivc].nsym;

  /* music or tablature? */
  if (is_tab_line(line)) {
	type=parse_tab_line (line);
  } else {
	type=parse_music_line (line);
  }

  if (db>1)  
    printf ("  parsed music symbols %d to %d for voice %d\n", 
            nsym0,voice[ivc].nsym-1,ivc);
  
  return type;
}

/* ----- do_index: print index of abc file ------ */
void do_index(FILE *fp, char *xref_str, int npat, char (*pat)[STRLFILE], int select_all, int search_field)
{
  int type,within_tune;
  string linestr;
  static char* line = NULL;

  linenum=0;
  verbose=vb;
  numtitle=0;
  write_history=0;
  within_tune=within_block=do_this_tune=0;
  reset_info (&default_info);
  info=default_info;
  
  for (;;) {
    if (!get_line(fp,&linestr)) break;
    if (is_comment(linestr.c_str())) continue;
    if (line) free(line);
    line = strdup(linestr.c_str());
    decomment_line (line);
    type=info_field (line);

    switch (type) {
        
    case XREF:
      if (within_block) 
        printf ("+++ Tune %d not closed properly \n", xrefnum);
      numtitle=0;
      within_tune=0;
      within_block=1;
      ntext=0;
      break;
      
    case KEY:
      if (!within_block) break;
      if (!within_tune) {
        tnum2++;
        if (is_selected (xref_str,npat,pat,select_all,search_field)) {
          printf ("  %-4d %-5s %-4s", xrefnum, info.key, info.meter);
          if      (search_field==S_SOURCE)   printf ("  %-15s", info.src);
          else if (search_field==S_RHYTHM)   printf ("  %-8s",  info.rhyth);
          else if (search_field==S_COMPOSER) printf ("  %-15s", info.comp[0]);
          if (numtitle==3) 
            printf ("  %s - %s - %s", info.title,info.title2,info.title3);
          if (numtitle==2) printf ("  %s - %s", info.title, info.title2);
          if (numtitle==1) printf ("  %s", info.title);
          
          printf ("\n");
          tnum1++;
        }
        within_tune=1;
      }  
      break;

    }
    
    if (isblankstr(line)) {
      if (within_block && !within_tune) 
        printf ("+++ Header not closed in tune %d\n", xrefnum);
      within_tune=0;
      within_block=0;
      info=default_info;
    }
  }
  if (within_block && !within_tune) 
    printf ("+++ Header not closed in for tune %d\n", xrefnum);

}





