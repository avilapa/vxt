#pragma once

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

#include "types.h"

/**
* \file application.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Application framework and main() function macro.
*
*/
namespace vxt
{
  class Application
  {

  public:
    Application();
    virtual ~Application();

    virtual void init();
    virtual void start();
    virtual void update();
    virtual void stop();

    virtual bool is_exiting();

    uint32 run(int argc, char** argv);

  protected:
    bool exit_ = false;
  };

} /* end of vxt namespace */

#define VXR_DEFINE_MAIN(function) \
int main(int argc, char** argv) \
{ \
	return function(argc, argv); \
}

#define VXR_DEFINE_APP_MAIN(className) \
int runApp(int argc, char** argv) \
{ \
	className app; \
	return app.run(argc, argv); \
} \
VXR_DEFINE_MAIN(runApp);