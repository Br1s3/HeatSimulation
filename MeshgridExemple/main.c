#include <stdio.h>
#include <raylib.h>

#define MESHGRIDLIB_IMPLEMENTATION
#include "../meshgridlib.h"

#define FPS 30
#define MESHGRID 20
#define WIDTH (8*100)
#define HEIGHT (6*100)

int main()
{
    InitWindow(WIDTH, HEIGHT, "Simulation of the Heat Equation");
    SetTargetFPS(FPS);

    Window wind;
    SetMeshGrid(&wind, WIDTH, HEIGHT, MESHGRID);

    printf("X-Offset: %d,, Y-Offset: %d\n", wind.Offset.x, wind.Offset.y);
    printf("X-wind: %d,, Y-wind: %d\n", wind.x, wind.y);
    printf("Grid: %d,, meshgrid: %d\n", wind.grid, MESHGRID);

    int i = 0, j = 0;
    // unsigned char R = 0, B = 0xff;
    while (!WindowShouldClose()) {

	// printf("i: %d, : %d:: x-grid: %d, y-grid: %d\n", i, j, wind.grid, wind.grid);
	if (i++ >= wind.grid) {
	    i = 0;
	    if (j++ >= wind.grid) j = 0;
	}

	BeginDrawing();
	ClearBackground(BLACK);
	DrawRectangle(wind.Offset.x + (i*wind.x), wind.Offset.y + (j*wind.y), wind.x, wind.y, WHITE);
	// DrawRectangle(wind.Offset.x + (i*wind.x), wind.Offset.y + (j*wind.y), wind.x, wind.y, (Color){R++, 0, 0, 0xff});
	DrawFPS(10, 10);
	EndDrawing();
    }
    
    return 0;
}
