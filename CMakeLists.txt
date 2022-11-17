cmake_minimum_required(VERSION 3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(libkcnf VERSION 0.1
		   DESCRIPTION "Koko's Config File Library"
		   LANGUAGES C)

add_library(kcnf STATIC src/kcnf.c src/base64.c src/base64.h include/libkcnf/kcnf.h)

target_include_directories(kcnf PUBLIC include)

