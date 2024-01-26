#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <graphx.h>
#include <keypadc.h>
#include "func.h"
#include "gfx/gfx.h"
#include <stdbool.h>
#include <sys\util.h>
#include <sys\rtc.h>
#include <string.h>

int bird_Y = GFX_LCD_HEIGHT / 2;
float velY;
Position pipeRims[4] = {{-50, 0}, {-50, 0}, {-50, 0}, {-50, 0}};
int pipeOverwrite = 0;
int checkPipe = 0;
int bottomPipeSects;
int topPipeSects;
int sect_X;
int score;

int main() {

    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    srand(rtc_Time());
    init_Pipes();

    while (step()) { // No rendering allowed in step!
        draw(); // as little non-render logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    fail_Screen();
    gfx_End();
    return 0;
}


bool step(){

    // collision logic
    if (bird_Y <= 0 || bird_Y >= GRASS_Y)
        return false;
    
    if (gfx_CheckRectangleHotspot(BIRD_X, bird_Y, FlappyBird_width, FlappyBird_height, pipeRims[checkPipe].x, 0, Pipe_width, pipeRims[checkPipe].y - PIPE_GAP) || gfx_CheckRectangleHotspot(BIRD_X, bird_Y, FlappyBird_width, FlappyBird_height, pipeRims[checkPipe].x, pipeRims[checkPipe].y, Pipe_width, Pipe_height))
        return false;


    kb_Scan();
    static bool enter, prevEnter;
    enter = kb_Data[6] & kb_Enter;
    
    if (kb_Data[6] & kb_Clear)
        return false;
    
    if (enter && !prevEnter)
        velY = STEP_VEL;

    prevEnter = enter;
    velY += GRAVITY; // apply gravity every tick
    bird_Y -= velY;


    // pipe logic
    if (pipeOverwrite >= 4){
        pipeOverwrite = 0;
        set_Pipes();
    } else if (pipeRims[pipeOverwrite].x <= PIPE_BUFFER){
        pipeOverwrite++;
        set_Pipes();
    }

    if (pipeRims[checkPipe].x < BIRD_X){
        checkPipe++;
        score++;
    }
    if (checkPipe >= 4)
        checkPipe = 0;

    for (int i = 0; i < 4; i++){
        pipeRims[i].x -= 2;
    }

    return true;
}


void draw(){

    gfx_FillScreen(1); // blue sky
    
    for (int i = 0; i < 4; i++){
        int topPipeY = (pipeRims[i].y  - PIPE_GAP) - pipeRim_height;
        gfx_Sprite(pipeRim, pipeRims[i].x, pipeRims[i].y);
        gfx_Sprite(pipeRim, pipeRims[i].x, topPipeY);

        bottomPipeSects = (GFX_LCD_HEIGHT - (grass_height + SAND_HEIGHT)) - (pipeRims[i].y + pipeRim_height);
        topPipeSects = (GFX_LCD_HEIGHT - topPipeY);

        for (int j = 0; j < bottomPipeSects; j += pipeSect_height){
            gfx_Sprite(pipeSect, pipeRims[i].x + 1, pipeRims[i].y + j + pipeRim_height);
        }
        for (int j = 0; j < topPipeSects; j += pipeSect_height){
            gfx_Sprite(pipeSect, pipeRims[i].x + 1, (topPipeY - j) - pipeSect_height);
        }
    }
    gfx_SetColor(2);
    gfx_FillRectangle_NoClip(SAND_X, SAND_Y, SAND_WIDTH, SAND_HEIGHT);
    for (int i = 0; i < 40; i++){
        gfx_Sprite_NoClip(grass, i * grass_width, GRASS_Y); // grass in segments
    }
    gfx_TransparentSprite_NoClip(FlappyBird, BIRD_X, bird_Y);
}


void init_Pipes(){

    pipeRims[pipeOverwrite].y = randInt(68, 184);
    pipeRims[pipeOverwrite].x = PIPE_BUFFER + 100;
}


void set_Pipes(){

    pipeRims[pipeOverwrite].y = randInt(68, 184);
    pipeRims[pipeOverwrite].x = GFX_LCD_WIDTH;
}

void fail_Screen(){

    char str[10] = "Score: ";

    kb_Scan();
    while(!(kb_Data[6] & kb_Clear)){
        gfx_FillScreen(1);
        gfx_SetTextXY((GFX_LCD_WIDTH / 2) - 50, (GFX_LCD_HEIGHT / 2) - 5);
        gfx_SetTextScale(2, 2);
        gfx_SetTextFGColor(3);
        gfx_PrintString(str);
        gfx_PrintUInt(score, 1);
        gfx_SwapDraw();
        kb_Scan();
    }
}