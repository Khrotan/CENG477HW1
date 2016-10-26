cmake_minimum_required(VERSION 3.6)
project(Edit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    cameras/att_test_camera.txt
    cameras/bunny_camera.txt
    cameras/spheres_camera.txt
    cameras/teapot_camera.txt
    images/att_test.png
    images/bunny.png
    images/spheres.png
    images/teapot.png
    scenes/att_test_scene.txt
    scenes/bunny_scene.txt
    scenes/spheres_scene.txt
    scenes/teapot_scene.txt
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
    Vertex.h)

include_directories(.)
include_directories(cameras)
include_directories(images)
include_directories(scenes)

add_executable(Edit ${SOURCE_FILES})