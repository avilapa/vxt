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

#include "main.h"

#include "../../include/renderer/renderer.h"

VXR_DEFINE_APP_MAIN(vxt::Main)

namespace vxt
{

  void Main::init()
  {
    constexpr uint32 width = 400;
    constexpr uint32 height = 400;
    constexpr uint32 samples_per_pixel = 500;
    constexpr uint32 num_threads = 8;

    // Note: Use Background for lit scenes, and GradientBackground for unlit scenes!
    Renderer renderer(width, height, samples_per_pixel);
    renderer.render(new CornellBoxScene(), new Background(), CornellBoxScene::camera(width, height), "output.bmp", num_threads);

    exit_ = true;
    Application::init();
  }

} /* end of vxt namespace */