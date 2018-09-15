INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-mpi main.cpp ${srcs})
TARGET_LINK_Y(test-mpi mpi)
TARGET_LINK_LIBRARIES(test-mpi ${MPI_LIBRARIES})
