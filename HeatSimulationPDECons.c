#include <stdio.h>
#include <unistd.h>

#define DEVER_IMPLEMENTATION
#include "dever.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define GRAPHLIB_IMPLEMENTATION
#include "graphlib.h"

#define NB_FRAME 1000
#define MESHGRID 100
#define WIDTH (16*8)
#define HEIGHT (9*8)

#define alpha 0.01f
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

void map_init(double **map, int x, int y)
{
    for (int i = 0; i < y; i++) {
	for (int j = 0; j < x; j++) {
	    map[i][j] = 0;
	}
    }
    // for (int i = 1; i < y-1; i++) map[i][1] = 5000;
    // for (int i = 1; i < y-1; i++) map[i][x-2] = 5000;
    // for (int i = 1; i < x-1; i++) map[1][i] = 5000;
    // for (int i = 1; i < x-1; i++) map[y-2][i] = 5000;
    // map[y/2][x/2] = 5000;
}

int main()
{
    Window wind;
    SetMeshGrid(&wind, WIDTH, HEIGHT, MESHGRID);

    GRAPHLIB_MALLOC2D(char, console, WIDTH, HEIGHT);

    double pl[wind.y][wind.x];
    STAT_ALLOC(pl, T, double, wind.y);
    map_init(T, wind.x, wind.y);
    double plnew[wind.y][wind.x];
    STAT_ALLOC(plnew, Tn, double, wind.y);
    map_init(Tn, wind.x, wind.y);

    double Tmin = 0, Tmax = 20;
    double t = 0;
    const double dt = 0.1f;
    const double dxy = 0.1f;
    for (int i = 0; i < NB_FRAME; i++) {
	ConsoleClear(console, WIDTH, HEIGHT, ' ');

	for (int y = 1; y < wind.y-1; y++) {
	    for (int x = 1; x < wind.x-1; x++) {
		double T_norm = norm(T[y][x], Tmin, Tmax);
		int Light = (int)(T_norm*(sizeof(palette)-1))%sizeof(palette);

		PrintRectangle(console, WIDTH, HEIGHT, MESHOFFSET_X(wind, x), MESHOFFSET_X(wind, y), wind.grid, wind.grid, palette[Light]);

		Tn[y][x] = T[y][x] + dt * alpha * FDM2Cent3p2D(dxy, T[y+1][x], T[y][x+1], T[y][x], T[y][x-1], T[y-1][x]);

	    }
	}

	for (int y = 0; y < wind.y-0; y++) {
	    for (int x = 0; x < wind.x-0; x++) {
		T[y][x] = Tn[y][x];
	    }
	}
	
	pl[wind.y/2][wind.x/2] = 200;
	// PrintConsoleSpace(console, WIDTH, HEIGHT);
	PrintConsolePadded(console, WIDTH, HEIGHT);
	printf("t = %0.2lf    \n", t);
	// usleep(50000);
	t += dt;
	// for (int i = 1; i < wind.y-1; i++) pl[i][1] = 20;
	// for (int i = 1; i < wind.y-1; i++) pl[i][wind.x-2] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[1][i] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[wind.y-2][i] = 20;
    }
    GRAPHLIB_FREE2D(console, HEIGHT);
    return 0;
}

/*
* TODO: Find the right FDM2Cent3p2D for the differentes edge t_i-2,j-2 with i=0 and j=2 => t_-2,0
* Here is are problems:
* 
* 1: The Most efficient method -> 1 buffer:
*     [ a x x ] (a is the first pixel and b the last)
*     [ x x x ]
*     [ x x b ]
* b(t=i) is calculated with a(t=i+1) and not a(t=i)
* The real problem here, to solve this problem, is that we need two buffers that require a lot of memory.
*
* while(1) {
*     double **T;
*     for (int i = 0; i < HEIGHT; i++) {
*         for (int j = 0; j < WIDTH; j++) {
*             T[i][j] = T[i][j] + dt * heat_equation;
*         }
*     }
* }
*
* 2: The Best visual one:
*     2 buffer + 2 for loop to update 1 buffer
*
* while(1) {
*     double **T;
*     double **Tn;
*     for (int i = 0; i < HEIGHT; i++) {
*         for (int j = 0; j < WIDTH; j++) {
*             Tn[i][j] = T[i][j] + dt * heat_equation;
*         }
*     }
*     for (int i = 0; i < HEIGHT; i++) {
*         for (int j = 0; j < WIDTH; j++) {
*             T[i][j] = Tn[i][j];
*         }
*     }
* }
* 
* 3: The Red-Black ordering (chessboard)
*     1 buffer + 2 for loop to do the odd part
*
* while(1) {
*     double **T;
*     for (int i = 0; i < HEIGHT; i++) {
*         for (int j = 0; j < WIDTH; j++) {
*             if ((i+j)%2 == 0) {
*                 T[i][j] = T[i][j] + dt * heat_equation;
*             }
*         }
*     }
*     for (int i = 0; i < HEIGHT; i++) {
*         for (int j = 0; j < WIDTH; j++) {
*             if ((i+j)%2 == 1) {
*                 T = T + dt * heat_equation;
*             }
*         }
*     }
* }
*
*/
