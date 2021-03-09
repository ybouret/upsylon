#include "y/utest/driver.hpp"

Y_UTEST_INIT(256)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(binary);
    Y_UTEST(cast);
    Y_UTEST(chkbnd);
    Y_UTEST(i2i);
    Y_UTEST(base2);
    Y_UTEST(decomp);
    Y_UTEST(standard);
    Y_UTEST(lockable);
    Y_UTEST(rtl);

    Y_UTEST(cull);
    Y_UTEST(cull_info);
    Y_UTEST(type_authority);
    Y_UTEST(zset_perf);
    Y_UTEST(move_perf);

    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(bits);
    Y_UTEST(code);
    Y_UTEST(alloc);

    Y_UTEST(core_pool);
    Y_UTEST(core_list);
    Y_UTEST(core_clist);
    Y_UTEST(core_stack);
    Y_UTEST(core_roll);
    Y_UTEST(core_nodes);
    Y_UTEST(core_rnode);
    Y_UTEST(core_momentary);


    Y_UTEST(isqrt);
    Y_UTEST(primality);
    Y_UTEST(object0);
    Y_UTEST(objectY);
    Y_UTEST(zblock);
    Y_UTEST(xslot);
    Y_UTEST(ints_display);
    Y_UTEST(i2r);
    Y_UTEST(marker);

    Y_UTEST(tight_chunk);
    Y_UTEST(tight_arena);
    Y_UTEST(tight_zcache);
    Y_UTEST(tight_blocks);   
    Y_UTEST(tight_compact);
    Y_UTEST(tight_vein);
    Y_UTEST(tight_quarry);
    Y_UTEST(tight_object);
    Y_UTEST(tight_wedge);
    Y_UTEST(tight_twain);
    Y_UTEST(tight_supply);
    Y_UTEST(magazine);
    Y_UTEST(buffers);
    Y_UTEST(shack);
    Y_UTEST(dblocks);

    Y_UTEST(joint_section);
    Y_UTEST(joint_ward);
    Y_UTEST(joint_compact);
    
    Y_UTEST(mlock);

    Y_UTEST(mloops);
    Y_UTEST(comb); Y_UTEST(Comb);
    Y_UTEST(perm);
    Y_UTEST(permutations);
    Y_UTEST(part);
    Y_UTEST(parts);
    Y_UTEST(bell);
    Y_UTEST(anagram);
    Y_UTEST(counting);
    Y_UTEST(symm_pair);
    Y_UTEST(dancing);
    Y_UTEST(scrabble);
    Y_UTEST(phrases);
    Y_UTEST(metaloop);
    Y_UTEST(counting_perf);
    Y_UTEST(drawers);


    Y_UTEST(embed);
    Y_UTEST(hr_ints);

    Y_UTEST(string);
    Y_UTEST(string_convert);
    Y_UTEST(string_defrag);
    Y_UTEST(esma);
    Y_UTEST(srz_io);
    Y_UTEST(string_time);

    Y_UTEST(tmpname);
    Y_UTEST(tokenizer);
    Y_UTEST(traits);
    Y_UTEST(ptr);
    Y_UTEST(slots);
    Y_UTEST(crates);
    Y_UTEST(env);
    Y_UTEST(list);
    Y_UTEST(sequence);
    Y_UTEST(pipe);
    Y_UTEST(arrays);
    Y_UTEST(endian);
    Y_UTEST(key_address);
    Y_UTEST(addressable);
    Y_UTEST(factory);

    Y_UTEST(heap);
    Y_UTEST(avl);
    Y_UTEST(ordered);
    Y_UTEST(pq);
    Y_UTEST(const_field);
    Y_UTEST(for_each);

    Y_UTEST(thr_context);
    Y_UTEST(thr_topology);
    Y_UTEST(thr_thread);
    Y_UTEST(thr_loop);
    Y_UTEST(thr_pi_simt);
    Y_UTEST(thr_pipeline);
    
    Y_UTEST(complex);
    Y_UTEST(xbitrev);


    Y_UTEST(hashing);
    Y_UTEST(mperf);
    Y_UTEST(pearson);
    Y_UTEST(hmac);
    Y_UTEST(ostreams);
    Y_UTEST(istreams);
    Y_UTEST(mstreams);
    Y_UTEST(upacker);
    Y_UTEST(cmd);
    Y_UTEST(md);
    Y_UTEST(conveyor);
    Y_UTEST(conveyors);
    Y_UTEST(ios_align);
    Y_UTEST(rename);

    // assoc
    Y_UTEST(lexicon);
    Y_UTEST(catalog);
    Y_UTEST(ledger);
    Y_UTEST(suffix);
    Y_UTEST(hashed);
    
    Y_UTEST(fft);
    Y_UTEST(network_sort);
    Y_UTEST(sort);
    Y_UTEST(sort_merge);
    Y_UTEST(unique);
    Y_UTEST(classes);
    Y_UTEST(maintain);
    Y_UTEST(groups);
    

    Y_UTEST(query);
    
    Y_UTEST(matrix);
    Y_UTEST(functor);
    Y_UTEST(vfs);
    Y_UTEST(find);
    Y_UTEST(triplet);
    Y_UTEST(points);
    Y_UTEST(geomat);
    Y_UTEST(trigo);
    Y_UTEST(tuple);
    Y_UTEST(release);
    Y_UTEST(spec);
    Y_UTEST(spec_db);

    Y_UTEST(mtf);
    Y_UTEST(delta);
    Y_UTEST(bwt);
    Y_UTEST(IOBits);
    Y_UTEST(qcodec);
    Y_UTEST(huffman);
    Y_UTEST(shannon_fano);
    Y_UTEST(qrl);
    Y_UTEST(run_length);
    Y_UTEST(pack);
    Y_UTEST(bytewise);
    Y_UTEST(base64);
    Y_UTEST(codecs);   
 
    Y_UTEST(local_file);
    Y_UTEST(disk_file);
    Y_UTEST(disk_std);
    Y_UTEST(serializer);
    Y_UTEST(serialize_tools);

    Y_UTEST(bin2dat);
    Y_UTEST(digest);
    Y_UTEST(locate);
    Y_UTEST(blk);
    Y_UTEST(ppty);

    Y_UTEST(progress);
    Y_UTEST(dll);
    Y_UTEST(timings);
    Y_UTEST(sobol);
    

    Y_UTEST(kr_bc);
    Y_UTEST(kr_sc);
    Y_UTEST(uuid);

    Y_UTEST(sparse_array);
    Y_UTEST(sparse_matrix);

    Y_UTEST(zfi);

    Y_UTEST(tensors);

    Y_UTEST(yap_n);
    Y_UTEST(yap_z);
    Y_UTEST(yap_q);
    Y_UTEST(yap_p);
    Y_UTEST(yap_prm);
    Y_UTEST(yap_pf);
    Y_UTEST(yap_db);
    Y_UTEST(yap_pr);
    Y_UTEST(yap_sprp);
    Y_UTEST(yap_log);
    Y_UTEST(yap_pt);
    Y_UTEST(factorial);
    Y_UTEST(yap_mod_exp);
    Y_UTEST(yap_lcm);
    
    Y_UTEST(jive_char);
    Y_UTEST(jive_tags);
    Y_UTEST(jive_module);
    Y_UTEST(jive_token);
    Y_UTEST(jive_source);
    Y_UTEST(jive_patterns);
    Y_UTEST(jive_rework);
    Y_UTEST(jive_posix);
    Y_UTEST(jive_opt);
    Y_UTEST(jive_rx);
    Y_UTEST(jive_match);
    Y_UTEST(jive_leading);

    Y_UTEST(gfx_colors);
    Y_UTEST(gfx_hist);
    Y_UTEST(gfx_conv);
    Y_UTEST(gfx_pixels);
}
Y_UTEST_EXEC()

