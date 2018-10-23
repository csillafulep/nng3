cmake_minimum_required(VERSION 3.5)

project(_2_main)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror")

set(_ALLOWED_SRC_EXTENSIONS "cpp")
set(_ALLOWED_INC_EXTENSIONS 
	"hpp"
	"h"
)

set(SRC "")
set(INC "")
foreach(_dir IN ITEMS ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(_extension IN ITEMS ${_ALLOWED_SRC_EXTENSIONS})
		file(
			GLOB _src_list 
			LIST_DIRECTORIES false
			"${_dir}/*.${_extension}"
		)
		set(SRC "${SRC};${_src_list}")
	endforeach()

	foreach(_extension IN ITEMS ${_ALLOWED_INC_EXTENSIONS})
		file(
			GLOB _inc_list 
			LIST_DIRECTORIES false
			"${_dir}/*.${_extension}"
		)
		set(INC "${INC};${_inc_list}")
	endforeach()
endforeach()

message(STATUS "src: ${SRC}")
message(STATUS "inc: ${INC}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/tmools")

ADD_EXECUTABLE(_2_main ${SRC} ${INC})