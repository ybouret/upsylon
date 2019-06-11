PROJECT(y-m C CXX)

MESSAGE( STATUS "<Mathematics>" )

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

FILE(GLOB opt_decl y/math/opt/*.hpp)
FILE(GLOB opt_copt y/math/opt/*.cpp)
SET(opt_srcs ${opt_decl} ${opt_copt})
SOURCE_GROUP( opt FILES ${opt_srcs} )
INSTALL( FILES ${opt_decl} DESTINATION include/y/math/opt )

FILE(GLOB signal_decl y/math/signal/*.hpp)
FILE(GLOB signal_csignal y/math/signal/*.cpp)
SET(signal_srcs ${signal_decl} ${signal_csignal})
SOURCE_GROUP( signal FILES ${signal_srcs} )
INSTALL( FILES ${signal_decl} DESTINATION include/y/math/signal )

FILE(GLOB stat_decl y/math/stat/*.hpp)
FILE(GLOB stat_cstat y/math/stat/*.cpp)
SET(stat_srcs ${stat_decl} ${stat_cstat})
SOURCE_GROUP( stat FILES ${stat_srcs} )
INSTALL( FILES ${stat_decl} DESTINATION include/y/math/stat )

FILE(GLOB geom_decl y/math/geom/*.hpp)
FILE(GLOB geom_cgeom y/math/geom/*.cpp)
SET(geom_srcs ${geom_decl} ${geom_cgeom})
SOURCE_GROUP( geom FILES ${geom_srcs} )
INSTALL( FILES ${geom_decl} DESTINATION include/y/math/geom )

ADD_LIBRARY(y-m STATIC
${main_srcs}
${kernel_srcs}
${io_srcs}
${fcn_srcs}
${ode_srcs} ${ode_expl_srcs} ${ode_impl_srcs}
${fit_srcs}
${opt_srcs}
${signal_srcs}
${stat_srcs}
${geom_srcs}
)
INSTALL( TARGETS y-m ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
MESSAGE( STATUS "<Mathematics/>" )
MESSAGE(STATUS "")

