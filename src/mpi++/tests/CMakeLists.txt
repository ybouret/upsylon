PROJECT(test-mpi++)

INCLUDE_DIRECTORIES(../../main/tests ../../spade)
FILE(GLOB srcs test-*.cpp)

CMAKE_POLICY(SET CMP0004 OLD)
ADD_EXECUTABLE(test-mpi++ main.cpp ${srcs})
TARGET_LINK_Y(test-mpi++ mpi++ spade)
TARGET_LINK_LIBRARIES(test-mpi++ ${MPI_LIBRARIES} ${MPI_LINK_FLAGS})

ENABLE_TESTING()
Y_MPI(init      test-mpi++ init)
Y_MPI(init_env  test-mpi++ init_env)
Y_MPI(ring      test-mpi++ ring)
Y_MPI(coll      test-mpi++ coll)
Y_MPI(xmpi      test-mpi++ xmpi)
Y_MPI(spade     test-mpi++ spade)

ADD_CUSTOM_TARGET(utest-mpi++
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-mpi++)

INSTALL( TARGETS test-mpi++ RUNTIME DESTINATION var/y )
