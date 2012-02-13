#include "stb_image_aug.h"

#include <exception>
#include <cstdlib>
#include <cassert>

static int bmp_test(stbi *s)
{
   int sz;
   if (get8(s) != 'B') return 0;
   if (get8(s) != 'M') return 0;
   get32le(s); 
   get16le(s); 
   get16le(s); 
   get32le(s); 
   sz = get32le(s);
   if (sz == 12 || sz == 40 || sz == 56 || sz == 108) return 1;
   return 0;
}

int      stbi_bmp_test_memory      (stbi_uc const *buffer, int len)
{
   stbi s;
   start_mem(&s, buffer, len);
   return bmp_test(&s);
}


static int high_bit(unsigned int z)
{
   int n=0;
   if (z == 0) return -1;
   if (z >= 0x10000) n += 16, z >>= 16;
   if (z >= 0x00100) n +=  8, z >>=  8;
   if (z >= 0x00010) n +=  4, z >>=  4;
   if (z >= 0x00004) n +=  2, z >>=  2;
   if (z >= 0x00002) n +=  1, z >>=  1;
   return n;
}

static int bitcount(unsigned int a)
{
   a = (a & 0x55555555) + ((a >>  1) & 0x55555555); 
   a = (a & 0x33333333) + ((a >>  2) & 0x33333333); 
   a = (a + (a >> 4)) & 0x0f0f0f0f; 
   a = (a + (a >> 8)); 
   a = (a + (a >> 16)); 
   return a & 0xff;
}

static int shiftsigned(int v, int shift, int bits)
{
   int result;
   int z=0;

   if (shift < 0) v <<= -shift;
   else v >>= shift;
   result = v;

   z = bits;
   while (z < 8) {
      result += v >> z;
      z += bits;
   }
   return result;
}

static stbi_uc *bmp_load(stbi *s, int *x, int *y, int *comp, int req_comp)
{
   unsigned char *out;
   unsigned int mr=0,mg=0,mb=0,ma=0;
   stbi_uc pal[256][4];
   int psize=0,i,j,compress=0,width;
   int bpp, flip_vertically, pad, target, offset, hsz;
   if (get8(s) != 'B' || get8(s) != 'M') throw std::exception();
   get32le(s); 
   get16le(s); 
   get16le(s); 
   offset = get32le(s);
   hsz = get32le(s);
   if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108) throw std::exception();
   if (hsz == 12) {
      s->img_x = get16le(s);
      s->img_y = get16le(s);
   } else {
      s->img_x = get32le(s);
      s->img_y = get32le(s);
   }
   if (get16le(s) != 1) return 0;
   bpp = get16le(s);
   if (bpp == 1) throw std::exception();
   flip_vertically = ((int) s->img_y) > 0;
   s->img_y = abs((int) s->img_y);
   if (hsz == 12) {
      if (bpp < 24)
         psize = (offset - 14 - 24) / 3;
   } else {
      compress = get32le(s);
      if (compress == 1 || compress == 2) throw std::exception();
      get32le(s); 
      get32le(s); 
      get32le(s); 
      get32le(s); 
      get32le(s); 
      if (hsz == 40 || hsz == 56) {
         if (hsz == 56) {
            get32le(s);
            get32le(s);
            get32le(s);
            get32le(s);
         }
         if (bpp == 16 || bpp == 32) {
            mr = mg = mb = 0;
            if (compress == 0) {
               if (bpp == 32) {
                  mr = 0xff << 16;
                  mg = 0xff <<  8;
                  mb = 0xff <<  0;
               } else {
                  mr = 31 << 10;
                  mg = 31 <<  5;
                  mb = 31 <<  0;
               }
            } else if (compress == 3) {
               mr = get32le(s);
               mg = get32le(s);
               mb = get32le(s);
               
               if (mr == mg && mg == mb) {
                  
                  return NULL;
               }
            } else
               return NULL;
         }
      } else {
         assert(hsz == 108);
         mr = get32le(s);
         mg = get32le(s);
         mb = get32le(s);
         ma = get32le(s);
         get32le(s); 
         for (i=0; i < 12; ++i)
            get32le(s); 
      }
      if (bpp < 16)
         psize = (offset - 14 - hsz) >> 2;
   }
   s->img_n = ma ? 4 : 3;
   if (req_comp && req_comp >= 3) 
      target = req_comp;
   else
      target = s->img_n; 
   out = (stbi_uc *) malloc(target * s->img_x * s->img_y);
   if (!out)  throw std::exception();
   if (bpp < 16) {
      int z=0;
      if (psize == 0 || psize > 256) { free(out); throw std::exception(); }
      for (i=0; i < psize; ++i) {
         pal[i][2] = get8(s);
         pal[i][1] = get8(s);
         pal[i][0] = get8(s);
         if (hsz != 12) get8(s);
         pal[i][3] = 255;
      }
      skip(s, offset - 14 - hsz - psize * (hsz == 12 ? 3 : 4));
      if (bpp == 4) width = (s->img_x + 1) >> 1;
      else if (bpp == 8) width = s->img_x;
      else { free(out); throw std::exception(); }
      pad = (-width)&3;
      for (j=0; j < (int) s->img_y; ++j) {
         for (i=0; i < (int) s->img_x; i += 2) {
            int v=get8(s),v2=0;
            if (bpp == 4) {
               v2 = v & 15;
               v >>= 4;
            }
            out[z++] = pal[v][0];
            out[z++] = pal[v][1];
            out[z++] = pal[v][2];
            if (target == 4) out[z++] = 255;
            if (i+1 == (int) s->img_x) break;
            v = (bpp == 8) ? get8(s) : v2;
            out[z++] = pal[v][0];
            out[z++] = pal[v][1];
            out[z++] = pal[v][2];
            if (target == 4) out[z++] = 255;
         }
         skip(s, pad);
      }
   } else {
      int rshift=0,gshift=0,bshift=0,ashift=0,rcount=0,gcount=0,bcount=0,acount=0;
      int z = 0;
      int easy=0;
      skip(s, offset - 14 - hsz);
      if (bpp == 24) width = 3 * s->img_x;
      else if (bpp == 16) width = 2*s->img_x;
      else width=0;
      pad = (-width) & 3;
      if (bpp == 24) {
         easy = 1;
      } else if (bpp == 32) {
         if (mb == 0xff && mg == 0xff00 && mr == 0xff000000 && ma == 0xff000000)
            easy = 2;
      }
      if (!easy) {
         if (!mr || !mg || !mb) throw std::exception();
         
         rshift = high_bit(mr)-7; rcount = bitcount(mr);
         gshift = high_bit(mg)-7; gcount = bitcount(mr);
         bshift = high_bit(mb)-7; bcount = bitcount(mr);
         ashift = high_bit(ma)-7; acount = bitcount(mr);
      }
      for (j=0; j < (int) s->img_y; ++j) {
         if (easy) {
            for (i=0; i < (int) s->img_x; ++i) {
               stbi_uc a;
               out[z+2] = get8(s);
               out[z+1] = get8(s);
               out[z+0] = get8(s);
               z += 3;
               a = (easy == 2 ? get8(s) : 255);
               if (target == 4) out[z++] = a;
            }
         } else {
            for (i=0; i < (int) s->img_x; ++i) {
               unsigned int v = (bpp == 16 ? get16le(s) : get32le(s));
               stbi_uc a;
               out[z++] = (unsigned char)shiftsigned(v & mr, rshift, rcount);
               out[z++] = (unsigned char)shiftsigned(v & mg, gshift, gcount);
               out[z++] = (unsigned char)shiftsigned(v & mb, bshift, bcount);
               a = (ma ? (unsigned char)shiftsigned(v & ma, ashift, acount) : 255);
               if (target == 4) out[z++] = a;
            }
         }
         skip(s, pad);
      }
   }
   if (flip_vertically) {
      stbi_uc t;
      for (j=0; j < (int) s->img_y>>1; ++j) {
         stbi_uc *p1 = out +      j     *s->img_x*target;
         stbi_uc *p2 = out + (s->img_y-1-j)*s->img_x*target;
         for (i=0; i < (int) s->img_x*target; ++i) {
            t = p1[i], p1[i] = p2[i], p2[i] = t;
         }
      }
   }

   *x = s->img_x;
   *y = s->img_y;
   if (comp) *comp = target;
   return out;
}

stbi_uc *stbi_bmp_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   stbi s;
   start_mem(&s, buffer, len);
   return bmp_load(&s, x,y,comp,req_comp);
}

