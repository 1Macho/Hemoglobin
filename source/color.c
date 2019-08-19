
#include "color.h"

Color Color_FromHSV (unsigned char h, unsigned char s, unsigned char v) {
    Color rgb;
    unsigned char region, remainder, p, q, t;

    if (s == 0)
    {
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
        return rgb;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v;
            break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q;
            break;
    }
    return rgb;
}

u32* Color_GeneratePalete(unsigned char s, unsigned char v) {
  u32* result = malloc(sizeof(u32)*256);
  for (unsigned short i = 0; i < 256; i++) {
    Color thisOne = Color_FromHSV(i,s,v);
    result[i] = C2D_Color32(thisOne.r, thisOne.g, thisOne.b, 0xFF);
  }
  return result;
}
