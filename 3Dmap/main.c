#include <stdlib.h>

#include "mlx.h"

/* window size */
#define WIDTH 640
#define HEIGHT 480

/* number of img files */
#define XPM_NUM 11

/* size of img file */
#define TEXW 64
#define TEXH 64

typedef struct s_img
{
	void *img;
	int img_width;
	int img_height;

	int *data;
	int size_line;
	int bpp;
	int endian;
} t_img;

typedef struct s_info
{
	void *mlx;
	void *win;

	t_img img;

	int buf[HEIGHT][WIDTH];
	int **texture;
} t_info;

void load_image(t_info *info, int *texture, char *path, t_img *img)
{
/* store info.texture with xpm image, and destroy temporary img identifier. */
	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->img_width, &img->img_height);
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_line, &img->endian);
	for (int y = 0; y < img->img_height; ++y)
		for (int x = 0; x < img->img_width; ++x)
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
	mlx_destroy_image(info->mlx, img->img);
}

void load_texture(t_info *info)
{
/* load xpm textures, store in info structure. */
	t_img	img; /* it is a temporary container to store info.texture */

	load_image(info, info->texture[0], "textures/eagle.xpm", &img);
	load_image(info, info->texture[1], "textures/redbrick.xpm", &img);
	load_image(info, info->texture[2], "textures/purplestone.xpm", &img);
	load_image(info, info->texture[3], "textures/greystone.xpm", &img);
	load_image(info, info->texture[4], "textures/bluestone.xpm", &img);
	load_image(info, info->texture[5], "textures/mossy.xpm", &img);
	load_image(info, info->texture[6], "textures/wood.xpm", &img);
	load_image(info, info->texture[7], "textures/colorstone.xpm", &img);
	load_image(info, info->texture[8], "textures/barrel.xpm", &img);
	load_image(info, info->texture[9], "textures/pillar.xpm", &img);
	load_image(info, info->texture[10], "textures/greenlight.xpm", &img);
}

/* draw screen using info.buf */
void draw(t_info *info)
{
	for (int y = 0; y < HEIGHT; ++y)
		for (int x = 0; x < WIDTH; ++x)
			info->img.data[y * WIDTH + x] = info->buf[y][x];
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

int main_loop(t_info *info)
{
	draw(info);
	return (0);
}

int main()
{
	t_info info;
	info.mlx = mlx_init();

/* buffer(screen pixels) initialize */
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			info.buf[i][j] = 0;
	
/* texture initialize. each texture pixel stored 1D array. */
	info.texture = malloc(sizeof(int *) * XPM_NUM);
	for (int i = 0; i < XPM_NUM; ++i)
		info.texture[i] = malloc(sizeof(int) * (TEXH * TEXW));
	for (int i = 0; i < XPM_NUM; ++i)
		for (int j = 0; j < TEXH * TEXW; j++)
			info.texture[i][j] = 0;

/* store all xpm image's pixels in info.texture. */
	load_texture(&info);

/* create new empty window */
	info.win = mlx_new_window(info.mlx, WIDTH, HEIGHT, "3Dmap");
	info.img.img = mlx_new_image(info.mlx, WIDTH, HEIGHT);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_line, &info.img.endian);

	mlx_loop_hook(info.mlx, &main_loop, &info);
	
	mlx_loop(info.mlx);
}
