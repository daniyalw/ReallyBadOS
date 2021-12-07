namespace Graphic {

/*
skiftOS

Copyright © 2018-2021 N. Van Bossuyt & contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
// there are a few modifications by me

void saturation(int * arr, int _x, int _y, int _w, int _h, float value)
{
    int col;

    for (int y = 0; y < _h; y++)
    {
        for (int x = 0; x < _w; x++)
        {
            colors_t color = get_colors(arr[(_x + x) + (_y + y) * _w]);

            // weights from CCIR 601 spec
            // https://stackoverflow.com/questions/13806483/increase-or-decrease-color-saturation
            double gray = 0.2989 * color.red + 0.5870 * color.green + 0.1140 * color.blue;

            uint8_t red = (uint8_t)clamp(-gray * value + color.red * (1 + value), 0, 255);
            uint8_t green = (uint8_t)clamp(-gray * value + color.green * (1 + value), 0, 255);
            uint8_t blue = (uint8_t)clamp(-gray * value + color.blue * (1 + value), 0, 255);

            col = Graphic::rgb(red, green, blue);

            // saving the new pixel colors in the back buffer
            // and then blitting the changes is faster than
            // writing to video memory for each pixel...
            //
            // like far faster
            arr[(_x + x) + (_y + y) * _w] = col;
        }
    }
}

}
