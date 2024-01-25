#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <graphx.h>
#include <keypadc.h>
#include "func.h"
#include "gfx/gfx.h"
#include <stdbool.h>

int bird_Y = GFX_LCD_HEIGHT / 2;
float velY;
bool clipped = false;

int main() {
    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);

    while (step()) { // No rendering allowed in step!
        if (clipped)
            break;
        draw(); // as little non-render logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End();
    return 0;
}


bool step(){
    if (bird_Y <= 0 || bird_Y >= GRASS_Y)
        clipped = true;

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

    return true;
}


void draw(){
    gfx_FillScreen(1); // blue sky
    gfx_SetColor(2);
    gfx_FillRectangle_NoClip(SAND_X, SAND_Y, SAND_WIDTH, SAND_HEIGHT);
    for (int i = 0; i < 40; i++){
        gfx_Sprite_NoClip(grass, i * grass_width, GRASS_Y); // grass in segments
    }
    gfx_TransparentSprite_NoClip(FlappyBird, BIRD_X, bird_Y);
}