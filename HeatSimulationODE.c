#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define ODESOLVERLIB_IMPLEMENTATION
#include "ODEsolverlib.h"

#define MESHGRIDLIB_IMPLEMENTATION
#include "meshgridlib.h"

#define FPS 100
#define MESHGRID 100
#define WIDTH (8*100)
#define HEIGHT (6*100)


#define alpha 0.1f

#define UNUSED(x) (void)x


#define STAT_ALLOC(src, dest, type, h)			\
    type *dest[h];					\
    do {						\
	for (int i = 0; i < (h); i++) dest[i] = src[i];	\
    } while (0)

double norm(double x, double xmin, double xmax)
{
    return (x - xmin)/(xmax - xmin);
}

/*
double Diff2Forward3p2D(double h, double f_ip1j, double f_ijp1, double f_ij, double f_ijm1, double f_im1j)
{
    return (f_ip1j - 2.f*f_ij + f_im1j)/SQ_ODESOLVER(h) + (f_ijp1 - 2.f*f_ij + f_ijm1)/SQ_ODESOLVER(h);
    // return (f_ip1j + f_im1j + f_ijp1 + f_ijm1 - 4.f*f_ij)/SQ_ODESOLVER(h); // TODO: check
}

double Diff2Backward3p2D(double h, double f_ip1j, double f_ijp1, double f_ij, double f_ijm1, double f_im1j)
{
    return (f_ip1j - 2.f*f_ij + f_im1j)/SQ_ODESOLVER(h) + (f_ijp1 - 2.f*f_ij + f_ijm1)/SQ_ODESOLVER(h);
    // return (f_ip1j + f_im1j + f_ijp1 + f_ijm1 - 4.f*f_ij)/SQ_ODESOLVER(h); // TODO: check
}
*/


// int heat_equ_raw(int i, int j, double h, double **T, double p, int Lx, int Ly)
int heat_equ_raw(int i, int j, double h, double **T, double p)
{
    // if (i-1 < 0 && j-1 < 0) {          // Diff backward i and j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i+1][j] + T[i+2][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j+1] + T[i][j+2])/SQ_ODESOLVER(h)));
    // }
    // else if (i-1 < 0 && j+1 >= Ly) {   // Diff backward i, Diff forward j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i+1][j] + T[i+2][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j-1] + T[i][j-2])/SQ_ODESOLVER(h)));
    // }
    // else if (i-1 < 0) {                // Diff backward i, Diff centered j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i+1][j] + T[i+2][j])/SQ_ODESOLVER(h) + (T[i][j+1] - 2.f*p + T[i][j-1])/SQ_ODESOLVER(h)));
    // }
    // else if (i+1 >= Lx && j-1 < 0) {   // Diff forward i, Diff backward j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i-1][j] + T[i-2][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j+1] + T[i][j+2])/SQ_ODESOLVER(h)));
    // }
    // else if (i+1 >= Lx && j+1 >= Ly) { // Diff forward i, Diff forward j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i-1][j] + T[i-2][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j-1] + T[i][j-2])/SQ_ODESOLVER(h)));
    // }
    // else if (i+1 >= Lx) {              // Diff forward i, Diff centered j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((p - 2.f*T[i-1][j] + T[i-2][j])/SQ_ODESOLVER(h) + (T[i][j+1] - 2.f*p + T[i][j-1])/SQ_ODESOLVER(h)));
    // }
    // else if (j-1 < 0) {                // Diff centered i, Diff backward j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((T[i+1][j] - 2.f*p + T[i-1][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j+1] + T[i][j+2])/SQ_ODESOLVER(h)));
    // }
    // else if (j+1 >= Ly) {              // Diff centered i, Diff forward j stencil 3 points
    // // puts("ICI");
    // 	return (alpha * ((T[i+1][j] - 2.f*p + T[i-1][j])/SQ_ODESOLVER(h) + (p - 2.f*T[i][j-1] + T[i][j-2])/SQ_ODESOLVER(h)));
    // }

    // return (alpha * ((T[i+1][j] - 2.f*p + T[i-1][j])/SQ_ODESOLVER(h) + ((T[i][j+1] - 2.f*p + T[i][j-1]))/SQ_ODESOLVER(h)));
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

    for (int i = 1; i < wind.y-1; i++) pl[1][i] = 20000;
    for (int i = 1; i < wind.y-1; i++) pl[wind.x-2][i] = 20000;
    for (int i = 1; i < wind.x-1; i++) pl[i][1] = 20000;
    for (int i = 1; i < wind.x-1; i++) pl[i][wind.y-2] = 20000;
    
    STAT_ALLOC(pl, T, double, wind.x);

    double Tmin = 0, Tmax = 20;
    // for (int i = 0; i < wind.x; i++) {
    // 	for (int j = 0; j < wind.y; j++) {
    // 	    if (Tmax < T[i][j]) Tmax = T[i][j];
    // 	    if (Tmin > T[i][j]) Tmin = T[i][j];
    // 	}
    // }


    double t = 0;
    double h = 0.1f;
    int enter = 0;
    while (!WindowShouldClose()) {
	if (!IsKeyDown(KEY_ENTER) && enter == 0 && t > 0) {BeginDrawing(); EndDrawing(); continue;}
	else if (t > 0) enter = 1;
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}
	BeginDrawing();
	ClearBackground(BLACK);

	for (int x = 2; x < wind.x-2; x++) {
	// for (int x = 0; x < wind.x-0; x++) {
	    for (int y = 2; y < wind.y-2; y++) {
	    // for (int y = 0; y < wind.y-0; y++) {
		
		double heat_equ(double t, double p, double v)
		{
		    UNUSED(t);
		    UNUSED(v);
		    return heat_equ_raw(x, y, h, T, p);
		    // return heat_equ_raw(x, y, h, T, p, wind.x, wind.y);
		}

		double tmp = 0;
		if (SymplecticEuler(h, 0, &T[x][y], &tmp, heat_equ) < 0)
		// if (RK4(h, 0, &T[x][y], &tmp, heat_equ) < 0)
		    fprintf(stderr, "WARNING: Error calculation\n");

		// double temp = T[x][y];
		// if (Tmax < temp) Tmax = temp;
		// if (Tmin > temp) Tmin = temp;
		double T_norm = norm(T[x][y], Tmin, Tmax);

		// printf("%.2lf ", T_norm);
		// printf("%d ", (int)(T_norm/(double)0xff));

		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){(T_norm)*0xff, 0x00, (T_norm)*0xff, 0xff});
		// DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){0xff - (1-T_norm)*0xff/2, 0x00, 0xff - (T_norm)*0xff/2, 0xff});
	    }
	    // printf("m: %.2lf, M: %.2lf\n", Tmin, Tmax);
	    // printf("\n");
	}
	// printf("=====================================================\n");

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
