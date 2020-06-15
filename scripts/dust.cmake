
SET(scripts 
clean
nosdk 
nodoc
make-clean
)

FOREACH(script IN LISTS scripts )
MESSAGE( STATUS " << executing ${script}.cmake >> ")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -P scripts/${script}.cmake)
ENDFOREACH()
