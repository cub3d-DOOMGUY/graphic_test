#include "mlx.h"

typedef struct s_img
{
	void *img;
	int *data;
	int size_line;
	int bpp;
	int endian;
	int img_width;
	int img_heigth;
} t_img;

typedef struct s_info
{
	void *mlx;
	void *win;
	t_img img;
} t_info;

int main()
{
	t_info info;
	t_img *img;

	info.mlx = mlx_init();
	info.win = mlx_new_window(info.mlx, 640, 480, "test");
	img = mlx_new_image(info.mlx, 640, 480);
	info.img.data = (int *)mlx_get_data_addr(img, &img->bpp, &img->size_line, &img->endian);

	mlx_loop(info.mlx);
	return (0);
}
