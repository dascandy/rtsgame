#ifndef HEADER_STB_IMAGE_AUGMENTED
#define HEADER_STB_IMAGE_AUGMENTED

#define STBI_VERSION 1

enum
{
   STBI_default = 0, // only used for req_comp

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4,
};

typedef unsigned char stbi_uc;

typedef struct
{
   unsigned int img_x, img_y;
   int img_n, img_out_n;

   unsigned char *img_buffer, *img_buffer_end;
} stbi;

enum
{
   SCAN_load=0,
   SCAN_type,
   SCAN_header,
};


extern stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

extern float *stbi_loadf_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

extern void   stbi_hdr_to_ldr_gamma(float gamma);
extern void   stbi_hdr_to_ldr_scale(float scale);

extern void   stbi_ldr_to_hdr_gamma(float gamma);
extern void   stbi_ldr_to_hdr_scale(float scale);

// free the loaded image -- this is just free()
extern void     stbi_image_free      (void *retval_from_stbi_load);

extern char *stbi_zlib_decode_malloc_guesssize(const char *buffer, int len, int initial_size, int *outlen);
extern char *stbi_zlib_decode_malloc(const char *buffer, int len, int *outlen);
extern int   stbi_zlib_decode_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);

extern char *stbi_zlib_decode_noheader_malloc(const char *buffer, int len, int *outlen);
extern int   stbi_zlib_decode_noheader_buffer(char *obuffer, int olen, const char *ibuffer, int ilen);

extern int      stbi_png_test_memory      (stbi_uc const *buffer, int len);
extern stbi_uc *stbi_png_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
extern int      stbi_png_info_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp);

extern int      stbi_bmp_test_memory      (stbi_uc const *buffer, int len);

extern stbi_uc *stbi_bmp_load             (char const *filename,     int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_bmp_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

extern int      stbi_hdr_test_memory      (stbi_uc const *buffer, int len);

extern float *  stbi_hdr_load             (char const *filename,     int *x, int *y, int *comp, int req_comp);
extern float *  stbi_hdr_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_hdr_load_rgbe        (char const *filename,           int *x, int *y, int *comp, int req_comp);
extern float *  stbi_hdr_load_from_memory (stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

typedef struct
{
   int       (*test_memory)(stbi_uc const *buffer, int len);
   stbi_uc * (*load_from_memory)(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);
} stbi_loader;

void start_mem(stbi *s, unsigned char const *buffer, int len);
stbi_uc get8(stbi *s);
int at_eof(stbi *s);
unsigned char get8u(stbi *s);
void skip(stbi *s, int n);
int get16(stbi *s);
unsigned int get32(stbi *s);
int get16le(stbi *s);
unsigned int get32le(stbi *s);
void getn(stbi *s, stbi_uc *buffer, int n);
unsigned char compute_y(int r, int g, int b);

#endif

