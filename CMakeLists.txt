cmake_minimum_required(VERSION 3.6)
project(Edit)

include_directories(GnuWin32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(Eigen)
include_directories(Eigen/src)

set(SOURCE_FILES
        jpeg_reader/aoa_jpeg.c
        jpeg_reader/aoa_jpeg.h
        Camera.h
        Color.h
        Image.cpp
        Image.h
        main.cpp
        Material.h
        Mesh.h
        PointLight.h
        Ray.h
        Scene.h
        Sphere.h
        Triangle.h
        Vector3.h
        Vertex.h Camera.cpp Sphere.cpp Scene.cpp Vector3.cpp Triangle.cpp Mesh.cpp Texture.h Texture.cpp Translation.cpp Translation.h Scaling.cpp Scaling.h Rotation.cpp Rotation.h Cube.cpp Cube.h Transformation.cpp Transformation.h)

include_directories(.)
include_directories(cameras)
include_directories(images)
include_directories(scenes)

add_executable(Edit GnuWin32 Eigen/src/ ${SOURCE_FILES} Texture.h Texture.cpp Translation.cpp Translation.h Scaling.cpp Scaling.h Rotation.cpp Rotation.h Cube.cpp Cube.h Transformation.cpp Transformation.h)