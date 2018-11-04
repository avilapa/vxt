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

#ifdef _WIN32

#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtx/transform.hpp"
#include "../deps/glm//gtc/quaternion.hpp"
#include "../deps/glm//gtx/matrix_decompose.hpp"

#include <windows.h>
#include <conio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <random>
#include <chrono>


namespace vxt
{

  typedef ::int8_t			int8;
  typedef ::int16_t			int16;
  typedef ::int32_t			int32;

  typedef ::uint8_t			uint8;
  typedef ::uint16_t		uint16;
  typedef ::uint32_t		uint32;

  typedef glm::vec2			vec2;
  typedef glm::vec3			vec3;
  typedef glm::vec4			vec4;

  typedef glm::dvec2		dvec2;
  typedef glm::dvec3		dvec3;
  typedef glm::dvec4		dvec4;

  typedef glm::ivec2		ivec2;
  typedef glm::ivec3		ivec3;
  typedef glm::ivec4		ivec4;

  typedef glm::uvec2		uvec2;
  typedef glm::uvec3		uvec3;
  typedef glm::uvec4		uvec4;

  typedef glm::mat3			mat3;
  typedef glm::mat4			mat4;

  typedef std::string   string;

  struct Color
  {
    uint8 b;
    uint8 g;
    uint8 r;
    uint8 a;
  };

  static std::mt19937 random_number_engine(time(0));

  static float randomFloat01()
  {
    return (rand() / (RAND_MAX + 1.0));
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(random_number_engine);
  }

  static vec3 randomUnitSphere()
  {
    vec3 p;
    do
    {
      p = 2.0f * vec3(randomFloat01(), randomFloat01(), randomFloat01()) - vec3(1.0f);
    } while ((p.x * p.x + p.y * p.y + p.z * p.z) >= 1.0f);
    return p;
  }

} /* end of vxt namespace */

#else
#error Missing Platform
#endif

