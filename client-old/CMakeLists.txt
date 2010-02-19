if (NOT WITH_GUI)
	set (WITH_GUI ON CACHE BOOL "Build with SDL gui?")
endif (NOT WITH_GUI)

file (GLOB_RECURSE client_SRCS *.cpp ../common/*.cpp)

include_directories (${Bonsai_SOURCE_DIR})

add_executable (bonsai_client ${client_SRCS})
target_link_libraries (bonsai_client Imlib2)
