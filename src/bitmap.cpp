// ----------------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2018 Víctor Ávila
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ----------------------------------------------------------------------------------------

#include "../include/bitmap.h"

namespace vxt
{

  void WriteBitmap(uint32 width, uint32 height, uint32 bytes_per_pixel, const uint8* pixels)
  {
    // see : http://paulbourke.net/dataformats/bitmaps/
    // and : https://www.siggraph.org/education/materials/HyperVis/asp_data/compimag/bmpfile.htm

    const uint32 pixelBytes = width * height*bytes_per_pixel;

    BITMAPFILEHEADER bmfh;
    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
    bmfh.bfType = 19778; // magic 'BM'
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pixelBytes;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER bmih;
    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biPlanes = 0; // or 0?
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;

    std::ofstream bmpFile("output.bmp", std::ios::out | std::ios::binary);
    bmpFile.write(reinterpret_cast<const char*>(&bmfh), sizeof(BITMAPFILEHEADER));
    bmpFile.write(reinterpret_cast<const char*>(&bmih), sizeof(BITMAPINFOHEADER));
    bmpFile.write(reinterpret_cast<const char*>(pixels), pixelBytes);
  }

} /* end of vxt namespace */