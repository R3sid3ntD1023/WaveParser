project "waveparser"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	systemversion "latest"
	staticruntime "On"
	
	targetdir("%{wks.location}/bin/" .. OUTPUT .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. OUTPUT.. "/%{prj.name}")
	
	files
	{
		"waveparser/src/**.cpp",
		"waveparser/include/**.h"
	}
	
	includedirs
	{
		"waveparser/include"
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
	
	
	