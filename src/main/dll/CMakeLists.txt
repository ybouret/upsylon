
ADD_LIBRARY(engine MODULE engine.cpp 
engine.py test-engine.py test-engine-numpy.py)
TARGET_LINK_Y(engine)

SET(the_engine ${CMAKE_CURRENT_SOURCE_DIR}/engine.dll)
add_custom_target(engine_copy ALL
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:engine> ${the_engine}
DEPENDS engine
COMMENT "Copy local engine.dll"
)
