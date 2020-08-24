/*
chart.exe display tl tr [bl] [br]
chart.exe display 0x0 0x2

   16-bit RGB565

   Bit	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
Colour	R4	R3	R2	R1	R0	G5	G4	G3	G2	G1	G0	B4	B3	B2	B1	B0
*/

#include <iostream>
#include "Display.h"

struct Colour
{
    uint16_t red_component;
    uint16_t green_component;
    uint16_t blue_component;
};

void break_into_components(uint16_t colour_int, Colour* colour_struct)
{
    //on rgb565 https://docs.microsoft.com/en-us/windows/win32/directshow/working-with-16-bit-rgb
    colour_struct->red_component = (colour_int & 0xf800) >> 11; 
    colour_struct->green_component = (colour_int & 0x07e0) >> 5;
    colour_struct->blue_component = (colour_int & 0x001F);
}

/*to move gradually from colour A to B would be to progressively reduce the A value while simultaneously increasing the B value.*/
uint16_t mix_components(const uint16_t colour1_component, const uint16_t colour2_component, int step)
{
    uint16_t mixedColour_component;
    
    if (colour1_component > colour2_component)
        mixedColour_component = colour1_component - step;
    else if (colour1_component < colour2_component)
        mixedColour_component = colour1_component + step;
    else
        mixedColour_component = colour1_component;
    
    return mixedColour_component;
}


Colour mix_colours(const Colour colour1, const Colour colour2, int step)
{
    Colour mixedColour;

    mixedColour.red_component = mix_components(colour1.red_component, colour2.red_component, step);
    mixedColour.green_component = mix_components(colour1.green_component, colour2.green_component, step);
    mixedColour.blue_component = mix_components(colour1.blue_component, colour2.blue_component, step);
    
    return mixedColour;
}

/*quick test*/
int main()
{
    std::string displayName;
    displayName = "display";

    Display* display = new Display(displayName);
    
    Point starting_point;
    starting_point.x = 0;
    starting_point.y = 0;    

    uint16_t colour_tl = 0x0000; //red
    uint16_t colour_tr = 0x000f; //blue

    //Convert colour to struct
    Colour top_left, top_right, bottom_left, bottom_right;

    break_into_components(colour_tl, &top_left);
    break_into_components(colour_tr, &top_right);

    uint16_t PixelArray[100]; //TODO change this to vector
    //uint16_t* pPixelArray = &PixelArray[0];

    Dimension displaySize = display->size();

    Colour pixelColour = top_left;
    //make a row
    for (auto i = 0; i < displaySize.width; i++)
    {
        //reconstruct in rgb565 format
        PixelArray[i] = (pixelColour.red_component << 11) | (pixelColour.green_component << 5) | (pixelColour.blue_component);

        pixelColour = mix_colours(pixelColour, top_right, 1);

        //PixelArray[i] = PixelArray[i] | (pixelColour.green_component << 5);
        //PixelArray[i] = PixelArray[i] | (pixelColour.blue_component << 3);

        //*pPixelArray = i;
        //pPixelArray++;
    }

    //draw on all rows of the display
    for (auto i = 0; i < displaySize.height; i++)
    {
        display->draw(starting_point, PixelArray, displaySize.width);
        if (starting_point.y<displaySize.height) starting_point.y++;
    }

    display->present();
    getchar();
}
