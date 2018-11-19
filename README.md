![Samples](/assets/textures/readme/raytracing_collage.jpg)

_version 0.2.1_

# What is vxt?

__vxt__ is a CPU based multithreaded path tracing renderer written in C++, based on the book Ray Tracing the Next Week by Peter Shirley. The purpose of this project is purely academic as this is my first contact with path tracing techniques! So far so fun :^) 

# Features

- Multithreaded Tiled Rendering in CPU.
- Creation of Image Previews in the process.
- Objects: Sphere, Box, Axis Aligned Plane, Volume, AABB (all capable of being Translated and Rotated).
- Bounding Volume Hierarchy Acceleration Structure.
- Materials: Lambertian, Metal, Glass, Isotropic (Fog/Smoke), Diffuse Light.
- Textures (used in all materials): Image Based, Noise, Custom.
- Realistic Camera: Depth of Field, Motion Blur.


# Building

_[A vs2017 Solution is provided by default]_

This project uses [GENie.lua](https://github.com/bkaradzic/GENie) to create the project solution. Open a CMD in the root directory and execute the following commands to re-create a solution:

	cd project
	..\tools\genie [parameters] <vs2017|vs2015>
	
The solution will be located in _project/vs/vxt-Raytracer.sln_.
  
# API

```c++
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
    renderer.render(new CornellBoxScene(), 
		    new Background(), 
		    CornellBoxScene::camera(width, height), 
		    "output.bmp", num_threads);

    exit_ = true;
    Application::init();
  }

} /* end of vxt namespace */
```

# Dependencies

- [glm](https://github.com/g-truc/glm)
- [stb_image.h](https://github.com/nothings/stb)
  
# [MIT License](/LICENSE)

# Contact

Mail: victorap97@gmail.com

Twitter: https://twitter.com/97torvic

Web/Blog/Portfolio: https://avilapa.github.io/portfolio

# Gallery

![Final](/assets/textures/readme/output_final_10000spp.jpg)
![Cornell](/assets/textures/readme/output_cornell_smoke_scene_500spp.jpg)
![Lights](/assets/textures/readme/output_light_250spp.jpg)
![Random](/assets/textures/readme/output_final_100spp.jpg)
![Earth](/assets/textures/readme/output_earth_250spp.jpg)
![Reflections](/assets/textures/readme/output_ch08_1000spp.jpg)
![Noise](/assets/textures/readme/output_noises_200spp.jpg)


