#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef	struct s_zone
{
	int width;
	int height;
	char background;
}				t_zone;

typedef	struct s_shape
{
	char type;
	float x;
	float y;
	float radius;
	char cha;
}			t_shape;

void	ft_bzero(void *s, size_t n)
{
	int		i;
	char	*start;

	i = 0;
	start = s;
	while (n)
	{
		start[i] = 0;
		i++;
		n--;
	}
}

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int ft_error(char *s)
{
	if (s)
		write(1, s, ft_strlen(s));
	return (1);
}

int clear(FILE *file, char *drawing, char const *str)
{
	if (file)
		fclose(file);
	if (drawing)
		free(drawing);
	if (str)
		ft_error((char *)str);
	return (1);
}

int		within_circle(float x, float y, t_shape *shape)
{
	float dist;
	dist = sqrtf(powf(x - shape->x, 2.) + powf(y - shape->y, 2.));
	if (dist <= shape->radius)
	{
		if ((shape->radius - dist) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	draw(t_zone *zone, char *drawing)
{
	int i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write (1, "\n", 1);
		i++;
	}
}

void	draw_circle(t_zone *zone, char *drawing, t_shape *shape)
{
	int y;
	int x;
	int within;

	y = 0;
	while (y < zone->height)
	{
		x  = 0;
		while (x < zone->width)
		{
			within = within_circle((float)x, (float)y, shape);
			if ((shape->type == 'c' && within == 2) || (shape->type == 'C' && within))
				drawing[(y * zone->width) + x] = shape->cha;
			x++;
		}
		y++;
	}
}


int		draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
	t_shape shape;
	int ret;

	while (( ret = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.cha)) == 5)
	{
		if (shape.radius <= 0.00000000 || (shape.type != 'c' && shape.type != 'C'))
			return (0);
		draw_circle(zone, drawing, &shape);
	}
	if (ret != -1)
		return (0);
	return (1); //si pb
}

char *get_zone(FILE *file, t_zone *zone)
{
	int i;
	char *drawing;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(drawing = (char *)malloc(sizeof(*drawing) * (zone->width * zone->height))))
		return (NULL);
	i = 0;
	while (i < zone->width * zone->height)
		drawing[i++] = zone->background;
	return (drawing);
}


int main(int argc, char **argv)
{
	FILE	*file;
	t_zone	zone;
	char *drawing;

	ft_bzero(&zone, sizeof(t_zone));
	if (argc != 2)
		return (ft_error("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_error("Error: Operation file corrupted\n"));
	if (!(drawing = get_zone(file, &zone)))
		return (clear(file, NULL, "Error: Operation file corrupted\n"));
	if (!(draw_shapes(file, &zone, drawing)))
		return (clear(file, drawing, "Error: Operation file corrupted\n"));
	draw(&zone, drawing);
	clear(file, drawing, NULL);
	return (0);
}	
