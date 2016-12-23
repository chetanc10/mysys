
#ifndef __OPTIMUS_H
#define __OPTIMUS_H

/*CIRC_INC: range shall be 0 to ((lmt) - 1)*/
#define CIRC_INC_CMP(var, lmt) {(var) = (((var) + 1) == (lmt)) ? 0 : (var) + 1; }
#define CIRC_INC_BITS(var, lmt) {(var) = ((var) + 1) & ((lmt) - 1); }
#define CIRC_INC_IF(var, lmt) {++(var); if ((var) == (lmt)) (var) = 0;}

#endif /*__OPTIMUS_H*/
