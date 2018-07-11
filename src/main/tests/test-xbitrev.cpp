#include "y/code/xbitrev.hpp"
#include "y/utest/run.hpp"
#include "y/memory/dyadic.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/cfile.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        typedef complex<T> cplx;
        for(size_t n=1;n<=4096;n*=2)
        {
            vector<cplx,memory::dyadic> v(n);
            xbitrev::run_safe( &v[1].re-1,v.size());
        }
    }

    static inline void save_indices(ios::cfile &src, const array<size_t> &indices)
    {
        for(size_t i=1;i<=indices.size();++i)
        {
            fprintf(*src,"%u",(unsigned)indices[i]);
            if(i<indices.size()) fprintf(*src,",");
        }
        fprintf(*src,"\n");
    }

    static inline void generate(const size_t  size,
                                ios::cfile   &hdr,
                                ios::cfile   &imp,
                                ios::cfile   &src)
    {

        {
            std::cerr << "size=" << size << std::endl;
            vector<size_t> idx(size,as_capacity);
            vector<size_t> jdx(size,as_capacity);

            const size_t n = size << 1;
            size_t       j = 1;
            for( size_t i=1; i<n; i+=2)
            {
                if(j>i)
                {
                    //core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
                    //std::cerr << "(" << i << "," << j << ")" << std::endl;
                    idx.push_back(i);
                    jdx.push_back(j);
                }
                size_t m = size;
                while( (m>=2) && (j>m) )
                {
                    j -=  m;
                    m >>= 1;
                }
                j += m;
            }
            const size_t nx = idx.size();
            assert(jdx.size()==idx.size());
            std::cerr << "\tnx=" << nx << std::endl;
            fprintf(*imp,"case %u:", (unsigned)size);
            if(nx>0)
            {
                fprintf(*hdr,"static const size_t indx%u[%u];//!< first  index\n",(unsigned)size,unsigned(nx));
                fprintf(*hdr,"static const size_t jndx%u[%u];//!< second index\n",(unsigned)size,unsigned(nx));
                fprintf(*hdr,"\n");

                fprintf(*src,"const size_t xbitrev::indx%u[%u] ={\n",(unsigned)size,unsigned(nx));
                save_indices(src,idx);
                fprintf(*src, "};\n");

                fprintf(*src,"const size_t xbitrev::jndx%u[%u] ={\n",(unsigned)size,unsigned(nx));
                save_indices(src,jdx);
                fprintf(*src, "};\n\n");

                fprintf(*imp," for(size_t i=0;i<%u;++i) Y_XBITREV_SWAP(indx%u[i],jndx%u[i]); ", unsigned(nx), unsigned(size), unsigned(size) );
            }
            fprintf(*imp,"break;\n");
        }
    }
}

Y_UTEST(xbitrev)
{
    do_test<float>();
    do_test<double>();

    if(argc>1&& 0==strcmp(argv[1],"true"))
    {
        ios::cfile hdr("xbitrev-decl.hxx",ios::cfile::open_write);
        ios::cfile imp("xbitrev-impl.hxx",ios::cfile::open_write);
        ios::cfile src("xbitrev-data.cxx",ios::cfile::open_write);

        generate(0,hdr,imp,src);
        for(size_t size=1;size<=4096;size*=2)
        {
            generate(size,hdr,imp,src);
        }
    }

}
Y_UTEST_DONE()

