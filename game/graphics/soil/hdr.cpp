#include "stb_image_aug.h"

#include <exception>
#include <cmath>
#include <cstring>
#include <cstdlib>

static int hdr_test(stbi *s)
{
   char *signature = "#?RADIANCE\n";
   int i;
   for (i=0; signature[i]; ++i)
      if (get8(s) != signature[i])
         return 0;
	return 1;
}

int stbi_hdr_test_memory(stbi_uc const *buffer, int len)
{
   stbi s;
	start_mem(&s, buffer, len);
	return hdr_test(&s);
}

#define HDR_BUFLEN  1024
static char *hdr_gettoken(stbi *z, char *buffer)
{
   int len=0;
	
	char c = '\0';

   c = get8(z);

	while (!at_eof(z) && c != '\n') {
		buffer[len++] = c;
      if (len == HDR_BUFLEN-1) {
         
         while (!at_eof(z) && get8(z) != '\n')
            ;
         break;
      }
      c = get8(z);
	}

   buffer[len] = 0;
	return buffer;
}

static void hdr_convert(float *output, stbi_uc *input, int req_comp)
{
	if( input[3] != 0 ) {
      float f1;
	  f1 = ldexp(1.0f, input[3] - (int)(128 + 8));
      if (req_comp <= 2)
         output[0] = (input[0] + input[1] + input[2]) * f1 / 3;
      else {
         output[0] = input[0] * f1;
         output[1] = input[1] * f1;
         output[2] = input[2] * f1;
      }
      if (req_comp == 2) output[1] = 1;
      if (req_comp == 4) output[3] = 1;
	} else {
      switch (req_comp) {
         case 4: output[3] = 1;
         case 3: output[0] = output[1] = output[2] = 0;
                 break;
         case 2: output[1] = 1;
         case 1: output[0] = 0;
                 break;
      }
	}
}


static float *hdr_load(stbi *s, int *x, int *y, int *comp, int req_comp)
{
   char buffer[HDR_BUFLEN];
	char *token;
	int valid = 0;
	int width, height;
   stbi_uc *scanline;
	float *hdr_data;
	stbi_uc len;
	unsigned char count, value;
	int i, j, k, z;
	stbi_uc c1,c2;


	
	if (strcmp(hdr_gettoken(s,buffer), "#?RADIANCE") != 0)
		throw std::exception();

	
	while(!valid) {
		token = hdr_gettoken(s,buffer);
		if (token[0] == 0) break;
		if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = 1;
   }

	if (!valid)    throw std::exception();

   
   
   token = hdr_gettoken(s,buffer);
   if (strncmp(token, "-Y ", 3))  throw std::exception();
   token += 3;
   height = strtol(token, &token, 10);
   while (*token == ' ') ++token;
   if (strncmp(token, "+X ", 3))  throw std::exception();
   token += 3;
   width = strtol(token, NULL, 10);

	*x = width;
	*y = height;

   *comp = 3;
	if (req_comp == 0) req_comp = 3;

	
	hdr_data = (float *) malloc(height * width * req_comp * sizeof(float));

	
   
	if( width < 8 || width >= 32768) {
		
      for (j=0; j < height; ++j) {
         for (i=0; i < width; ++i) {
            stbi_uc rgbe[4];
           main_decode_loop:
            getn(s, rgbe, 4);
            hdr_convert(hdr_data + j * width * req_comp + i * req_comp, rgbe, req_comp);
         }
      }
	} else {
		
		scanline = NULL;

		for (j = 0; j < height; ++j) {
         c1 = get8(s);
         c2 = get8(s);
         len = get8(s);
         if (c1 != 2 || c2 != 2 || (len & 0x80)) {
            
            
            stbi_uc rgbe[4] = { c1,c2,len, get8(s) };
            hdr_convert(hdr_data, rgbe, req_comp);
            i = 1;
            j = 0;
            free(scanline);
            goto main_decode_loop; 
         }
         len <<= 8;
         len |= get8(s);
         if (len != width) { free(hdr_data); free(scanline); throw std::exception(); }
         if (scanline == NULL) scanline = (stbi_uc *) malloc(width * 4);

			for (k = 0; k < 4; ++k) {
				i = 0;
				while (i < width) {
					count = get8(s);
					if (count > 128) {
						
						value = get8(s);
                  count -= 128;
						for (z = 0; z < count; ++z)
							scanline[i++ * 4 + k] = value;
					} else {
						
						for (z = 0; z < count; ++z)
							scanline[i++ * 4 + k] = get8(s);
					}
				}
			}
         for (i=0; i < width; ++i)
            hdr_convert(hdr_data+(j*width + i)*req_comp, scanline + i*4, req_comp);
		}
      free(scanline);
	}

   return hdr_data;
}

static stbi_uc *hdr_load_rgbe(stbi *s, int *x, int *y, int *comp, int req_comp)
{
   char buffer[HDR_BUFLEN];
	char *token;
	bool valid = false;
	int width, height;
   stbi_uc *scanline;
	stbi_uc *rgbe_data;
	stbi_uc len, c1,c2;
	unsigned char count, value;
	int i, j, k, z;


	
	if (strcmp(hdr_gettoken(s,buffer), "#?RADIANCE") != 0)
		throw std::exception();

	
	while(!valid) {
		token = hdr_gettoken(s,buffer);
		if (token[0] == 0) break;
		if (strcmp(token, "FORMAT=32-bit_rle_rgbe") == 0) valid = true;
   }

	if (!valid)    throw std::exception();

   
   
   token = hdr_gettoken(s,buffer);
   if (strncmp(token, "-Y ", 3))  throw std::exception();
   token += 3;
   height = strtol(token, &token, 10);
   while (*token == ' ') ++token;
   if (strncmp(token, "+X ", 3))  throw std::exception();
   token += 3;
   width = strtol(token, NULL, 10);

	*x = width;
	*y = height;

	
   *comp = 4;
	req_comp = 4;

	
	rgbe_data = (stbi_uc *) malloc(height * width * req_comp * sizeof(stbi_uc));
	
	scanline = rgbe_data;

	
   
	if( width < 8 || width >= 32768) {
		
      for (j=0; j < height; ++j) {
         for (i=0; i < width; ++i) {
           main_decode_loop:
            
            getn(s,scanline, 4);
			scanline += 4;
         }
      }
	} else {
		
		for (j = 0; j < height; ++j) {
         c1 = get8(s);
         c2 = get8(s);
         len = get8(s);
         if (c1 != 2 || c2 != 2 || (len & 0x80)) {
            
            
            scanline[0] = c1;
            scanline[1] = c2;
            scanline[2] = len;
            scanline[3] = get8(s);
            scanline += 4;
            i = 1;
            j = 0;
            goto main_decode_loop; 
         }
         len <<= 8;
         len |= get8(s);
         if (len != width) { free(rgbe_data); throw std::exception(); }
			for (k = 0; k < 4; ++k) {
				i = 0;
				while (i < width) {
					count = get8(s);
					if (count > 128) {
						
						value = get8(s);
                  count -= 128;
						for (z = 0; z < count; ++z)
							scanline[i++ * 4 + k] = value;
					} else {
						
						for (z = 0; z < count; ++z)
							scanline[i++ * 4 + k] = get8(s);
					}
				}
			}
			
			scanline += 4 * width;
		}
	}

   return rgbe_data;
}

float *stbi_hdr_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   stbi s;
   start_mem(&s,buffer, len);
   return hdr_load(&s,x,y,comp,req_comp);
}

stbi_uc *stbi_hdr_load_rgbe_memory(stbi_uc *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
   stbi s;
   start_mem(&s,buffer, len);
   return hdr_load_rgbe(&s,x,y,comp,req_comp);
}

