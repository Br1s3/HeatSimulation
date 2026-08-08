#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define ODESOLVERLIB_IMPLEMENTATION
#include "ODEsolverlib.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define FPS 60
#define MESHGRID 200
#define WIDTH (16*100)
#define HEIGHT (9*100)

#define alpha 0.01f
#define UNUSED(x) (void)x

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

int heat_equ_raw(int i, int j, double h, double **T, double p)
{
    // TODO: Find the right Diff2Cent3p2D for the differentes edge t_i-2,j-2 with i=0 and j=2 => t_-2,0
    // return alpha * Diff2Cent3p2D(h, T[i+1][j], T[i][j+1], p, T[i][j-1], T[i-1][j]);
    return alpha * Diff2Cent5p2D(h, T[i+2][j], T[i+1][j], T[i][j+2], T[i][j+1], p, T[i][j-1], T[i][j-2], T[i-1][j], T[i-2][j]);
    // The most part of the error appear because of the exemple:
    // [ a x x ] (a is the first pixel and b the last)
    // [ x x x ]
    // [ x x b ]
    // b(t=i) is calculated with a(t=i+1) and not a(t=i)
    // The real problem here, to solve this problem, is that we need two buffers that require a lot of memory.
}


int main()
{
    InitWindow(WIDTH, HEIGHT, "Simulation of the Heat Equation");
    SetTargetFPS(FPS);

    Window wind;
    SetMeshGrid(&wind, WIDTH, HEIGHT, MESHGRID);

    double pl[wind.x][wind.y];
    for (int i = 0; i < wind.x; i++) {
	for (int j = 0; j < wind.y; j++) {
	    pl[i][j] = 0;
	}
    }

    // for (int i = 1; i < wind.y-1; i++) pl[1][i] = 200;
    // for (int i = 1; i < wind.y-1; i++) pl[wind.x-2][i] = 200;
    // for (int i = 1; i < wind.x-1; i++) pl[i][1] = 200;
    // for (int i = 1; i < wind.x-1; i++) pl[i][wind.y-2] = 200;
    
    STAT_ALLOC(pl, T, double, wind.x);

    double Tmin = 0, Tmax = 20;

    double t = 0;
    const double dt = 0.1f;
    const double dx = 0.1f; // [0.049 - XXX] low value like 0.049 represents an EXTREMELY tight grid
    // int enter = 0;
    while (!WindowShouldClose()) {
	// if (!IsKeyDown(KEY_ENTER) && enter == 0 && t > 0) {BeginDrawing(); EndDrawing(); continue;}
	// else if (t > 0) enter = 1;
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) == 1) {
	    Vector2 MousePosf = GetMousePosition();
	    Vec2 MousePos = {(int)MousePosf.x/wind.grid, (int)MousePosf.y/wind.grid};
	    if (MousePos.x > wind.x-3) MousePos.x = wind.x-3;
	    if (MousePos.x < 3) MousePos.x = 3;
	    if (MousePos.y > wind.y-3) MousePos.y = wind.y-3;
	    if (MousePos.y < 3) MousePos.y = 3;
	    T[MousePos.x][MousePos.y] = 1000.f;
	}

	BeginDrawing();
	ClearBackground(BLACK);

	for (int x = 2; x < wind.x-2; x++) {
	    for (int y = 2; y < wind.y-2; y++) {

		double heat_equ(double t, double p, double v)
		{
		    UNUSED(t);
		    UNUSED(p);
		    return heat_equ_raw(x, y, dx, T, v);
		}

		double tmp = 0;
		// if (RK4(dt, t, &tmp, &T[x][y], heat_equ) < 0)
		if (ExplicitEuler(dt, t, &tmp, &T[x][y], heat_equ) < 0)
		    fprintf(stderr, "WARNING: Error calculation\n");

		double T_norm = norm(T[x][y], Tmin, Tmax);
		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, BLUERED(T_norm));
	    }
	}
	printf("%lf\n", norm(T[wind.x/2][wind.y/2], Tmin, Tmax));

	DrawFPS(10, 10);
	EndDrawing();
	t += dt;
	for (int i = 1; i < wind.y-1; i++) pl[1][i] = 2000;
	for (int i = 1; i < wind.y-1; i++) pl[wind.x-2][i] = 2000;
	for (int i = 1; i < wind.x-1; i++) pl[i][1] = 2000;
	for (int i = 1; i < wind.x-1; i++) pl[i][wind.y-2] = 2000;

    }
    
    return 0;
}
