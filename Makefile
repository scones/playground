CC             = g++

INCLUDES       = -Iinclude -I/c/msys64/mingw64/include -I/c/msys64/mingw64/x86_64-w64-mingw32/include

ifeq ($(MSYSTEM), MINGW64)
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

ifeq ($(MSYSTEM), MSYS)
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif

CFLAGS         = $(INCLUDES) -O1 -c -Wall -pedantic -std=c++11 -finput-charset=UTF-8 -DGLEW_STATIC -DCHAISCRIPT_NO_THREADS -DDEBUG -ggdb

LIB_PATHS      = -Llib
LIBS_STATIC_OPENGL    = -lglew32 -lglfw3 -lopengl32 -lgdi32
LIBS_STATIC_FREETYPE  = -lfreetype64 #-lpng -lz -lharfbuzz -lbz2 #-lglib-2.0 -lbz2 -lintl -liconv -ole32
LIBS_STATIC_BOOST     = -lboost_system-mt -lboost_program_options-mt -lboost_filesystem-mt
LIBS_STATIC_SYSTEM    = -lwinmm
LIBS_STATIC_JS        = -lduktape
LIBS_SHARED_SYSTEM    = -lluajit-5.1

ifeq ($(MSYSTEM), MINGW64)
	LIBS_SYSTEM += -lwinmm -mwindows
else
endif

LIBS_STATIC    = -Wl,-Bstatic  $(LIBS_STATIC_FREETYPE) $(LIBS_STATIC_OPENGL) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_BOOST) $(LIBS_STATIC_SYSTEM) $(LIBS_STATIC_JS)
LIBS_SHARED    = -Wl,-Bdynamic $(LIBS_SHARED_SYSTEM)
LIBS           = $(LIBS_STATIC) $(LIBS_SHARED)

LIBS_LIB       =
LDFLAGS        = ${LIB_PATHS} ${LIBS}
LDFLAGS_LIB    = ${LIB_PATHS} ${LIBS_LIB}
MAKE           = make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_LIB       = $(call find, src, *.cpp)
SOURCES           = $(SOURCES_LIB) main.cpp
OBJECTS           = $(SOURCES:.cpp=.o)
OBJECTS_LIB       = $(SOURCES_LIB:.cpp=.o)
STATIC_TARGET     = lib/libplayground.a
EXECUTABLE_TARGET = playground.exe

FOO = $(call pwd)

all: $(SOURCES) $(STATIC_TARGET) $(EXECUTABLE_TARGET)
#	./$(EXECUTABLE_TARGET)


$(EXECUTABLE_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


$(STATIC_TARGET): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	rm -f $(OBJECTS)
	rm -f $(STATIC_TARGET)
	rm -f $(EXECUTABLE_TARGET)
	rm -f gmon.out

