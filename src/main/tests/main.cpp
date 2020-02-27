#include "y/utest/driver.hpp"

Y_UTEST_INIT(256)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(binary);
    Y_UTEST(cast);
    Y_UTEST(chkbnd);
    Y_UTEST(i2i);
    Y_UTEST(standard);
    Y_UTEST(lockable);
    Y_UTEST(bswap);
    Y_UTEST(bmove);
    Y_UTEST(zset);
    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(bits);
    Y_UTEST(chunk);
    Y_UTEST(code);
    Y_UTEST(alloc);

    Y_UTEST(core_pool);
    Y_UTEST(core_list);
    Y_UTEST(core_clist);
    Y_UTEST(core_stack);


    Y_UTEST(isqrt);
    Y_UTEST(arena);
    Y_UTEST(blocks);
    Y_UTEST(slab);
    Y_UTEST(primality);
    Y_UTEST(object0);
    Y_UTEST(objectY);
    Y_UTEST(slice);
    Y_UTEST(carver);
    Y_UTEST(pooled);
    Y_UTEST(buffers);
    Y_UTEST(xslot);
    Y_UTEST(static_slots);
    Y_UTEST(ints_display);
    Y_UTEST(i2r);
    
    Y_UTEST(mloops);
    Y_UTEST(comb);
    Y_UTEST(perm);
    Y_UTEST(anagram);
    Y_UTEST(dancing);
    Y_UTEST(scrabble);

    Y_UTEST(embed);
    Y_UTEST(hr_ints);

    Y_UTEST(string);
    Y_UTEST(string_convert);
    Y_UTEST(string_defrag);
    Y_UTEST(esma);


    Y_UTEST(tmpname);
    Y_UTEST(tokenizer);
    Y_UTEST(nugget);
    Y_UTEST(nuggets);
    Y_UTEST(vein);
    Y_UTEST(traits);
    Y_UTEST(ptr);
    Y_UTEST(slots);
    Y_UTEST(env);
    Y_UTEST(sequence);
    Y_UTEST(pipe);
    Y_UTEST(arrays);
    Y_UTEST(dyadic);
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

    Y_UTEST(parallel);
    Y_UTEST(threads);
    Y_UTEST(simd);
    Y_UTEST(server);
    Y_UTEST(servpi);

    Y_UTEST(complex);
    Y_UTEST(xbitrev);
    Y_UTEST(mpn);
    Y_UTEST(mpn_perf);
    Y_UTEST(mpn_fact);
    Y_UTEST(mpz);
    Y_UTEST(mpq);
    Y_UTEST(mprm);
    Y_UTEST(mprm1000);
    Y_UTEST(prime_factor);
    Y_UTEST(prime_ratio);
    Y_UTEST(ilog);
    Y_UTEST(mp_io);
    Y_UTEST(rsa);
    Y_UTEST(rsa_keys);
    Y_UTEST(rsa_io);
    

    Y_UTEST(hashing);
    Y_UTEST(mperf);
    Y_UTEST(pearson);
    Y_UTEST(hmac);
    Y_UTEST(ostreams);
    Y_UTEST(istreams);
    Y_UTEST(mstreams);
    Y_UTEST(upack);
    Y_UTEST(ios_plugin);
    Y_UTEST(cmd);
    Y_UTEST(md);

    Y_UTEST(fft);
    Y_UTEST(sort);
    Y_UTEST(unique);
    Y_UTEST(classes);
    Y_UTEST(maintain);
    Y_UTEST(associative);
    Y_UTEST(map);
    Y_UTEST(set);
    Y_UTEST(btree);
    Y_UTEST(assoc_perf);
    Y_UTEST(strings);
    Y_UTEST(query);

    Y_UTEST(matrix);
    Y_UTEST(functor);
    Y_UTEST(vfs);
    Y_UTEST(nwsrt);
    Y_UTEST(triplet);
    Y_UTEST(points);
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
    Y_UTEST(rle);
    Y_UTEST(pack);
    Y_UTEST(bytewise);

    Y_UTEST(local_file);
    Y_UTEST(disk_file);
    Y_UTEST(disk_std);
    Y_UTEST(serializer);
    Y_UTEST(serialize_tools);

    Y_UTEST(bin2dat);
    Y_UTEST(digest);
    Y_UTEST(locate);
    Y_UTEST(ppty);

    Y_UTEST(progress);
    Y_UTEST(dll);
    Y_UTEST(timings);
    Y_UTEST(sobol);


    Y_UTEST(token);
    Y_UTEST(source);
    Y_UTEST(pattern);
    Y_UTEST(common);
    Y_UTEST(posix);
    Y_UTEST(regex);
    Y_UTEST(matching);
    Y_UTEST(scanner);
    Y_UTEST(lex);
    Y_UTEST(token_convert);
    Y_UTEST(sed);

    Y_UTEST(contour2d);
    Y_UTEST(contour3d);

    Y_UTEST(kr_bc);
    Y_UTEST(kr_sc);
    Y_UTEST(uuid);

    Y_UTEST(sparse_array);
    Y_UTEST(sparse_matrix);

    Y_UTEST(oxide);
    Y_UTEST(oxide_fields);
    Y_UTEST(oxide_io);
    Y_UTEST(oxide_dct);
    Y_UTEST(oxide_split);
    Y_UTEST(oxide_max_splits);
    Y_UTEST(oxide_ranks);
    Y_UTEST(oxide_layouts);
    Y_UTEST(oxide_wksp);
    Y_UTEST(oxide_allw);
    Y_UTEST(oxide_mpi);
    Y_UTEST(oxide_vtk);
    Y_UTEST(oxide_par);
    Y_UTEST(oxide_grids);

    Y_UTEST(zfi);
    Y_UTEST(closest);
}
Y_UTEST_EXEC()

