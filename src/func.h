#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>


typedef struct{
    float x;
    float y;
} Position;

#define GRAVITY -0.4f
#define STEP_VEL 3.8f
#define BIRD_X 80
#define GRASS_Y 204
#define SAND_X 0
#define SAND_Y 208
#define SAND_WIDTH 320
#define SAND_HEIGHT 32
#define PIPE_BUFFER 216
#define PIPE_GAP 48
#define Pipe_width 26
#define Pipe_height 188

bool step();
void draw();
void init_Pipes();
void set_Pipes();
void fail_Screen();