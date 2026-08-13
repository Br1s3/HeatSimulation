#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <string.h>

#define DEVER_IMPLEMENTATION
#include "dever.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define FPS 100
#define MESHGRID 200
#define WIDTH (16*100)
#define HEIGHT (9*100)

#define alpha 0.01f

#define BLACKPURPLE(x) (Color){(x)*0xff, 0x00, (x)*0xff, 0xff}
#define BLUERED(x) (Color){0xff - (1-x)*0xff/2, 0x00, 0xff - (x)*0xff/2, 0xff}

typedef struct {
    int x;
    int y;
} Vec2;

#define STAT_ALLOC(src, dest, type, h)			\
    type *dest[h];					\
    do {						\
	for (int i = 0; i < (h); i++) dest[i] = src[i];	\
    } while (0)

double norm(double x, double xmin, double xmax)
{
    return (x - xmin)/(xmax - xmin);
}

void map_init(double **map, int x, int y)
{
    for (int i = 0; i < x; i++) {
	for (int j = 0; j < y; j++) {
	    map[i][j] = 0;
	}
    }
    // for (int i = 1; i < y-1; i++) map[1][i] = 2000;
    // for (int i = 1; i < y-1; i++) map[x-2][i] = 2000;
    // for (int i = 1; i < x-1; i++) map[i][1] = 2000;
    // for (int i = 1; i < x-1; i++) map[i][y-2] = 2000;
    // map[x/2][y/2] = 5000;
}

void AddHeatwithMouse(int IsClicking, Window wind, double **map)
{
    if (IsClicking == 1) {
	Vector2 MousePosf = GetMousePosition();
	Vec2 MousePos = {(int)MousePosf.x/wind.grid, (int)MousePosf.y/wind.grid};
	if (MousePos.x > wind.x-2) MousePos.x = wind.x-2;
	if (MousePos.x < 2) MousePos.x = 2;
	if (MousePos.y > wind.y-2) MousePos.y = wind.y-2;
	if (MousePos.y < 2) MousePos.y = 2;
	map[MousePos.y][MousePos.x] = 100.f;
    }
}

// double HeatEquation(double t, double p, double v);

int main()
{
    InitWindow(WIDTH, HEIGHT, "Simulation of the Heat Equation");
    SetTargetFPS(FPS);

    Window wind;
    SetMeshGrid(&wind, WIDTH, HEIGHT, MESHGRID);

    double pl[wind.y][wind.x];
    STAT_ALLOC(pl, T, double, wind.y);
    map_init(T, wind.y, wind.x);
    double pln[wind.y][wind.x];
    STAT_ALLOC(pln, Tn, double, wind.y);
    map_init(Tn, wind.y, wind.x);

    double Tmin = 0, Tmax = 20;
    double t = 0;

/* Stability condition:
 * dt <= dx²/(4α)
 * if dx=0.1 & α=0.01 then dt<=0.25
 */

    const double dt = 0.1f;
    const double dx = 0.1f;

#define DERICH
// #define PERIODIC
    char tab[30] = {' '};
    snprintf(tab, 30, "ERROR: Calculation overflow");
    while (!WindowShouldClose()) {
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}
	AddHeatwithMouse(IsMouseButtonDown(MOUSE_BUTTON_LEFT), wind, T);

	BeginDrawing();
	ClearBackground(BLACK);
	for (int y = 0; y < wind.y-0; y++) {
	    for (int x = 0; x < wind.x-0; x++) {
		double T_norm = norm(T[y][x], Tmin, Tmax);
		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, BLUERED(T_norm));

#ifdef DERICH
		// Boundary condition: Derichlet -> T(t, 0, y) = T(t, x, 0) = 0
		int xb = x, yb = y;
		// if (yb < 1 || xb < 1 || xb > wind.x-2 || yb > wind.y-2) continue;
		if (yb < 2 || xb < 2 || xb > wind.x-3 || yb > wind.y-3) continue;
#endif
#ifdef PERIODIC
                // Boundary condition: Periodic -> T[HEIGHT][x] <=> T[0][x] and T[y][WIDTH] <=> T[y][0]
		int xb = x, yb = y;
		if (yb-1 < 0)             yb = wind.y-2;
		else if (yb+1 > wind.y-1) yb = 1;
		if (xb-1 < 0)             xb = wind.x-2;
		else if (xb+1 > wind.x-1) xb = 1;
#endif

		double HeatEquation(double t, double p, double v)
		{
		    (void)t; (void)p; (void)v;
		    // return alpha * FDM2Cent3p2D(dx, T[yb+1][xb], T[yb][xb+1], T[yb][xb], T[yb][xb-1], T[yb-1][xb]);
		    return alpha * FDM2Cent5p2D(dx, T[yb+2][xb], T[yb+1][xb], T[yb][xb+2], T[yb][xb+1], T[yb][xb], T[yb][xb-1], T[yb][xb-2], T[yb-1][xb], T[yb-2][xb]);
		}
		double Tnext = T[yb][xb];
		double tmp;
		if (ExplicitEuler(dt, t, &tmp, &Tnext, HeatEquation) < 0)
		    DrawText(tab, WIDTH/2-200, 10, 50, RED);
		Tn[y][x] = Tnext;
		// Tn[y][x] = T[yb][xb] + dt * alpha * FDM2Cent3p2D(dx, T[yb+1][xb], T[yb][xb+1], T[yb][xb], T[yb][xb-1], T[yb-1][xb]);
		// Tn[y][x] = T[y][x] + dt * alpha * FDM2Cent5p2D(dx, T[yb+2][xb], T[yb+1][xb], T[yb][xb+2], T[yb][xb+1], T[yb][xb], T[yb][xb-1], T[yb][xb-2], T[yb-1][xb], T[yb-2][xb]);
	    }
	}
	// printf("%.2lf,%lf\n", t, norm(T[wind.y/2][wind.x/2], Tmin, Tmax));

	// memcpy(&T[0][0], &Tn[0][0], wind.x*wind.y*sizeof(double));
	for (int y = 0; y < wind.y-0; y++) {
	    for (int x = 0; x < wind.x-0; x++) {
		T[y][x] = Tn[y][x];
	    }
	}
	
	snprintf(tab, 15, "t = %.2lf", t);
	t += dt;
	DrawText(tab, 20, 40, 20, GREEN);
	DrawFPS(10, 10);
	EndDrawing();
	// for (int i = 1; i < wind.y-1; i++) T[i][1] = 2000;
	// for (int i = 1; i < wind.y-1; i++) T[i][wind.x-2] = 2000;
	// for (int i = 1; i < wind.x-1; i++) T[1][i] = 2000;
	// for (int i = 1; i < wind.x-1; i++) T[wind.y-2][i] = 2000;
    }
    
    return 0;
}

/*
 * TODO:
 * - Add the Neumann bounderies conditions (T[0][j] = T[1][j] - q*dx)
 *     if q == 0 then Insulating wall (adiabatic)
 *     if q <  0 then Heat goes out of the domain
 *     if q >  0 then Heat enters in the domain
 *     Like:
 *         for (int i = 0; i < N_x; i++) {
 *             T[i][0] = T[i][1];
 *         }
 *         for (int j = 0; j < N_y; j++) {
 *             T[0][j] = T[1][j] - q * dx;
 *         }
 * ...
 * - Add the Robin bounderies conditions ...
 * - Find the right FDM2Cent3p2D for the differentes edge t_i-2,j-2 with i=0 and j=2 => t_-2,0
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
