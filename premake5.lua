workspace "fxm"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    location "."

project "fxm_tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	location "bin/fixedmath_test"
	files { "src/**.cpp", "include/**.h", "*.natvis" }
	includedirs { "include" }

	filter "platforms:x64"
		system "Windows"
		architecture "x86_64"

	filter "configurations:Debug"
		optimize "Off"
		symbols "On"
		defines { "_DEBUG" }
		
	filter "configurations:Release"
		optimize "Full"
		defines { "_NDEBUG" }
