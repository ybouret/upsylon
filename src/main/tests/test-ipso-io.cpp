
#include "y/ipso/field3d.hpp"
#include "y/ipso/field-io.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/cblock.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;
using namespace ipso;

namespace
{

    template <typename T>
    static inline size_t hash_of( const field<T> &f )
    {
        hashing::sha1 H;
        H.set();
        size_t n=f.size();
        while(n-->0)
        {
            H.run_type(f.entry[n]);
        }
        return H.key<size_t>();
        
    }

    template <typename T>
    static inline void fill( ipso::field1D<T> &F )
    {
        for(ipso::coord1D i=F.lower;i<=F.upper;++i)
        {
            F[i] = support::get<T>();
        }
    }

    template <typename T>
    static inline void fill( ipso::field2D<T> &F )
    {
        for(ipso::coord1D j=F.lower.y;j<=F.upper.y;++j)
        {
            for(ipso::coord1D i=F.lower.x;i<=F.upper.x;++i)
            {
                F[j][i] = support::get<T>();
            }
        }
    }



    template <typename T>
    static inline void fill( ipso::field3D<T> &F )
    {
        for(ipso::coord1D k=F.lower.z;k<=F.upper.z;++k)
        {
            for(ipso::coord1D j=F.lower.y;j<=F.upper.y;++j)
            {
                for(ipso::coord1D i=F.lower.x;i<=F.upper.x;++i)
                {
                    F[k][j][i] = support::get<T>();
                }
            }
        }
    }

    template <typename T, typename U, typename SEQUENCE>
    static inline void __io_for(field<T> &F,
                                field<U> &G,
                                const SEQUENCE &indices)
    {
        const size_t               n = indices.size();
        memory::cblock_of<uint8_t> blk( n * ( sizeof(T)+sizeof(U)) );

        const size_t f0 = hash_of(F);
        const size_t g0 = hash_of(G);
        {
            uint8_t *p =  blk.data;
            field_io::query(p,F,indices);
            field_io::query(p,G,indices);
        }
        Y_ASSERT(hash_of(F)==f0);
        Y_ASSERT(hash_of(G)==g0);

        {
            const uint8_t *p = blk.data;
            field_io::store(F,p,indices);
            field_io::store(G,p,indices);
        }
        Y_ASSERT(hash_of(F)==f0);
        Y_ASSERT(hash_of(G)==g0);
    }



    template <typename T, typename U>
    static inline void io_for(field1D<T> &F,
                              field1D<U> &G )
    {
        std::cerr << "full=" << F << std::endl;
        assert(F.same_metrics_than(G));
        std::cerr << "io for " << F.name << ", " << G.name << std::endl;

        for(size_t iter=16;iter>0;--iter)
        {
            fill<T>(F);
            fill<U>(G);

            const unit_t x1 = alea.range(F.lower,F.upper);
            const unit_t x2 = alea.range(F.lower,F.upper);
            const layout1D sub(x1,x2);
            std::cerr << "sub =" << sub << std::endl;
            Y_ASSERT(F.contains(sub));
            const size_t    len = sub.items;
            vector<coord1D> idx(len,as_capacity);

            field_io::collect(idx,F,sub);
            __io_for(F,G,idx);
        }
    }

    template <typename T, typename U>
    static inline void io_for(field2D<T> &F,
                              field2D<U> &G )
    {
        std::cerr << "full=" << F << std::endl;
        assert(F.same_metrics_than(G));
        std::cerr << "io for " << F.name << ", " << G.name << std::endl;
        for(size_t iter=16;iter>0;--iter)
        {
            fill<T>(F);
            fill<U>(G);
            const unit_t x1 = alea.range(F.lower.x,F.upper.x);
            const unit_t x2 = alea.range(F.lower.x,F.upper.x);
            const unit_t y1 = alea.range(F.lower.y,F.upper.y);
            const unit_t y2 = alea.range(F.lower.y,F.upper.y);
            const layout2D sub( coord2D(x1,y1), coord2D(x2,y2) );
            std::cerr << "sub =" << sub << std::endl;
            Y_ASSERT(F.contains(sub));
            const size_t    len = sub.items;
            vector<coord1D> idx(len,as_capacity);

            field_io::collect(idx,F,sub);
            __io_for(F,G,idx);
        }
    }

    template <typename T, typename U>
    static inline void io_for(field3D<T> &F,
                              field3D<U> &G )
    {
        std::cerr << "full=" << F << std::endl;
        assert(F.same_metrics_than(G));
        std::cerr << "io for " << F.name << ", " << G.name << std::endl;
        for(size_t iter=16;iter>0;--iter)
        {
            fill<T>(F);
            fill<U>(G);
            const unit_t x1 = alea.range(F.lower.x,F.upper.x);
            const unit_t x2 = alea.range(F.lower.x,F.upper.x);
            const unit_t y1 = alea.range(F.lower.y,F.upper.y);
            const unit_t y2 = alea.range(F.lower.y,F.upper.y);
            const unit_t z1 = alea.range(F.lower.z,F.upper.z);
            const unit_t z2 = alea.range(F.lower.z,F.upper.z);

            const layout3D sub( coord3D(x1,y1,z1), coord3D(x2,y2,z2) );
            std::cerr << "sub =" << sub << std::endl;
            Y_ASSERT(F.contains(sub));
            const size_t    len = sub.items;
            vector<coord1D> idx(len,as_capacity);

            field_io::collect(idx,F,sub);
            __io_for(F,G,idx);
        }
    }


}

Y_UTEST(ipso_io)
{
    {
        const unit_t x1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t x2 = ipso::coord1D( alea.leq(100) ) - 50;
        layout1D L1(x1,x2);

        {
            field1D<double> F("f1d",L1);
            field1D<float>  G("f1f",L1);
            io_for(F,G);
        }
        
        const unit_t y1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t y2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord2D> L2( ipso::coord2D(x1,y1), ipso::coord2D(x2,y2) );

        {
            field2D<double> F("f2d",L2);
            field2D<short>  G("f2s",L2);
            io_for(F,G);
        }

        const unit_t z1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t z2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord3D> L3( ipso::coord3D(x1,y1,z1), ipso::coord3D(x2,y2,z2) );

        {
            field3D<unit_t> F("f3u",L3);
            field3D<float>  G("f3f",L3);
            io_for(F,G);
        }

    }
}
Y_UTEST_DONE()

