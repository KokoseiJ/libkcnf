cmake_minimum_required(VERSION 3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.24)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.24)
endif()

project(libkcnf VERSION 0.1
		DESCRIPTION "Koko's Config File Library"
		LANGUAGES C)

add_library(kcnf STATIC src/value.c src/list.c src/pair.c src/parse.c src/util.c src/base64.c)

add_executable(demo1 demo/kcnf.c)

target_include_directories(kcnf PUBLIC include)

target_link_libraries(demo1 kcnf)

