#include <stdio.h>
#include <raylib.h>
#include <math.h>

// #define ODESOLVERLIB_IMPLEMENTATION
// #include "ODEsolverlib.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define FPS 30
#define MESHGRID 20
#define WIDTH (8*100)
#define HEIGHT (6*100)



#define alpha 0.1f
#define N 12
#define carre(x) ((x)*(x))

#define STAT_ALLOC(src, dest, type, h)			\
    type *dest[h];					\
    do {						\
	for (int i = 0; i < (h); i++) dest[i] = src[i];	\
    } while (0)

double norm(double x, double xmin, double xmax)
{
    return (x - xmin)/(xmax - xmin);
}

double heat_equ(double px, double py, double t, double **T, double Lx, double Ly)
{
    double res = 0;
     double tmp2[N-1][N-1] = {0};
    if (tmp2[0][0] == 0) {
	for (int n = 1; n < N; n++) {
	    for (int m = 1; m < N; m++) {
		for (int phi = 0; phi < Lx; phi++) {
		    for (int psi = 0; psi < Ly; psi++) {
			tmp2[n-1][m-1] += T[phi][psi] * sin((double)n * (M_PI/Lx) * (double)phi) * sin((double)m * (M_PI/Ly) * (double)psi);
		    }
		}
	    }
	}
    }

    double tmp1 = 4.f/(Lx * Ly);
    for (int n = 1; n < N; n++) {
	for (int m = 1; m < N; m++) {
	    double tmp3 = sin((double)n * (M_PI/Lx) * px) * sin((double)m * (M_PI/Ly) * py) * exp(carre(M_PI) * (carre((double)n/Lx) + carre((double)m/Ly)) * (-alpha) * t);
	    res += tmp1 * tmp2[n-1][m-1] * tmp3;
	}
    }
    return res;
}


int main()
{
    double dt = 1;

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

    // // pl[wind.grid/2-3][wind.grid/2-3].p.x = 0x8f; pl[wind.grid/2-3][wind.grid/2-3].p.y = 0x8f;
    // pl[wind.x/2-3][wind.y/2-2] = 200;
    // pl[wind.x/2-3][wind.y/2-1] = 200;
    // pl[wind.x/2-3][wind.y/2] = 200;
    // pl[wind.x/2-3][wind.y/2+1] = 200;
    // pl[wind.x/2-3][wind.y/2+2] = 200;
    // // pl[wind.grid/2-3][wind.grid/2+3] = 200; pl[wind.grid/2-3][wind.grid/2+3].p.y = 200;
    
    // pl[wind.x/2-2][wind.y/2-3] = 200;
    // pl[wind.x/2-1][wind.y/2-3] = 200;
    // pl[wind.x/2][wind.y/2-3] = 200;
    // pl[wind.x/2+1][wind.y/2-3] = 200;
    // pl[wind.x/2+2][wind.y/2-3] = 200;
    // // pl[wind.grid/2+3][wind.grid/2-3] = 200; pl[wind.grid/2+3][wind.grid/2-3].p.y = 200;
    
    // pl[wind.x/2-2][wind.y/2+3] = 200;
    // pl[wind.x/2-1][wind.y/2+3] = 200;
    // pl[wind.x/2][wind.y/2+3] = 200;
    // pl[wind.x/2+1][wind.y/2+3] = 200;
    // pl[wind.x/2+2][wind.y/2+3] = 200;
    // // pl[wind.grid/2+3][wind.grid/2+3] = 200; pl[wind.grid/2+3][wind.grid/2+3].p.y = 200;
    
    // pl[wind.x/2+3][wind.y/2-2] = 200;
    // pl[wind.x/2+3][wind.y/2-1] = 200;
    // pl[wind.x/2+3][wind.y/2] = 200;
    // pl[wind.x/2+3][wind.y/2+1] = 200;
    // pl[wind.x/2+3][wind.y/2+2] = 200;

    for (int i = 0; i < wind.y; i++) pl[1][i] = 200;
    for (int i = 0; i < wind.y; i++) pl[wind.x-2][i] = 200; 
    for (int i = 0; i < wind.x; i++) pl[i][1] = 200;
    for (int i = 0; i < wind.x; i++) pl[i][wind.y-2] = 200;
    
    STAT_ALLOC(pl, T, double, wind.x);

    double Tmin = 0, Tmax = 0.01;
    for (int i = 0; i < wind.x; i++) {
	for (int j = 0; j < wind.y; j++) {
	    if (Tmax < T[i][j]) Tmax = T[i][j];
	    if (Tmin > T[i][j]) Tmin = T[i][j];
	}
    }

    double t = 0;
    int enter = 0;
    while (!WindowShouldClose()) {
	if (!IsKeyDown(KEY_ENTER) && enter == 0) {BeginDrawing(); EndDrawing(); continue;}
	else enter = 1;
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}
	BeginDrawing();
	ClearBackground(BLACK);

	for (int x = 0; x < wind.x; x++) {
	    for (int y = 0; y < wind.y; y++) {
		double temp = heat_equ((double)x, (double)y, t, T, wind.x-1, wind.y-1);
		if (Tmax < temp) Tmax = temp;
		if (Tmin > temp) Tmin = temp;
		double tmp = norm(temp, Tmin, Tmax);

		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){(tmp)*0xff, 0x00, (tmp)*0xff, 0xff});

		// printf("%.3lf ", tmp);
		// if (carre(temp) > 0.f) printf("%.2lf ", T[x][y]);
		// else                   printf("     ");
	    }
	    // printf("\n");
	}
	// printf("-------------------------------------------------------------------------------\n");

	DrawFPS(10, 10);
	EndDrawing();
	t += dt;
    }
    
    return 0;
}
