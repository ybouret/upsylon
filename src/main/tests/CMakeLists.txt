PROJECT(test-main)
 
FILE(GLOB srcs test-*.cpp)
SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

#ADD_CUSTOM_COMMAND( OUTPUT  ${HERE}/rsa-keys.inc
#COMMAND y_bin2dat ${HERE}/rsa-keys.bin ${HERE}/rsa-keys.inc
#DEPENDS y_bin2dat ${HERE}/rsa-keys.bin
#COMMENT "Generating Included RSA Keys" )

ADD_EXECUTABLE(test-main 
main.cpp ${srcs} 
support.hpp 
)
TARGET_LINK_Y(test-main)

SET(the_module ${HERE}/module.dll)

ADD_LIBRARY(module MODULE module.cpp)

add_custom_target(module_copy 
COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:module> ${the_module}
DEPENDS module
COMMENT "Copy local module.dll"
)


ADD_DEPENDENCIES(test-main module)

ENABLE_TESTING()
SOURCE_GROUP(os FILES test-platform.cpp test-types.cpp test-sys.cpp test-at-exit.cpp test-endian.cpp test-env.cpp 
test-chkbnd.cpp 
test-hr-ints.cpp test-ints-display.cpp test-cast.cpp  test-standard.cpp test-base2.cpp
test-run-time-log.cpp
)
ADD_TEST(os:platform     test-main platform)
ADD_TEST(os:types        test-main types)
ADD_TEST(os:sys          test-main sys)
ADD_TEST(os:atexit       test-main at_exit)
ADD_TEST(os:endian       test-main endian)
ADD_TEST(os:env          test-main env)
ADD_TEST(os:chkbnd       test-main chkbnd)
ADD_TEST(os:hr-ints      test-main hr_ints)
ADD_TEST(os:ints-display test-main ints_display)
ADD_TEST(os:cast         test-main cast)
ADD_TEST(os:standard     test-main standard)
ADD_TEST(os:base2        test-main base2)
ADD_TEST(os:rtl          test-main rtl)

SOURCE_GROUP(type FILES 
test-type-cull.cpp
test-ints-to-real.cpp
test-i2i.cpp
test-decomp.cpp
test-type-authority.cpp
test-rtti.cpp
)
ADD_TEST(type:cull         test-main cull)
ADD_TEST(type:ints-to-real test-main i2r)
ADD_TEST(type:int-to-int   test-main i2i)
ADD_TEST(type:decomp       test-main decomp)
ADD_TEST(type:authority    test-main type_authority)
ADD_TEST(type:info         test-main rtti)


SOURCE_GROUP(core FILES test-ppty.cpp test-lockable.cpp 
test-code.cpp test-bits.cpp
test-core-pool.cpp  
test-core-list.cpp test-core-clist.cpp test-core-stack.cpp
test-core-roll.cpp
test-core-addr-node.cpp
test-progress.cpp
test-functor.cpp test-tuple.cpp test-traits.cpp  test-isqrt.cpp test-zfi.cpp test-binary.cpp
test-meta-loop.cpp
test-core-nodes.cpp
test-core-momentary.cpp
test-core-rnode.cpp
 )
ADD_TEST(core:ppty     test-main ppty)
ADD_TEST(core:lockable test-main lockable)
ADD_TEST(core:code        test-main code)
ADD_TEST(core:bits        test-main bits)
ADD_TEST(core:pool        test-main core_pool)
ADD_TEST(core:list        test-main core_list)
ADD_TEST(core:roll        test-main core_roll)
ADD_TEST(core:clist       test-main core_clist)
ADD_TEST(core:stack       test-main core_stack)
ADD_TEST(core:progress    test-main progress)
ADD_TEST(core:functor     test-main functor)
ADD_TEST(core:tuple       test-main tuple)
ADD_TEST(core:traits      test-main traits)
ADD_TEST(core:isqrt       test-main isqrt)
ADD_TEST(core:zfi         test-main zfi)
ADD_TEST(core:binary      test-main binary)
ADD_TEST(core:metaloop    test-main metaloop)
ADD_TEST(core:nodes       test-main core_nodes)
ADD_TEST(core:momentary   test-main core_momentary)
ADD_TEST(core:rnode       test-main core_rnode)
ADD_TEST(core:anode       test-main core_anode)

SOURCE_GROUP(mem FILES test-alloc.cpp 
test-object.cpp test-embed.cpp
test-xslot.cpp test-ptr.cpp test-release.cpp 
test-memory-marker.cpp
test-memory-joint-section.cpp
test-memory-joint-ward.cpp
test-memory-tight-chunk.cpp
test-memory-tight-arena.cpp
test-memory-tight-zcache.cpp
test-memory-tight-blocks.cpp
test-memory-tight-vein.cpp
test-memory-tight-quarry.cpp
test-memory-tight-object.cpp
test-memory-tight-wedge.cpp
test-memory-tight-twain.cpp
test-memory-zblock.cpp
test-memory-tight-supply.cpp
test-memory-magazine.cpp
test-memory-buffers.cpp
test-memory-shack.cpp
test-memory-dblocks.cpp
test-memory-lock.cpp
)
ADD_TEST(mem:alloc         test-main alloc)
ADD_TEST(mem:object0       test-main object0)
ADD_TEST(mem:objectY       test-main objectY)
ADD_TEST(mem:xslot         test-main xslot)
ADD_TEST(mem:ptr           test-main ptr)
ADD_TEST(mem:release       test-main release)
ADD_TEST(mem:embed         test-main embed)
ADD_TEST(mem:marker        test-main marker)
ADD_TEST(mem:zblock        test-main zblock)
ADD_TEST(mem:tight:chunk   test-main tight_chunk)
ADD_TEST(mem:tight:arena   test-main tight_arena)
ADD_TEST(mem:tight:zcache  test-main tight_zcache)
ADD_TEST(mem:tight:blocks  test-main tight_blocks)
ADD_TEST(mem:tight:compact test-main tight_compact)
ADD_TEST(mem:tight:vein    test-main tight_vein)
ADD_TEST(mem:tight:quarry  test-main tight_quarry)
ADD_TEST(mem:tight:object  test-main tight_object)
ADD_TEST(mem:tight:wedge   test-main tight_wedge)
ADD_TEST(mem:tight:supply  test-main tight_supply)
ADD_TEST(mem:joint:section test-main joint_section)
ADD_TEST(mem:joint:ward    test-main joint_ward)
ADD_TEST(mem:joint:compact test-main joint_compact)
ADD_TEST(mem:magazine      test-main magazine)
ADD_TEST(mem:shack         test-main shack)
ADD_TEST(mem:dblocks       test-main dblocks)
ADD_TEST(mem:lock          test-main mlock)

SOURCE_GROUP(thr FILES 
test-thr-context.cpp
test-thr-topology.cpp
test-thr-thread.cpp
test-thr-loop.cpp
test-thr-runic.cpp
test-thr-pi-simt.cpp
test-thr-pipeline.cpp
)
ADD_TEST(thr:context    test-main thr_context)
ADD_TEST(thr:topology   test-main thr_topology)
ADD_TEST(thr:thread     test-main thr_thread)
ADD_TEST(thr:loop       test-main thr_loop)
ADD_TEST(thr:runic      test-main thr_runic)
ADD_TEST(thr:pi_simt    test-main thr_pi_simt)
ADD_TEST(thr:pipeline   test-main thr_pipeline)

SOURCE_GROUP(str FILES test-string.cpp test-string-convert.cpp test-tokenizer.cpp test-string-defrag.cpp test-esma.cpp test-string-time.cpp) 
ADD_TEST(str:string      test-main string)
ADD_TEST(str:strconv     test-main string_convert s 123456 s 0xff i -23 i 908 s 0x34)
ADD_TEST(str:tokenizer   test-main tokenizer)
ADD_TEST(str:defrag      test-main string_defrag)
ADD_TEST(str:esma        test-main esma foo ${HERE}/words.db)
ADD_TEST(str:time        test-main string_time)


SOURCE_GROUP(ios FILES test-ostreams.cpp test-istreams.cpp
test-vfs.cpp test-dll.cpp test-tmpname.cpp test-local-file.cpp
test-bin2dat.cpp
test-disk-file.cpp test-disk-std.cpp test-serializer.cpp test-memory-streams.cpp
test-cmd.cpp
test-serialize-tools.cpp
test-srz-io.cpp
test-upacker.cpp
test-ios-align.cpp
test-rename.cpp
test-echo.cpp
test-ios-carrier.cpp
test-ios-scribe.cpp
test-ios-xpaths.cpp
)
ADD_TEST(ios:ostreams    test-main ostreams)
ADD_TEST(ios:istreams    test-main istreams ${HERE}/words.db)
ADD_TEST(ios:mstreams    test-main mstreams)
ADD_TEST(ios:vfs         test-main vfs .)
ADD_TEST(ios:find        test-main find)
ADD_TEST(NAME ios:dll COMMAND test-main dll $<TARGET_FILE:module>)
ADD_TEST(ios:tmpname     test-main tmpname)
ADD_TEST(ios:local-file  test-main local_file)
ADD_TEST(ios:bin2dat     test-main bin2dat)
ADD_TEST(ios:disk_file   test-main disk_file)
ADD_TEST(ios:serializer  test-main serializer)
ADD_TEST(ios:cmd         test-main cmd)
ADD_TEST(ios:serialized  test-main serialize_tools)
ADD_TEST(ios:srz_io      test-main srz_io)
ADD_TEST(ios:upacker     test-main upacker)
ADD_TEST(ios:align       test-main ios_align)
ADD_TEST(ios:rename      test-main rename)
ADD_TEST(ios:echo        test-main echo)
ADD_TEST(ios:carrier     test-main ios_carrier)
ADD_TEST(ios:scribe      test-main ios_scribe)
ADD_TEST(ios:xpaths      test-main xpaths)
ADD_TEST(ios:endl        test-main endl)


SOURCE_GROUP(crypto FILES test-hashing.cpp
test-hmac.cpp
test-kr-sc.cpp test-kr-bc.cpp test-uuid.cpp test-digest.cpp 
test-mperf.cpp
test-pearson.cpp
test-md.cpp
test-kr-rsa.cpp)

ADD_TEST(crypto:hashing     test-main hashing)
ADD_TEST(crypto:mperf       test-main mperf ${HERE}/ten.db)
ADD_TEST(crypto:pearson     test-main pearson)
ADD_TEST(crypto:hmac        test-main hmac)
ADD_TEST(crypto:kr_sc       test-main kr_sc)
ADD_TEST(crypto:kr_bc       test-main kr_bc)
ADD_TEST(crypto:uuid        test-main uuid)
ADD_TEST(crypto:digest      test-main digest)
ADD_TEST(crypto:md          test-main md ${HERE}/words.db)
ADD_TEST(crypto:rsa         test-main kr_rsa ${HERE}/sample512.key)
ADD_TEST(crypto:rsa2        test-main sc_rsa ${HERE}/sample512.key ${HERE}/sample1024.key)

SOURCE_GROUP(mk FILES test-triplet.cpp test-points.cpp test-primality.cpp test-complex.cpp test-sobol.cpp test-timings.cpp test-geomat.cpp test-trigo.cpp test-fft1.cpp test-xbra.cpp test-sincos.cpp)
ADD_TEST(mk:triplet     test-main triplet)
ADD_TEST(mk:points      test-main points)
ADD_TEST(mk:primality   test-main primality)
ADD_TEST(mk:complex     test-main complex)
ADD_TEST(mk:sobol       test-main sobol)
ADD_TEST(mk:timings     test-main timings)
ADD_TEST(mk:geomat      test-main geomat)
ADD_TEST(mk:trigo       test-main trigo)
ADD_TEST(mk:fft1        test-main fft1 0.02)
ADD_TEST(mk:xbra        test-main xbra)
ADD_TEST(mk:sincos      test-main sincos)

SOURCE_GROUP(sort FILES test-sort.cpp test-unique.cpp test-maintain.cpp test-locate.cpp test-classes.cpp test-network-sort.cpp test-blk.cpp test-groups.cpp
test-sort-merge-list.cpp
test-sort-merge-pool.cpp)
ADD_TEST(sort:network     test-main network_sort)
ADD_TEST(sort:sort        test-main sort perf)
ADD_TEST(sort:merge_list  test-main sort_merge_list)
ADD_TEST(sort:merge_pool  test-main sort_merge_pool)
ADD_TEST(sort:unique      test-main unique)
ADD_TEST(sort:maintain    test-main maintain)
ADD_TEST(sort:locate      test-main locate)
ADD_TEST(sort:classes     test-main classes)
ADD_TEST(sort:groups      test-main groups)
ADD_TEST(sort:blk         test-main blk)


SOURCE_GROUP(information FILES  
test-info-base64.cpp test-mtf.cpp test-delta.cpp test-bwt.cpp test-info-iobits.cpp test-huffman.cpp test-shannon-fano.cpp test-qrl.cpp test-pack.cpp
test-info-bytewise.cpp test-qcodec.cpp test-info-codecs.cpp test-info-run-length.cpp
)
ADD_TEST(info:mtf          test-main mtf)
ADD_TEST(info:delta        test-main delta)
ADD_TEST(info:bwt          test-main bwt)
ADD_TEST(info:iobits       test-main IOBits)
ADD_TEST(info:huffman      test-main huffman ${HERE}/words.db)
ADD_TEST(info:shannon_fano test-main shannon_fano ${HERE}/words.db)
ADD_TEST(info:qrl          test-main qrl ${HERE}/words.db)
ADD_TEST(info:run_length   test-main run_length)
ADD_TEST(info:pack         test-main pack ${HERE}/words.db)
ADD_TEST(info:bytewise     test-main bytewise ${HERE}/words.db)
ADD_TEST(info:qcodec       test-main qcodec ${HERE}/thousand.db)
ADD_TEST(info:base64       test-main base64)
ADD_TEST(info:codecs       test-main codecs)

SOURCE_GROUP(data FILES
test-matrix.cpp test-slots.cpp test-sequence.cpp test-pipe.cpp test-arrays.cpp test-heap.cpp test-avl.cpp 
test-frozen.cpp test-for-each.cpp test-ordered.cpp test-key-address.cpp test-addressable.cpp test-query.cpp
test-pq.cpp test-factory.cpp test-list.cpp test-tensors.cpp
test-crates.cpp
test-hunt.cpp
test-addresses.cpp
)

ADD_TEST(data:matrix      test-main matrix)
ADD_TEST(data:slots       test-main slots)
ADD_TEST(data:crates      test-main crates)
ADD_TEST(data:list        test-main list)
ADD_TEST(data:sequence    test-main sequence)
ADD_TEST(data:pipe        test-main pipe)
ADD_TEST(data:arrays      test-main arrays)
ADD_TEST(data:heap        test-main heap)
ADD_TEST(data:avl         test-main avl)
ADD_TEST(data:frozen      test-main frozen)
ADD_TEST(data:for_each    test-main for_each)
ADD_TEST(data:ordered     test-main ordered)
ADD_TEST(data:key-address test-main key_address)
ADD_TEST(data:addressable test-main addressable)
ADD_TEST(data:query       test-main query ${HERE}/words.db)
ADD_TEST(data:pq          test-main pq)
ADD_TEST(data:factory     test-main factory)
ADD_TEST(data:tensors     test-main tensors)
ADD_TEST(data:hunt        test-main hunt)
ADD_TEST(data:addresses   test-main addresses)


SOURCE_GROUP(associative FILES
test-lexicon.cpp
test-catalog.cpp
test-ledger.cpp
test-suffix.cpp test-suffix-address.cpp
test-hashed.cpp
)
ADD_TEST(associative:lexicon test-main lexicon)
ADD_TEST(associative:catalog test-main catalog)
ADD_TEST(associative:ledger  test-main ledger)
ADD_TEST(associative:suffix  test-main suffix ${HERE}/words.db)
ADD_TEST(associative:suffix  test-main suffix_address)
ADD_TEST(associative:hashed  test-main hashed)

SOURCE_GROUP(sparse FILES test-sparse-array.cpp test-sparse-matrix.cpp)
ADD_TEST(sparse:array  test-main sparse_array)
ADD_TEST(sparse:matrix test-main sparse_matrix)

SOURCE_GROUP(counting FILES
test-comb.cpp
test-perm.cpp test-anagram.cpp 
test-mloop.cpp
test-dancing.cpp 
test-scrabble.cpp 
test-phrases.cpp 
test-part.cpp
test-counting.cpp
test-symm-pair.cpp
test-counting-perf.cpp
test-drawers.cpp
)
ADD_TEST(counting:comb     test-main comb 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
ADD_TEST(counting:perm     test-main perm 1 2 3 4 5 6 7)
ADD_TEST(counting:perms    test-main permutations aaaaaaaa)
ADD_TEST(counting:anagram  test-main anagram yannb)
ADD_TEST(counting:mloops   test-main mloops)
ADD_TEST(counting:dancing  test-main dancing)
ADD_TEST(counting:scrabble test-main scrabble estelle ${HERE}/words.db)
ADD_TEST(counting:phrases  test-main phrases  fighter ${HERE}/words.db)
ADD_TEST(counting:part     test-main part)
ADD_TEST(counting:parts    test-main parts)
ADD_TEST(counting:bell     test-main bell)
ADD_TEST(counting:check    test-main counting)
ADD_TEST(counting:symm     test-main symm_pair)
ADD_TEST(counting:meta     test-main metaloop)
ADD_TEST(counting:perf     test-main counting_perf)
ADD_TEST(counting:drawers  test-main drawers)

SOURCE_GROUP(yap FILES
test-yap-n.cpp
test-yap-z.cpp
test-yap-q.cpp
test-yap-p.cpp
test-yap-prm.cpp
test-yap-pf.cpp
test-yap-db.cpp
test-yap-pr.cpp
test-yap-sprp.cpp
test-yap-log.cpp
test-yap-pt.cpp
test-yap-mod-exp.cpp
test-yap-lcm.cpp
test-yap-mul.cpp
test-yap-sq.cpp
test-yap-w2r.cpp
test-yap-hmul.cpp
)
ADD_TEST(yap:n    test-main yap_n)
ADD_TEST(yap:z    test-main yap_z)
ADD_TEST(yap:q    test-main yap_q)
ADD_TEST(yap:p    test-main yap_p)
ADD_TEST(yap:prm  test-main yap_prm ${HERE}/1000.txt)
ADD_TEST(yap:pf   test-main yap_pf 6666 5789)
ADD_TEST(yap:db   test-main yap_db 5000)
ADD_TEST(yap:pr   test-main yap_pr)
ADD_TEST(yap:sprp test-main yap_sprp)
ADD_TEST(yap:log  test-main yap_log)
ADD_TEST(yap:pt   test-main yap_pt)
ADD_TEST(fatorial test-main factorial 1 2 3 4 5 6 10 20 30)
ADD_TEST(yap:mod_exp test-main yap_mod_exp 7 4 5)
ADD_TEST(yap:lcm  test-main yap_lcm 10 11 12)
ADD_TEST(yap:mul  test-main yap_mul)
ADD_TEST(yap:sq   test-main yap_sq)
ADD_TEST(yap:hmul test-main yap_hmul)


SOURCE_GROUP(jive FILES
test-jive-tags.cpp
test-jive-char.cpp
test-jive-module.cpp
test-jive-source.cpp
test-jive-token.cpp
test-jive-patterns.cpp
test-jive-rework.cpp
test-jive-posix.cpp
test-jive-opt.cpp
test-jive-rx.cpp
test-jive-match.cpp
test-jive-leading.cpp
)
ADD_TEST(jive:tags     test-main jive_tags)
ADD_TEST(jive:char     test-main jive_char)
ADD_TEST(jive:module   test-main jive_module)
ADD_TEST(jive:source   test-main jive_source ${HERE}/1000.txt ${HERE}/10000.txt)
ADD_TEST(jive:token    test-main jive_token)
ADD_TEST(jive:patterns test-main jive_patterns)
ADD_TEST(jive:rework   test-main jive_rework)
ADD_TEST(jive:posix    test-main jive_posix)
ADD_TEST(jive:opt      test-main jive_opt)
ADD_TEST(jive:rx       test-main jive_rx "{INT}" ${HERE}/1000.txt)
ADD_TEST(jive:match    test-main jive_match "[:word:]+" ${HERE}/1000.txt)
ADD_TEST(jive:leading  test-main jive_leading)

SOURCE_GROUP(gfx FILES
test-gfx-colors.cpp
test-gfx-hist.cpp
test-gfx-conv.cpp
test-gfx-pixels.cpp
test-gfx-blend.cpp
)
ADD_TEST(gfx:colors test-main gfx_colors)
ADD_TEST(gfx:hist   test-main gfx_hist)
ADD_TEST(gfx:conv   test-main gfx_conv)
ADD_TEST(gfx:pixels test-main gfx_pixels)
ADD_TEST(gfx:blend  test-main gfx_blend)



ADD_CUSTOM_TARGET(utest-main
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-main)

INSTALL( TARGETS test-main RUNTIME DESTINATION var/y )

