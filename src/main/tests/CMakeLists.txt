
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
ADD_TEST(Platform    test-main platform)
ADD_TEST(Types       test-main types)
ADD_TEST(Sys         test-main sys)
ADD_TEST(AtExit      test-main at_exit)
ADD_TEST(Endian      test-main endian)
ADD_TEST(Env         test-main env)
ADD_TEST(Spec        test-main spec)

SOURCE_GROUP(mem FILES test-chunk.cpp test-arena.cpp test-alloc.cpp test-blocks.cpp test-slab.cpp test-object.cpp test-slice.cpp
test-carver.cpp test-pooled.cpp test-buffers.cpp test-cslot.cpp test-nugget.cpp test-vein.cpp test-dyadic.cpp)
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

SOURCE_GROUP(par FILES test-parallel.cpp test-threads.cpp test-simd.cpp)
ADD_TEST(par:parallel    test-main parallel)
ADD_TEST(par:threads     test-main threads)
ADD_TEST(par:simd        test-main simd)

SOURCE_GROUP(str FILES test-string.cpp test-string-convert.cpp)
ADD_TEST(str:string      test-main string)
ADD_TEST(str:strconv     test-main string_convert s 123456 s 0xff i -23 i 908 s 0x34)



SOURCE_GROUP(ios FILES test-ostreams.cpp test-istreams.cpp test-vfs.cpp test-dll.cpp test-tmpname.cpp)
ADD_TEST(ios:ostreams    test-main ostreams)
ADD_TEST(ios:istreams    test-main istreams)
ADD_TEST(ios:vfs         test-main vfs .)
ADD_TEST(NAME ios:dll COMMAND test-main dll $<TARGET_FILE:module>)
ADD_TEST(ios:tmpname     test-main tmpname)



SOURCE_GROUP(crypto FILES test-hashing.cpp test-hmac.cpp test-kr-sc.cpp test-kr-bc.cpp test-uuid.cpp test-digest.cpp)
ADD_TEST(crypto:hashing     test-main hashing)
ADD_TEST(crypto:hmac        test-main hmac)
ADD_TEST(crypto:kr_sc       test-main kr_sc)
ADD_TEST(crypto:kr_bc       test-main kr_bc)
ADD_TEST(crypto:uuid        test-main uuid)
ADD_TEST(crypto:digest      test-main digest)

SOURCE_GROUP(mk FILES test-xbitrev.cpp test-fft.cpp test-triplet.cpp test-points.cpp)
ADD_TEST(mk:xbitrev     test-main xbitrev 0.0)
ADD_TEST(mk:fft         test-main fft)
ADD_TEST(mk:triplet     test-main triplet)
ADD_TEST(mk:points      test-main points)
ADD_TEST(mk:primality   test-main primality)
ADD_TEST(mk:complex     test-main complex)


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



ADD_TEST(Sort        test-main sort)
ADD_TEST(Unique      test-main unique)
ADD_TEST(Maintain    test-main maintain)
ADD_TEST(Associative test-main associative)
ADD_TEST(Set         test-main set)
ADD_TEST(Map         test-main map)
ADD_TEST(Matrix      test-main matrix)
ADD_TEST(Functor     test-main functor)
ADD_TEST(nwsrt       test-main nwsrt)

ADD_TEST(Tuple       test-main tuple)
ADD_TEST(Release     test-main release)
ADD_TEST(Base64      test-main base64 NULL)
ADD_TEST(LZO         test-main lzo)
ADD_TEST(IObits      test-main iobits)
ADD_TEST(MTF         test-main  mtf)
ADD_TEST(Delta       test-main  delta)
ADD_TEST(ASF         test-main  asf)
ADD_TEST(Locate      test-main  locate)
ADD_TEST(Progress    test-main  progress)
ADD_TEST(Sobol       test-main  sobol)
ADD_TEST(Ppty        test-main ppty)
ADD_TEST(Lockable    test-main lockable)
ADD_TEST(Swap        test-main swap)
ADD_TEST(Zset        test-main zset)
ADD_TEST(Code        test-main code)
ADD_TEST(Bits        test-main bits)
ADD_TEST(Core        test-main core)
ADD_TEST(Ptr         test-main ptr)
ADD_TEST(Slots       test-main slots)
ADD_TEST(Sequence    test-main sequence)
ADD_TEST(Pipe        test-main pipe)
ADD_TEST(Arrays      test-main arrays)

ADD_TEST(MPN         test-main mpn)
ADD_TEST(MPZ         test-main mpz)
ADD_TEST(MPZ         test-main mpq)

ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-main)
