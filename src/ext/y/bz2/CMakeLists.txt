PROJECT(bz2 C)

SET(srcs
	bzlib.h 
	bzlib_private.h
	blocksort.c 
	huffman.c   
	crctable.c  
	randtable.c 
	compress.c  
	decompress.c
	bzlib.c
)

ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)
ADD_LIBRARY(y-bz2 STATIC ${srcs})
SOURCE_GROUP( bz2 FILES ${srcs} )
INSTALL( FILES   bzlib.h DESTINATION include/y/bz2 )
INSTALL( TARGETS y-bz2     ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(y_bzip2 bzip2.c)
TARGET_LINK_LIBRARIES(y_bzip2 y-bz2)

SET(file1 ${CMAKE_CURRENT_SOURCE_DIR}/sample1.ref)
SET(file2 ${CMAKE_CURRENT_SOURCE_DIR}/sample2.ref)
SET(file3 ${CMAKE_CURRENT_SOURCE_DIR}/sample3.ref)

ADD_CUSTOM_TARGET( utest-bz2
	COMMAND y_bzip2 -1 < ${file1} > sample1.bz2
	COMMAND y_bzip2 -d < sample1.bz2 > sample1.tst
	COMMAND ${CMAKE_COMMAND} -E compare_files sample1.tst ${file1}
	COMMAND y_bzip2  -2 < ${file2} > sample2.bz2
	COMMAND y_bzip2  -d < sample2.bz2 > sample2.tst
	COMMAND ${CMAKE_COMMAND}  -E compare_files sample2.tst ${file2}
	COMMAND y_bzip2  -3  < ${file3} > sample3.bz2
	COMMAND y_bzip2  -ds < sample3.bz2 > sample3.tst
	COMMAND ${CMAKE_COMMAND}  -E compare_files sample3.tst ${file3}
	COMMENT "Testing bz2" 
	)

