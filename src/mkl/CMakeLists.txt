PROJECT(y-mkl C CXX)

MESSAGE( STATUS "<MKL>" )

INCLUDE_DIRECTORIES(. ../main)

FILE(GLOB main_decl y/mkl/*.hpp)
FILE(GLOB main_code y/mkl/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( mkl FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y/mkl )

FILE(GLOB kernel_decl y/mkl/kernel/*.h??)
FILE(GLOB kernel_code y/mkl/kernel/*.cpp)
SET(kernel_srcs ${kernel_decl} ${kernel_code})
SOURCE_GROUP( kernel FILES ${kernel_srcs} )
INSTALL( FILES ${kernel_decl} DESTINATION include/y/mkl/kernel )

FILE(GLOB kernel_quark_v1_decl y/mkl/kernel/quark/v1/*.hpp)
FILE(GLOB kernel_quark_v1_code y/mkl/kernel/quark/v1/*.cpp)
SET(kernel_quark_v1_srcs ${kernel_quark_v1_decl} ${kernel_quark_v1_code})
SOURCE_GROUP( kernel/quark/v1 FILES ${kernel_quark_v1_srcs} )
INSTALL( FILES ${kernel_quark_v1_decl} DESTINATION include/y/mkl/kernel/quark/v1 )

FILE(GLOB kernel_quark_v2_decl y/mkl/kernel/quark/v2/*.hpp)
FILE(GLOB kernel_quark_v2_code y/mkl/kernel/quark/v2/*.cpp)
SET(kernel_quark_v2_srcs ${kernel_quark_v2_decl} ${kernel_quark_v2_code})
SOURCE_GROUP( kernel/quark/v2 FILES ${kernel_quark_v2_srcs} )
INSTALL( FILES ${kernel_quark_v2_decl} DESTINATION include/y/mkl/kernel/quark/v2 )

FILE(GLOB kernel_quark_v3_decl y/mkl/kernel/quark/v3/*.hpp)
FILE(GLOB kernel_quark_v3_code y/mkl/kernel/quark/v3/*.cpp)
SET(kernel_quark_v3_srcs ${kernel_quark_v3_decl} ${kernel_quark_v3_code})
SOURCE_GROUP( kernel/quark/v3 FILES ${kernel_quark_v3_srcs} )
INSTALL( FILES ${kernel_quark_v3_decl} DESTINATION include/y/mkl/kernel/quark/v3 )


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

FILE(GLOB fit_decl y/mkl/fit/*.h??)
FILE(GLOB fit_code y/mkl/fit/*.cpp)
SET(fit_srcs ${fit_decl} ${fit_code})
SOURCE_GROUP( fit FILES ${fit_srcs} )
INSTALL( FILES ${fit_decl} DESTINATION include/y/mkl/fit )

FILE(GLOB fit_sample_decl y/mkl/fit/sample/*.hpp)
FILE(GLOB fit_sample_code y/mkl/fit/sample/*.cpp)
SET(fit_sample_srcs ${fit_sample_decl} ${fit_sample_code})
SOURCE_GROUP( fit/sample FILES ${fit_sample_srcs} )
INSTALL( FILES ${fit_sample_decl} DESTINATION include/y/mkl/fit/sample )

FILE(GLOB fit_variable_decl y/mkl/fit/variable/*.hpp)
FILE(GLOB fit_variable_code y/mkl/fit/variable/*.cpp)
SET(fit_variable_srcs ${fit_variable_decl} ${fit_variable_code})
SOURCE_GROUP( fit/variable FILES ${fit_variable_srcs} )
INSTALL( FILES ${fit_variable_decl} DESTINATION include/y/mkl/fit/variable )

FILE(GLOB fit_sequential_decl y/mkl/fit/sequential/*.hpp)
FILE(GLOB fit_sequential_code y/mkl/fit/sequential/*.cpp)
SET(fit_sequential_srcs ${fit_sequential_decl} ${fit_sequential_code})
SOURCE_GROUP( fit/sequential FILES ${fit_sequential_srcs} )
INSTALL( FILES ${fit_sequential_decl} DESTINATION include/y/mkl/fit/sequential )

FILE(GLOB fit_share_decl y/mkl/fit/share/*.hpp)
FILE(GLOB fit_share_code y/mkl/fit/share/*.cpp)
SET(fit_share_srcs ${fit_share_decl} ${fit_share_code})
SOURCE_GROUP( fit/share FILES ${fit_share_srcs} )
INSTALL( FILES ${fit_share_decl} DESTINATION include/y/mkl/fit/share )

ADD_LIBRARY(y-mkl STATIC
${main_srcs}
${kernel_srcs} 
${kernel_quark_v1_srcs} ${kernel_quark_v2_srcs} ${kernel_quark_v3_srcs}
${io_srcs}
${fcn_srcs}
${root_srcs}
${ode_srcs} ${ode_expl_srcs} ${ode_impl_srcs}
${opt_srcs}
${signal_srcs}
${stat_srcs}
${euclidean_srcs} ${euclidean_arc_srcs}
${fit_srcs} ${fit_sample_srcs} ${fit_variable_srcs} ${fit_sequential_srcs} ${fit_share_srcs} 
)
INSTALL( TARGETS y-mkl ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-mkl" PARENT_SCOPE)
MESSAGE( STATUS "<MKL/>" )
MESSAGE(STATUS "")

