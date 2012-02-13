#include "stb_image_aug.h"

#include <exception>
#include <cstring>
#include <cassert>
#include <cstdlib>
#define ZFAST_BITS  9
#define ZFAST_MASK  ((1 << ZFAST_BITS) - 1)

typedef struct
{
   unsigned short fast[1 << ZFAST_BITS];
   unsigned short firstcode[16];
   int maxcode[17];
   unsigned short firstsymbol[16];
   unsigned char  size[288];
   unsigned short value[288];
} zhuffman;

inline static int bitreverse16(int n)
{
  n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
  n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
  n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
  n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
  return n;
}

inline static int bit_reverse(int v, int bits)
{
   assert(bits <= 16);
   
   
   return bitreverse16(v) >> (16-bits);
}

static int zbuild_huffman(zhuffman *z, unsigned char *sizelist, int num)
{
   int i,k=0;
   int code, next_code[16], sizes[17];

   
   memset(sizes, 0, sizeof(sizes));
   memset(z->fast, 255, sizeof(z->fast));
   for (i=0; i < num; ++i)
      ++sizes[sizelist[i]];
   sizes[0] = 0;
   for (i=1; i < 16; ++i)
      assert(sizes[i] <= (1 << i));
   code = 0;
   for (i=1; i < 16; ++i) {
      next_code[i] = code;
      z->firstcode[i] = (unsigned short) code;
      z->firstsymbol[i] = (unsigned short) k;
      code = (code + sizes[i]);
      if (sizes[i])
         if (code-1 >= (1 << i)) throw std::exception();
      z->maxcode[i] = code << (16-i); 
      code <<= 1;
      k += sizes[i];
   }
   z->maxcode[16] = 0x10000; 
   for (i=0; i < num; ++i) {
      int s = sizelist[i];
      if (s) {
         int c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
         z->size[c] = (unsigned char)s;
         z->value[c] = (unsigned short)i;
         if (s <= ZFAST_BITS) {
            int k = bit_reverse(next_code[s],s);
            while (k < (1 << ZFAST_BITS)) {
               z->fast[k] = (unsigned short) c;
               k += (1 << s);
            }
         }
         ++next_code[s];
      }
   }
   return 1;
}

typedef struct
{
   unsigned char *zbuffer, *zbuffer_end;
   int num_bits;
   unsigned int code_buffer;

   char *zout;
   char *zout_start;
   char *zout_end;
   int   z_expandable;

   zhuffman z_length, z_distance;
} zbuf;

inline static int zget8(zbuf *z)
{
   if (z->zbuffer >= z->zbuffer_end) return 0;
   return *z->zbuffer++;
}

static void fill_bits(zbuf *z)
{
   do {
      assert(z->code_buffer < (1U << z->num_bits));
      z->code_buffer |= zget8(z) << z->num_bits;
      z->num_bits += 8;
   } while (z->num_bits <= 24);
}

inline static unsigned int zreceive(zbuf *z, int n)
{
   unsigned int k;
   if (z->num_bits < n) fill_bits(z);
   k = z->code_buffer & ((1 << n) - 1);
   z->code_buffer >>= n;
   z->num_bits -= n;
   return k;
}

inline static int zhuffman_decode(zbuf *a, zhuffman *z)
{
   int b,s,k;
   if (a->num_bits < 16) fill_bits(a);
   b = z->fast[a->code_buffer & ZFAST_MASK];
   if (b < 0xffff) {
      s = z->size[b];
      a->code_buffer >>= s;
      a->num_bits -= s;
      return z->value[b];
   }

   
   
   k = bit_reverse(a->code_buffer, 16);
   for (s=ZFAST_BITS+1; ; ++s)
      if (k < z->maxcode[s])
         break;
   if (s == 16) return -1; 
   
   b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
   assert(z->size[b] == s);
   a->code_buffer >>= s;
   a->num_bits -= s;
   return z->value[b];
}

static int expand(zbuf *z, int n)  
{
   char *q;
   int cur, limit;
   if (!z->z_expandable) throw std::exception();
   cur   = (int) (z->zout     - z->zout_start);
   limit = (int) (z->zout_end - z->zout_start);
   while (cur + n > limit)
      limit *= 2;
   q = (char *) realloc(z->zout_start, limit);
   if (q == NULL) throw std::exception();
   z->zout_start = q;
   z->zout       = q + cur;
   z->zout_end   = q + limit;
   return 1;
}

static int length_base[31] = {
   3,4,5,6,7,8,9,10,11,13,
   15,17,19,23,27,31,35,43,51,59,
   67,83,99,115,131,163,195,227,258,0,0 };

static int length_extra[31]=
{ 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0 };

static int dist_base[32] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0};

static int dist_extra[32] =
{ 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

static int parse_huffman_block(zbuf *a)
{
   for(;;) {
      int z = zhuffman_decode(a, &a->z_length);
      if (z < 256) {
         if (z < 0) throw std::exception(); 
         if (a->zout >= a->zout_end) if (!expand(a, 1)) return 0;
         *a->zout++ = (char) z;
      } else {
         unsigned char *p;
         int len,dist;
         if (z == 256) return 1;
         z -= 257;
         len = length_base[z];
         if (length_extra[z]) len += zreceive(a, length_extra[z]);
         z = zhuffman_decode(a, &a->z_distance);
         if (z < 0) throw std::exception();
         dist = dist_base[z];
         if (dist_extra[z]) dist += zreceive(a, dist_extra[z]);
         if (a->zout - a->zout_start < dist) throw std::exception();
         if (a->zout + len > a->zout_end) if (!expand(a, len)) return 0;
         p = (unsigned char *) (a->zout - dist);
         while (len--)
            *a->zout++ = *p++;
      }
   }
}

static int compute_huffman_codes(zbuf *a)
{
   static unsigned char length_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
   static zhuffman z_codelength; 
   unsigned char lencodes[286+32+137];
   unsigned char codelength_sizes[19];
   int i,n;

   int hlit  = zreceive(a,5) + 257;
   int hdist = zreceive(a,5) + 1;
   int hclen = zreceive(a,4) + 4;

   memset(codelength_sizes, 0, sizeof(codelength_sizes));
   for (i=0; i < hclen; ++i) {
      int s = zreceive(a,3);
      codelength_sizes[length_dezigzag[i]] = (unsigned char) s;
   }
   if (!zbuild_huffman(&z_codelength, codelength_sizes, 19)) return 0;

   n = 0;
   while (n < hlit + hdist) {
      int c = zhuffman_decode(a, &z_codelength);
      assert(c >= 0 && c < 19);
      if (c < 16)
         lencodes[n++] = (unsigned char) c;
      else if (c == 16) {
         c = zreceive(a,2)+3;
         memset(lencodes+n, lencodes[n-1], c);
         n += c;
      } else if (c == 17) {
         c = zreceive(a,3)+3;
         memset(lencodes+n, 0, c);
         n += c;
      } else {
         assert(c == 18);
         c = zreceive(a,7)+11;
         memset(lencodes+n, 0, c);
         n += c;
      }
   }
   if (n != hlit+hdist) throw std::exception();
   if (!zbuild_huffman(&a->z_length, lencodes, hlit)) return 0;
   if (!zbuild_huffman(&a->z_distance, lencodes+hlit, hdist)) return 0;
   return 1;
}

static int parse_uncompressed_block(zbuf *a)
{
   unsigned char header[4];
   int len,nlen,k;
   if (a->num_bits & 7)
      zreceive(a, a->num_bits & 7); 
   
   k = 0;
   while (a->num_bits > 0) {
      header[k++] = (unsigned char) (a->code_buffer & 255); 
      a->code_buffer >>= 8;
      a->num_bits -= 8;
   }
   assert(a->num_bits == 0);
   
   while (k < 4)
      header[k++] = (unsigned char) zget8(a);
   len  = header[1] * 256 + header[0];
   nlen = header[3] * 256 + header[2];
   if (nlen != (len ^ 0xffff)) throw std::exception();
   if (a->zbuffer + len > a->zbuffer_end) throw std::exception();
   if (a->zout + len > a->zout_end)
      if (!expand(a, len)) return 0;
   memcpy(a->zout, a->zbuffer, len);
   a->zbuffer += len;
   a->zout += len;
   return 1;
}

static int parse_zlib_header(zbuf *a)
{
   int cmf   = zget8(a);
   int cm    = cmf & 15;
   
   int flg   = zget8(a);
   if ((cmf*256+flg) % 31 != 0) throw std::exception(); 
   if (flg & 32) throw std::exception(); 
   if (cm != 8) throw std::exception(); 
   
   return 1;
}

static unsigned char default_length[288], default_distance[32];
static void init_defaults(void)
{
   int i;   
   for (i=0; i <= 143; ++i)     default_length[i]   = 8;
   for (   ; i <= 255; ++i)     default_length[i]   = 9;
   for (   ; i <= 279; ++i)     default_length[i]   = 7;
   for (   ; i <= 287; ++i)     default_length[i]   = 8;

   for (i=0; i <=  31; ++i)     default_distance[i] = 5;
}

static int parse_zlib(zbuf *a, int parse_header)
{
   int final, type;
   if (parse_header)
      if (!parse_zlib_header(a)) return 0;
   a->num_bits = 0;
   a->code_buffer = 0;
   do {
      final = zreceive(a,1);
      type = zreceive(a,2);
      if (type == 0) {
         if (!parse_uncompressed_block(a)) return 0;
      } else if (type == 3) {
         return 0;
      } else {
         if (type == 1) {
            
            if (!default_distance[31]) init_defaults();
            if (!zbuild_huffman(&a->z_length  , default_length  , 288)) return 0;
            if (!zbuild_huffman(&a->z_distance, default_distance,  32)) return 0;
         } else {
            if (!compute_huffman_codes(a)) return 0;
         }
         if (!parse_huffman_block(a)) return 0;
      }
   } while (!final);
   return 1;
}

static int do_zlib(zbuf *a, char *obuf, int olen, int exp, int parse_header)
{
   a->zout_start = obuf;
   a->zout       = obuf;
   a->zout_end   = obuf + olen;
   a->z_expandable = exp;

   return parse_zlib(a, parse_header);
}

char *stbi_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen)
{
   zbuf a;
   char *p = (char *) malloc(initial_size);
   if (p == NULL) return NULL;
   a.zbuffer = (unsigned char *) buffer;
   a.zbuffer_end = (unsigned char *) buffer + len;
   if (do_zlib(&a, p, initial_size, 1, 1)) {
      if (outlen) *outlen = (int) (a.zout - a.zout_start);
      return a.zout_start;
   } else {
      free(a.zout_start);
      return NULL;
   }
}

char *stbi_zlib_decode_malloc(char const *buffer, int len, int *outlen)
{
   return stbi_zlib_decode_malloc_guesssize(buffer, len, 16384, outlen);
}

int stbi_zlib_decode_buffer(char *obuffer, int olen, char const *ibuffer, int ilen)
{
   zbuf a;
   a.zbuffer = (unsigned char *) ibuffer;
   a.zbuffer_end = (unsigned char *) ibuffer + ilen;
   if (do_zlib(&a, obuffer, olen, 0, 1))
      return (int) (a.zout - a.zout_start);
   else
      return -1;
}

char *stbi_zlib_decode_noheader_malloc(char const *buffer, int len, int *outlen)
{
   zbuf a;
   char *p = (char *) malloc(16384);
   if (p == NULL) return NULL;
   a.zbuffer = (unsigned char *) buffer;
   a.zbuffer_end = (unsigned char *) buffer+len;
   if (do_zlib(&a, p, 16384, 1, 0)) {
      if (outlen) *outlen = (int) (a.zout - a.zout_start);
      return a.zout_start;
   } else {
      free(a.zout_start);
      return NULL;
   }
}

int stbi_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen)
{
   zbuf a;
   a.zbuffer = (unsigned char *) ibuffer;
   a.zbuffer_end = (unsigned char *) ibuffer + ilen;
   if (do_zlib(&a, obuffer, olen, 0, 0))
      return (int) (a.zout - a.zout_start);
   else
      return -1;
}

