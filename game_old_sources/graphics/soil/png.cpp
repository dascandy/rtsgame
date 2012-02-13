#include "stb_image_aug.h"

#include <exception>
#include <cstdlib>
#include <cstring>
#include <cassert>

typedef struct
{
   unsigned int length;
   unsigned int type;
} chunk;

#define PNG_TYPE(a,b,c,d)  (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

static chunk get_chunk_header(stbi *s)
{
   chunk c;
   c.length = get32(s);
   c.type   = get32(s);
   return c;
}

static int check_png_header(stbi *s)
{
   static unsigned char png_sig[8] = { 137,80,78,71,13,10,26,10 };
   int i;
   for (i=0; i < 8; ++i)
      if (get8(s) != png_sig[i]) throw std::exception();
   return 1;
}

typedef struct
{
   stbi s;
   unsigned char *idata, *expanded, *out;
} png;


enum {
   F_none=0, F_sub=1, F_up=2, F_avg=3, F_paeth=4,
   F_avg_first, F_paeth_first,
};

static unsigned char first_row_filter[5] =
{
   F_none, F_sub, F_none, F_avg_first, F_paeth_first
};

static int paeth(int a, int b, int c)
{
   int p = a + b - c;
   int pa = abs(p-a);
   int pb = abs(p-b);
   int pc = abs(p-c);
   if (pa <= pb && pa <= pc) return a;
   if (pb <= pc) return b;
   return c;
}


static int create_png_image(png *a, unsigned char *raw, unsigned int raw_len, int out_n)
{
   stbi *s = &a->s;
   unsigned int i,j,stride = s->img_x*out_n;
   int k;
   int img_n = s->img_n; 
   assert(out_n == s->img_n || out_n == s->img_n+1);
   a->out = (unsigned char *) malloc(s->img_x * s->img_y * out_n);
   if (!a->out) throw std::exception();
   if (raw_len != (img_n * s->img_x + 1) * s->img_y) throw std::exception();
   for (j=0; j < s->img_y; ++j) {
      unsigned char *cur = a->out + stride*j;
      unsigned char *prior = cur - stride;
      int filter = *raw++;
      if (filter > 4) throw std::exception();
      
      if (j == 0) filter = first_row_filter[filter];
      
      for (k=0; k < img_n; ++k) {
         switch(filter) {
            case F_none       : cur[k] = raw[k]; break;
            case F_sub        : cur[k] = raw[k]; break;
            case F_up         : cur[k] = raw[k] + prior[k]; break;
            case F_avg        : cur[k] = raw[k] + (prior[k]>>1); break;
            case F_paeth      : cur[k] = (unsigned char) (raw[k] + paeth(0,prior[k],0)); break;
            case F_avg_first  : cur[k] = raw[k]; break;
            case F_paeth_first: cur[k] = raw[k]; break;
         }
      }
      if (img_n != out_n) cur[img_n] = 255;
      raw += img_n;
      cur += out_n;
      prior += out_n;
      
      if (img_n == out_n) {
         #define CASE(f) \
             case f:     \
                for (i=s->img_x-1; i >= 1; --i, raw+=img_n,cur+=img_n,prior+=img_n) \
                   for (k=0; k < img_n; ++k)
         switch(filter) {
            CASE(F_none)  cur[k] = raw[k]; break;
            CASE(F_sub)   cur[k] = raw[k] + cur[k-img_n]; break;
            CASE(F_up)    cur[k] = raw[k] + prior[k]; break;
            CASE(F_avg)   cur[k] = raw[k] + ((prior[k] + cur[k-img_n])>>1); break;
            CASE(F_paeth)  cur[k] = (unsigned char) (raw[k] + paeth(cur[k-img_n],prior[k],prior[k-img_n])); break;
            CASE(F_avg_first)    cur[k] = raw[k] + (cur[k-img_n] >> 1); break;
            CASE(F_paeth_first)  cur[k] = (unsigned char) (raw[k] + paeth(cur[k-img_n],0,0)); break;
         }
         #undef CASE
      } else {
         assert(img_n+1 == out_n);
         #define CASE(f) \
             case f:     \
                for (i=s->img_x-1; i >= 1; --i, cur[img_n]=255,raw+=img_n,cur+=out_n,prior+=out_n) \
                   for (k=0; k < img_n; ++k)
         switch(filter) {
            CASE(F_none)  cur[k] = raw[k]; break;
            CASE(F_sub)   cur[k] = raw[k] + cur[k-out_n]; break;
            CASE(F_up)    cur[k] = raw[k] + prior[k]; break;
            CASE(F_avg)   cur[k] = raw[k] + ((prior[k] + cur[k-out_n])>>1); break;
            CASE(F_paeth)  cur[k] = (unsigned char) (raw[k] + paeth(cur[k-out_n],prior[k],prior[k-out_n])); break;
            CASE(F_avg_first)    cur[k] = raw[k] + (cur[k-out_n] >> 1); break;
            CASE(F_paeth_first)  cur[k] = (unsigned char) (raw[k] + paeth(cur[k-out_n],0,0)); break;
         }
         #undef CASE
      }
   }
   return 1;
}

static int compute_transparency(png *z, unsigned char tc[3], int out_n)
{
   stbi *s = &z->s;
   unsigned int i, pixel_count = s->img_x * s->img_y;
   unsigned char *p = z->out;

   
   
   assert(out_n == 2 || out_n == 4);

   if (out_n == 2) {
      for (i=0; i < pixel_count; ++i) {
         p[1] = (p[0] == tc[0] ? 0 : 255);
         p += 2;
      }
   } else {
      for (i=0; i < pixel_count; ++i) {
         if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
            p[3] = 0;
         p += 4;
      }
   }
   return 1;
}

static int expand_palette(png *a, unsigned char *palette, int, int pal_img_n)
{
   unsigned int i, pixel_count = a->s.img_x * a->s.img_y;
   unsigned char *p, *temp_out, *orig = a->out;

   p = (unsigned char *) malloc(pixel_count * pal_img_n);
   if (p == NULL) throw std::exception();

   
   temp_out = p;

   if (pal_img_n == 3) {
      for (i=0; i < pixel_count; ++i) {
         int n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p += 3;
      }
   } else {
      for (i=0; i < pixel_count; ++i) {
         int n = orig[i]*4;
         p[0] = palette[n  ];
         p[1] = palette[n+1];
         p[2] = palette[n+2];
         p[3] = palette[n+3];
         p += 4;
      }
   }
   free(a->out);
   a->out = temp_out;
   return 1;
}

static int parse_png_file(png *z, int scan, int req_comp)
{
   unsigned char palette[1024], pal_img_n=0;
   unsigned char has_trans=0, tc[3];
   unsigned int ioff=0, idata_limit=0, i, pal_len=0;
   int first=1,k;
   stbi *s = &z->s;

   if (!check_png_header(s)) return 0;

   if (scan == SCAN_type) return 1;

   for(;;first=0) {
      chunk c = get_chunk_header(s);
      if (first && c.type != PNG_TYPE('I','H','D','R'))
         throw std::exception();
      switch (c.type) {
         case PNG_TYPE('I','H','D','R'): {
            int depth,color,interlace,comp,filter;
            if (!first) throw std::exception();
            if (c.length != 13) throw std::exception();
            s->img_x = get32(s); if (s->img_x > (1 << 24)) throw std::exception();
            s->img_y = get32(s); if (s->img_y > (1 << 24)) throw std::exception();
            depth = get8(s);  if (depth != 8)        throw std::exception();
            color = get8(s);  if (color > 6)         throw std::exception();
            if (color == 3) pal_img_n = 3; else if (color & 1) throw std::exception();
            comp  = get8(s);  if (comp) throw std::exception();
            filter= get8(s);  if (filter) throw std::exception();
            interlace = get8(s); if (interlace) throw std::exception();
            if (!s->img_x || !s->img_y) throw std::exception();
            if (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               if ((1 << 30) / s->img_x / s->img_n < s->img_y) throw std::exception();
               if (scan == SCAN_header) return 1;
            } else {
               
               
               s->img_n = 1;
               if ((1 << 30) / s->img_x / 4 < s->img_y) throw std::exception();
               
            }
            break;
         }

         case PNG_TYPE('P','L','T','E'):  {
            if (c.length > 256*3) throw std::exception();
            pal_len = c.length / 3;
            if (pal_len * 3 != c.length) throw std::exception();
            for (i=0; i < pal_len; ++i) {
               palette[i*4+0] = get8u(s);
               palette[i*4+1] = get8u(s);
               palette[i*4+2] = get8u(s);
               palette[i*4+3] = 255;
            }
            break;
         }

         case PNG_TYPE('t','R','N','S'): {
            if (z->idata) throw std::exception();
            if (pal_img_n) {
               if (scan == SCAN_header) { s->img_n = 4; return 1; }
               if (pal_len == 0) throw std::exception();
               if (c.length > pal_len) throw std::exception();
               pal_img_n = 4;
               for (i=0; i < c.length; ++i)
                  palette[i*4+3] = get8u(s);
            } else {
               if (!(s->img_n & 1)) throw std::exception();
               if (c.length != (unsigned int) s->img_n*2) throw std::exception();
               has_trans = 1;
               for (k=0; k < s->img_n; ++k)
                  tc[k] = (unsigned char) get16(s); 
            }
            break;
         }

         case PNG_TYPE('I','D','A','T'): {
            if (pal_img_n && !pal_len) throw std::exception();
            if (scan == SCAN_header) { s->img_n = pal_img_n; return 1; }
            if (ioff + c.length > idata_limit) {
               unsigned char *p;
               if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
               while (ioff + c.length > idata_limit)
                  idata_limit *= 2;
               p = (unsigned char *) realloc(z->idata, idata_limit); if (p == NULL) throw std::exception();
               z->idata = p;
            }
            memcpy(z->idata+ioff, s->img_buffer, c.length);
            s->img_buffer += c.length;
            ioff += c.length;
            break;
         }

         case PNG_TYPE('I','E','N','D'): {
            unsigned int raw_len;
            if (scan != SCAN_load) return 1;
            if (z->idata == NULL) throw std::exception();
            z->expanded = (unsigned char *) stbi_zlib_decode_malloc((char *) z->idata, ioff, (int *) &raw_len);
            if (z->expanded == NULL) return 0; 
            free(z->idata); z->idata = NULL;
            if ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_out_n = s->img_n+1;
            else
               s->img_out_n = s->img_n;
            if (!create_png_image(z, z->expanded, raw_len, s->img_out_n)) return 0;
            if (has_trans)
               if (!compute_transparency(z, tc, s->img_out_n)) return 0;
            if (pal_img_n) {
               
               s->img_n = pal_img_n; 
               s->img_out_n = pal_img_n;
               if (req_comp >= 3) s->img_out_n = req_comp;
               if (!expand_palette(z, palette, pal_len, s->img_out_n))
                  return 0;
            }
            free(z->expanded); z->expanded = NULL;
            return 1;
         }

         default:
            
            if ((c.type & (1 << 29)) == 0) {
               throw std::exception();
            }
            skip(s, c.length);
            break;
      }
      
      get32(s);
   }
}

static unsigned char *do_png(png *p, int *x, int *y, int *n, int req_comp)
{
   unsigned char *result=NULL;
   p->expanded = NULL;
   p->idata = NULL;
   p->out = NULL;
   if (req_comp < 0 || req_comp > 4) throw std::exception();
   if (parse_png_file(p, SCAN_load, req_comp)) {
      result = p->out;
      p->out = NULL;
      *x = p->s.img_x;
      *y = p->s.img_y;
      if (n) *n = p->s.img_n;
   }
   free(p->out);      p->out      = NULL;
   free(p->expanded); p->expanded = NULL;
   free(p->idata);    p->idata    = NULL;

   return result;
}

unsigned char *stbi_png_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   png p;
   start_mem(&p.s, buffer,len);
   return do_png(&p, x,y,comp,req_comp);
}

int stbi_png_test_memory(stbi_uc const *buffer, int len)
{
   png p;
   start_mem(&p.s, buffer, len);
   return parse_png_file(&p, SCAN_type,STBI_default);
}

extern int      stbi_png_info_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp);

