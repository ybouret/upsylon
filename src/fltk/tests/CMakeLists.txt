PROJECT(test-fltk)
FILE(GLOB test_srcs test-*.cpp)
FLUID_UIC(matte)
FLUID_UIC(canvas)

ADD_EXECUTABLE(test-fltk 
main.cpp 
${test_srcs} 
${canvas_SOURCES}
${matte_SOURCES}
)

TARGET_LINK_Y( test-fltk fltk)
TARGET_LINK_FLTK( test-fltk )

