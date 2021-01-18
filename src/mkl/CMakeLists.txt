PROJECT(y-mkl C CXX)

MESSAGE( STATUS "<MKL>" )

INCLUDE_DIRECTORIES(. ../main ../spade)

FILE(GLOB main_decl y/mkl/*.hpp)
FILE(GLOB main_code y/mkl/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( mkl FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/mkl )

FILE(GLOB tao_decl y/mkl/tao/*.hpp)
FILE(GLOB tao_code y/mkl/tao/*.cpp)
SET(tao_srcs ${tao_decl} ${tao_code})
SOURCE_GROUP( tao FILES ${tao_srcs} )
INSTALL( FILES ${tao_decl} DESTINATION include/y/mkl/tao )


FILE(GLOB kernel_decl y/mkl/kernel/*.hpp)
FILE(GLOB kernel_code y/mkl/kernel/*.cpp)
SET(kernel_srcs ${kernel_decl} ${kernel_code})
SOURCE_GROUP( kernel FILES ${kernel_srcs} )
INSTALL( FILES ${kernel_decl} DESTINATION include/y/mkl/kernel )

FILE(GLOB io_decl y/mkl/io/*.hpp)
FILE(GLOB io_code y/mkl/io/*.cpp)
SET(io_srcs ${io_decl} ${io_code})
SOURCE_GROUP( io FILES ${io_srcs} )
INSTALL( FILES ${io_decl} DESTINATION include/y/mkl/io )

FILE(GLOB fcn_decl y/mkl/fcn/*.hpp)
FILE(GLOB fcn_code y/mkl/fcn/*.cpp)
SET(fcn_srcs ${fcn_decl} ${fcn_code})
SOURCE_GROUP( fcn FILES ${fcn_srcs} )
INSTALL( FILES ${fcn_decl} DESTINATION include/y/mkl/fcn )

FILE(GLOB root_decl y/mkl/root/*.hpp)
FILE(GLOB root_code y/mkl/root/*.cpp)
SET(root_srcs ${root_decl} ${root_code})
SOURCE_GROUP( root FILES ${root_srcs} )
INSTALL( FILES ${root_decl} DESTINATION include/y/mkl/root )

FILE(GLOB ode_decl y/mkl/ode/*.hpp)
FILE(GLOB ode_code y/mkl/ode/*.cpp)
SET(ode_srcs ${ode_decl} ${ode_code})
SOURCE_GROUP( ode FILES ${ode_srcs} )
INSTALL( FILES ${ode_decl} DESTINATION include/y/mkl/ode )

FILE(GLOB ode_expl_decl y/mkl/ode/explicit/*.hpp)
FILE(GLOB ode_expl_code y/mkl/ode/explicit/*.cpp)
SET(ode_expl_srcs ${ode_expl_decl} ${ode_expl_code})
SOURCE_GROUP( ode/explicit FILES ${ode_expl_srcs} )
INSTALL( FILES ${ode_expl_decl} DESTINATION include/y/mkl/ode/explicit )

FILE(GLOB ode_impl_decl y/mkl/ode/implicit/*.hpp)
FILE(GLOB ode_impl_code y/mkl/ode/implicit/*.cpp)
SET(ode_impl_srcs ${ode_impl_decl} ${ode_impl_code})
SOURCE_GROUP( ode/implicit FILES ${ode_impl_srcs} )
INSTALL( FILES ${ode_impl_decl} DESTINATION include/y/mkl/ode/implicit )

FILE(GLOB opt_decl y/mkl/opt/*.hpp)
FILE(GLOB opt_copt y/mkl/opt/*.cpp)
SET(opt_srcs ${opt_decl} ${opt_copt})
SOURCE_GROUP( opt FILES ${opt_srcs} )
INSTALL( FILES ${opt_decl} DESTINATION include/y/mkl/opt )

FILE(GLOB signal_decl y/mkl/signal/*.hpp)
FILE(GLOB signal_csignal y/mkl/signal/*.cpp)
SET(signal_srcs ${signal_decl} ${signal_csignal})
SOURCE_GROUP( signal FILES ${signal_srcs} )
INSTALL( FILES ${signal_decl} DESTINATION include/y/mkl/signal )

FILE(GLOB stat_decl y/mkl/stat/*.hpp)
FILE(GLOB stat_cstat y/mkl/stat/*.cpp)
SET(stat_srcs ${stat_decl} ${stat_cstat})
SOURCE_GROUP( stat FILES ${stat_srcs} )
INSTALL( FILES ${stat_decl} DESTINATION include/y/mkl/stat )

FILE(GLOB euclidean_decl y/mkl/euclidean/*.hpp)
FILE(GLOB euclidean_code y/mkl/euclidean/*.cpp)
SET(euclidean_srcs ${euclidean_decl} ${euclidean_code})
SOURCE_GROUP( euclidean FILES ${euclidean_srcs} )
INSTALL( FILES ${euclidean_decl} DESTINATION include/y/mkl/euclidean )

FILE(GLOB euclidean_arc_decl y/mkl/euclidean/arc/*.hpp)
FILE(GLOB euclidean_arc_code y/mkl/euclidean/arc/*.cpp)
SET(euclidean_arc_srcs ${euclidean_arc_decl} ${euclidean_arc_code})
SOURCE_GROUP( euclidean/arc FILES ${euclidean_arc_srcs} )
INSTALL( FILES ${euclidean_arc_decl} DESTINATION include/y/mkl/euclidean/arc )


FILE(GLOB geometric_decl y/mkl/geometric/*.hpp)
FILE(GLOB geometric_code y/mkl/geometric/*.cpp)
SET(geometric_srcs ${geometric_decl} ${geometric_code})
SOURCE_GROUP( geometric FILES ${geometric_srcs} )
INSTALL( FILES ${geometric_decl} DESTINATION include/y/mkl/geometric )


FILE(GLOB geometric_arc_decl y/mkl/geometric/arc/*.hpp)
FILE(GLOB geometric_arc_code y/mkl/geometric/arc/*.cpp)
SET(geometric_arc_srcs ${geometric_arc_decl} ${geometric_arc_code})
SOURCE_GROUP( geometric/arc FILES ${geometric_arc_srcs} )
INSTALL( FILES ${geometric_arc_decl} DESTINATION include/y/mkl/geometric/arc )

FILE(GLOB fitting_decl y/mkl/fitting/*.h??)
FILE(GLOB fitting_code y/mkl/fitting/*.cpp)
SET(fitting_srcs ${fitting_decl} ${fitting_code})
SOURCE_GROUP( fitting FILES ${fitting_srcs} )
INSTALL( FILES ${fitting_decl} DESTINATION include/y/mkl/fitting )

FILE(GLOB fitting_variable_decl y/mkl/fitting/variable/*.hpp)
FILE(GLOB fitting_variable_code y/mkl/fitting/variable/*.cpp)
SET(fitting_variable_srcs ${fitting_variable_decl} ${fitting_variable_code})
SOURCE_GROUP( fitting/variable FILES ${fitting_variable_srcs} )
INSTALL( FILES ${fitting_variable_decl} DESTINATION include/y/mkl/fitting/variable )

FILE(GLOB fitting_sample_decl y/mkl/fitting/sample/*.hpp)
FILE(GLOB fitting_sample_code y/mkl/fitting/sample/*.cpp)
SET(fitting_sample_srcs ${fitting_sample_decl} ${fitting_sample_code})
SOURCE_GROUP( fitting/sample FILES ${fitting_sample_srcs} )
INSTALL( FILES ${fitting_sample_decl} DESTINATION include/y/mkl/fitting/sample )

FILE(GLOB fitting_sequential_decl y/mkl/fitting/sequential/*.hpp)
FILE(GLOB fitting_sequential_code y/mkl/fitting/sequential/*.cpp)
SET(fitting_sequential_srcs ${fitting_sequential_decl} ${fitting_sequential_code})
SOURCE_GROUP( fitting/sequential FILES ${fitting_sequential_srcs} )
INSTALL( FILES ${fitting_sequential_decl} DESTINATION include/y/mkl/fitting/sequential )

FILE(GLOB fitting_built_in_decl y/mkl/fitting/built-in/*.hpp)
FILE(GLOB fitting_built_in_code y/mkl/fitting/built-in/*.cpp)
SET(fitting_built_in_srcs ${fitting_built_in_decl} ${fitting_built_in_code})
SOURCE_GROUP( fitting/built-in FILES ${fitting_built_in_srcs} )
INSTALL( FILES ${fitting_built_in_decl} DESTINATION include/y/mkl/fitting/built-in )


ADD_LIBRARY(y-mkl STATIC
${main_srcs}
${tao_srcs}
${kernel_srcs}
${io_srcs}
${fcn_srcs}
${root_srcs}
${ode_srcs} ${ode_expl_srcs} ${ode_impl_srcs}
${opt_srcs}
${signal_srcs}
${stat_srcs}
${geometric_srcs} ${geometric_arc_srcs}
${euclidean_srcs} ${euclidean_arc_srcs}
${fit_srcs} ${fit_sample_srcs} ${fit_variable_srcs} ${fit_sequential_srcs} ${fit_share_srcs} 
${fitting_srcs} ${fitting_variable_srcs} ${fitting_sample_srcs} ${fitting_sequential_srcs}  ${fitting_built_in_srcs} 
)
INSTALL( TARGETS y-mkl ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-mkl" PARENT_SCOPE)
MESSAGE( STATUS "<MKL/>" )
MESSAGE(STATUS "")

