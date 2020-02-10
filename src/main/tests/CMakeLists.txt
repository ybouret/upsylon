PROJECT(test-main)

ADD_EXECUTABLE(test-lzo ../y/codec/lzo/testmini.c)
TARGET_LINK_Y(test-lzo)
ADD_CUSTOM_COMMAND(
TARGET             test-lzo
POST_BUILD COMMAND test-lzo
COMMENT            "Testing LZO"
)

FILE(GLOB srcs test-*.cpp)
SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND( OUTPUT  ${HERE}/rsa-keys.inc
COMMAND y_bin2dat ${HERE}/rsa-keys.bin ${HERE}/rsa-keys.inc
DEPENDS y_bin2dat ${HERE}/rsa-keys.bin
COMMENT "Generating Included RSA Keys" )

ADD_EXECUTABLE(test-main main.cpp ${srcs} support.hpp rsa-keys.inc)
TARGET_LINK_Y(test-main)

ADD_LIBRARY(module MODULE module.cpp)

ADD_DEPENDENCIES(test-main module)

ENABLE_TESTING()
SOURCE_GROUP(os FILES test-platform.cpp test-types.cpp test-sys.cpp test-at-exit.cpp test-endian.cpp test-env.cpp test-spec.cpp test-chkbnd.cpp test-i2i.cpp
test-hr-ints.cpp test-ints-display.cpp test-cast.cpp test-ints-to-real.cpp test-standard.cpp)
ADD_TEST(os:platform     test-main platform)
ADD_TEST(os:types        test-main types)
ADD_TEST(os:sys          test-main sys)
ADD_TEST(os:atexit       test-main at_exit)
ADD_TEST(os:endian       test-main endian)
ADD_TEST(os:env          test-main env)
ADD_TEST(os:spec         test-main spec)
ADD_TEST(os:chkbnd       test-main chkbnd)
ADD_TEST(os:hr-ints      test-main hr_ints)
ADD_TEST(os:ints-display test-main ints_display)
ADD_TEST(os:cast         test-main cast)
ADD_TEST(os:ints-to-real test-main i2r)
ADD_TEST(os:int-to-int   test-main i2i)
ADD_TEST(os:standard     test-main standard)

SOURCE_GROUP(core FILES test-ppty.cpp test-lockable.cpp test-zset.cpp test-code.cpp test-bits.cpp 
test-core-pool.cpp  test-core-list.cpp test-core-clist.cpp
test-progress.cpp
 test-functor.cpp test-tuple.cpp test-traits.cpp test-bswap.cpp test-bmove.cpp test-isqrt.cpp test-zfi.cpp test-binary.cpp)
ADD_TEST(core:ppty     test-main ppty)
ADD_TEST(core:lockable test-main lockable)
ADD_TEST(core:zset        test-main zset)
ADD_TEST(core:bswap       test-main bswap)
ADD_TEST(core:bmove       test-main bmove)
ADD_TEST(core:code        test-main code)
ADD_TEST(core:bits        test-main bits)
ADD_TEST(core:pool        test-main core_pool)
ADD_TEST(core:list        test-main core_list)
ADD_TEST(core:clist       test-main core_clist)
ADD_TEST(core:progress    test-main progress)
ADD_TEST(core:functor     test-main functor)
ADD_TEST(core:tuple       test-main tuple)
ADD_TEST(core:traits      test-main traits)
ADD_TEST(core:isqrt       test-main isqrt)
ADD_TEST(core:zfi         test-main zfi)
ADD_TEST(core:binary      test-main binary)

SOURCE_GROUP(mem FILES test-chunk.cpp test-arena.cpp test-alloc.cpp test-blocks.cpp test-slab.cpp test-object.cpp test-slice.cpp test-embed.cpp
test-carver.cpp test-pooled.cpp test-buffers.cpp test-xslot.cpp test-nugget.cpp test-vein.cpp test-dyadic.cpp test-ptr.cpp test-release.cpp test-static-slots.cpp)
ADD_TEST(mem:chunk       test-main chunk)
ADD_TEST(mem:arena       test-main arena)
ADD_TEST(mem:alloc       test-main alloc)
ADD_TEST(mem:blocks      test-main blocks)
ADD_Test(mem:slab        test-main slab)
ADD_TEST(mem:object0     test-main object0)
ADD_TEST(mem:objectY     test-main objectY)
ADD_TEST(mem:slice       test-main slice)
ADD_TEST(mem:carver      test-main carver)
ADD_TEST(mem:pooled      test-main pooled)
ADD_TEST(mem:buffers     test-main buffers)
ADD_TEST(mem:xslot       test-main xslot)
ADD_TEST(mem:nugget      test-main nugget)
ADD_TEST(mem:nuggets     test-main nuggets)
ADD_TEST(mem:vein        test-main vein)
ADD_TEST(mem:dyadic      test-main dyadic)
ADD_TEST(mem:ptr         test-main ptr)
ADD_TEST(mem:release     test-main release)
ADD_TEST(mem:embed       test-main embed)
ADD_TEST(mem:slots       test-main static_slots)

SOURCE_GROUP(par FILES test-parallel.cpp test-threads.cpp test-simd.cpp test-server.cpp)
ADD_TEST(par:parallel    test-main parallel)
ADD_TEST(par:threads     test-main threads)
ADD_TEST(par:simd        test-main simd)
ADD_TEST(par:server      test-main server)
ADD_TEST(par:servpi      test-main servpi)

SOURCE_GROUP(str FILES test-string.cpp test-string-convert.cpp test-tokenizer.cpp test-string-defrag.cpp test-esma.cpp) 
ADD_TEST(str:string      test-main string)
ADD_TEST(str:strconv     test-main string_convert s 123456 s 0xff i -23 i 908 s 0x34)
ADD_TEST(str:tokenizer   test-main tokenizer)
ADD_TEST(str:defrag      test-main string_defrag)
ADD_TEST(str:esma        test-main esma foo ${HERE}/words.db)


SOURCE_GROUP(ios FILES test-ostreams.cpp test-istreams.cpp
test-vfs.cpp test-dll.cpp test-tmpname.cpp test-local-file.cpp
test-bin2dat.cpp
test-disk-file.cpp test-disk-std.cpp test-serializer.cpp test-upack.cpp test-memory-streams.cpp
test-ios-plugin.cpp
test-cmd.cpp
test-serialize-tools.cpp
)
ADD_TEST(ios:ostreams    test-main ostreams)
ADD_TEST(ios:istreams    test-main istreams)
ADD_TEST(ios:mstreams    test-main mstreams)
ADD_TEST(ios:vfs         test-main vfs .)
ADD_TEST(NAME ios:dll COMMAND test-main dll $<TARGET_FILE:module>)
ADD_TEST(ios:tmpname     test-main tmpname)
ADD_TEST(ios:local-file  test-main local_file)
ADD_TEST(ios:bin2dat     test-main bin2dat)
ADD_TEST(ios:disk_file   test-main disk_file)
ADD_TEST(ios:serializer  test-main serializer)
ADD_TEST(ios:upack       test-main upack)
ADD_TEST(ios:plugin      test-main ios_plugin)
ADD_TEST(ios:cmd         test-main cmd)
ADD_TEST(ios:serialized  test-main serialize_tools)

SOURCE_GROUP(crypto FILES test-hashing.cpp test-hmac.cpp test-kr-sc.cpp test-kr-bc.cpp test-uuid.cpp test-digest.cpp test-rsa.cpp test-rsa-io.cpp test-mperf.cpp test-pearson.cpp) 
ADD_TEST(crypto:hashing     test-main hashing)
ADD_TEST(crypto:mperf       test-main mperf)
ADD_TEST(crypto:pearson     test-main pearson)
ADD_TEST(crypto:hmac        test-main hmac)
ADD_TEST(crypto:kr_sc       test-main kr_sc)
ADD_TEST(crypto:kr_bc       test-main kr_bc)
ADD_TEST(crypto:uuid        test-main uuid)
ADD_TEST(crypto:digest      test-main digest)
ADD_TEST(crypto:rsa1        test-main rsa 11 3 3)
ADD_TEST(crypto:rsa2        test-main rsa 62507 55487 65537)
ADD_TEST(crypto:rsa3        test-main rsa 3935648537 3730255349 65537)
ADD_TEST(crypto:rsa4        test-main rsa 18300546520784097569 16483490701427528033 65537)
ADD_TEST(crypto:rsa_keys    test-main rsa_keys ${HERE}/rsa-keys.bin)
ADD_TEST(crypto:rsa_io      test-main rsa_io 0 2 4 8)

SOURCE_GROUP(mk FILES test-xbitrev.cpp test-fft.cpp test-triplet.cpp test-points.cpp test-primality.cpp test-complex.cpp test-sobol.cpp test-timings.cpp)
ADD_TEST(mk:xbitrev     test-main xbitrev 0.0)
ADD_TEST(mk:fft         test-main fft)
ADD_TEST(mk:triplet     test-main triplet)
ADD_TEST(mk:points      test-main points)
ADD_TEST(mk:primality   test-main primality)
ADD_TEST(mk:complex     test-main complex)
ADD_TEST(mk:sobol       test-main sobol)
ADD_TEST(mk:timings     test-main timings)

SOURCE_GROUP(lang FILES test-token.cpp test-source.cpp test-pattern.cpp 
test-posix.cpp test-regex.cpp test-matching.cpp test-scanner.cpp test-lex.cpp test-common.cpp test-token-convert.cpp test-sed.cpp)
ADD_TEST(Lang:Token    test-main  token)
ADD_TEST(Lang:Source   test-main  source NULL)
ADD_TEST(Lang:Pattern  test-main  pattern)
ADD_TEST(Lang:Common   test-main  common)
ADD_TEST(Lang:Posix    test-main  posix)
ADD_TEST(Lang:RegEx    test-main  regex)
ADD_TEST(Lang:Matching test-main  matching)
ADD_TEST(Lang:Scanner  test-main  scanner NULL)
ADD_TEST(Lang:Lex      test-main  lex     NULL)
ADD_TEST(Lang:Convert  test-main  token_convert -1 1 0 1234 -4321)
ADD_TEST(Lang:SED      test-main  sed)

SOURCE_GROUP(geom FILES test-contour2d.cpp test-contour3d.cpp test-closest.cpp)
ADD_TEST(geom:contour2d test-main contour2d)
ADD_TEST(geom:contour3d test-main contour3d)
ADD_TEST(geom:closest   test-main closest)

SOURCE_GROUP(mp FILES test-mpn.cpp test-mpz.cpp test-mpq.cpp test-mprm.cpp test-ilog.cpp 
test-prime-factor.cpp test-prime-ratio.cpp test-mp-io.cpp test-mpn-perf.cpp) 
ADD_TEST(mp:n         test-main mpn)
ADD_TEST(mp:perf      test-main mpn_perf 0.01 1)
ADD_TEST(mp:fact      test-main mpn_fact 1 0x0a 20 30 50)
ADD_TEST(mp:z         test-main mpz)
ADD_TEST(mp:q         test-main mpq)
ADD_TEST(mp:prm       test-main mprm 512)
ADD_TEST(mp:prm1000   test-main mprm1000 ${HERE}/1000.txt)
ADD_TEST(mp:ilog      test-main ilog)
ADD_TEST(mp:primes    test-main prime_factor 0 1 2 3 4 0x3E8 0x2711)
ADD_TEST(mp:primesQ   test-main prime_ratio)
ADD_TEST(mp:io        test-main mp_io)

SOURCE_GROUP(sort FILES test-sort.cpp test-unique.cpp test-maintain.cpp test-nwsrt.cpp test-locate.cpp test-classes.cpp)
ADD_TEST(sort:sort        test-main sort)
ADD_TEST(sort:unique      test-main unique)
ADD_TEST(sort:maintain    test-main maintain)
ADD_TEST(sort:nw          test-main nwsrt)
ADD_TEST(sort:locate      test-main locate)
ADD_TEST(sort:classes     test-main classes)

SOURCE_GROUP(codec FILES test-iobits.cpp test-base64.cpp test-lzo.cpp test-mtf.cpp test-delta.cpp test-asf.cpp test-bwt.cpp)
ADD_TEST(codec:iobits test-main iobits)
ADD_TEST(codec:base64 test-main base64 NULL)
ADD_TEST(codec:lzo    test-main lzo)
ADD_TEST(codec:mtf    test-main mtf)
ADD_TEST(codec:delta  test-main delta)
ADD_TEST(codec:asf    test-main asf)
ADD_TEST(codec:bwt    test-main bwt)
ADD_TEST(codec:b64id  test-main b64id)

SOURCE_GROUP(data FILES
test-associative.cpp test-set.cpp test-map.cpp test-matrix.cpp test-slots.cpp test-sequence.cpp test-pipe.cpp test-arrays.cpp test-heap.cpp test-avl.cpp test-btree.cpp
test-const-field.cpp test-for-each.cpp test-ordered.cpp test-key-address.cpp test-addressable.cpp test-query.cpp)
ADD_TEST(data:associative test-main associative)
ADD_TEST(data:set         test-main set)
ADD_TEST(data:map         test-main map)
ADD_TEST(data:btree       test-main btree)
ADD_TEST(data:matrix      test-main matrix)
ADD_TEST(data:slots       test-main slots)
ADD_TEST(data:sequence    test-main sequence)
ADD_TEST(data:pipe        test-main pipe)
ADD_TEST(data:arrays      test-main arrays)
ADD_TEST(data:heap        test-main heap)
ADD_TEST(data:avl         test-main avl)
ADD_TEST(data:const-field test-main const_field)
ADD_TEST(data:for_each    test-main for_each)
ADD_TEST(data:ordered     test-main ordered)
ADD_TEST(data:key-address test-main key_address)
ADD_TEST(data:strings     test-main strings ${HERE}/words.db)
ADD_TEST(data:addressable test-main addressable)
ADD_TEST(data:query       test-main query ${HERE}/words.db)

SOURCE_GROUP(sparse FILES test-sparse-array.cpp test-sparse-matrix.cpp)
ADD_TEST(sparse:array  test-main sparse_array)
ADD_TEST(sparse:matrix test-main sparse_matrix)

SOURCE_GROUP(counting FILES test-comb.cpp test-perm.cpp test-mloop.cpp test-dancing.cpp test-scrabble.cpp)
ADD_TEST(counting:comb     test-main comb 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
ADD_TEST(counting:perm     test-main perm 1 2 3 4 5 6 7)
ADD_TEST(counting:anagram  test-main anagram yann)
ADD_TEST(counting:mloops   test-main mloops)
ADD_TEST(counting:dancing  test-main dancing)
ADD_TEST(counting:scrabble test-main scrabble estelle ${HERE}/words.db)


SOURCE_GROUP(oxide FILES test-oxide.cpp test-oxide-fields.cpp
test-oxide-io.cpp test-oxide-dct.cpp test-oxide-split.cpp test-oxide-ranks.cpp
test-oxide-layouts.cpp test-oxide-wksp.cpp test-oxide-allw.cpp test-oxide-mpi-sim.cpp
test-oxide-vtk.cpp test-oxide-par.cpp test-oxide-grids.cpp
)
ADD_TEST(oxide:main       test-main  oxide)
ADD_TEST(oxide:fields     test-main  oxide_fields)
ADD_TEST(oxide:io         test-main  oxide_io)
ADD_TEST(oxide:dct        test-main  oxide_dct)
ADD_TEST(oxide:split      test-main  oxide_split)
ADD_TEST(oxide:ranks      test-main  oxide_ranks)
ADD_TEST(oxide:layouts    test-main  oxide_layouts)
ADD_TEST(oxide:wksp       test-main  oxide_wksp)
ADD_TEST(oxide:allw       test-main  oxide_allw)
ADD_TEST(oxide:mpi        test-main  oxide_mpi)
ADD_TEST(oxide:vtk        test-main  oxide_vtk)
ADD_TEST(oxide:par        test-main  oxide_par)
ADD_TEST(oxide:grids      test-main  oxide_grids)

ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-main)
