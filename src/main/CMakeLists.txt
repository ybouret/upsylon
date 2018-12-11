PROJECT(y C CXX)

INCLUDE_DIRECTORIES(.)

FILE(GLOB main_decl y/*.hpp)
FILE(GLOB main_code y/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( main FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y )

FILE(GLOB os_decl y/os/*.h*)
FILE(GLOB os_code y/os/*.cpp)
SET(os_srcs ${os_decl} ${os_code})
SOURCE_GROUP( os FILES ${os_srcs} )
INSTALL( FILES ${os_decl} DESTINATION include/y/os )

FILE(GLOB utest_decl y/utest/*.hpp)
FILE(GLOB utest_code y/utest/*.cpp)
SET(utest_srcs ${utest_decl} ${utest_code})
SOURCE_GROUP( utest FILES ${utest_srcs} )
INSTALL( FILES ${utest_decl} DESTINATION include/y/utest )

FILE(GLOB type_decl y/type/*.hpp)
FILE(GLOB type_code y/type/*.cpp)
SET(type_srcs ${type_decl} ${type_code})
SOURCE_GROUP( type FILES ${type_srcs} )
INSTALL( FILES ${type_decl} DESTINATION include/y/type )

FILE(GLOB code_decl y/code/*.hpp)
FILE(GLOB code_code y/code/*.cpp)
SET(code_srcs ${code_decl} ${code_code})
SOURCE_GROUP( code FILES ${code_srcs} )
INSTALL( FILES ${code_decl} DESTINATION include/y/code )

FILE(GLOB core_decl y/core/*.hpp)
FILE(GLOB core_code y/core/*.cpp)
SET(core_srcs ${core_decl} ${core_code})
SOURCE_GROUP( core FILES ${core_srcs} )
INSTALL( FILES ${core_decl} DESTINATION include/y/core )

FILE(GLOB sort_decl y/sort/*.hpp)
FILE(GLOB sort_code y/sort/*.cpp)
SET(sort_srcs ${sort_decl} ${sort_code})
SOURCE_GROUP( sort FILES ${sort_srcs} )
INSTALL( FILES ${sort_decl} DESTINATION include/y/sort )

FILE(GLOB randomized_decl y/randomized/*.hpp)
FILE(GLOB randomized_code y/randomized/*.cpp)
SET(randomized_srcs ${randomized_decl} ${randomized_code})
SOURCE_GROUP( randomized FILES ${randomized_srcs} )
INSTALL( FILES ${randomized_decl} DESTINATION include/y/randomized )

FILE(GLOB memory_decl   y/memory/*.hpp)
FILE(GLOB memory_code y/memory/*.cpp)
SET(memory_srcs ${memory_decl} ${memory_code})
SOURCE_GROUP( memory FILES ${memory_srcs} )
INSTALL( FILES ${memory_decl} DESTINATION include/y/memory )

FILE(GLOB string_decl   y/string/*.hpp)
FILE(GLOB string_code y/string/*.cpp)
SET(string_srcs ${string_decl} ${string_code})
SOURCE_GROUP( string FILES ${string_srcs} )
INSTALL( FILES ${string_decl} DESTINATION include/y/string )

FILE(GLOB ptr_decl   y/ptr/*.hpp)
FILE(GLOB ptr_code y/ptr/*.cpp)
SET(ptr_srcs ${ptr_decl} ${ptr_code})
SOURCE_GROUP( ptr FILES ${ptr_srcs} )
INSTALL( FILES ${ptr_decl} DESTINATION include/y/ptr )

FILE(GLOB iterate_decl y/iterate/*.hpp)
FILE(GLOB iterate_code y/iterate/*.cpp)
SET(iterate_srcs ${iterate_decl} ${iterate_code})
SOURCE_GROUP( iterate FILES ${iterate_srcs} )
INSTALL( FILES ${iterate_decl} DESTINATION include/y/iterate )

FILE(GLOB container_decl   y/container/*.hpp)
FILE(GLOB container_code y/container/*.cpp)
SET(container_srcs ${container_decl} ${container_code})
SOURCE_GROUP( container FILES ${container_srcs} )
INSTALL( FILES ${container_decl} DESTINATION include/y/container )

FILE(GLOB sequence_decl   y/sequence/*.hpp)
FILE(GLOB sequence_code y/sequence/*.cpp)
SET(sequence_srcs ${sequence_decl} ${sequence_code})
SOURCE_GROUP( sequence FILES ${sequence_srcs} )
INSTALL( FILES ${sequence_decl} DESTINATION include/y/sequence )

FILE(GLOB associative_decl   y/associative/*.hpp)
FILE(GLOB associative_code y/associative/*.cpp)
SET(associative_srcs ${associative_decl} ${associative_code})
SOURCE_GROUP( associative FILES ${associative_srcs} )
INSTALL( FILES ${associative_decl} DESTINATION include/y/associative )

FILE(GLOB hashing_decl y/hashing/*.hpp)
FILE(GLOB hashing_code y/hashing/*.cpp)
SET(hashing_srcs ${hashing_decl} ${hashing_code})
SOURCE_GROUP( hashing FILES ${hashing_srcs} )
INSTALL( FILES ${hashing_decl} DESTINATION include/y/hashing )

FILE(GLOB concurrent_decl y/concurrent/*.hpp)
FILE(GLOB concurrent_code y/concurrent/*.cpp)
SET(concurrent_srcs ${concurrent_decl} ${concurrent_code})
SOURCE_GROUP( concurrent FILES ${concurrent_srcs} )
INSTALL( FILES ${concurrent_decl} DESTINATION include/y/concurrent )

FILE(GLOB concurrent_scheme_decl y/concurrent/scheme/*.hpp)
FILE(GLOB concurrent_scheme_code y/concurrent/scheme/*.cpp)
SET(concurrent_scheme_srcs ${concurrent_scheme_decl} ${concurrent_scheme_code})
SOURCE_GROUP( concurrent/scheme FILES ${concurrent_scheme_srcs} )
INSTALL( FILES ${concurrent_scheme_decl} DESTINATION include/y/concurrent/scheme )

FILE(GLOB ios_decl y/ios/*.hpp)
FILE(GLOB ios_code y/ios/*.cpp)
SET(ios_srcs ${ios_decl} ${ios_code})
SOURCE_GROUP( ios FILES ${ios_srcs} )
INSTALL( FILES ${ios_decl} DESTINATION include/y/ios )


FILE(GLOB codec_decl y/codec/*.hpp)
FILE(GLOB codec_code y/codec/*.cpp)
SET(codec_srcs ${codec_decl} ${codec_code})
SOURCE_GROUP( codec FILES ${codec_srcs} )
INSTALL( FILES ${codec_decl} DESTINATION include/y/codec )

SET(lzo_srcs y/codec/lzo/minilzo.c)
SOURCE_GROUP( codec/lzo FILES ${lzo_srcs} )

FILE(GLOB fft_decl y/fft/*.hpp)
FILE(GLOB fft_code y/fft/*.cpp)
SET(fft_srcs ${fft_decl} ${fft_code})
SOURCE_GROUP( fft FILES ${fft_srcs} )
INSTALL( FILES ${fft_decl} DESTINATION include/y/fft )


FILE(GLOB mpl_decl y/mpl/*.hpp)
FILE(GLOB mpl_code y/mpl/*.cpp)
SET(mpl_srcs ${mpl_decl} ${mpl_code})
SOURCE_GROUP( mpl FILES ${mpl_srcs} )
INSTALL( FILES ${mpl_decl} DESTINATION include/y/mpl )


FILE(GLOB mpl_natural_decl y/mpl/natural/*.hpp)
FILE(GLOB mpl_natural_code y/mpl/natural/*.cpp)
SET(mpl_natural_srcs ${mpl_natural_decl} ${mpl_natural_code})
SOURCE_GROUP( mpl/natural FILES ${mpl_natural_srcs} )
INSTALL( FILES ${mpl_natural_decl} DESTINATION include/y/mpl/natural )


FILE(GLOB math_decl y/math/*.hpp)
FILE(GLOB math_code y/math/*.cpp)
SET(math_srcs ${math_decl} ${math_code})
SOURCE_GROUP( math FILES ${math_srcs} )
INSTALL( FILES ${math_decl} DESTINATION include/y/math )


FILE(GLOB functor_decl y/functor/*.hpp)
FILE(GLOB functor_code y/functor/*.cpp)
SET(functor_srcs ${functor_decl} ${functor_code})
SOURCE_GROUP( functor FILES ${functor_srcs} )
INSTALL( FILES ${functor_decl} DESTINATION include/y/functor )

FILE(GLOB fs_decl y/fs/*.hpp)
FILE(GLOB fs_code y/fs/*.cpp)
SET(fs_srcs ${fs_decl} ${fs_code})
SOURCE_GROUP( fs FILES ${fs_srcs} )
INSTALL( FILES ${fs_decl} DESTINATION include/y/fs )

FILE(GLOB ipso_decl y/ipso/*.hpp)
FILE(GLOB ipso_code y/ipso/*.cpp)
SET(ipso_srcs ${ipso_decl} ${ipso_code})
SOURCE_GROUP( ipso FILES ${ipso_srcs} )
INSTALL( FILES ${ipso_decl} DESTINATION include/y/ipso )

FILE(GLOB rtld_decl y/rtld/*.h*)
FILE(GLOB rtld_code y/rtld/*.cpp)
SET(rtld_srcs ${rtld_decl} ${rtld_code})
SOURCE_GROUP( rtld FILES ${rtld_srcs} )
INSTALL( FILES ${rtld_decl} DESTINATION include/y/rtld )

FILE(GLOB lang_decl y/lang/*.hpp)
FILE(GLOB lang_code y/lang/*.cpp)
SET(lang_srcs ${lang_decl} ${lang_code})
SOURCE_GROUP( lang FILES ${lang_srcs} )
INSTALL( FILES ${lang_decl} DESTINATION include/y/lang )

FILE(GLOB lang_pattern_decl y/lang/pattern/*.hpp)
FILE(GLOB lang_pattern_code y/lang/pattern/*.cpp)
SET(lang_pattern/srcs ${lang_pattern_decl} ${lang_pattern_code})
SOURCE_GROUP( lang/pattern FILES ${lang_pattern_srcs} )
INSTALL( FILES ${lang_pattern_decl} DESTINATION include/y/lang/pattern )

ADD_LIBRARY(y STATIC 
${main_srcs} 
${os_srcs}
${utest_srcs} 
${type_srcs}
${core_srcs}
${code_srcs}
${sort_srcs}
${randomized_srcs}
${hashing_srcs}
${memory_srcs}
${string_srcs}
${ptr_srcs}
${iterate_srcs}
${container_srcs}
${sequence_srcs}
${associative_srcs}
${concurrent_srcs}
${concurrent_scheme_srcs}
${ios_srcs}
${codec_srcs} ${lzo_srcs}
${fft_srcs}
${mpl_srcs}
${mpl_natural_srcs}
${math_srcs}
${functor_srcs}
${fs_srcs}
${ipso_srcs}
${rtld_srcs}
${lang_srcs} ${lang_pattern_srcs}
)
 
INSTALL( TARGETS y ARCHIVE DESTINATION lib )

IF(FALSE)
  ADD_EXECUTABLE(nwsrt nwsrt.cpp)
  TARGET_LINK_Y(nwsrt)
ENDIF()


ADD_SUBDIRECTORY(tests)

