#ifndef _styleH
#define _styleH
/*  
 *  This file is part of abctab2ps, 
 *  See file abctab2ps.cpp for details.
 */

/*   Global style parameters for the note spacing and Knuthian glue. */

/*   Parameters here are used to set spaces around notes.
     Names ending in p: prefered natural spacings
     Names ending in x: expanded spacings  
     Units: everything is based on a staff which is 24 points high
     (ie. 6 pt between two staff lines). */

/* name for this set of parameters */
#define STYLE "std"

/* ----- set_style_pars: set the parameters which control note spacing ---- */
void set_style_pars (float strictness);

#endif // _styleH
