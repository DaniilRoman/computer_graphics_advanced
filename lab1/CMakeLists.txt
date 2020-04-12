cmake_minimum_required(VERSION 2.8)

project(VTK-TUTORIALS CXX)

# if not build uncomment line bellow
 SET(VTK_DIR /home/daniil/Documents/D/study/masters/comp_graphics/VTK-build)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(BasikPipeline src/basic_pipeline.cpp src/PlaneMoveCallback.h)
target_link_libraries(BasikPipeline ${VTK_LIBRARIES})