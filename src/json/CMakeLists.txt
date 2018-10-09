PROJECT(json)

MACRO(Y_DYNAMO file_g)
SET(local_file_g ${CMAKE_CURRENT_SOURCE_DIR}/${file_g})
get_filename_component(local_dir  ${local_file_g} DIRECTORY)
get_filename_component(local_base ${local_file_g} NAME_WE)
SET(local_file_def "${local_dir}/${local_base}.def" )
SET(local_file_inc "${local_dir}/${local_base}.inc" )
MESSAGE( STATUS "dynamo[${local_file_g}]")
MESSAGE( STATUS "|_____[${local_file_def}]")
MESSAGE( STATUS "|_____[${local_file_inc}]")
ADD_CUSTOM_COMMAND(
OUTPUT ${local_file_def} ${local_file_inc}
COMMAND y-dynamo ${local_file_g}
DEPENDS y-dynamo ${local_file_g}
)
ENDMACRO(Y_DYNAMO)

#ADD_CUSTOM_COMMAND(
#OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/y/json/json.def ${CMAKE_CURRENT_SOURCE_DIR}/y/json/json.inc
#COMMAND y-dynamo ${CMAKE_CURRENT_SOURCE_DIR}/y/json/json.g
#DEPENDS y-dynamo ${CMAKE_CURRENT_SOURCE_DIR}/y/json/json.g)

Y_DYNAMO(y/json/json.g)

FILE( GLOB decl y/json/*.hpp)
FILE( GLOB impl y/json/*.cpp)
SET( srcs ${decl} ${impl} y/json/json.g y/json/json.def y/json/json.inc)
INSTALL( FILES ${decl} DESTINATION include/y/json )
SOURCE_GROUP( json FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../main ../lang)

ADD_LIBRARY( y-json STATIC ${srcs}  )
INSTALL( TARGETS y-json ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
