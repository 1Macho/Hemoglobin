#ifndef COLORS
#define COLORS


#include <citro2d.h>
#include <stdlib.h>

typedef struct Color Color;
struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};
Color Color_FromHSV(unsigned char h,unsigned char s,unsigned char v);

u32* Color_GeneratePalete(unsigned char s, unsigned char v);

#endif
