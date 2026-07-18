#ifndef MESHGRIDLIB_H_INCLUED
#define MESHGRIDLIB_H_INCLUED
#include <stdio.h>

typedef struct
{
    int grid;
    
    struct
    {
	int x, y;
    } Offset;
    
    int x;
    int y;
} Window;

void SetMeshGrid(Window *win, unsigned int w, unsigned int h, unsigned int meshgrid);

# ifdef MESHGRIDLIB_IMPLEMENTATION


void SetMeshGrid(Window *win, unsigned int w, unsigned int h, unsigned int meshgrid)
{
    if (meshgrid <= 0 || meshgrid > (h + w)) {
	fprintf(stdout, "WARNING: The meshgrid parameter is <= 0 or > %d\n", (h + w));
	meshgrid = 1;
    }
    win->grid = meshgrid;
    // win->grid = ((h + w)/meshgrid);
    
    win->x = w/win->grid;
    win->y = h/win->grid;
    win->Offset.x = ((int)((((float)w/(float)win->grid) - (w/win->grid))*win->grid/2));
    win->Offset.y = ((int)((((float)h/(float)win->grid) - (h/win->grid))*win->grid/2));
}

# endif // MESHGRIDLIB_IMPLEMENTATION
#endif // MESHGRIDLIB_H_INCLUED
