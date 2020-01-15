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

FILE(GLOB kernel_tao_decl y/math/kernel/tao/*.hxx)
FILE(GLOB kernel_tao_code y/math/kernel/tao/*.cpp)
SET(kernel_tao_srcs ${kernel_tao_decl} ${kernel_tao_code})
SOURCE_GROUP( kernel/tao FILES ${kernel_tao_srcs} )
INSTALL( FILES ${kernel_tao_decl} DESTINATION include/y/math/kernel/tao )

FILE(GLOB kernel_atom_decl y/math/kernel/atom/*.hpp)
FILE(GLOB kernel_atom_code y/math/kernel/atom/*.cpp)
SET(kernel_atom_srcs ${kernel_atom_decl} ${kernel_atom_code})
SOURCE_GROUP( kernel/atom FILES ${kernel_atom_srcs} )
INSTALL( FILES ${kernel_atom_decl} DESTINATION include/y/math/kernel/atom )

FILE(GLOB kernel_quark_v1_decl y/math/kernel/quark/v1/*.hpp)
FILE(GLOB kernel_quark_v1_code y/math/kernel/quark/v1/*.cpp)
SET(kernel_quark_v1_srcs ${kernel_quark_v1_decl} ${kernel_quark_v1_code})
SOURCE_GROUP( kernel/quark/v1 FILES ${kernel_quark_v1_srcs} )
INSTALL( FILES ${kernel_quark_v1_decl} DESTINATION include/y/math/kernel/quark/v1 )


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

FILE(GLOB euclidean_decl y/math/euclidean/*.hpp)
FILE(GLOB euclidean_code y/math/euclidean/*.cpp)
SET(euclidean_srcs ${euclidean_decl} ${euclidean_code})
SOURCE_GROUP( euclidean FILES ${euclidean_srcs} )
INSTALL( FILES ${euclidean_decl} DESTINATION include/y/math/euclidean )

FILE(GLOB euclidean_arc_decl y/math/euclidean/arc/*.hpp)
FILE(GLOB euclidean_arc_code y/math/euclidean/arc/*.cpp)
SET(euclidean_arc_srcs ${euclidean_arc_decl} ${euclidean_arc_code})
SOURCE_GROUP( euclidean/arc FILES ${euclidean_arc_srcs} )
INSTALL( FILES ${euclidean_arc_decl} DESTINATION include/y/math/euclidean/arc )

FILE(GLOB fit_decl y/math/fit/*.h??)
FILE(GLOB fit_code y/math/fit/*.cpp)
SET(fit_srcs ${fit_decl} ${fit_code})
SOURCE_GROUP( fit FILES ${fit_srcs} )
INSTALL( FILES ${fit_decl} DESTINATION include/y/math/fit )

FILE(GLOB fit_sample_decl y/math/fit/sample/*.hpp)
FILE(GLOB fit_sample_code y/math/fit/sample/*.cpp)
SET(fit_sample_srcs ${fit_sample_decl} ${fit_sample_code})
SOURCE_GROUP( fit/sample FILES ${fit_sample_srcs} )
INSTALL( FILES ${fit_sample_decl} DESTINATION include/y/math/fit/sample )

FILE(GLOB fit_variable_decl y/math/fit/variable/*.hpp)
FILE(GLOB fit_variable_code y/math/fit/variable/*.cpp)
SET(fit_variable_srcs ${fit_variable_decl} ${fit_variable_code})
SOURCE_GROUP( fit/variable FILES ${fit_variable_srcs} )
INSTALL( FILES ${fit_variable_decl} DESTINATION include/y/math/fit/variable )

FILE(GLOB fit_sequential_decl y/math/fit/sequential/*.hpp)
FILE(GLOB fit_sequential_code y/math/fit/sequential/*.cpp)
SET(fit_sequential_srcs ${fit_sequential_decl} ${fit_sequential_code})
SOURCE_GROUP( fit/sequential FILES ${fit_sequential_srcs} )
INSTALL( FILES ${fit_sequential_decl} DESTINATION include/y/math/fit/sequential )

FILE(GLOB fit_share_decl y/math/fit/share/*.hpp)
FILE(GLOB fit_share_code y/math/fit/share/*.cpp)
SET(fit_share_srcs ${fit_share_decl} ${fit_share_code})
SOURCE_GROUP( fit/share FILES ${fit_share_srcs} )
INSTALL( FILES ${fit_share_decl} DESTINATION include/y/math/fit/share )

ADD_LIBRARY(y-m STATIC
${main_srcs}
${kernel_srcs} ${kernel_atom_srcs} ${kernel_quark_v1_srcs}
${io_srcs}
${fcn_srcs}
${ode_srcs} ${ode_expl_srcs} ${ode_impl_srcs}
${opt_srcs}
${signal_srcs}
${stat_srcs}
${euclidean_srcs} ${euclidean_arc_srcs}
${fit_srcs} ${fit_sample_srcs} ${fit_variable_srcs} ${fit_sequential_srcs} ${fit_share_srcs} 
)
INSTALL( TARGETS y-m ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-m" PARENT_SCOPE)
MESSAGE( STATUS "<Mathematics/>" )
MESSAGE(STATUS "")

