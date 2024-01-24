#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <graphx.h>
#include <keypadc.h>
#include <func.h>

int Y = GFX_LCD_HEIGHT / 2;
int X = GFX_LCD_WIDTH / 4;
float velY;

void main() {
    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts

    while (step()) { // No rendering allowed in step!
        draw(); // as little non-render logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End();
}


bool step(){

    kb_Scan();

    if (kb_Data[6] & kb_Clear)
        return false;
    
    if ((kb_Data[6] & kb_Enter) && velY < MAX_VEL)
        velY += INIT_VEL;

    velY += GRAVITY; // apply gravity every tick

    return true;
}


void draw(){

    
}