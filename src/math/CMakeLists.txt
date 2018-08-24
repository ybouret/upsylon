PROJECT(y-m C CXX)

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/math/*.hpp)
FILE(GLOB main_code y/math/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( math FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/math )

FILE(GLOB kernel_decl y/math/kernel/*.h??)
FILE(GLOB kernel_code y/math/kernel/*.cpp)
SET(kernel_srcs ${kernel_decl} ${kernel_code})
SOURCE_GROUP( kernel FILES ${kernel_srcs} )
INSTALL( FILES ${kernel_decl} DESTINATION include/y/math/kernel )

FILE(GLOB io_decl y/math/io/*.hpp)
FILE(GLOB io_code y/math/io/*.cpp)
SET(io_srcs ${io_decl} ${io_code})
SOURCE_GROUP( io FILES ${io_srcs} )
INSTALL( FILES ${io_decl} DESTINATION include/y/math/io )

FILE(GLOB fcn_decl y/math/fcn/*.hpp)
FILE(GLOB fcn_code y/math/fcn/*.cpp)
SET(fcn_srcs ${fcn_decl} ${fcn_code})
SOURCE_GROUP( fcn FILES ${fcn_srcs} )
INSTALL( FILES ${fcn_decl} DESTINATION include/y/math/fcn )

FILE(GLOB ode_decl y/math/ode/*.hpp)
FILE(GLOB ode_code y/math/ode/*.cpp)
SET(ode_srcs ${ode_decl} ${ode_code})
SOURCE_GROUP( ode FILES ${ode_srcs} )
INSTALL( FILES ${ode_decl} DESTINATION include/y/math/ode )

FILE(GLOB ode_expl_decl y/math/ode/explicit/*.hpp)
FILE(GLOB ode_expl_code y/math/ode/explicit/*.cpp)
SET(ode_expl_srcs ${ode_expl_decl} ${ode_expl_code})
SOURCE_GROUP( ode/explicit FILES ${ode_expl_srcs} )
INSTALL( FILES ${ode_expl_decl} DESTINATION include/y/math/ode/explicit )

FILE(GLOB ode_impl_decl y/math/ode/implicit/*.hpp)
FILE(GLOB ode_impl_code y/math/ode/implicit/*.cpp)
SET(ode_impl_srcs ${ode_impl_decl} ${ode_impl_code})
SOURCE_GROUP( ode/implicit FILES ${ode_impl_srcs} )
INSTALL( FILES ${ode_impl_decl} DESTINATION include/y/math/ode/implicit )

FILE(GLOB fit_decl y/math/fit/*.hpp)
FILE(GLOB fit_cfit y/math/fit/*.cpp)
SET(fit_srcs ${fit_decl} ${fit_cfit})
SOURCE_GROUP( fit FILES ${fit_srcs} )
INSTALL( FILES ${fit_decl} DESTINATION include/y/math/fit )


ADD_LIBRARY(y-m STATIC
${main_srcs}
${kernel_srcs}
${io_srcs}
${fcn_srcs}
${ode_srcs} ${ode_expl_srcs} ${ode_impl_srcs}
${fit_srcs}
)
INSTALL( TARGETS y-m ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
