// Filter bank used to estimate notes

#include "filters.h"

const float filter_coeff[R_NOTE] = {
   -1.9764953703101444,
   -1.9735990674403348,
   -1.9705355792250348,
   -1.966191312752684,
   -1.962738759043643,
   -1.9578767716746182,
   -1.9527199352654159,
   -1.947269026416631,
   -1.9400431057640242,
   -1.9339336025602833,
   -1.9258872746820304,
   -1.9156336695268052,
   -1.9065930145875807,
   -1.8951522303236619,
   -1.8809933310651248,
   -1.86816641895751,
   -1.85240196007178,
   -1.8333397862608871,
   -1.8131732771157405,
   -1.7919145801873755,
   -1.7667090100646898,
   -1.7371240405285624,
   -1.7059041506486288,
   -1.6730787234924795,
   -1.6316125053252515,
   -1.5879350755991233,
   -1.5421056273131424,
   -1.4860010441112008,
   -1.4271554658642118,
   -1.3611883835709504,
   -1.2876502091487252,
   -1.2061211357487258
};
