SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
MESSAGE("")
MESSAGE( STATUS "cleaning up...")
SET(to_remove "")

FUNCTION(__ADD_PATTERN pattern)
	FILE(GLOB tmp ${pattern})
	SET(to_remove ${to_remove} ${tmp} PARENT_SCOPE)
ENDFUNCTION(__ADD_PATTERN)

MESSAGE( STATUS "collecting local temporary files")
__ADD_PATTERN(*.o)
__ADD_PATTERN(*.dat)
__ADD_PATTERN(*.bin)
__ADD_PATTERN(*.png)
__ADD_PATTERN(*.jpg)
__ADD_PATTERN(*.eps)
__ADD_PATTERN(*.ppm)
__ADD_PATTERN(*.bmp)
__ADD_PATTERN(*.tif)
__ADD_PATTERN(*.stl)
__ADD_PATTERN(*.vtk)
__ADD_PATTERN(*.dot)


IF(APPLE)
MESSAGE( STATUS "collecting MacOS dumps")
FILE(GLOB_RECURSE tmp .DS_Store)
SET(to_remove ${to_remove} ${tmp})
ENDIF()

FILE(GLOB_RECURSE tmp .Rhistory)
SET(to_remove ${to_remove} ${tmp})


MESSAGE( STATUS "collecting old backup files" )
FILE(GLOB_RECURSE tmp "src/*~")
SET(to_remove ${to_remove} ${tmp})

#specific stuff
LIST( APPEND to_remove "src/r++/tests/rmodule.so"  )
LIST( APPEND to_remove "src/r++/tests/rmodule.dll" )
LIST( APPEND to_remove "src/main/tests/module.dll" )
LIST( APPEND to_remove "src/main/dll/engine.dll"   )
LIST( APPEND to_remove "src/main/dll/__pycache__"  )

FOREACH(item IN LISTS to_remove)
	IF(EXISTS "${item}")
		MESSAGE("removing ${item}")
		IF(IS_DIRECTORY "${item}")
 			EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E rm -Rf ${item})
		ELSE()
			EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E rm -f ${item})
		ENDIF()
	ENDIF()
ENDFOREACH(item)

MESSAGE("")


