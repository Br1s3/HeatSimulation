#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define ODESOLVERLIB_IMPLEMENTATION
#include "ODEsolverlib.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define FPS 60
#define MESHGRID 100
#define WIDTH (16*100)
#define HEIGHT (9*100)


#define alpha 0.1f
#define UNUSED(x) (void)x

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

    // for (int i = 1; i < wind.y-1; i++) pl[1][i] = 2000;
    // for (int i = 1; i < wind.y-1; i++) pl[wind.x-2][i] = 2000;
    // for (int i = 1; i < wind.x-1; i++) pl[i][1] = 2000;
    // for (int i = 1; i < wind.x-1; i++) pl[i][wind.y-2] = 2000;
    
    STAT_ALLOC(pl, T, double, wind.x);

    double Tmin = 0, Tmax = 20;

    double t = 0;
    double h = 0.1f;
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
	    T[MousePos.x][MousePos.y] = 200.f;
	}

	BeginDrawing();
	ClearBackground(BLACK);

	for (int x = 2; x < wind.x-2; x++) {
	    for (int y = 2; y < wind.y-2; y++) {

		double heat_equ(double t, double p, double v)
		{
		    UNUSED(t);
		    UNUSED(v);
		    return heat_equ_raw(x, y, h, T, p);
		}

		double tmp = 0;
		// if (SymplecticEuler(h, 0, &T[x][y], &tmp, heat_equ) < 0)
		if (RK4(h, t, &T[x][y], &tmp, heat_equ) < 0)
		// if (DOPRI45(h, t, 0.001, &T[x][y], &tmp, heat_equ) < 0)
		    fprintf(stderr, "WARNING: Error calculation\n");

		double T_norm = norm(T[x][y], Tmin, Tmax);

		// DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){(T_norm)*0xff, 0x00, (T_norm)*0xff, 0xff});
		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){0xff - (1-T_norm)*0xff/2, 0x00, 0xff - (T_norm)*0xff/2, 0xff});
	    }
	}

	DrawFPS(10, 10);
	EndDrawing();
	t += h;
	// for (int i = 1; i < wind.y-1; i++) pl[1][i] = 20;
	// for (int i = 1; i < wind.y-1; i++) pl[wind.x-2][i] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[i][1] = 20;
	// for (int i = 1; i < wind.x-1; i++) pl[i][wind.y-2] = 20;

    }
    
    return 0;
}
