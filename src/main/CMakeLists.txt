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

FILE(GLOB type_block_decl y/type/block/*.hpp)
FILE(GLOB type_block_code y/type/block/*.cpp)
SET(type_block_srcs ${type_block_decl} ${type_block_code})
SOURCE_GROUP( type/block FILES ${type_block_srcs} )
INSTALL( FILES ${type_block_decl} DESTINATION include/y/type/block )

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

FILE(GLOB sort_network_decl y/sort/network/*.hpp)
FILE(GLOB sort_network_code y/sort/network/*.cpp)
SET(sort_network_srcs ${sort_network_decl} ${sort_network_code})
SOURCE_GROUP( sort/network FILES ${sort_network_srcs} )
INSTALL( FILES ${sort_network_decl} DESTINATION include/y/sort/network )

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

FILE(GLOB memory_allocator_decl   y/memory/allocator/*.hpp)
FILE(GLOB memory_allocator_code y/memory/allocator/*.cpp)
SET(memory_allocator_srcs ${memory_allocator_decl} ${memory_allocator_code})
SOURCE_GROUP( memory/allocator FILES ${memory_allocator_srcs} )
INSTALL( FILES ${memory_allocator_decl} DESTINATION include/y/memory/allocator )

FILE(GLOB memory_tight_decl   y/memory/tight/*.hpp)
FILE(GLOB memory_tight_code y/memory/tight/*.cpp)
SET(memory_tight_srcs ${memory_tight_decl} ${memory_tight_code})
SOURCE_GROUP( memory/tight FILES ${memory_tight_srcs} )
INSTALL( FILES ${memory_tight_decl} DESTINATION include/y/memory/tight )

FILE(GLOB memory_joint_decl   y/memory/joint/*.hpp)
FILE(GLOB memory_joint_code y/memory/joint/*.cpp)
SET(memory_joint_srcs ${memory_joint_decl} ${memory_joint_code})
SOURCE_GROUP( memory/joint FILES ${memory_joint_srcs} )
INSTALL( FILES ${memory_joint_decl} DESTINATION include/y/memory/joint )


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

FILE(GLOB associative_suffix_decl   y/associative/suffix/*.hpp)
FILE(GLOB associative_suffix_code y/associative/suffix/*.cpp)
SET(associative_suffix_srcs ${associative_suffix_decl} ${associative_suffix_code})
SOURCE_GROUP( associative/suffix FILES ${associative_suffix_srcs} )
INSTALL( FILES ${associative_suffix_decl} DESTINATION include/y/associative/suffix )

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

FILE(GLOB ios_conveyor_decl y/ios/conveyor/*.hpp)
FILE(GLOB ios_conveyor_code y/ios/conveyor/*.cpp)
SET(ios_conveyor_srcs ${ios_conveyor_decl} ${ios_conveyor_code})
SOURCE_GROUP( ios/conveyor FILES ${ios_conveyor_srcs} )
INSTALL( FILES ${ios_conveyor_decl} DESTINATION include/y/ios/conveyor )

FILE(GLOB information_decl y/information/*.hpp)
FILE(GLOB information_code y/information/*.cpp)
SET(information_srcs ${information_decl} ${information_code})
SOURCE_GROUP( information FILES ${information_srcs} )
INSTALL( FILES ${information_decl} DESTINATION include/y/information )

FILE(GLOB information_modulation_decl y/information/modulation/*.hpp)
FILE(GLOB information_modulation_code y/information/modulation/*.cpp)
SET(information_modulation_srcs ${information_modulation_decl} ${information_modulation_code})
SOURCE_GROUP( information/modulation FILES ${information_modulation_srcs} )
INSTALL( FILES ${information_modulation_decl} DESTINATION include/y/information/modulation )

FILE(GLOB information_translator_decl y/information/translator/*.hpp)
FILE(GLOB information_translator_code y/information/translator/*.cpp)
SET(information_translator_srcs ${information_translator_decl} ${information_translator_code})
SOURCE_GROUP( information/translator FILES ${information_translator_srcs} )
INSTALL( FILES ${information_translator_decl} DESTINATION include/y/information/translator )

FILE(GLOB information_translator_huffman_decl y/information/translator/huffman/*.hpp)
FILE(GLOB information_translator_huffman_code y/information/translator/huffman/*.cpp)
SET(information_translator_huffman_srcs ${information_translator_huffman_decl} ${information_translator_huffman_code})
SOURCE_GROUP( information/translator/huffman FILES ${information_translator_huffman_srcs} )
INSTALL( FILES ${information_translator_huffman_decl} DESTINATION include/y/information/translator/huffman )

FILE(GLOB information_translator_shannon_fano_decl y/information/translator/shannon-fano/*.hpp)
FILE(GLOB information_translator_shannon_fano_code y/information/translator/shannon-fano/*.cpp)
SET(information_translator_shannon_fano_srcs ${information_translator_shannon_fano_decl} ${information_translator_shannon_fano_code})
SOURCE_GROUP( information/translator/shannon-fano FILES ${information_translator_shannon_fano_srcs} )
INSTALL( FILES ${information_translator_shannon_fano_decl} DESTINATION include/y/information/translator/shannon-fano )

FILE(GLOB information_translator_qrl_decl y/information/translator/qrl/*.hpp)
FILE(GLOB information_translator_qrl_code y/information/translator/qrl/*.cpp)
SET(information_translator_qrl_srcs ${information_translator_qrl_decl} ${information_translator_qrl_code})
SOURCE_GROUP( information/translator/qrl FILES ${information_translator_qrl_srcs} )
INSTALL( FILES ${information_translator_qrl_decl} DESTINATION include/y/information/translator/qrl )

FILE(GLOB information_translator_run_length_decl y/information/translator/run-length/*.hpp)
FILE(GLOB information_translator_run_length_code y/information/translator/run-length/*.cpp)
SET(information_translator_run_length_srcs ${information_translator_run_length_decl} ${information_translator_run_length_code})
SOURCE_GROUP( information/translator/run_length FILES ${information_translator_run_length_srcs} )
INSTALL( FILES ${information_translator_run_length_decl} DESTINATION include/y/information/translator/run-length )

FILE(GLOB information_translator_base64_decl y/information/translator/base64/*.hpp)
FILE(GLOB information_translator_base64_code y/information/translator/base64/*.cpp)
SET(information_translator_base64_srcs ${information_translator_base64_decl} ${information_translator_base64_code})
SOURCE_GROUP( information/translator/base64 FILES ${information_translator_base64_srcs} )
INSTALL( FILES ${information_translator_base64_decl} DESTINATION include/y/information/translator/base64 )


FILE(GLOB fft_decl y/fft/*.hpp)
FILE(GLOB fft_code y/fft/*.cpp)
SET(fft_srcs ${fft_decl} ${fft_code})
SOURCE_GROUP( fft FILES ${fft_srcs} )
INSTALL( FILES ${fft_decl} DESTINATION include/y/fft )

FILE(GLOB sibyl_decl y/sibyl/*.hpp)
FILE(GLOB sibyl_code y/sibyl/*.cpp)
SET(sibyl_srcs ${sibyl_decl} ${sibyl_code})
SOURCE_GROUP( sibyl FILES ${sibyl_srcs} )
INSTALL( FILES ${sibyl_decl} DESTINATION include/y/sibyl )

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



FILE(GLOB math_decl y/mkl/*.hpp)
FILE(GLOB math_code y/mkl/*.cpp)
SET(math_srcs ${math_decl} ${math_code})
SOURCE_GROUP( mkl FILES ${math_srcs} )
INSTALL( FILES ${math_decl} DESTINATION include/y/mkl )


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


FILE(GLOB jargon_decl y/jargon/*.hpp)
FILE(GLOB jargon_code y/jargon/*.cpp)
SET(jargon_srcs ${jargon_decl} ${jargon_code})
SOURCE_GROUP( jargon FILES ${jargon_srcs} )
INSTALL( FILES ${jargon_decl} DESTINATION include/y/jargon )

FILE(GLOB jargon_pattern_decl y/jargon/pattern/*.hpp)
FILE(GLOB jargon_pattern_code y/jargon/pattern/*.cpp)
SET(jargon_pattern_srcs ${jargon_pattern_decl} ${jargon_pattern_code})
SOURCE_GROUP( jargon/pattern FILES ${jargon_pattern_srcs} )
INSTALL( FILES ${jargon_pattern_decl} DESTINATION include/y/jargon/pattern )

FILE(GLOB jargon_token_decl y/jargon/token/*.hpp)
FILE(GLOB jargon_token_code y/jargon/token/*.cpp)
SET(jargon_token_srcs ${jargon_token_decl} ${jargon_token_code})
SOURCE_GROUP( jargon/token FILES ${jargon_token_srcs} )
INSTALL( FILES ${jargon_token_decl} DESTINATION include/y/jargon/token )

FILE(GLOB jargon_pattern_basic_decl y/jargon/pattern/basic/*.hpp)
FILE(GLOB jargon_pattern_basic_code y/jargon/pattern/basic/*.cpp)
SET(jargon_pattern_basic_srcs ${jargon_pattern_basic_decl} ${jargon_pattern_basic_code})
SOURCE_GROUP( jargon/pattern/basic FILES ${jargon_pattern_basic_srcs} )
INSTALL( FILES ${jargon_pattern_basic_decl} DESTINATION include/y/jargon/pattern/basic )

FILE(GLOB jargon_pattern_logic_decl y/jargon/pattern/logic/*.hpp)
FILE(GLOB jargon_pattern_logic_code y/jargon/pattern/logic/*.cpp)
SET(jargon_pattern_logic_srcs ${jargon_pattern_logic_decl} ${jargon_pattern_logic_code})
SOURCE_GROUP( jargon/pattern/logic FILES ${jargon_pattern_logic_srcs} )
INSTALL( FILES ${jargon_pattern_logic_decl} DESTINATION include/y/jargon/pattern/logic )

FILE(GLOB jargon_pattern_joker_decl y/jargon/pattern/joker/*.hpp)
FILE(GLOB jargon_pattern_joker_code y/jargon/pattern/joker/*.cpp)
SET(jargon_pattern_joker_srcs ${jargon_pattern_joker_decl} ${jargon_pattern_joker_code})
SOURCE_GROUP( jargon/pattern/joker FILES ${jargon_pattern_joker_srcs} )
INSTALL( FILES ${jargon_pattern_joker_decl} DESTINATION include/y/jargon/pattern/joker )

FILE(GLOB tensor_decl y/tensor/*.hpp)
FILE(GLOB tensor_code y/tensor/*.cpp)
SET(tensor_srcs ${tensor_decl} ${tensor_code})
SOURCE_GROUP( tensor FILES ${tensor_srcs} )
INSTALL( FILES ${tensor_decl} DESTINATION include/y/tensor ) 

FILE(GLOB yap_decl y/yap/*.hpp)
FILE(GLOB yap_code y/yap/*.cpp)
SET(yap_srcs ${yap_decl} ${yap_code})
SOURCE_GROUP( yap FILES ${yap_srcs} )
INSTALL( FILES ${yap_decl} DESTINATION include/y/yap )

FILE(GLOB yap_natural_decl y/yap/natural/*.hpp)
FILE(GLOB yap_natural_code y/yap/natural/*.cpp)
SET(yap_natural_srcs ${yap_natural_decl} ${yap_natural_code})
SOURCE_GROUP( yap/natural FILES ${yap_natural_srcs} )
INSTALL( FILES ${yap_natural_decl} DESTINATION include/y/yap/natural )


ADD_LIBRARY(y STATIC 
${main_srcs} 
${os_srcs}
${utest_srcs} 
${type_srcs} ${type_block_srcs}
${core_srcs}
${code_srcs}
${sort_srcs}
${sort_network_srcs}
${randomized_srcs}
${hashing_srcs}
${memory_srcs} ${memory_allocator_srcs} ${memory_tight_srcs} ${memory_joint_srcs}
${string_srcs}
${ptr_srcs}
${iterate_srcs}
${container_srcs}
${sequence_srcs}
${ordered_srcs}
${associative_srcs} ${associative_suffix_srcs}
${concurrent_srcs}
${concurrent_scheme_srcs}
${ios_srcs} ${ios_tools_srcs} ${ios_conveyor_srcs} 
${information_srcs} 
${information_modulation_srcs} 
${information_translator_srcs} 
${information_translator_huffman_srcs}  
${information_translator_shannon_fano_srcs}
${information_translator_qrl_srcs} 
${information_translator_run_length_srcs}
${information_translator_base64_srcs} 
${codec_srcs} ${lzo_srcs}
${fft_srcs}
${mpl_srcs} ${mpl_natural_srcs} ${mpl_prime_srcs} ${mpl_rsa_srcs}
${math_srcs}
${functor_srcs}
${fs_srcs}
${fs_disk_srcs}
${fs_local_srcs}
${rtld_srcs}
${geometry_srcs} ${geometry_iso2d_srcs} ${geometry_iso3d_srcs}
${kr_sc_srcs} ${kr_bc_srcs}
${sparse_srcs}
${counting_srcs}
${oxide_srcs} ${oxide_field_srcs} ${oxide_grid_srcs} ${oxide_algo_srcs} ${oxide_mpi_srcs}
${jargon_srcs} 
${jargon_token_srcs} 
${jargon_pattern_srcs} 
${jargon_pattern_basic_srcs}
${jargon_pattern_logic_srcs}
${jargon_pattern_joker_srcs}
${tensor_srcs}
${sibyl_srcs}
${yap_srcs} ${yap_natural_srcs}
)
 
INSTALL( TARGETS y ARCHIVE DESTINATION lib )


ADD_SUBDIRECTORY(cli)


ADD_SUBDIRECTORY(tests)
SET(UTESTS "${UTESTS} utest-main" PARENT_SCOPE)

MESSAGE( STATUS "<Upsylon/>" )
MESSAGE(STATUS "")
