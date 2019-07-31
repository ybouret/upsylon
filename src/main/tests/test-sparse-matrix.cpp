#include "y/sparse/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/map.hpp"
#include "y/utest/timings.hpp"
#include "y/hashing/sfh.hpp"
#include "y/hashing/adler32.hpp"
#include "y/hashing/crc32.hpp"
#include "y/hashing/bjh32.hpp"

#include "y/sort/classes.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    typedef vector<size_t> key_db;

    static inline
    void perform(const array<sparse::matrix_key> &mk,
                 hashing::function               &h,
                 key_db                          &db)
    {
        const size_t nk = mk.size();
        db.ensure(nk);
        std::cerr << "| checking with " << h.name() << " | #keys=" << nk << std::endl;
        db.free();
        double spd = 0;
        Y_TIMINGS(spd,0.5,
                  for(size_t i=nk;i>0;--i)
                  {
                      (void) h.key<size_t>( &mk[i], sizeof(sparse::matrix_key) );
                  }
                  );
        spd *= nk;
        spd /= 1e6;
        std::cerr << "| |_speed   = " << spd << " M/s" << std::endl;

        // std::cerr << "| collecting..." << std::endl;
        for(size_t i=nk;i>0;--i)
        {
            const size_t k = h.key<size_t>( &mk[i], sizeof(sparse::matrix_key) );
            db.push_back_( k );
        }
        //std::cerr << "| counting classes" << std::endl;
        const size_t ncls = sort_classes(db);
        //std::cerr << "| |_#class   = " << ncls << std::endl;
        std::cerr << "| |_collide = " << nk-ncls << std::endl;


        std::cerr << "|" << std::endl;
    }


    template <typename T>
    void fill_sparse()
    {
        sparse_matrix<T> sm( 1 + alea.leq(10), 1 + alea.leq(10) );
        std::cerr << sm.rows << "x" << sm.cols << std::endl;
        for(size_t j=sm.count/2;j>0;--j)
        {
            const T tmp = support::get<T>();
            sm( alea.range<size_t>(1,sm.rows), alea.range<size_t>(1,sm.cols) ) = tmp;
        }
        std::cerr << "sm.core.size()=" << sm.core.size() << "/" << sm.count << std::endl;

        std::cerr << "raw:";
        for( typename sparse_matrix<T>::iterator i=sm.begin(); i!=sm.end();++i)
        {
            std::cerr << ' ' << **i;
        }
        std::cerr << std::endl;
        sm.update();
        std::cerr << "srt:";
        for( typename sparse_matrix<T>::iterator i=sm.begin(); i!=sm.end();++i)
        {
            std::cerr << ' ' << **i;
        }
        std::cerr << std::endl;


        std::cerr << "sm=" << sm << std::endl;
    }
}

#include "y/string/convert.hpp"

Y_UTEST(sparse_matrix)
{

    size_t n = 10;
    if(argc>1) n = string_convert::to<size_t>( argv[1], "n" );

    std::cerr << "| sizeof(sparse::matrix_key)=" << sizeof(sparse::matrix_key) << std::endl;
    { sparse::matrix_info m(0,0); }
    { sparse::matrix_info m(1,1); }

    if(false)
    {
        std::cerr << "| generating keys" << std::endl;
        vector<sparse::matrix_key> mk(square_of(n+1),as_capacity);
        for(size_t r=0;r<=n;++r)
        {
            for(size_t c=0;c<=n;++c)
            {
                const sparse::matrix_key k(r,c);
                mk.push_back(k);
            }
        }
        std::cerr << "|_#keys=" << mk.size() << std::endl;

        std::cerr << "| checking values" << std::endl;
        for(size_t i=1;i<n;++i)
        {
            Y_ASSERT(mk[i]==mk[i]);
            for(size_t j=i+1;j<=n;++j)
            {
                Y_ASSERT(mk[i]!=mk[j]);
            }
        }

        hashing::fnv     h_fnv;
        hashing::sha1    h_sha1;
        hashing::sfh     h_sfh;
        hashing::adler32 h_adler32;
        hashing::crc32   h_crc32;
        hashing::bjh32   h_bjh32;

        hashing::function *hfn[] =
        {
            & h_fnv,
            & h_sha1,
            & h_sfh,
            & h_adler32,
            & h_crc32,
            & h_bjh32
        };

        key_db db;

        for(size_t i=0;i<sizeof(hfn)/sizeof(hfn[0]);++i)
        {
            perform(mk,*hfn[i],db);
        }

        std::cerr << "|_done" << std::endl;
    }

    fill_sparse<double>();
    fill_sparse<int>();
    fill_sparse<string>();
    fill_sparse<mpn>();
    fill_sparse<mpz>();
    fill_sparse<mpq>();

    std::cerr << std::endl;
    {
        sparse_matrix<double> M(2,2);
        std::cerr << "M=" << M << std::endl;
        M(1,1) = 11;
        M(1,2) = 12;
        M(2,1) = 21;
        M(2,2) = 22;
        std::cerr << "M=" << M << std::endl;

    }


}
Y_UTEST_DONE()

