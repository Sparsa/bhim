cmake_minimum_required(VERSION 3.10)
project(bhim)

set(CMAKE_CXX_STANDARD 17)


include_directories(include)
include_directories(src)


add_executable(bhim
        include/automata.h
        include/concatenationMultiStack.h
	include/tinystr.h
	include/tinyxml.h
	include/boost/numeric/ublas/matrix.hpp
	src/tinyxml.cpp
	src/tinystr.cpp
	src/tinyxmlerror.cpp
	src/tinyxmlparser.cpp
        src/automata.cpp
        src/concatenationMultiStack.cpp
        src/main.cpp include/linkedlist.h )
target_link_libraries(bhim)
