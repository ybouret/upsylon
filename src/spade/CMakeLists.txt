PROJECT(spade)


FILE( GLOB decl y/spade/*.hpp)
FILE( GLOB impl y/spade/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/y/spade )
SOURCE_GROUP(spade FILES ${srcs} )

FILE( GLOB field_decl y/spade/field/*.hpp)
FILE( GLOB field_impl y/spade/field/*.cpp)
SET( field_srcs ${field_decl} ${field_impl} )
INSTALL( FILES ${field_decl} DESTINATION include/y/spade/field )
SOURCE_GROUP(spade/field FILES ${field_srcs} )

FILE( GLOB layout_decl y/spade/layout/*.hpp)
FILE( GLOB layout_impl y/spade/layout/*.cpp)
SET( layout_srcs ${layout_decl} ${layout_impl} )
INSTALL( FILES ${layout_decl} DESTINATION include/y/spade/layout )
SOURCE_GROUP(spade/layout FILES ${layout_srcs} )

FILE( GLOB network_decl y/spade/network/*.hpp)
FILE( GLOB network_impl y/spade/network/*.cpp)
SET( network_srcs ${network_decl} ${network_impl} )
INSTALL( FILES ${network_decl} DESTINATION include/y/spade/network )
SOURCE_GROUP(spade/network FILES ${network_srcs} )

FILE( GLOB mesh_decl y/spade/mesh/*.hpp)
FILE( GLOB mesh_impl y/spade/mesh/*.cpp)
SET( mesh_srcs ${mesh_decl} ${mesh_impl} )
INSTALL( FILES ${mesh_decl} DESTINATION include/y/spade/mesh )
SOURCE_GROUP(spade/mesh FILES ${mesh_srcs} )

FILE( GLOB vtk_decl y/spade/vtk/*.hpp)
FILE( GLOB vtk_impl y/spade/vtk/*.cpp)
SET( vtk_srcs ${vtk_decl} ${vtk_impl} )
INSTALL( FILES ${vtk_decl} DESTINATION include/y/spade/vtk )
SOURCE_GROUP(spade/vtk FILES ${vtk_srcs} )

INCLUDE_DIRECTORIES(. ../main)

ADD_LIBRARY( y-spade STATIC 
${srcs} 
${field_srcs}
${layout_srcs}
${network_srcs}
${mesh_srcs}
${vtk_srcs}
)
INSTALL( TARGETS y-spade ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
