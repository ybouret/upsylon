FILE(GLOB_RECURSE entries "./forge/*/Debug*" LIST_DIRECTORIES true)
FOREACH( ep IN ITEMS ${entries} )
	MESSAGE( ${ep} )
ENDFOREACH()
