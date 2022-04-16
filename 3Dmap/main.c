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

/* name of xpm textures */
#define EAGLE 0
#define REDBRICK 1
#define PURPLESTONE 2
#define GRAYSTONE 3
#define BLUESTONE 4
#define MOSSY 5
#define WOOD 6
#define COLORSTONE 7
#define BARRAL 8
#define PILLAR 9
#define GREENLIGHT 10

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

	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
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

/* ray casting using lodev algorithm */
void rayCasting(t_info *info)
{
	/* Floor casting */
	for (int y = HEIGHT / 2 + 1; y < HEIGHT; ++y)
	{
		// ray vector for leftmost & rightmost ray
		float rayDirX0 = info->dirX - info->planeX;
		float rayDirY0 = info->dirX - info->planeY;
		float rayDirX1 = info->dirX + info->planeX;
		float rayDirY1 = info->dirY + info->planeY;
		// Current y position starts from horizontal half of screen
		int p = y - HEIGHT / 2;
		// Vertical position of the camera
		float posZ = 0.5 * HEIGHT;
		// Horizontal distance from the camera to the floor for the current row.
		// 0.5 is the z position exactly in the middle between floor and ceiling.
		float rowDistance = posZ / p;
		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		float deltaX = rowDistance * (rayDirX1 - rayDirX0) / WIDTH;
		float deltaY = rowDistance * (rayDirY1 - rayDirY0) / WIDTH;
		// real world coordinates of the leftmost column. This will be updated as we step to the right.
		float floorX = info->posX + rowDistance * rayDirX0;
		float floorY = info->posY + rowDistance * rayDirY0;
		for (int x = 0; x < WIDTH; ++x)
		{
			int cellX = (int)floorX;
			int cellY = (int)floorY;
			// get the texture coordinate from the fractional part
			int tx = (int)(TEXW * (floorX - cellX)) & (TEXW - 1);
			int ty = (int)(TEXW * (floorY - cellY)) & (TEXH - 1);
			// make floor vector point next step.
			floorX += deltaX;
			floorY += deltaY;
			// draw floor
			int color;
			color = info->texture[GRAYSTONE][TEXW * ty + tx];
			color = (color >> 1) & 8355711; // make a bit darker
			// fill screen buffer with pixel
			info->buf[y][x] = color;
			//ceiling (symmetrical, at height - y - 1 instead of y)
			color = info->texture[WOOD][TEXW * ty + tx];
			color = (color >> 1) & 8355711; // make a bit darker
			info->buf[HEIGHT - y - 1][x] = color;
		}
	}
}

int main_loop(t_info *info)
{
	rayCasting(info);
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

/* set vectors for ray casting */
	info.posX = 22.0;
	info.posY = 11.5;
	info.dirX = -1.0;
	info.dirY = 0.0;
	info.planeX = 0.0;
	info.planeY = 0.66; /* FOV = 67 degree */

/* catching event */
	mlx_loop_hook(info.mlx, &main_loop, &info);
	
	mlx_loop(info.mlx);
}
