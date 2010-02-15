cmake_minimum_required (VERSION 2.6)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE "release" CACHE STRING
		"Choose the type of build, options are: release (default) debug profile")
endif (NOT CMAKE_BUILD_TYPE)

project (Bonsai)

add_subdirectory (client)
add_subdirectory (server)

include (CTest)
if (BUILD_TESTING)
	enable_testing ()
	add_subdirectory (tests)
endif (BUILD_TESTING)
