#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>

#define GRAVITY -0.2f
#define STEP_VEL 3.0f
#define BIRD_X 80
#define GRASS_Y 204
#define SAND_X 0
#define SAND_Y 208
#define SAND_WIDTH 320
#define SAND_HEIGHT 32

bool step();
void draw();