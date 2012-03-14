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

CFLAGS= $(FL_INCLUDE) -O2 -ggdb -pg

LFLAGS = -O2 -ggdb -pg

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
# DO NOT DELETE

Datafile.o: Datafile.h Point.h Vector.h /usr/include/GL/gl.h
Datafile.o: /usr/include/GL/glext.h /usr/include/inttypes.h
Datafile.o: /usr/include/stdint.h /usr/include/_ansi.h /usr/include/newlib.h
Datafile.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
Datafile.o: /usr/include/sys/features.h /usr/include/GL/glu.h
MST.o: MST.h Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
MST.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
MST.o: /usr/include/newlib.h /usr/include/sys/config.h
MST.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Path.o: Datafile.h Point.h Vector.h /usr/include/GL/gl.h
Path.o: /usr/include/GL/glext.h /usr/include/inttypes.h /usr/include/stdint.h
Path.o: /usr/include/_ansi.h /usr/include/newlib.h /usr/include/sys/config.h
Path.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h MST.h
Path.o: Path.h /usr/include/GL/glu.h QuadraticSegment.h Plane.h
Path.o: /usr/include/gl/glut.h /usr/include/gl/freeglut_std.h
Path.o: /usr/include/stdlib.h /usr/include/sys/reent.h
Path.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
Path.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
Path.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
Plane.o: Plane.h Point.h Vector.h /usr/include/GL/gl.h
Plane.o: /usr/include/GL/glext.h /usr/include/inttypes.h
Plane.o: /usr/include/stdint.h /usr/include/_ansi.h /usr/include/newlib.h
Plane.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
Plane.o: /usr/include/sys/features.h
pp1.o: /usr/include/GL/gl.h /usr/include/GL/glext.h /usr/include/inttypes.h
pp1.o: /usr/include/stdint.h /usr/include/_ansi.h /usr/include/newlib.h
pp1.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
pp1.o: /usr/include/sys/features.h /usr/include/GL/glu.h
pp1.o: /usr/include/GL/glut.h /usr/include/gl/freeglut_std.h
pp1.o: /usr/include/stdlib.h /usr/include/sys/reent.h
pp1.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
pp1.o: /usr/include/machine/_default_types.h /usr/include/sys/lock.h
pp1.o: /usr/include/machine/stdlib.h /usr/include/alloca.h pp1.h pp1_ui.h
pp1.o: /usr/include/FL/Fl.H /usr/include/FL/Enumerations.H
pp1.o: /usr/include/FL/Fl_Export.H /usr/include/FL/Fl_Gl_Window.h
pp1.o: /usr/include/FL/Fl_Window.H /usr/include/FL/Fl_Group.H
pp1.o: /usr/include/FL/Fl_Widget.H /usr/include/FL/Fl_Double_Window.H
pp1.o: /usr/include/FL/Fl_Box.H /usr/include/FL/Fl_Button.H
pp1.o: /usr/include/FL/Fl_Value_Slider.H /usr/include/FL/Fl_Slider.H
pp1.o: /usr/include/FL/Fl_Valuator.H /usr/include/math.h /usr/include/stdio.h
pp1.o: /usr/include/sys/types.h /usr/include/machine/types.h
pp1.o: /usr/include/sys/stdio.h /usr/include/sys/cdefs.h Datafile.h Point.h
pp1.o: Vector.h Matrix.h Tour.h Spline.h ControlPoint.h Triangulator.h
pp1.o: /usr/include/gl/GL.h minmaxer/source.triangulation/geometry.objects.h
pp1.o: minmaxer/source.triangulation/graph.h
pp1.o: minmaxer/source.triangulation/quadedge.h
pp1.o: minmaxer/source.triangulation/persistent.quadedge.h
pp1.o: minmaxer/source.triangulation/longmath.h
pp1.o: minmaxer/source.triangulation/triangulation.h
pp1_ui.o: pp1_ui.h /usr/include/FL/Fl.H /usr/include/FL/Enumerations.H
pp1_ui.o: /usr/include/FL/Fl_Export.H /usr/include/FL/Fl_Gl_Window.h
pp1_ui.o: /usr/include/FL/Fl_Window.H /usr/include/FL/Fl_Group.H
pp1_ui.o: /usr/include/FL/Fl_Widget.H /usr/include/FL/Fl_Double_Window.H
pp1_ui.o: /usr/include/FL/Fl_Box.H /usr/include/FL/Fl_Button.H
pp1_ui.o: /usr/include/FL/Fl_Value_Slider.H /usr/include/FL/Fl_Slider.H
pp1_ui.o: /usr/include/FL/Fl_Valuator.H
ProgramObject.o: ProgramObject.h Shader.h /usr/include/GL/gl.h
ProgramObject.o: /usr/include/GL/glext.h /usr/include/inttypes.h
ProgramObject.o: /usr/include/stdint.h /usr/include/_ansi.h
ProgramObject.o: /usr/include/newlib.h /usr/include/sys/config.h
ProgramObject.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Shader.o: Shader.h
Tour.o: Tour.h Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
Tour.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
Tour.o: /usr/include/newlib.h /usr/include/sys/config.h
Tour.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h Datafile.h
Tour.o: MST.h Path.h /usr/include/GL/glu.h QuadraticSegment.h Plane.h
Triangulator.o: Triangulator.h Datafile.h Point.h Vector.h
Triangulator.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
Triangulator.o: /usr/include/inttypes.h /usr/include/stdint.h
Triangulator.o: /usr/include/_ansi.h /usr/include/newlib.h
Triangulator.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
Triangulator.o: /usr/include/sys/features.h /usr/include/gl/GL.h
Triangulator.o: minmaxer/source.triangulation/geometry.objects.h
Triangulator.o: minmaxer/source.triangulation/graph.h
Triangulator.o: minmaxer/source.triangulation/quadedge.h
Triangulator.o: minmaxer/source.triangulation/persistent.quadedge.h
Triangulator.o: minmaxer/source.triangulation/longmath.h
Triangulator.o: minmaxer/source.triangulation/triangulation.h
Vector.o: Vector.h
ControlPoint.o: Point.h Vector.h
Datafile.o: Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
Datafile.o: /usr/include/inttypes.h /usr/include/stdint.h
Datafile.o: /usr/include/_ansi.h /usr/include/newlib.h
Datafile.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
Datafile.o: /usr/include/sys/features.h
MST.o: Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
MST.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
MST.o: /usr/include/newlib.h /usr/include/sys/config.h
MST.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Path.o: /usr/include/GL/glu.h /usr/include/GL/gl.h /usr/include/GL/glext.h
Path.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
Path.o: /usr/include/newlib.h /usr/include/sys/config.h
Path.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Path.o: QuadraticSegment.h Plane.h Point.h Vector.h
Plane.o: Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
Plane.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
Plane.o: /usr/include/newlib.h /usr/include/sys/config.h
Plane.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Point.o: Vector.h
pp1_ui.o: /usr/include/FL/Fl.H /usr/include/FL/Enumerations.H
pp1_ui.o: /usr/include/FL/Fl_Export.H /usr/include/FL/Fl_Gl_Window.h
pp1_ui.o: /usr/include/FL/Fl_Window.H /usr/include/FL/Fl_Group.H
pp1_ui.o: /usr/include/FL/Fl_Widget.H /usr/include/FL/Fl_Double_Window.H
pp1_ui.o: /usr/include/FL/Fl_Box.H /usr/include/FL/Fl_Button.H
pp1_ui.o: /usr/include/FL/Fl_Value_Slider.H /usr/include/FL/Fl_Slider.H
pp1_ui.o: /usr/include/FL/Fl_Valuator.H
ProgramObject.o: Shader.h /usr/include/GL/gl.h /usr/include/GL/glext.h
ProgramObject.o: /usr/include/inttypes.h /usr/include/stdint.h
ProgramObject.o: /usr/include/_ansi.h /usr/include/newlib.h
ProgramObject.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
ProgramObject.o: /usr/include/sys/features.h
QuadraticSegment.o: Plane.h Point.h Vector.h /usr/include/GL/gl.h
QuadraticSegment.o: /usr/include/GL/glext.h /usr/include/inttypes.h
QuadraticSegment.o: /usr/include/stdint.h /usr/include/_ansi.h
QuadraticSegment.o: /usr/include/newlib.h /usr/include/sys/config.h
QuadraticSegment.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
QuadraticSegment.o: /usr/include/GL/glu.h
Spline.o: ControlPoint.h Point.h Vector.h
Tour.o: Point.h Vector.h /usr/include/GL/gl.h /usr/include/GL/glext.h
Tour.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/_ansi.h
Tour.o: /usr/include/newlib.h /usr/include/sys/config.h
Tour.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Triangulator.o: Datafile.h Point.h Vector.h /usr/include/GL/gl.h
Triangulator.o: /usr/include/GL/glext.h /usr/include/inttypes.h
Triangulator.o: /usr/include/stdint.h /usr/include/_ansi.h
Triangulator.o: /usr/include/newlib.h /usr/include/sys/config.h
Triangulator.o: /usr/include/machine/ieeefp.h /usr/include/sys/features.h
Triangulator.o: /usr/include/gl/GL.h
Triangulator.o: minmaxer/source.triangulation/geometry.objects.h
Triangulator.o: minmaxer/source.triangulation/graph.h
Triangulator.o: minmaxer/source.triangulation/quadedge.h
Triangulator.o: minmaxer/source.triangulation/persistent.quadedge.h
Triangulator.o: minmaxer/source.triangulation/longmath.h
Triangulator.o: minmaxer/source.triangulation/triangulation.h
