PROJECT(y-aqua CXX)
MESSAGE( STATUS "<Aqua>" )

INCLUDE_DIRECTORIES(. ../main ../mkl )

FILE(GLOB main_decl y/aqua/*.hpp)
FILE(GLOB main_code y/aqua/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( aqua FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/aqua )

FILE(GLOB equilibrium_decl y/aqua/equilibrium/*.hpp)
FILE(GLOB equilibrium_code y/aqua/equilibrium/*.cpp)
SET(equilibrium_srcs ${equilibrium_decl} ${equilibrium_code})
SOURCE_GROUP( aqua/equilibrium FILES ${equilibrium_srcs} )
INSTALL( FILES ${equilibrium_decl} DESTINATION include/y/aqua/equilibrium ) 
 
FILE(GLOB engine_decl y/aqua/engine/*.hpp)
FILE(GLOB engine_code y/aqua/engine/*.cpp)
SET(engine_srcs ${engine_decl} ${engine_code})
SOURCE_GROUP( aqua/engine FILES ${engine_srcs} )
INSTALL( FILES ${engine_decl} DESTINATION include/y/aqua/engine )

FILE(GLOB boot_decl y/aqua/boot/*.hpp)
FILE(GLOB boot_code y/aqua/boot/*.cpp)
SET(boot_srcs ${boot_decl} ${boot_code})
SOURCE_GROUP( aqua/boot FILES ${boot_srcs} )
INSTALL( FILES ${boot_decl} DESTINATION include/y/aqua/boot )

ADD_LIBRARY(y-aqua STATIC 
${main_srcs}
${equilibrium_srcs}
${engine_srcs}
${boot_srcs}
)
INSTALL( TARGETS y-aqua ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-aqua" PARENT_SCOPE)

MESSAGE( STATUS "<Aqua/>" )
MESSAGE(STATUS "")
