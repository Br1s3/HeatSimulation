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

    while (!WindowShouldClose()) {
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}
	BeginDrawing();
	ClearBackground(BLACK);
	for (int x = 0; x < wind.x; x++) {
	    for (int y = 0; y < wind.y; y++) {
		DrawRectangle(MESHOFFSET_X(wind, x), MESHOFFSET_Y(wind, y), wind.grid, wind.grid, (Color){((float)x/(float)wind.x)*0xff, 0x00, ((float)y/(float)wind.y)*0xff, 0xff});
	    }
	}
	DrawFPS(10, 10);
	EndDrawing();
    }
    
    return 0;
}
