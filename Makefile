CXXFLAGS += -g #-D_REENTRANT $(shell sdl-config --cflags)
LDFLAGS += -Wall
#LDLIBS += -lpthread $(shell sdl-config --libs)

ofiles = BoundingBox.o Container.o Cube.o Cylinder.o Sphere.o \
	Polynomial.o Pixmap.o Transformation.o

client_ofiles = $(ofiles) client.o
server_ofiles = $(ofiles) server.o

.PHONY: all
all: client server

client: $(client_ofiles)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

server: $(server_ofiles)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

.PHONY: depend
depend:
	makedepend -Y -- $(CFLAGS) -- $(wildcard *.cpp)

.PHONY: clean
clean:
	-$(RM) client $(client_ofiles) server $(server_ofiles)

.PHONY: docs
docs:
	doxygen

# DO NOT DELETE

BoundingBox.o: BoundingBox.h Vector.h Transformation.h Ray.h
Container.o: Container.h Object.h Vector.h Transformation.h Ray.h
Container.o: Intersection.h utils.h
Cube.o: Cube.h Object.h Vector.h Transformation.h Ray.h Intersection.h
Cylinder.o: Cylinder.h Object.h Vector.h Transformation.h Ray.h
Cylinder.o: Intersection.h
Pixmap.o: Pixmap.h
Polynomial.o: Polynomial.h
Renderer.o: Renderer.h
Sphere.o: Sphere.h Object.h Vector.h Transformation.h Ray.h Intersection.h
Transformation.o: Transformation.h
