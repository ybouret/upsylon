PROJECT(test-mpi)

INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

CMAKE_POLICY(SET CMP0004 OLD)
ADD_EXECUTABLE(test-mpi main.cpp ${srcs})
TARGET_LINK_Y(test-mpi mpi)
TARGET_LINK_LIBRARIES(test-mpi ${MPI_LIBRARIES} ${MPI_LINK_FLAGS})

ENABLE_TESTING()
Y_MPI(init     test-mpi init)
Y_MPI(oxide    test-mpi oxide)
Y_MPI(io       test-mpi io)
Y_MPI(topology test-mpi topology)

ADD_CUSTOM_TARGET(utest-mpi
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-mpi)

