
ADD_EXECUTABLE(test-mini-lzo ../y/codec/lzo/testmini.c)
TARGET_LINK_Y(test-mini-lzo)
ADD_CUSTOM_COMMAND(
TARGET             test-mini-lzo
POST_BUILD COMMAND test-mini-lzo
COMMENT            "Testing miniLZO"
)

FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-main main.cpp ${srcs} support.hpp)
TARGET_LINK_Y(test-main)

ADD_LIBRARY(module MODULE module.cpp)


ENABLE_TESTING()
SOURCE_GROUP(os FILES test-platform.cpp test-types.cpp test-sys.cpp test-at-exit.cpp test-endian.cpp test-env.cpp test-spec.cpp)
ADD_TEST(os:platform    test-main platform)
ADD_TEST(os:types       test-main types)
ADD_TEST(os:sys         test-main sys)
ADD_TEST(os:atexit      test-main at_exit)
ADD_TEST(os:endian      test-main endian)
ADD_TEST(os:env         test-main env)
ADD_TEST(os:spec        test-main spec)

SOURCE_GROUP(core FILES test-ppty.cpp test-lockable.cpp test-swap.cpp test-zset.cpp test-code.cpp test-bits.cpp test-core.cpp test-progress.cpp
 test-functor.cpp test-tuple.cpp test-traits.cpp)
ADD_TEST(core:ppty     test-main ppty)
ADD_TEST(core:lockable test-main lockable)
ADD_TEST(core:swap        test-main swap)
ADD_TEST(core:zset        test-main zset)
ADD_TEST(core:code        test-main code)
ADD_TEST(core:bits        test-main bits)
ADD_TEST(core:main        test-main core)
ADD_TEST(core:progress    test-main progress)
ADD_TEST(core:functor     test-main functor)
ADD_TEST(core:tuple       test-main tuple)
ADD_TEST(core:traits      test-main traits)

SOURCE_GROUP(mem FILES test-chunk.cpp test-arena.cpp test-alloc.cpp test-blocks.cpp test-slab.cpp test-object.cpp test-slice.cpp
test-carver.cpp test-pooled.cpp test-buffers.cpp test-cslot.cpp test-nugget.cpp test-vein.cpp test-dyadic.cpp test-ptr.cpp test-release.cpp)
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
ADD_TEST(mem:cslot       test-main cslot)
ADD_TEST(mem:nugget      test-main nugget)
ADD_TEST(mem:nuggets     test-main nuggets)
ADD_TEST(mem:vein        test-main vein)
ADD_TEST(mem:dyadic      test-main dyadic)
ADD_TEST(mem:ptr         test-main ptr)
ADD_TEST(mem:release     test-main release)

SOURCE_GROUP(par FILES test-parallel.cpp test-threads.cpp test-simd.cpp)
ADD_TEST(par:parallel    test-main parallel)
ADD_TEST(par:threads     test-main threads)
ADD_TEST(par:simd        test-main simd)

SOURCE_GROUP(str FILES test-string.cpp test-string-convert.cpp test-tokenizer.cpp)
ADD_TEST(str:string      test-main string)
ADD_TEST(str:strconv     test-main string_convert s 123456 s 0xff i -23 i 908 s 0x34)
ADD_TEST(str:tokenizer   test-main tokenizer)


SOURCE_GROUP(ios FILES test-ostreams.cpp test-istreams.cpp test-vfs.cpp test-dll.cpp test-tmpname.cpp test-local-file.cpp test-ichannel.cpp test-bin2dat.cpp)
ADD_TEST(ios:ostreams    test-main ostreams)
ADD_TEST(ios:istreams    test-main istreams)
ADD_TEST(ios:vfs         test-main vfs .)
ADD_TEST(NAME ios:dll COMMAND test-main dll $<TARGET_FILE:module>)
ADD_TEST(ios:tmpname     test-main tmpname)
ADD_TEST(ios:local-file  test-main local_file)
ADD_TEST(ios:ichannel    test-main ichannel)
ADD_TEST(ios:bin2dat     test-main bin2dat)

SOURCE_GROUP(crypto FILES test-hashing.cpp test-hmac.cpp test-kr-sc.cpp test-kr-bc.cpp test-uuid.cpp test-digest.cpp)
ADD_TEST(crypto:hashing     test-main hashing)
ADD_TEST(crypto:hmac        test-main hmac)
ADD_TEST(crypto:kr_sc       test-main kr_sc)
ADD_TEST(crypto:kr_bc       test-main kr_bc)
ADD_TEST(crypto:uuid        test-main uuid)
ADD_TEST(crypto:digest      test-main digest)

SOURCE_GROUP(mk FILES test-xbitrev.cpp test-fft.cpp test-triplet.cpp test-points.cpp test-primality.cpp test-complex.cpp test-sobol.cpp test-timings.cpp)
ADD_TEST(mk:xbitrev     test-main xbitrev 0.0)
ADD_TEST(mk:fft         test-main fft)
ADD_TEST(mk:triplet     test-main triplet)
ADD_TEST(mk:points      test-main points)
ADD_TEST(mk:primality   test-main primality)
ADD_TEST(mk:complex     test-main complex)
ADD_TEST(mk:sobol       test-main sobol)
ADD_TEST(mk:timings     test-main timings)


SOURCE_GROUP(ispo FILES test-ipso.cpp test-ipso-io.cpp test-ipso-gh.cpp test-ipso-dct.cpp test-ipso-c2d.cpp)
ADD_TEST(ipso:main   test-main  ipso)
ADD_TEST(ipso:io     test-main  ipso_io)
ADD_TEST(ipso:gh     test-main  ipso_gh)
ADD_TEST(ipso:dct    test-main  ipso_dct)
ADD_TEST(ipso:c2d    test-main  ipso_c2d)

SOURCE_GROUP(lang FILES test-token.cpp test-source.cpp test-pattern.cpp test-posix.cpp test-regex.cpp test-matching.cpp test-scanner.cpp test-lex.cpp)
ADD_TEST(Lang:Token    test-main  token)
ADD_TEST(Lang:Source   test-main  source NULL)
ADD_TEST(Lang:Pattern  test-main  pattern)
ADD_TEST(Lang:Posix    test-main  posix)
ADD_TEST(Lang:RegEx    test-main  regex)
ADD_TEST(Lang:Matching test-main  matching)
ADD_TEST(Lang:Scanner  test-main  scanner NULL)
ADD_TEST(Lang:Lex      test-main  lex     NULL)

SOURCE_GROUP(geom FILES test-geom-iso2d.cpp)
ADD_TEST(geom:iso2d test-main geom_iso2d)

SOURCE_GROUP(mp FILES test-mpn.cpp test-mpz.cpp test-mpq.cpp)
ADD_TEST(mp:n         test-main mpn)
ADD_TEST(mp:fact      test-main mpn_fact 1 10 20 30 50)
ADD_TEST(mp:z         test-main mpz)
ADD_TEST(mp:q         test-main mpq)

SOURCE_GROUP(sort FILES test-sort.cpp test-unique.cpp test-maintain.cpp test-nwsrt.cpp test-locate.cpp)
ADD_TEST(sort:sort        test-main sort)
ADD_TEST(sort:unique      test-main unique)
ADD_TEST(sort:maintain    test-main maintain)
ADD_TEST(sort:nw          test-main nwsrt)
ADD_TEST(sort:locate      test-main locate)

SOURCE_GROUP(codec FILES test-iobits.cpp test-base64.cpp test-lzo.cpp test-mtf.cpp test-delta.cpp test-asf.cpp test-bwt.cpp)
ADD_TEST(codec:iobits test-main iobits)
ADD_TEST(codec:base64 test-main base64 NULL)
ADD_TEST(codec:lzo    test-main lzo)
ADD_TEST(codec:mtf    test-main mtf)
ADD_TEST(codec:delta  test-main delta)
ADD_TEST(codec:asf    test-main asf)
ADD_TEST(codec:bwt    test-main bwt)

SOURCE_GROUP(data FILES test-associative.cpp test-set.cpp test-map.cpp test-matrix.cpp test-slots.cpp test-sequence.cpp test-pipe.cpp test-arrays.cpp)
ADD_TEST(data:associative test-main associative)
ADD_TEST(data:set         test-main set)
ADD_TEST(data:map         test-main map)
ADD_TEST(data:matrix      test-main matrix)
ADD_TEST(data:slots       test-main slots)
ADD_TEST(data:sequence    test-main sequence)
ADD_TEST(data:pipe        test-main pipe)
ADD_TEST(data:arrays      test-main arrays)



ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-main)
