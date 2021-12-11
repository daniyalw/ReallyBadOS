#pragma once

namespace Graphic {
    __attribute__((optimize("O3")))
    void stackblur(unsigned char *src,  ///< input image data
                   int w,      ///< image width
                   int h,      ///< image height
                   int radius, ///< blur intensity (should be in 2..254 range)
                   int minX,
                   int maxX,
                   int minY,
                   int maxY);
}
