#include "stb_image_aug.h"

#include <cstring>
#include <cstdlib>

void stbi_image_free(void *retval_from_stbi_load)
{
   free(retval_from_stbi_load);
}

static float h2l_gamma_i=1.0f/2.2f, h2l_scale_i=1.0f;
static float l2h_gamma=2.2f, l2h_scale=1.0f;

void   stbi_hdr_to_ldr_gamma(float gamma) { h2l_gamma_i = 1/gamma; }
void   stbi_hdr_to_ldr_scale(float scale) { h2l_scale_i = 1/scale; }

void   stbi_ldr_to_hdr_gamma(float gamma) { l2h_gamma = gamma; }
void   stbi_ldr_to_hdr_scale(float scale) { l2h_scale = scale; }

void start_mem(stbi *s, unsigned char const *buffer, int len)
{
   s->img_buffer = (unsigned char *) buffer;
   s->img_buffer_end = (unsigned char *) buffer+len;
}

stbi_uc get8(stbi *s)
{
   if (s->img_buffer < s->img_buffer_end)
      return *s->img_buffer++;
   return 0;
}

int at_eof(stbi *s)
{
   return s->img_buffer >= s->img_buffer_end;
}

unsigned char get8u(stbi *s)
{
   return (unsigned char) get8(s);
}

void skip(stbi *s, int n)
{
   s->img_buffer += n;
}

int get16(stbi *s)
{
   int z = get8(s);
   return (z << 8) + get8(s);
}

unsigned int get32(stbi *s)
{
   unsigned int z = get16(s);
   return (z << 16) + get16(s);
}

int get16le(stbi *s)
{
   int z = get8(s);
   return z + (get8(s) << 8);
}

unsigned int get32le(stbi *s)
{
   unsigned int z = get16le(s);
   return z + (get16le(s) << 16);
}

void getn(stbi *s, stbi_uc *buffer, int n)
{
   memcpy(buffer, s->img_buffer, n);
   s->img_buffer += n;
}

unsigned char compute_y(int r, int g, int b)
{
   return (unsigned char) (((r*77) + (g*150) +  (29*b)) >> 8);
}

