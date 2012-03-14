###############################################################################
# complier definition and its flags
##############################################################################
CC= g++

SHELL=/bin/sh

# FLTK directories -- edit these to specify non-default FLTK location
FL_INCLUDE =
FL_LIBDIR =
FL_LIBS = -lfltk_gl -lfltk

# Additional libraries needed to link with OpenGL and any platform specific stuff
GLLIBS = -lGL -lGLU -lglut -lX11 -lm -ltriangulation -Lminmaxer/source.triangulation

CFLAGS= $(FL_INCLUDE) -O3 -ggdb -pg

LFLAGS = -O3 -ggdb -pg

###############################################################################
# basic headers and files *.o to be linked together
###############################################################################
HEADBAS= pp1_ui.h pp1.h
OBJBAS= pp1.o Datafile.o MST.o Path.o Plane.o Tour.o Triangulator.o Vector.o pp1_ui.o

###############################################################################
# files to be build in default case
###############################################################################
all: pp1

###############################################################################
# linking
###############################################################################
pp1: $(OBJBAS)
		@echo Linking pp1
		$(CC) -o pp1 $^ $(FL_INCLUDE) $(LFLAGS) $(FL_LIBDIR) \
		$(FL_LIBS) $(GLLIBS) 

%.o: %.cxx
	$(CC) $(CFLAGS) -c $<

###############################################################################
# compilation of main file
###############################################################################
pp1.o: pp1_ui.h pp1.cxx pp1.h
		@echo Compiling pp1
		$(CC) $(CFLAGS) -c pp1.cxx

###############################################################################
# compilation of other files
###############################################################################
pp1_ui.o: pp1_ui.h pp1_ui.cxx
		@echo Compiling pp1_ui
		$(CC) $(CFLAGS) -c pp1_ui.cxx

pp1_ui.cxx: pp1_ui.fd
		fluid -c pp1_ui.fd

pp1_ui.h: pp1_ui.cxx

###############################################################################
# initial procedure
###############################################################################
clean:
		@rm -fr *.o *% .*% \
		pp1_ui.cxx pp1_ui.h \
		pp1
