cmake_minimum_required(VERSION 3.12)

include("./CMakeShared.txt")

set(PROJ_NAME pr_python)

project(${PROJ_NAME} CXX)

link_external_library(python)

add_external_library(luajit)
if(WIN32)
	add_external_library(lua)
endif()

add_include_dir(glm)

link_generic_module_libraries()
register_pragma_module(${PROJ_NAME})
