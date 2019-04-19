FILE( GLOB_RECURSE Makefiles "makefile")
#MESSAGE( ${Makefiles} )

FOREACH(f IN LISTS Makefiles )
MESSAGE(${f})
ENDFOREACH(f)
