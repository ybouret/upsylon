
ADD_LIBRARY(engine MODULE engine.cpp)
TARGET_LINK_LIBRARIES(engine y)

SET(the_engine ${CMAKE_CURRENT_SOURCE_DIR}/engine.dll)
add_custom_target(engine_copy 
COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:engine> ${the_engine}
DEPENDS engine
COMMENT "Copy local engine.dll"
)
