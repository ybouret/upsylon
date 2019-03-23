#include "y/utest/driver.hpp"

Y_UTEST_INIT(128)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(lockable);
    Y_UTEST(swap);
    Y_UTEST(zset);
    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(bits);
    Y_UTEST(chunk);
    Y_UTEST(code);
    Y_UTEST(alloc);
    Y_UTEST(core);
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
    Y_UTEST(string);
    Y_UTEST(string_convert);
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

    Y_UTEST(parallel);
    Y_UTEST(threads);
    Y_UTEST(simd);
    Y_UTEST(server);
    Y_UTEST(servpi);

    Y_UTEST(complex);
    Y_UTEST(xbitrev);
    Y_UTEST(mpn);
    Y_UTEST(mpn_fact);
    Y_UTEST(mpz);
    Y_UTEST(mpq);
    Y_UTEST(hashing);
    Y_UTEST(hmac);
    Y_UTEST(ostreams);
    Y_UTEST(istreams);
    Y_UTEST(fft);
    Y_UTEST(sort);
    Y_UTEST(unique);
    Y_UTEST(maintain);
    Y_UTEST(associative);
    Y_UTEST(map);
    Y_UTEST(set);
    Y_UTEST(matrix);
    Y_UTEST(functor);
    Y_UTEST(vfs);
    Y_UTEST(nwsrt);
    Y_UTEST(triplet);
    Y_UTEST(points);
    Y_UTEST(tuple);
    Y_UTEST(release);
    Y_UTEST(base64);
    Y_UTEST(u64);
    Y_UTEST(spec);
    Y_UTEST(lzo);
    Y_UTEST(iobits);
    Y_UTEST(mtf);
    Y_UTEST(delta);
    Y_UTEST(asf);
    Y_UTEST(bwt);
    
    Y_UTEST(local_file);
    Y_UTEST(bin2dat);
    Y_UTEST(digest);
    Y_UTEST(locate);
    Y_UTEST(ppty);

    Y_UTEST(ipso);
    Y_UTEST(ipso_io);
    Y_UTEST(ipso_gh);
    Y_UTEST(ipso_dct);
    Y_UTEST(ipso_c2d);

    Y_UTEST(progress);
    Y_UTEST(dll);
    Y_UTEST(timings);
    Y_UTEST(ichannel);
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
    
    Y_UTEST(geom_iso2d);

    Y_UTEST(kr_bc);
    Y_UTEST(kr_sc);
    Y_UTEST(uuid);
}
Y_UTEST_EXEC()

