#include "y/sort/index.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/network/all.hpp"
#include "y/type/rtti.hpp"

#include "support.hpp"

using namespace upsylon;

namespace
{
    template <const size_t N>
    struct newtork_index
    {
        typedef network_sort<N> nw;
        static  const size_t    nt = sizeof(nw::I)/sizeof(nw::I[0]);

        template <typename T, typename COMPARE> static inline
        void make( size_t *indx, const T *data, COMPARE &compare)
        {
            for(size_t k=0;k<N;++k) indx[k] = k;

            for(size_t k=0;k<nt;++k)
            {
                size_t &i = indx[nw::I[k]];
                size_t &j = indx[nw::J[k]];
                if( compare(data[j],data[i]) < 0 )
                {
                    bswap(i,j);
                }
            }
        }

    };


    template <typename T,const size_t N> static inline
    void doIndxSmall()
    {
        std::cerr << "indx<" << rtti::name_of<T>() << "," << N << ">" << std::endl;
        for(size_t loop=0;loop<2;++loop)
        {
            vector<T>      data(N,as_capacity);
            for(size_t i=N;i>0;--i)
            {
                const T tmp = support::get<T>();
                data.push_back_(tmp);
            }
            vector<size_t> indx(N,0);
            const T *arr = *data;
            size_t  *idx = *indx;
            newtork_index<N>::make(idx,arr,comparison::increasing<T>);


            std::cerr << "data: " << data << std::endl;
            std::cerr << "indx: " << indx << std::endl;
            for(size_t i=0;i<N-1;++i)
            {
                //std::cerr << "i=" << i << " idx: " << idx[i] << "," << idx[i+1] << std::endl;
                Y_ASSERT(arr[ idx[i] ]<=arr[ idx[i+1] ]);
            }
            Y_CHECK( say("indexed") );
        }

        std::cerr << std::endl;
    }

    template <typename T> static inline
    void doIndxAll()
    {
        doIndxSmall<T,2>();
        doIndxSmall<T,3>();
        doIndxSmall<T,4>();
        doIndxSmall<T,5>();
        doIndxSmall<T,6>();
        doIndxSmall<T,7>();
        doIndxSmall<T,8>();
    }


    template <typename T>
    static inline void doIndx()
    {
        std::cerr << "indexing<" << rtti::name_of<T>() << ">" << std::endl;
        vector<T>      data;
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            const T tmp = support::get<T>();
            data.push_back(tmp);
        }

        vector<size_t> indx(data.size(),0);
        indexing::make(indx,comparison::increasing<T>,data);
        vector<T>      outp(data);
        indexing::rank(outp,data,indx);
        std::cerr << "data: " << data << std::endl;
        std::cerr << "indx: " << indx << std::endl;
        std::cerr << "outp: " << data << std::endl;

        for(size_t i=1;i<outp.size();++i)
        {
            Y_ASSERT(outp[i]<=outp[i+1]);
        }
        Y_CHECK( say("sorted...") );
        std::cerr << std::endl;
    }
}

Y_UTEST(index)
{
    doIndx<uint16_t>();
    doIndx<string>();
    doIndx<apq>();

    doIndxAll<uint16_t>();
    doIndxAll<apq>();


}
Y_UTEST_DONE()

