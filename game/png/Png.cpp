#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>

struct mem_block {
	mem_block(char *data) 
	: data(data)
	, index(0)
	{}
	char *data;
	int index;
};

static void read_mem_block(png_structp png_ptr, png_bytep buffer, png_size_t length) {
	mem_block *mb = (mem_block *)pngptr->io_ptr;
	memcpy(buffer, mb->data + mb->index, length);
	mb->index += length;
}

static void write_mem_block(png_structp png_ptr, png_bytep buffer, png_size_t length) {
	mem_block *mb = (mem_block *)pngptr->io_ptr;
	memcpy(mb->data + mb->index, buffer, length);
	mb->index += length;
}

void read_png_file(char *compressed, char *&uncompressed, int &x, int &y)
{
	mem_block block(compressed);
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr) throw 1;

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) throw 1;

        png_set_read_fn(png_ptr, &block, read_mem_block);

        png_read_info(png_ptr, info_ptr);

        x = png_get_image_width(png_ptr, info_ptr);
        y = png_get_image_height(png_ptr, info_ptr);

        png_read_update_info(png_ptr, info_ptr);

	uncompressed = malloc(x * y * 4);
        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (yp=0; yp<height; yp++)
                row_pointers[yp] = (png_byte*) (uncompressed + 4 * x * yp);
        free(row_pointers);

        png_read_image(png_ptr, row_pointers);

        fclose(fp);
}

void write_png_file(char* uncompressed, char *&compressed, int x, int y)
{
	compressed = malloc(x * y * 4 + 4096);
	mem_block block(compressed);
	
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) throw 1;

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) throw 1;

        png_set_write_fn(png_ptr, &block, write_mem_block);

        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (yp=0; yp<height; yp++)
                row_pointers[yp] = (png_byte*) (uncompressed + 4 * x * yp);
        free(row_pointers);

        png_write_image(png_ptr, row_pointers);

        png_write_end(png_ptr, NULL);

        fclose(fp);
}


