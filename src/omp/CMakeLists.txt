PROJECT(pi C CXX)

INCLUDE_DIRECTORIES(../main)

add_executable(pi-driver pi-driver.cpp)
TARGET_LINK_Y(pi-driver)

FIND_PACKAGE(openmp)

IF(OpenMP_C_FOUND)
    add_executable(pi pi.c)
    target_link_libraries(pi PUBLIC OpenMP::OpenMP_C)
ENDIF(OpenMP_C_FOUND)
