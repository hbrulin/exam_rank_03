#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
    int width;
    int height;
    char background;
}               t_zone;

typedef struct s_shape
{
    char type;
    float x;
    float y;
    float radius;
    char cha;
}               t_shape;

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

int clear(FILE *file, char *drawing, char *s)
{
    if (file)
        fclose(file);
    if (drawing)
        free(drawing);
    if (s)
        ft_error(s);
    return (1);
}


char *get_zone(t_zone *zone, FILE *file)
{
    char *drawing;
    int i = 0;

    if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
        return (NULL);
    if (zone->height > 300 || zone->height <= 0 || zone->width > 300 || zone->width <= 0)
        return (NULL);
    if (!(drawing = (char *)malloc(sizeof(*drawing) * (zone->height * zone->width)))) //parentheses importantes
        return (NULL);
    while (i < zone->height * zone->width)
        drawing[i++] = zone->background;
    return (drawing);
}

int within_circle(float x, float y, t_shape *shape)
{
    float dist;
    dist = sqrtf(powf(x - shape->x, 2.) + powf(y - shape->y, 2.));
    if (dist <= shape->radius)
    {
        //dans cet ordre
        if (shape->radius - dist < 1.00000000)
            return (2);
        return(1);
    }
    return (0);
}

void    draw_circle(t_shape *shape, t_zone *zone, char *drawing)
{
    int x;
    int y;
    int within;

    y = 0;
    while(y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            within = within_circle((float)x, (float)y, shape); //float important
            if ((shape->type == 'r' && within == 2) || (shape->type == 'R' && within))
                drawing[y * zone->width + x] = shape->cha;
            x++;
        }
        y++;
    }
}

int     draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
    int ret;
    t_shape shape;

    while ((ret = fscanf(file, "%c %f %f %f %c", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.cha)) == 5)
    {
        if (shape.radius <= 0.00000000 || (shape.type != 'c' && shape.type != 'C'))
            return (0);
        draw_circle(&shape, zone, drawing);
    }
    if (ret != -1)
        return (0);
    return (1);
}

void    draw(t_zone *zone, char *drawing)
{
    int y = 0;

    while (y < zone->height * zone->width)
    {
        write(1, drawing + (y * zone->width), zone->width);
        write (1, "\n", 1); //ne pas oublier retour
        y++;
    }
}

int main (int argc, char **argv)
{
    t_zone zone;
    FILE *file;
    char *drawing;

    zone.width = 0;
    zone.height = 0;
    zone.background = 0;

    if (argc != 2)
        return(ft_error("Error: Argument\n"));
    if (!(file = fopen(argv[1], "r")))
        return(ft_error("Error: Operation file corrupted\n"));
    if (!(drawing = get_zone(&zone, file)))
        return(clear(file, NULL, "Error: Operation file corrupted\n"));
    if (!(draw_shapes(file, &zone, drawing)))
        return(clear(file, drawing, "Error: Operation file corrupted\n"));
    draw(&zone, drawing);
    clear(file, drawing, NULL);
    return(0);
}