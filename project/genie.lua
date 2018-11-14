-- ----------------------------------------------------------------------------------------
-- MIT License
-- 
-- Copyright(c) 2018 Víctor Ávila
-- 
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files(the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions :
-- 
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
-- 
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.
-- ----------------------------------------------------------------------------------------

PROJ_DIR = path.getabsolute("../")

-- // -- Options -- // ---------------------------------------------------------------------------------

newoption {
	trigger = "no-console",
	description = "Launch a console with the window.",
}

-- // -- vxt-Raytracing Solution -- // ------------------------------------------------------------------

solution "vxt-Raytracing"

	location "../project/vs/"
	configurations 		{ "Debug", "Release" }
	platforms 			{ "x32", "x64", "Native" }
	flags 				{  }

	configuration "Debug"
		defines 		{ "VXT_DEBUG" }
		flags 			{ "Symbols", "ExtraWarnings" }

	configuration "Release"
		defines 		{ "VXT_RELEASE" }
		flags 			{ "Optimize" }

	configuration 		{ "vs*" }
		buildoptions{
			"/wd4100",
			"/wd4127",
			"/wd4244",
		}

	configuration "vs2015"
		windowstargetplatformversion "8.1"

	configuration "vs2017"
		windowstargetplatformversion "10.0.17134.0"

	language "C++"
	startproject "dev"


-- // -- vxt.lib -- // ---------------------------------------------------------------------------------

project "vxt"
	kind 		"StaticLib"
	language 	"C++"
	includedirs { 
		path.join(PROJ_DIR, "include/**.h"),
		-- // Dependencies // --
		path.join(PROJ_DIR, "deps/glm/**.h*"),
		path.join(PROJ_DIR, "deps/stb/**.h"),
	}
	files {
		path.join(PROJ_DIR, "include/**.h"),
		path.join(PROJ_DIR, "src/**.h"),
		path.join(PROJ_DIR, "src/**.cpp"),
		-- // Dependencies // --
		path.join(PROJ_DIR, "deps/glm/**"),
		path.join(PROJ_DIR, "deps/stb/**"),
	}
	excludes {
		
	}
	defines { 

	}
	links { 
		
	}

-- // -- Projects -- // ---------------------------------------------------------------------------------

function makeProject(name)
	project (name)
		if _OPTIONS["no-console"] 	then
			kind "WindowedApp"
		else
			kind "ConsoleApp"
		end
		debugdir(path.join(PROJ_DIR, "examples/" .. name))
		includedirs 		{ path.join(PROJ_DIR, "examples/" .. name .. "/**") }
		files 				{ path.join(PROJ_DIR, "examples/" .. name .. "/**") }
		links 				{ "vxt" }
		language        	"C++"
end

makeProject("dev")