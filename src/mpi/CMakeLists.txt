PROJECT(y-mpi CXX)
MESSAGE( STATUS "<MPI>" )
INCLUDE_DIRECTORIES(. ../main)
INCLUDE(../../scripts/mpi.cmake)

IF(MPI_FOUND)
	FILE(GLOB main_decl y/mpi/*.hpp)
	FILE(GLOB main_code y/mpi/*.cpp)
	SET(main_srcs ${main_decl} ${main_code})
	SOURCE_GROUP( mpi FILES ${main_srcs} )
	INSTALL( FILES ${main_decl} DESTINATION include/y/mpi )
	
	ADD_LIBRARY(y-mpi STATIC 
	${main_srcs}
	)
	INSTALL( TARGETS y-mpi ARCHIVE DESTINATION lib )
	
	ADD_SUBDIRECTORY(tests)
	SET(UTESTS "${UTESTS} utest-mpi" PARENT_SCOPE)
ENDIF()

MESSAGE( STATUS "<MPI/>" )
MESSAGE(STATUS "")
