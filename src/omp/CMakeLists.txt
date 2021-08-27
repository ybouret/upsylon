PROJECT(pi C CXX)

INCLUDE_DIRECTORIES(../main)

ADD_EXECUTABLE(y_pi_driver y_pi_driver.cpp)
TARGET_LINK_Y(y_pi_driver)
INSTALL( TARGETS y_pi_driver RUNTIME DESTINATION bin )

IF( "${CMAKE_GENERATOR}" MATCHES "Xcode" )
	#Bypass Xcode testing, not supported by apple clang so far
	MESSAGE( STATUS "Skipping OpenMP for Xcode" )
ELSE()
FIND_PACKAGE(OpenMP)




IF(OpenMP_C_FOUND)

	# check the need to libm
	IF(NOT MSVC)
		SET(LIBM m)
	ELSE(NOT MSVC)
		SET(LIBM)
	ENDIF(NOT MSVC)

	# check the need for libatomic
	SET(LIBATOMIC)
	IF(Y_FREEBSD AND Y_CLANG)
		#MESSAGE( STATUS "need libatomic" )
		#FIND_FILE(LIBATOMIC libatomic.a PATHS /usr/local/lib/gcc9)
		#MESSAGE( STATUS "LIBATOMIC=${LIBATOMIC}" )		
	ENDIF(Y_FREEBSD AND Y_CLANG)

    ADD_EXECUTABLE(pi pi.c)
    TARGET_LINK_LIBRARIES(pi PUBLIC OpenMP::OpenMP_C ${LIBM} ${LIBATOMIC})
	
    ADD_EXECUTABLE(pi-ng pi-ng.c)
    TARGET_LINK_LIBRARIES(pi-ng PUBLIC OpenMP::OpenMP_C ${LIBM} ${LIBATOMIC})
	
    ADD_EXECUTABLE(pi-v2 pi-v2.c)
    TARGET_LINK_LIBRARIES(pi-v2 PUBLIC OpenMP::OpenMP_C ${LIBM} ${LIBATOMIC})


ENDIF(OpenMP_C_FOUND)

ENDIF()
