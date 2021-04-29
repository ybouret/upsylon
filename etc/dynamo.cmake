# macro to generate definition and code for langage compiler

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
