cmake_minimum_required(VERSION 2.8.3)
project(video_creator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
)

include(FindPkgConfig)
pkg_check_modules(CAIRO_FT REQUIRED cairo-ft)
add_definitions(${CAIRO_FT_CFLAGS})
link_libraries(${CAIRO_FT_LDFLAGS})


set(INCLUDE_FILES
    include/video_creator/frame_visualizer.h
    include/video_creator/math/math.h
    include/video_creator/frame_processor.h
    include/video_creator/color.h
    include/video_creator/script_interpreter.h

    include/video_creator/r3d/mesh.h
    include/video_creator/r3d/camera.h
    include/video_creator/r3d/primitives.h
    include/video_creator/r3d/scene.h

    include/video_creator/text/text.h
    include/video_creator/text/text_impl.h

)

set(SOURCE_FILES
    src/frame_visualizer.cpp
    src/frame_processor.cpp
    src/script_interpreter.cpp

    src/r3d/scene.cpp
    src/r3d/camera.cpp
    src/r3d/mesh.cpp
    src/r3d/primitives.cpp

    src/text/text.cpp
    src/text/text_impl.cpp
)

add_library(video_creator
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)
target_link_libraries(video_creator ${OpenCV_LIBRARIES})

add_executable(cst-presentation tools/cst-presentation.cpp)
target_link_libraries(cst-presentation ${OpenCV_LIBRARIES} video_creator)

