#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
}		t_zone;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char cha;
}		t_shape;

int	ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return(i);
}

int	ft_error(char *s)
{
	if (s)
		write(1, s, ft_strlen(s));
	return(1);
}

int	clear(FILE *file, char *drawing, char *s)
{
	if (file)
		fclose(file);
	if (drawing)
		free(drawing);
	if (s)
		ft_error(s);
	return(1);
}

void	draw(char *drawing, t_zone *zone)
{
	int y = 0;

	while (y < zone->height)
	{
		write(1, drawing + (y * zone->width), zone->width);
		write(1, "\n", 1);
		y++;
	}
}

int	within_rec(float x, float y, t_shape *shape)
{
	if (x < shape->x || shape->x + shape->width < x || y < shape->y || shape->y + shape->height < y)
		return(0);
	if (x - shape->x < 1.00000000 || shape->x + shape->width - x < 1.0000000 || y - shape->y < 1.00000000 || shape->y + shape->height - y < 1.000000000)
		return (2);
	return(1);
}

void	draw_rec(t_shape *shape, t_zone *zone, char *drawing)
{
	int x;
	int y;
	int within;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			within = within_rec((float)x, (float)y, shape);
			if ((shape->type == 'r' && within == 2) || (shape->type == 'R' && within))
				drawing[(y * zone->width) + x] = shape->cha;
			x++;
		}
		y++;
	}
}

int	draw_shapes(t_zone *zone, FILE *file, char *drawing)
{
	t_shape shape;
	int ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.cha)) == 6)
	{
		if (shape.width <= 0.000000000 || shape.height <= 0.000000000 || (shape.type != 'r' && shape.type != 'R'))
			return (0);
		draw_rec(&shape, zone, drawing);
	}
	if (ret != -1)
		return (0);
	return (1);
}

char *get_zone(t_zone *zone, FILE *file)
{
	char *drawing;
	int i;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(drawing = (char *)malloc(sizeof(*drawing) * (zone->height * zone->width))))
		return (NULL);
	i = 0;
	while (i < zone->height * zone->width)
		drawing[i++] = zone->background;
	return (drawing);
}

int main(int argc, char **argv)
{
	t_zone zone;
	char *drawing;
	FILE *file;
	
	zone.width = 0;
    	zone.height = 0;
   	zone.background = 0;

	if (argc != 2)
		return(ft_error("Error: Argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return(ft_error("Error: Operation file corrupted\n"));
	if (!(drawing = get_zone(&zone, file)))
		return(clear(file, NULL, "Error: Operation file corrupted\n"));
	if (!(draw_shapes(&zone, file, drawing)))
		return(clear(file, drawing, "Error: Operation file corrupted\n"));
	draw(drawing, &zone);
	clear(file, drawing, NULL);
	return(0);
}

