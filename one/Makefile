INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./vecmath/include

LINKFLAGS = -lglut -lGL -lGLU
#LINKFLAGS += -L /mit/6.837/public/lib -lvecmath

CFLAGS    = -g3 -Wall -ansi -DSOLN
CC        = g++
SRCS      = main.cpp parse.cpp curve.cpp surf.cpp camera.cpp
SRCS     += $(wildcard vecmath/src/*.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a1

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:

	rm $(OBJS) $(PROG)
# DO NOT DELETE

main.o: ./vecmath/include/vecmath.h ./vecmath/include/Matrix2f.h
main.o: ./vecmath/include/Matrix3f.h ./vecmath/include/Matrix4f.h
main.o: ./vecmath/include/Quat4f.h ./vecmath/include/Vector2f.h
main.o: ./vecmath/include/Vector3f.h ./vecmath/include/Vector4f.h parse.h
main.o: curve.h tuple.h surf.h extra.h camera.h
parse.o: parse.h curve.h tuple.h ./vecmath/include/vecmath.h
parse.o: ./vecmath/include/Matrix2f.h ./vecmath/include/Matrix3f.h
parse.o: ./vecmath/include/Matrix4f.h ./vecmath/include/Quat4f.h
parse.o: ./vecmath/include/Vector2f.h ./vecmath/include/Vector3f.h
parse.o: ./vecmath/include/Vector4f.h surf.h
curve.o: curve.h tuple.h ./vecmath/include/vecmath.h
curve.o: ./vecmath/include/Matrix2f.h ./vecmath/include/Matrix3f.h
curve.o: ./vecmath/include/Matrix4f.h ./vecmath/include/Quat4f.h
curve.o: ./vecmath/include/Vector2f.h ./vecmath/include/Vector3f.h
curve.o: ./vecmath/include/Vector4f.h extra.h
surf.o: surf.h curve.h tuple.h ./vecmath/include/vecmath.h
surf.o: ./vecmath/include/Matrix2f.h ./vecmath/include/Matrix3f.h
surf.o: ./vecmath/include/Matrix4f.h ./vecmath/include/Quat4f.h
surf.o: ./vecmath/include/Vector2f.h ./vecmath/include/Vector3f.h
surf.o: ./vecmath/include/Vector4f.h extra.h
camera.o: camera.h ./vecmath/include/vecmath.h ./vecmath/include/Matrix2f.h
camera.o: ./vecmath/include/Matrix3f.h ./vecmath/include/Matrix4f.h
camera.o: ./vecmath/include/Quat4f.h ./vecmath/include/Vector2f.h
camera.o: ./vecmath/include/Vector3f.h ./vecmath/include/Vector4f.h extra.h
vecmath/src/Vector3f.o: ./vecmath/include/Vector3f.h
vecmath/src/Vector3f.o: ./vecmath/include/Vector2f.h
vecmath/src/Vector2f.o: ./vecmath/include/Vector2f.h
vecmath/src/Vector2f.o: ./vecmath/include/Vector3f.h
vecmath/src/Matrix3f.o: ./vecmath/include/Matrix3f.h
vecmath/src/Matrix3f.o: ./vecmath/include/Matrix2f.h
vecmath/src/Matrix3f.o: ./vecmath/include/Quat4f.h
vecmath/src/Matrix3f.o: ./vecmath/include/Vector3f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Matrix4f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Matrix2f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Matrix3f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Quat4f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Vector3f.h
vecmath/src/Matrix4f.o: ./vecmath/include/Vector4f.h
vecmath/src/Vector4f.o: ./vecmath/include/Vector4f.h
vecmath/src/Vector4f.o: ./vecmath/include/Vector2f.h
vecmath/src/Vector4f.o: ./vecmath/include/Vector3f.h
vecmath/src/Quat4f.o: ./vecmath/include/Quat4f.h ./vecmath/include/Matrix3f.h
vecmath/src/Quat4f.o: ./vecmath/include/Vector3f.h
vecmath/src/Quat4f.o: ./vecmath/include/Vector4f.h
vecmath/src/Matrix2f.o: ./vecmath/include/Matrix2f.h
vecmath/src/Matrix2f.o: ./vecmath/include/Vector2f.h
