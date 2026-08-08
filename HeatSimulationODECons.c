#include <stdio.h>
#include <unistd.h>

#define ODESOLVERLIB_IMPLEMENTATION
#include "ODEsolverlib.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define GRAPHLIB_IMPLEMENTATION
#include "graphlib.h"

#define NB_FRAME 1000
#define MESHGRID 100
#define WIDTH (16*10)
#define HEIGHT (9*8)

#define alpha 0.1f
#define UNUSED(x) (void)x


#define STAT_ALLOC(src, dest, type, h)			\
    type *dest[h];					\
    do {						\
	for (int i = 0; i < (h); i++) dest[i] = src[i];	\
    } while (0)


    // char palette[] = {' ', '`', '.', ':', ';', '*', '!', '/', '^', 'o', 'O', 'G', 'M', '%', '#', '@'};
char palette[] = {' ', '`', '.', ':', ';', ',', '-', '~', '_', '^','"', '\'', '!', '/', '\\', '|', '(', ')', '[', ']','{', '}', '<', '>', '+', '=', '*', 'x', 'X', 'o','O', '0', 'Q', 'D', 'B', 'P', 'M', 'W', '&', '$','8', '9', '#', '%', '@', 'A', 'K', 'R', 'U', 'Z'};
// char palette[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

double norm(double x, double xmin, double xmax)
{
    return (x - xmin)/(xmax - xmin);
}

int heat_equ_raw(int i, int j, double h, double **T, double p)
{
    // TODO: Find the right Diff2Cent3p2D for the differentes edge t_i-2,j-2 with i=0 and j=2 => t_-2,0
    // return alpha * Diff2Cent3p2D(h, T[i+1][j], T[i][j+1], p, T[i][j-1], T[i-1][j]);
    return alpha * Diff2Cent5p2D(h, T[i+2][j], T[i+1][j], T[i][j+2], T[i][j+1], p, T[i][j-1], T[i][j-2], T[i-1][j], T[i-2][j]);
}


int main()
{
    Window wind;
    SetMeshGrid(&wind, WIDTH, HEIGHT, MESHGRID);

    double pl[wind.y][wind.x];
    for (int i = 0; i < wind.y; i++) {
	for (int j = 0; j < wind.x; j++) {
	    pl[i][j] = 0;
	}
    }

    // for (int i = 1; i < wind.y-1; i++) pl[i][1] = 5000;
    // for (int i = 1; i < wind.y-1; i++) pl[i][wind.x-2] = 5000;
    // for (int i = 1; i < wind.x-1; i++) pl[1][i] = 5000;
    // for (int i = 1; i < wind.x-1; i++) pl[wind.y-2][i] = 5000;
    
    pl[wind.y/2][wind.x/2] = 5000;
    
    GRAPHLIB_MALLOC2D(char, console, HEIGHT, WIDTH);
    
    STAT_ALLOC(pl, T, double, wind.y);

    double Tmin = 0, Tmax = 20;
    double t = 0;
    double h = 0.1f;
    for (int i = 0; i < NB_FRAME; i++) {
	ConsoleClear(console, WIDTH, HEIGHT, ' ');

	for (int y = 2; y < wind.y-2; y++) {
	    for (int x = 2; x < wind.x-2; x++) {

		double heat_equ(double t, double p, double v)
		{
		    UNUSED(t);
		    UNUSED(v);
		    return heat_equ_raw(y, x, h, T, p);
		}

		double tmp = 0;
		if (RK4(h, t, &T[y][x], &tmp, heat_equ) < 0)
		    fprintf(stderr, "WARNING: Error calculation\n");

		double T_norm = norm(T[y][x], Tmin, Tmax);
		int Light = (int)(T_norm*(sizeof(palette)-1))%sizeof(palette);

		PrintRectangle(console, WIDTH, HEIGHT, MESHOFFSET_X(wind, x), MESHOFFSET_X(wind, y), wind.grid, wind.grid, palette[Light]);
	    }
	}
	// PrintConsoleSpace(console, WIDTH, HEIGHT);
	PrintConsole(console, WIDTH, HEIGHT);
	// usleep(50000);

	// pl[wind.y/2][wind.x/2] = 2000;
	
	// for (int i = 1; i < wind.y-1; i++) pl[i][1] = 20;
	// for (int i = 1; i < wind.y-1; i++) pl[i][wind.x-2] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[1][i] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[wind.y-2][i] = 20;
    }
    GRAPHLIB_FREE2D(console, HEIGHT);
    return 0;
}
