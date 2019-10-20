PROJECT(pi C CXX)

INCLUDE_DIRECTORIES(../main)

add_executable(y_pi_driver y_pi_driver.cpp)
TARGET_LINK_Y(y_pi_driver)
INSTALL( TARGETS y_pi_driver RUNTIME DESTINATION bin )

FIND_PACKAGE(OpenMP)

IF(OpenMP_C_FOUND)
    add_executable(pi pi.c)
    target_link_libraries(pi PUBLIC OpenMP::OpenMP_C)
	
	add_executable(pi-ng pi-ng.c)
    target_link_libraries(pi-ng PUBLIC OpenMP::OpenMP_C)
	
    add_executable(pi_v2 pi_v2.c)
    target_link_libraries(pi_v2 PUBLIC OpenMP::OpenMP_C)
ENDIF(OpenMP_C_FOUND)
