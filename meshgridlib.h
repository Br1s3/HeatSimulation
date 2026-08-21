#ifndef MESHGRIDLIB_H_INCLUED
#define MESHGRIDLIB_H_INCLUED

#define MESHOFFSET_X(M, px) (M.Offset.x + (M.grid*px))
#define MESHOFFSET_Y(M, py) (M.Offset.y + (M.grid*py))


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

int SetMeshGrid(Window *win, unsigned int w, unsigned int h, unsigned int meshgrid);

# ifdef MESHGRIDLIB_IMPLEMENTATION


int SetMeshGrid(Window *win, unsigned int w, unsigned int h, unsigned int meshgrid)
{
    if (meshgrid == 0) {
	// WARNING: The parameter meshgrid == 0 so there is a division by 0
	meshgrid = 1;
	return -1;
    }
    win->grid = ((h + w)/meshgrid);
    if (win->grid == 0) {
	// WARNING: Grid < pixel size
	win->grid = 1;
    }
    win->x = w/win->grid;
    win->y = h/win->grid;
    win->Offset.x = ((int)((((float)w/(float)win->grid) - (w/win->grid))*win->grid/2));
    win->Offset.y = ((int)((((float)h/(float)win->grid) - (h/win->grid))*win->grid/2));
    return 0;
}

# endif // MESHGRIDLIB_IMPLEMENTATION
#endif // MESHGRIDLIB_H_INCLUED
