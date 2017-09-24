/*
 * font.c
 *
 *  Created on: Sep 25, 2017
 *      Author: Michael
 */
 
#include <chip.h>
#include <pm_graphics.h>

/*
     
     
 *** 
    *
 ****
*   *
 ****
     
*/
_U08 a[CHAR_ROWS][CHAR_WIDTH] = {
	{32, 84, 84, 84, 120}
};

/*
 **  
*  * 
* *  
 *   
* * *
*  * 
 ** *
*/
_U08 andperand[CHAR_ROWS][CHAR_WIDTH] = {
	{54, 73, 85, 34, 80}
};

/*
     
  *  
* * *
 *** 
* * *
  *  
     
*/
_U08 asterisk[CHAR_ROWS][CHAR_WIDTH] = {
	{20, 8, 62, 8, 20}
};

/*
 *** 
*   *
    *
 ** *
* * *
* * *
 *** 
*/
_U08 at[CHAR_ROWS][CHAR_WIDTH] = {
	{50, 73, 121, 65, 62}
};

/*
*    
*    
* ** 
**  *
*   *
*   *
**** 
*/
_U08 b[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 72, 68, 68, 56}
};

/*
     
*    
 *   
  *  
   * 
    *
*/
_U08 backslash[CHAR_ROWS][CHAR_WIDTH] = {
	{2, 4, 8, 16, 32}
};

/*
 *   
  *  
   * 
     
     
     
     
*/
_U08 backtick[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 1, 2, 4, 0}
};

/*
     
     
 *** 
*    
*    
*   *
 *** 
     
*/
_U08 c[CHAR_ROWS][CHAR_WIDTH] = {
	{56, 68, 68, 68, 32}
};

/*
  *  
 * * 
*   *
     
     
     
*/
_U08 carat[CHAR_ROWS][CHAR_WIDTH] = {
	{4, 2, 1, 2, 4}
};

/*
 *   
  *  
   * 
    *
   * 
  *  
 *   
*/
_U08 close_angle[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 34, 20, 8}
};

/*
 *  
  * 
   *
   *
   *
  * 
 *  
*/
_U08 close_bracket[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 34, 28}
};

/*
 *   
  *  
  *  
   * 
  *  
  *  
 *   
*/
_U08 close_curly[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 54, 8, 0}
};

/*
 ***
   *
   *
   *
   *
   *
 ***
*/
_U08 close_square[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 65, 127}
};

/*
     
 **  
 **  
     
 **  
 **  
     
*/
_U08 colon[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 54, 54, 0, 0}
};

/*
     
     
     
     
 **  
  *  
 *   
*/
_U08 comma[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 80, 48, 0, 0}
};

/*
    *
    *
 ** *
*  **
*   *
*   *
 ****
*/
_U08 d[CHAR_ROWS][CHAR_WIDTH] = {
	{56, 68, 68, 72, 127}
};

/*
  *  
 ****
* *  
 *** 
  * *
**** 
  *  
*/
_U08 dollar[CHAR_ROWS][CHAR_WIDTH] = {
	{36, 42, 127, 42, 18}
};

/*
     
     
     
     
     
 **  
 **  
*/
_U08 dot[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 96, 96, 0, 0}
};

/*
     
     
 *** 
*   *
*****
*    
 *** 
*/
_U08 e[CHAR_ROWS][CHAR_WIDTH] = {
	{56, 84, 84, 84, 24}
};

/*
     
     
*****
     
*****
     
*/
_U08 equal[CHAR_ROWS][CHAR_WIDTH] = {
	{20, 20, 20, 20, 20}
};

/*
  *  
  *  
  *  
  *  
     
     
  *  
*/
_U08 exclamation[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 0, 79, 0, 0}
};

/*
  ** 
 *  *
 *   
***  
 *   
 *   
 *   
*/
_U08 f[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 126, 9, 1, 2}
};

/*
     
 ****
*   *
*   *
 ****
    *
 *** 
*/
_U08 g[CHAR_ROWS][CHAR_WIDTH] = {
	{12, 82, 82, 82, 62}
};

/*
*    
*    
* ** 
**  *
*   *
*   *
*   *
*/
_U08 h[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 8, 4, 4, 120}
};

/*
 * * 
 * * 
*****
 * * 
*****
 * * 
 * * 
*/
_U08 hash[CHAR_ROWS][CHAR_WIDTH] = {
	{20, 127, 20, 127, 20}
};

/*
     
     
     
*****
     
     
*/
_U08 hyphen[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 8, 8, 8, 8}
};

/*
  * 
    
 ** 
  * 
  * 
  * 
 ***
*/
_U08 i[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 68, 125, 64}
};

/*
   * 
     
  ** 
   * 
   * 
*  * 
 **  
*/
_U08 j[CHAR_ROWS][CHAR_WIDTH] = {
	{32, 64, 68, 61, 0}
};

/*
*    
*    
*  * 
* *  
**   
* *  
*  * 
*/
_U08 k[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 16, 40, 68, 0}
};

/*
 **  
  *  
  *  
  *  
  *  
  *  
 *** 
*/
_U08 l[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 127, 64, 0}
};

/*
     
     
** * 
* * *
* * *
*   *
*   *
*/
_U08 m[CHAR_ROWS][CHAR_WIDTH] = {
	{124, 4, 24, 4, 120}
};

/*
     
     
* ** 
**  *
*   *
*   *
*   *
*/
_U08 n[CHAR_ROWS][CHAR_WIDTH] = {
	{124, 8, 4, 4, 120}
};

/*
     
     
 *** 
*   *
*   *
*   *
 *** 
*/
_U08 o[CHAR_ROWS][CHAR_WIDTH] = {
	{56, 68, 68, 68, 56}
};

/*
   * 
  *  
 *   
*    
 *   
  *  
   * 
*/
_U08 open_angle[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 20, 34, 65, 0}
};

/*
   * 
  *  
 *   
 *   
 *   
  *  
   * 
*/
_U08 open_bracket[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 28, 34, 65, 0}
};

/*
   * 
  *  
  *  
 *   
  *  
  *  
   * 
*/
_U08 open_curly[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 8, 54, 65, 0}
};

/*
***  
*    
*    
*    
*    
*    
***  
*/
_U08 open_square[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 65, 65, 0, 0}
};

/*
     
     
**** 
*   *
**** 
*    
*    
*/
_U08 p[CHAR_ROWS][CHAR_WIDTH] = {
	{124, 20, 20, 20, 8}
};

/*
**   
**  *
   * 
  *  
 *   
*  **
   **
*/
_U08 percent[CHAR_ROWS][CHAR_WIDTH] = {
	{35, 19, 8, 100, 98}
};

/*
  *
  *
  *
  *
  *
  *
  *
*/
_U08 pipe[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 0, 127}
};

/*
     
  *  
  *  
*****
  *  
  *  
     
*/
_U08 plus[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 8, 62, 8, 8}
};

/*
     
     
 ** *
*  **
 ****
    *
    *
*/
_U08 q[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 20, 20, 24, 124}
};

/*
 *** 
*   *
    *
   * 
  *  
     
  *  
*/
_U08 question[CHAR_ROWS][CHAR_WIDTH] = {
	{2, 1, 81, 9, 6}
};

/*
 **  
  *  
 *   
     
     
     
     
*/
_U08 quote[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 5, 3, 0, 0}
};

/*
 * * 
 * * 
 * * 
     
     
     
     
*/
_U08 quote2[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 7, 0, 7, 0}
};

/*
     
     
* ** 
**  *
*    
*    
*    
*/
_U08 r[CHAR_ROWS][CHAR_WIDTH] = {
	{124, 8, 4, 4, 8}
};

/*
     
     
 *** 
*    
 *** 
    *
**** 
*/
_U08 s[CHAR_ROWS][CHAR_WIDTH] = {
	{72, 84, 84, 84, 32}
};

/*
     
 **  
 **  
     
 **  
  *  
 *   
*/
_U08 semicolon[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 86, 54, 0, 0}
};

/*
     
    *
   * 
  *  
 *   
*    
*/
_U08 slash[CHAR_ROWS][CHAR_WIDTH] = {
	{32, 16, 8, 4, 2}
};

/*
     
     
     
     
     
     
     
*/
_U08 space[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 0, 0, 0, 0}
};

/*
 *   
 *   
***  
 *   
 *   
 *  *
  ** 
*/
_U08 t[CHAR_ROWS][CHAR_WIDTH] = {
	{4, 63, 68, 64, 32}
};

/*
     
     
 *   
* * *
   * 
     
     
*/
_U08 tilda[CHAR_ROWS][CHAR_WIDTH] = {
	{8, 4, 8, 16, 8}
};

/*
     
     
*   *
*   *
*   *
*  **
 ** *
*/
_U08 u[CHAR_ROWS][CHAR_WIDTH] = {
	{60, 64, 64, 32, 124}
};

/*
     
     
     
     
     
     
*****
*/
_U08 underscore[CHAR_ROWS][CHAR_WIDTH] = {
	{64, 64, 64, 64, 64}
};

/*
     
     
*   *
*   *
*   *
 * * 
  *  
*/
_U08 v[CHAR_ROWS][CHAR_WIDTH] = {
	{28, 32, 64, 32, 28}
};

/*
     
     
*   *
* * *
* * *
* * *
 * * 
*/
_U08 w[CHAR_ROWS][CHAR_WIDTH] = {
	{60, 64, 56, 64, 60}
};

/*
     
     
*   *
 * * 
  *  
 * * 
*   *
*/
_U08 x[CHAR_ROWS][CHAR_WIDTH] = {
	{68, 40, 16, 40, 68}
};

/*
     
     
*   *
*   *
 ****
    *
 *** 
*/
_U08 y[CHAR_ROWS][CHAR_WIDTH] = {
	{12, 80, 80, 80, 60}
};

/*
     
     
*****
   * 
  *  
 *   
*****
*/
_U08 z[CHAR_ROWS][CHAR_WIDTH] = {
	{68, 100, 84, 76, 68}
};

/*
 *** 
*   *
*  **
* * *
**  *
*   *
 *** 
*/
_U08 _0[CHAR_ROWS][CHAR_WIDTH] = {
	{62, 81, 73, 69, 62}
};

/*
  *  
 **  
  *  
  *  
  *  
  *  
 *** 
*/
_U08 _1[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 66, 127, 64, 0}
};

/*
 *** 
*   *
    *
   * 
  *  
 *   
*****
*/
_U08 _2[CHAR_ROWS][CHAR_WIDTH] = {
	{66, 97, 81, 73, 70}
};

/*
*****
   * 
  *  
   * 
    *
*   *
 *** 
*/
_U08 _3[CHAR_ROWS][CHAR_WIDTH] = {
	{33, 65, 69, 75, 49}
};

/*
   * 
  ** 
 * * 
*  * 
*****
   * 
   * 
*/
_U08 _4[CHAR_ROWS][CHAR_WIDTH] = {
	{24, 20, 18, 127, 16}
};

/*
*****
*    
**** 
    *
    *
*   *
 *** 
*/
_U08 _5[CHAR_ROWS][CHAR_WIDTH] = {
	{39, 69, 69, 69, 57}
};

/*
  ** 
 *   
*    
**** 
*   *
*   *
 *** 
*/
_U08 _6[CHAR_ROWS][CHAR_WIDTH] = {
	{60, 74, 73, 73, 48}
};

/*
*****
*   *
    *
   * 
  *  
  *  
  *  
*/
_U08 _7[CHAR_ROWS][CHAR_WIDTH] = {
	{3, 1, 113, 9, 7}
};

/*
 *** 
*   *
*   *
 *** 
*   *
*   *
 *** 
*/
_U08 _8[CHAR_ROWS][CHAR_WIDTH] = {
	{54, 73, 73, 73, 54}
};

/*
 *** 
*   *
*   *
 ****
    *
   * 
 **  
*/
_U08 _9[CHAR_ROWS][CHAR_WIDTH] = {
	{6, 73, 73, 41, 30}
};

/*
 *** 
*   *
*   *
*   *
*****
*   *
*   *
*/
_U08 _a[CHAR_ROWS][CHAR_WIDTH] = {
	{126, 17, 17, 17, 126}
};

/*
**** 
*   *
*   *
**** 
*   *
*   *
**** 
*/
_U08 _b[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 73, 73, 73, 54}
};

/*
 *** 
*   *
*    
*    
*    
*   *
 *** 
*/
_U08 _c[CHAR_ROWS][CHAR_WIDTH] = {
	{62, 65, 65, 65, 34}
};

/*
***  
*  * 
*   *
*   *
*   *
*  * 
***  
*/
_U08 _d[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 65, 65, 34, 28}
};

/*
*****
*    
*    
*****
*    
*    
*****
*/
_U08 _e[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 73, 73, 73, 73}
};

/*
*****
*    
*    
**** 
*    
*    
*    
*/
_U08 _f[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 9, 9, 9, 1}
};

/*
 *** 
*   *
*    
* ***
*   *
*   *
 *** 
*/
_U08 _g[CHAR_ROWS][CHAR_WIDTH] = {
	{62, 65, 73, 73, 58}
};

/*
*   *
*   *
*   *
*****
*   *
*   *
*   *
*/
_U08 _h[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 8, 8, 8, 127}
};

/*
 ***
  * 
  * 
  * 
  * 
  * 
 ***
*/
_U08 _i[CHAR_ROWS][CHAR_WIDTH] = {
	{0, 65, 127, 65}
};

/*
  ***
   * 
   * 
   * 
   * 
*  * 
 **  
*/
_U08 _j[CHAR_ROWS][CHAR_WIDTH] = {
	{32, 64, 65, 63, 1}
};

/*
*   *
*  * 
* *  
**   
* *  
*  * 
*   *
*/
_U08 _k[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 8, 20, 34, 65}
};

/*
*    
*    
*    
*    
*    
*    
*****
*/
_U08 _l[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 64, 64, 64, 64}
};

/*
*   *
** **
* * *
* * *
*   *
*   *
*   *
*/
_U08 _m[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 2, 12, 2, 127}
};

/*
*   *
*   *
**  *
* * *
*  **
*   *
*   *
*/
_U08 _n[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 4, 8, 16, 127}
};

/*
 *** 
*   *
*   *
*   *
*   *
*   *
 *** 
*/
_U08 _o[CHAR_ROWS][CHAR_WIDTH] = {
	{62, 65, 65, 65, 62}
};

/*
**** 
*   *
*   *
**** 
*    
*    
*    
*/
_U08 _p[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 9, 9, 9, 6}
};

/*
 *** 
*   *
*   *
*   *
* * *
*  * 
 ** *
*/
_U08 _q[CHAR_ROWS][CHAR_WIDTH] = {
	{62, 65, 81, 33, 94}
};

/*
**** 
*   *
*   *
**** 
* *  
*  * 
*   *
*/
_U08 _r[CHAR_ROWS][CHAR_WIDTH] = {
	{127, 9, 25, 41, 70}
};

/*
 ****
*    
*    
 *** 
    *
    *
**** 
*/
_U08 _s[CHAR_ROWS][CHAR_WIDTH] = {
	{70, 73, 73, 73, 49}
};

/*
*****
  *  
  *  
  *  
  *  
  *  
  *  
*/
_U08 _t[CHAR_ROWS][CHAR_WIDTH] = {
	{1, 1, 127, 1, 1}
};

/*
*   *
*   *
*   *
*   *
*   *
*   *
 *** 
*/
_U08 _u[CHAR_ROWS][CHAR_WIDTH] = {
	{63, 64, 64, 64, 63}
};

/*
*   *
*   *
*   *
*   *
*   *
 * * 
  *  
*/
_U08 _v[CHAR_ROWS][CHAR_WIDTH] = {
	{31, 32, 64, 32, 31}
};

/*
*   *
*   *
 * * 
  *  
 * * 
*   *
*   *
*/
_U08 _x[CHAR_ROWS][CHAR_WIDTH] = {
	{99, 20, 8, 20, 99}
};

/*
*   *
*   *
*   *
 * * 
  *  
  *  
  *  
*/
_U08 _y[CHAR_ROWS][CHAR_WIDTH] = {
	{7, 8, 112, 8, 7}
};

/*
*****
    *
   * 
  *  
 *   
*    
*****
*/
_U08 _z[CHAR_ROWS][CHAR_WIDTH] = {
	{97, 81, 73, 69, 67}
};

_U08 (*font[NUM_CHARS])[CHAR_ROWS][CHAR_WIDTH] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&space,
	&exclamation,
	&quote2,
	&hash,
	&dollar,
	&percent,
	&andperand,
	&quote,
	&open_bracket,
	&close_bracket,
	&asterisk,
	&plus,
	&comma,
	&hyphen,
	&dot,
	&slash,
	&_0,
	&_1,
	&_2,
	&_3,
	&_4,
	&_5,
	&_6,
	&_7,
	&_8,
	&_9,
	&colon,
	&semicolon,
	&open_angle,
	&equal,
	&close_angle,
	&question,
	&at,
	&_a,
	&_b,
	&_c,
	&_d,
	&_e,
	&_f,
	&_g,
	&_h,
	&_i,
	&_j,
	&_k,
	&_l,
	&_m,
	&_n,
	&_o,
	&_p,
	&_q,
	&_r,
	&_s,
	&_t,
	&_u,
	&_v,
	NULL,
	&_x,
	&_y,
	&_z,
	&open_square,
	&backslash,
	&close_square,
	&carat,
	&underscore,
	&backtick,
	&a,
	&b,
	&c,
	&d,
	&e,
	&f,
	&g,
	&h,
	&i,
	&j,
	&k,
	&l,
	&m,
	&n,
	&o,
	&p,
	&q,
	&r,
	&s,
	&t,
	&u,
	&v,
	&w,
	&x,
	&y,
	&z,
	&open_curly,
	&pipe,
	&close_curly,
	&tilda,
	NULL
};
