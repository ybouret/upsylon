PROJECT(test-alchemy)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.lua)

ADD_EXECUTABLE(test-alchemy main.cpp ${srcs} ${data})
TARGET_LINK_Y(test-alchemy alchemy mkl lua++ jive)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(species test-alchemy species)
ADD_TEST(library test-alchemy library)
ADD_TEST(library test-alchemy actor)



ADD_CUSTOM_TARGET(utest-alchemy
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-alchemy)

INSTALL( TARGETS test-alchemy RUNTIME DESTINATION var/y )
