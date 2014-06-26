INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I 	./include/vecmath

LINKFLAGS  = -lglut -lGL -lGLU -lGLEW
#LINKFLAGS += -L /mit/6.837/public/lib -lvecmath

CFLAGS    = -O2 #-g3
CC        = g++
SRCS      = main.cpp
SRCS     += $(wildcard vecmath/*.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

# DO NOT DELETE

main.o: ./include/vecmath/vecmath.h ./include/vecmath/Matrix2f.h
main.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Matrix4f.h
main.o: ./include/vecmath/Quat4f.h ./include/vecmath/Vector2f.h
main.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Vector3f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector2f.h
vecmath/Vector2f.o: ./include/vecmath/Vector2f.h ./include/vecmath/Vector3f.h
vecmath/Matrix3f.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Matrix2f.h
vecmath/Matrix3f.o: ./include/vecmath/Quat4f.h ./include/vecmath/Vector3f.h
vecmath/Matrix4f.o: ./include/vecmath/Matrix4f.h ./include/vecmath/Matrix2f.h
vecmath/Matrix4f.o: ./include/vecmath/Matrix3f.h ./include/vecmath/Quat4f.h
vecmath/Matrix4f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Vector4f.o: ./include/vecmath/Vector4f.h ./include/vecmath/Vector2f.h
vecmath/Vector4f.o: ./include/vecmath/Vector3f.h
vecmath/Quat4f.o: ./include/vecmath/Quat4f.h ./include/vecmath/Matrix3f.h
vecmath/Quat4f.o: ./include/vecmath/Vector3f.h ./include/vecmath/Vector4f.h
vecmath/Matrix2f.o: ./include/vecmath/Matrix2f.h ./include/vecmath/Vector2f.h
