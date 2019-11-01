PROJECT(pi C CXX)

INCLUDE_DIRECTORIES(../main)

ADD_EXECUTABLE(y_pi_driver y_pi_driver.cpp)
TARGET_LINK_Y(y_pi_driver)
INSTALL( TARGETS y_pi_driver RUNTIME DESTINATION bin )

FIND_PACKAGE(OpenMP)

IF(OpenMP_C_FOUND)
    ADD_EXECUTABLE(pi pi.c)
    TARGET_LINK_LIBRARIES(pi PUBLIC OpenMP::OpenMP_C)
	
    ADD_EXECUTABLE(pi-ng pi-ng.c)
    TARGET_LINK_LIBRARIES(pi-ng PUBLIC OpenMP::OpenMP_C)
	
    ADD_EXECUTABLE(pi-v2 pi-v2.c)
    TARGET_LINK_LIBRARIES(pi-v2 PUBLIC OpenMP::OpenMP_C)

IF(NOT MSVC)
	target_link_libraries(pi    m)
	target_link_libraries(pi-v2 m)
	target_link_libraries(pi-ng m)
ENDIF(NOT MSVC)

ENDIF(OpenMP_C_FOUND)
