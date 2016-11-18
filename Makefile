SRC = jpeg_reader/aoa_jpeg.c Camera.cpp Cube.cpp Image.cpp main.cpp Mesh.cpp Rotation.cpp Scaling.cpp Scene.cpp Sphere.cpp Texture.cpp Transformation.cpp Translation.cpp Triangle.cpp Vector3.cpp

OBJ = $(SRC:.cpp=.o)
	CFLAGS = -std=c++11 -O3 -ljpeg

.cpp.o:
		g++ -c $< $(CFLAGS)

all: test

test: $(OBJ)
		g++ $(OBJ) -o raytracer $(CFLAGS)

clean:
		rm -f *.o raytracer *.ppm
