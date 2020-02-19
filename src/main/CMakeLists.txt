PROJECT(y C CXX)

MESSAGE( STATUS "<Upsylon>" )

INCLUDE_DIRECTORIES(.)

FILE(GLOB main_decl y/*.hpp)
FILE(GLOB main_code y/*.cpp)
SET(main_srcs ${main_decl} ${main_code})
SOURCE_GROUP( main FILES ${main_srcs} )
INSTALL( FILES ${main_decl} DESTINATION include/y )

FILE(GLOB os_decl y/os/*.h*)
FILE(GLOB os_code y/os/*.c*)
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
SET(sort_srcs ${sort_decl} ${sort_code} y/sort/nwsrt.db)
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

FILE(GLOB ordered_decl   y/ordered/*.hpp)
FILE(GLOB ordered_code y/ordered/*.cpp)
SET(ordered_srcs ${ordered_decl} ${ordered_code})
SOURCE_GROUP( ordered FILES ${ordered_srcs} )
INSTALL( FILES ${ordered_decl} DESTINATION include/y/ordered )

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

FILE(GLOB ios_tools_decl y/ios/tools/*.hpp)
FILE(GLOB ios_tools_code y/ios/tools/*.cpp)
SET(ios_tools_srcs ${ios_tools_decl} ${ios_tools_code})
SOURCE_GROUP( ios/tools FILES ${ios_tools_srcs} )
INSTALL( FILES ${ios_tools_decl} DESTINATION include/y/ios/tools )

FILE(GLOB information_decl y/information/*.hpp)
FILE(GLOB information_code y/information/*.cpp)
SET(information_srcs ${information_decl} ${information_code})
SOURCE_GROUP( information FILES ${information_srcs} )
INSTALL( FILES ${information_decl} DESTINATION include/y/information )

FILE(GLOB information_shift_decl y/information/shift/*.hpp)
FILE(GLOB information_shift_code y/information/shift/*.cpp)
SET(information_shift_srcs ${information_shift_decl} ${information_shift_code})
SOURCE_GROUP( information/shift FILES ${information_shift_srcs} )
INSTALL( FILES ${information_shift_decl} DESTINATION include/y/information/shift )

FILE(GLOB information_filter_decl y/information/filter/*.hpp)
FILE(GLOB information_filter_code y/information/filter/*.cpp)
SET(information_filter_srcs ${information_filter_decl} ${information_filter_code})
SOURCE_GROUP( information/filter FILES ${information_filter_srcs} )
INSTALL( FILES ${information_filter_decl} DESTINATION include/y/information/filter )

FILE(GLOB information_filter_huffman_decl y/information/filter/huffman/*.hpp)
FILE(GLOB information_filter_huffman_code y/information/filter/huffman/*.cpp)
SET(information_filter_huffman_srcs ${information_filter_huffman_decl} ${information_filter_huffman_code})
SOURCE_GROUP( information/filter/huffman FILES ${information_filter_huffman_srcs} )
INSTALL( FILES ${information_filter_huffman_decl} DESTINATION include/y/information/filter/huffman )

FILE(GLOB information_filter_shannon_fano_decl y/information/filter/shannon-fano/*.hpp)
FILE(GLOB information_filter_shannon_fano_code y/information/filter/shannon-fano/*.cpp)
SET(information_filter_shannon_fano_srcs ${information_filter_shannon_fano_decl} ${information_filter_shannon_fano_code})
SOURCE_GROUP( information/filter/shannon-fano FILES ${information_filter_shannon_fano_srcs} )
INSTALL( FILES ${information_filter_shannon_fano_decl} DESTINATION include/y/information/filter/shannon-fano )

FILE(GLOB information_filter_rle_decl y/information/filter/rle/*.hpp)
FILE(GLOB information_filter_rle_code y/information/filter/rle/*.cpp)
SET(information_filter_rle_srcs ${information_filter_rle_decl} ${information_filter_rle_code})
SOURCE_GROUP( information/filter/rle FILES ${information_filter_rle_srcs} )
INSTALL( FILES ${information_filter_rle_decl} DESTINATION include/y/information/filter/rle )

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

FILE(GLOB mpl_prime_decl y/mpl/prime/*.hpp)
FILE(GLOB mpl_prime_code y/mpl/prime/*.cpp)
SET(mpl_prime_srcs ${mpl_prime_decl} ${mpl_prime_code})
SOURCE_GROUP( mpl/prime FILES ${mpl_prime_srcs} )
INSTALL( FILES ${mpl_prime_decl} DESTINATION include/y/mpl/prime )

FILE(GLOB mpl_rsa_decl y/mpl/rsa/*.hpp)
FILE(GLOB mpl_rsa_code y/mpl/rsa/*.cpp)
SET(mpl_rsa_srcs ${mpl_rsa_decl} ${mpl_rsa_code})
SOURCE_GROUP( mpl/rsa FILES ${mpl_rsa_srcs} )
INSTALL( FILES ${mpl_rsa_decl} DESTINATION include/y/mpl/rsa )



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

FILE(GLOB fs_disk_decl y/fs/disk/*.hpp)
FILE(GLOB fs_disk_code y/fs/disk/*.cpp)
SET(fs_disk_srcs ${fs_disk_decl} ${fs_disk_code})
SOURCE_GROUP( fs/disk FILES ${fs_disk_srcs} )
INSTALL( FILES ${fs_disk_decl} DESTINATION include/y/fs/disk )

FILE(GLOB fs_local_decl y/fs/local/*.hpp)
FILE(GLOB fs_local_code y/fs/local/*.cpp)
SET(fs_local_srcs ${fs_local_decl} ${fs_local_code})
SOURCE_GROUP( fs/local FILES ${fs_local_srcs} )
INSTALL( FILES ${fs_local_decl} DESTINATION include/y/fs/local )

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
SET(lang_pattern_srcs ${lang_pattern_decl} ${lang_pattern_code})
SOURCE_GROUP( lang/pattern FILES ${lang_pattern_srcs} )
INSTALL( FILES ${lang_pattern_decl} DESTINATION include/y/lang/pattern )

FILE(GLOB lang_lexical_decl y/lang/lexical/*.hpp)
FILE(GLOB lang_lexical_code y/lang/lexical/*.cpp)
SET(lang_lexical_srcs ${lang_lexical_decl} ${lang_lexical_code})
SOURCE_GROUP( lang/lexical FILES ${lang_lexical_srcs} )
INSTALL( FILES ${lang_lexical_decl} DESTINATION include/y/lang/lexical )

FILE(GLOB lexical_plugin_decl y/lang/lexical/plugin/*.hpp)
FILE(GLOB lexical_plugin_code y/lang/lexical/plugin/*.cpp)
SET(lexical_plugin_srcs ${lexical_plugin_decl} ${lexical_plugin_code})
SOURCE_GROUP( lang/lexical/plugin FILES ${lexical_plugin_srcs} )
INSTALL( FILES ${lexical_plugin_decl} DESTINATION include/y/lang/lexical/plugin )

FILE(GLOB lang_stream_decl y/lang/stream/*.hpp)
FILE(GLOB lang_stream_code y/lang/stream/*.cpp)
SET(lang_stream_srcs ${lang_stream_decl} ${lang_stream_code})
SOURCE_GROUP( lang/stream FILES ${lang_stream_srcs} )
INSTALL( FILES ${lang_stream_decl} DESTINATION include/y/lang/stream )


FILE(GLOB geometry_decl y/geometry/*.hpp)
FILE(GLOB geometry_code y/geometry/*.cpp)
SET(geometry_srcs ${geometry_decl} ${geometry_code})
SOURCE_GROUP( geometry FILES ${geometry_srcs} )
INSTALL( FILES ${geometry_decl} DESTINATION include/y/geometry )

FILE(GLOB geometry_iso2d_decl y/geometry/iso2d/*.hpp)
FILE(GLOB geometry_iso2d_code y/geometry/iso2d/*.cpp)
SET(geometry_iso2d_srcs ${geometry_iso2d_decl} ${geometry_iso2d_code})
SOURCE_GROUP( geometry/iso2d FILES ${geometry_iso2d_srcs} )
INSTALL( FILES ${geometry_iso2d_decl} DESTINATION include/y/geometry/iso2d )

FILE(GLOB geometry_iso3d_decl y/geometry/iso3d/*.hpp)
FILE(GLOB geometry_iso3d_code y/geometry/iso3d/*.cpp)
SET(geometry_iso3d_srcs ${geometry_iso3d_decl} ${geometry_iso3d_code})
SOURCE_GROUP( geometry/iso3d FILES ${geometry_iso3d_srcs} )
INSTALL( FILES ${geometry_iso3d_decl} DESTINATION include/y/geometry/iso3d )

FILE(GLOB geometry_iso3d_decl y/geometry/iso2d/*.hpp)
FILE(GLOB geometry_iso3d_code y/geometry/iso2d/*.cpp)
SET(geometry_iso2d_srcs ${geometry_iso2d_decl} ${geometry_iso2d_code})
SOURCE_GROUP( geometry/iso2d FILES ${geometry_iso2d_srcs} )
INSTALL( FILES ${geometry_iso2d_decl} DESTINATION include/y/geometry/iso2d )

FILE(GLOB kr_sc_decl y/crypto/sc/*.hpp)
FILE(GLOB kr_sc_code y/crypto/sc/*.cpp)
SET(kr_sc_srcs ${kr_sc_decl} ${kr_sc_code})
SOURCE_GROUP( kr/sc FILES ${kr_sc_srcs} )
INSTALL( FILES ${kr_sc_decl} DESTINATION include/y/kr/sc )

FILE(GLOB kr_bc_decl y/crypto/bc/*.hpp)
FILE(GLOB kr_bc_code y/crypto/bc/*.cpp)
SET(kr_bc_srcs ${kr_bc_decl} ${kr_bc_code})
SOURCE_GROUP( kr/bc FILES ${kr_bc_srcs} )
INSTALL( FILES ${kr_bc_decl} DESTINATION include/y/kr/bc )

FILE(GLOB sparse_decl y/sparse/*.h*)
FILE(GLOB sparse_code y/sparse/*.cpp)
SET(sparse_srcs ${sparse_decl} ${sparse_code})
SOURCE_GROUP( sparse FILES ${sparse_srcs} )
INSTALL( FILES ${sparse_decl} DESTINATION include/y/sparse )

FILE(GLOB counting_decl y/counting/*.h*)
FILE(GLOB counting_code y/counting/*.cpp)
SET(counting_srcs ${counting_decl} ${counting_code})
SOURCE_GROUP( counting FILES ${counting_srcs} )
INSTALL( FILES ${counting_decl} DESTINATION include/y/counting )

FILE(GLOB oxide_decl y/oxide/*.hpp)
FILE(GLOB oxide_code y/oxide/*.cpp)
SET(oxide_srcs ${oxide_decl} ${oxide_code})
SOURCE_GROUP( oxide FILES ${oxide_srcs} )
INSTALL( FILES ${oxide_decl} DESTINATION include/y/oxide )

FILE(GLOB oxide_field_decl y/oxide/field/*.hpp)
FILE(GLOB oxide_field_code y/oxide/field/*.cpp)
SET(oxide_field_srcs ${oxide_field_decl} ${oxide_field_code})
SOURCE_GROUP( oxide/field FILES ${oxide_field_srcs} )
INSTALL( FILES ${oxide_field_decl} DESTINATION include/y/oxide/field )

FILE(GLOB oxide_grid_decl y/oxide/grid/*.hpp)
FILE(GLOB oxide_grid_code y/oxide/grid/*.cpp)
SET(oxide_grid_srcs ${oxide_grid_decl} ${oxide_grid_code})
SOURCE_GROUP( oxide/grid FILES ${oxide_grid_srcs} )
INSTALL( FILES ${oxide_grid_decl} DESTINATION include/y/oxide/grid )


FILE(GLOB oxide_algo_decl y/oxide/algo/*.hpp)
FILE(GLOB oxide_algo_code y/oxide/algo/*.cpp)
SET(oxide_algo_srcs ${oxide_algo_decl} ${oxide_algo_code})
SOURCE_GROUP( oxide/algo FILES ${oxide_algo_srcs} )
INSTALL( FILES ${oxide_algo_decl} DESTINATION include/y/oxide/algo )

FILE(GLOB oxide_mpi_decl y/oxide/mpi/*.hpp)
FILE(GLOB oxide_mpi_code y/oxide/mpi/*.cpp)
SET(oxide_mpi_srcs ${oxide_mpi_decl} ${oxide_mpi_code})
SOURCE_GROUP( oxide/mpi FILES ${oxide_mpi_srcs} )
INSTALL( FILES ${oxide_mpi_decl} DESTINATION include/y/oxide/mpi )

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
${ordered_srcs}
${associative_srcs}
${concurrent_srcs}
${concurrent_scheme_srcs}
${ios_srcs} ${ios_tools_srcs}
${information_srcs} ${information_shift_srcs} ${information_filter_srcs} ${information_filter_huffman_srcs} ${information_filter_shannon_fano_srcs} ${information_filter_rle_srcs}
${codec_srcs} ${lzo_srcs}
${fft_srcs}
${mpl_srcs} ${mpl_natural_srcs} ${mpl_prime_srcs} ${mpl_rsa_srcs}
${math_srcs}
${functor_srcs}
${fs_srcs}
${fs_disk_srcs}
${fs_local_srcs}
${rtld_srcs}
${lang_srcs} ${lang_pattern_srcs} ${lang_lexical_srcs} ${lexical_plugin_srcs} ${lang_stream_srcs}
${geometry_srcs} ${geometry_iso2d_srcs} ${geometry_iso3d_srcs}
${kr_sc_srcs} ${kr_bc_srcs}
${sparse_srcs}
${counting_srcs}
${oxide_srcs} ${oxide_field_srcs} ${oxide_grid_srcs} ${oxide_algo_srcs} ${oxide_mpi_srcs}
)
 
INSTALL( TARGETS y ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(cli)


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-main" PARENT_SCOPE)

MESSAGE( STATUS "<Upsylon/>" )
MESSAGE(STATUS "")
