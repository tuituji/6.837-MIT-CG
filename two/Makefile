#INCFLAGS = -I /mit/6.837/public/include/vecmath
#INCFLAGS += -I /mit/glut/include
#LINKFLAGS = -L /mit/6.837/public/lib -l vecmath
#LINKFLAGS += -L /mit/glut/lib -lGL -lGLU -lglut -lX11 -lXi
#INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./vecmath/include
#INCFLAGS += -I ~/vecmath/include

LINKFLAGS  = -lglut -lGL -lGLU
#LINKFLAGS += -L /mit/6.837/public/lib -lvecmath
#LINKFLAGS += -L ~/vecmath/lib -lvecmath
LINKFLAGS += -lfltk -lfltk_gl

CFLAGS    = -g
CFLAGS    += -DSOLN
CC        = g++
SRCS      = bitmap.cpp camera.cpp MatrixStack.cpp modelerapp.cpp modelerui.cpp ModelerView.cpp Joint.cpp SkeletalModel.cpp Mesh.cpp main.cpp
SRCS      += $(wildcard vecmath/src/*.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a2

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

bitmap.o: bitmap.h
camera.o: camera.h
Mesh.o: Mesh.h
MatrixStack.o: MatrixStack.h
modelerapp.o: modelerapp.h ModelerView.h modelerui.h bitmap.h camera.h
modelerui.o: modelerui.h ModelerView.h bitmap.h camera.h modelerapp.h
ModelerView.o: ModelerView.h camera.h
SkeletalModel.o: MatrixStack.h ModelerView.h Joint.h modelerapp.h

