#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
    int width; //int
    int height; //attention ordre, width avant height
    char background;
}               t_zone;

typedef struct s_shape
{
    char type;
    float x; //attention ordre
    float y;
    float width;
    float height;
    char cha;
}           t_shape;

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
        write (1, s, ft_strlen(s));
    return (1);
}


//file en param ici
int clear(FILE *file, char *drawing, char *s)
{
    if (file)
        fclose(file); //fclose
    if (drawing)
        free(drawing);
    if (s)
        ft_error(s);
    return (1);
}

//file en param ici
char *get_zone(FILE *file, t_zone *zone)
{
    char *drawing;
    int i;

    //scan, 2 d et un c
    if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
        return (NULL); //ici return null si error
    //check width et height
    if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
        return (NULL);
    //malloc drawing de width * height, sizeof *drawing
    if (!(drawing = (char *)malloc(sizeof(*drawing) * (zone->width * zone->height))))
        return (NULL);
    //fille drawing
    i = 0;
    while (i < zone->width * zone->height)
        drawing[i++] = zone->background;
    return (drawing);
}

int within_rec(float x, float y, t_shape *shape)
{
    //check si hors rectangle
    //x < shapex / shapex + width < x
    if (x < shape->x || shape->x + shape->width < x || y < shape->y || shape->y + shape->height < y)
        return (0);
    //check if bordure
    //x - shapex < 1 / shapx + width - x < 1
    if ((x - shape->x < 1.00000000) || (shape->x + shape->width - x < 1.00000000) || (y - shape->y < 1.00000000) || (shape->y + shape->height - y < 1.00000000))
        return (2);
    //check if within
    return (1);
}

//seule ft qui prend shape en param avec within rec
void    draw_rec(t_shape *shape, t_zone *zone, char *drawing)
{
    //decla
    int x;
    int y;
    int within;

    //boucle imbriquee
    y = 0;
    while (y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            within = within_rec(x, y, shape);
            if ((shape->type == 'r' && within == 2) || (shape->type == 'R' && within))
                drawing[y * zone->width + x] = shape->cha;
            x++;
        }
        y++;
    }
}

//return int ici, void pour les autres
int draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
    t_shape shape;
    int ret;

    //boucle while pour choper tous les rec
    //ne pas oublier le ret
    while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.cha)) == 6)
    {
        if (shape.width <= 0.00000000 || shape.height <= 0.00000000 || (shape.type != 'r' && shape.type != 'R'))
            return (0); //on ignore les rectangles qui ont largeur ou hauteur inf a 0 ou mauvais type
        draw_rec(&shape, zone, drawing);
    }
    if (ret != -1)
        return (0);
    return (1); //en cas de scan impossible

}

void    draw(t_zone *zone, char *drawing)
{
    int y = 0;
    //on itere juste sur y
    while (y < zone->height)
    {
        write(1, drawing + (y * zone->width), zone->width);
        write(1, "\n", 1);
        y++;
    }
}

int main(int argc, char **argv)
{
    //declaration
    t_zone zone;
    char *drawing;
    FILE *file;

    //set to 0
    zone.height = 0;
    zone.width = 0;
    zone.background = 0;

    //check argc
    if (argc != 2)
        return(ft_error("Error: Argument\n"));
    //open file
    if (!(file = fopen(argv[1], "r")))
        return(ft_error("Error: Operation file corrupted\n"));
    //set drawing
    if (!(drawing = get_zone(file, &zone)))
        return (clear(file, NULL, "Error: Operation file corrupted\n")); //ici drawing est null
    //draw
    if (!(draw_shapes(file, &zone, drawing)))
        return(clear(file, drawing, "Error: Operation file corrupted\n"));
    draw(&zone, drawing);
    //clear and return
    clear(file, drawing, NULL); //no error
    return (0);
}