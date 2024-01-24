#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>

void end();
bool step();
void draw();

void main() {
    begin(); // No rendering allowed
    gfx_Begin();
    gfx_SetDrawBuffer(); // Draw to the buffer to avoid rendering artifacts

    while (step()) { // No rendering allowed in step!
        draw(); // As little non-rendering logic as possible
        gfx_SwapDraw(); // Queue the buffered frame to be displayed
    }

    gfx_End();
    end();
}
