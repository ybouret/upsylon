#include "y/fft/xbitrev.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test(double D)
    {
        std::cerr << "sizeof(real)=" << sizeof(T) << std::endl;
        typedef complex<T> cplx;
        hashing::sha1 H;
        for(size_t n=1;n<=16384;n*=2)
        {
            std::cerr << "n=" << std::setw(5) << n << " : "; std::cerr.flush();
            vector<cplx> v0(n);
            vector<cplx> v(n);
            for(size_t iter=0;iter<10;++iter)
            {
                for(size_t i=1;i<=n;++i)
                {
                    v0[i].re = alea.to<T>();
                    v0[i].im = alea.to<T>();
                }
                for(size_t i=1;i<=n;++i)
                {
                    v[i] = v0[i];
                }
                xbitrev::run_safe( &v[1].re-1,v.size());
                H.set();
                H(v);
                const uint64_t k1 = H.key<uint64_t>();
                for(size_t i=1;i<=n;++i)
                {
                    v[i] = v0[i];
                }
                xbitrev::run( &v[1].re-1,v.size());
                H.set();
                H(v);
                const uint64_t k2 = H.key<uint64_t>();
                Y_ASSERT(k1==k2);
            }
            double speed1 = 0;
            Y_TIMINGS(speed1,D, xbitrev::run_safe( &v[1].re-1,v.size()));
            //std::cerr << "\tspeed1 = " << speed1 << std::endl;
            double speed2 = 0;
            Y_TIMINGS(speed2,D, xbitrev::run( &v[1].re-1,v.size()));
            //std::cerr << "\tspeed2 = " << speed2 << std::endl;
            std::cerr << "speedup: " << speed2/speed1 << std::endl;
        }
    }



    static inline void save_indices(ios::ostream &src, const array<size_t> &indices)
    {
        bool ret=false;
        for(size_t i=1;i<=indices.size();++i)
        {
            src("0x%x",(unsigned)indices[i]);
            if(i<indices.size()) src << ',';
            if(0==(i%16))
            {
                src << '\n';
                ret = true;
            }
            else
            {
                ret=false;
            }
        }
        if(!ret) src << '\n';
    }

    static inline void generate(const size_t   size,
                                ios::ostream   &hdr,
                                ios::ostream   &imp,
                                ios::ostream   &src,
                                ios::ostream   &im2,
                                ios::ostream   &inc)
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
            imp("case %5u:", (unsigned)size);
            im2("case %5u:", (unsigned)size);
            inc("template <typename T> struct xbitrev_for<T,%u> {\n",(unsigned)size);

            if(nx>0)
            {
                hdr("static const size_t indx%u[%u];//!< first  index\n",(unsigned)size,unsigned(nx));
                hdr("static const size_t jndx%u[%u];//!< second index\n",(unsigned)size,unsigned(nx));
                hdr("\n");

                src("const size_t xbitrev::indx%u[%u] ={\n",(unsigned)size,unsigned(nx));
                save_indices(src,idx);
                src("};\n");

                src("const size_t xbitrev::jndx%u[%u] ={\n",(unsigned)size,unsigned(nx));
                save_indices(src,jdx);
                src("};\n\n");

                imp(" for(size_t i=0;i<%5u;++i) Y_XBITREV_SWAP(indx%u[i],jndx%u[i]); ", unsigned(nx), unsigned(size), unsigned(size) );
                im2(" for(size_t i=0;i<%5u;++i) Y_XBITREV_SWAP2(indx%u[i],jndx%u[i]) ", unsigned(nx), unsigned(size), unsigned(size) );

                inc("\tstatic inline void run(T arr[]) throw() {\n");
                inc("\tassert(arr);\n");
                inc("\tfor(size_t i=0;i<%5u;++i) Y_XBITREV_SWAP(xbitrev::indx%u[i],xbitrev::jndx%u[i]);\n", unsigned(nx), unsigned(size), unsigned(size) );
                inc("\t};\n");
            }
            else
            {
                inc("\tstatic inline void run(T []) throw() {}\n");
            }
            inc("};\n\n");
            imp("break;\n");
            im2("break;\n");
        }
    }


    template <typename T,size_t n>
    static inline void do_test2(double D)
    {


        typedef complex<T> cplx;
        hashing::sha1 H;

        std::cerr << "n=" << std::setw(5) << n << " : "; std::cerr.flush();
        vector<cplx> v0(n);
        vector<cplx> v(n);
        for(size_t iter=0;iter<10;++iter)
        {
            for(size_t i=1;i<=n;++i)
            {
                v0[i].re = alea.to<T>();
                v0[i].im = alea.to<T>();
            }
            for(size_t i=1;i<=n;++i)
            {
                v[i] = v0[i];
            }
            xbitrev::run_safe(&v[1].re-1,n);
            H.set();
            H(v);
            const uint64_t k1 = H.key<uint64_t>();
            for(size_t i=1;i<=n;++i)
            {
                v[i] = v0[i];
            }
            xbitrev_for<T,n>::run( &v[1].re-1 );
            H.set();
            H(v);
            const uint64_t k2 = H.key<uint64_t>();
            Y_ASSERT(k1==k2);
        }
        //std::cerr << std::endl; return;

        double speed1 = 0;
        Y_TIMINGS(speed1,D, xbitrev::run_safe( &v[1].re-1,n) );
        double speed2 = 0;
        Y_TIMINGS(speed2,D, (xbitrev_for<T,n>::run( &v[1].re-1)) );
        std::cerr << "speedup: " << speed2/speed1 << std::endl;
    }

    template <typename T>
    static inline void do_tests(double D)
    {
        std::cerr << "templated sizeof(real)=" << sizeof(T) << std::endl;
        do_test2<T,1>(D);
        do_test2<T,2>(D);
        do_test2<T,4>(D);
        do_test2<T,8>(D);
        do_test2<T,16>(D);
        do_test2<T,32>(D);
        do_test2<T,64>(D);
        do_test2<T,128>(D);
        do_test2<T,256>(D);
        do_test2<T,512>(D);
        do_test2<T,1024>(D);
        do_test2<T,2048>(D);
        do_test2<T,4096>(D);
        do_test2<T,8192>(D);
        do_test2<T,16384>(D);

    }

}

Y_UTEST(xbitrev)
{
    double D = 0.1;
    if(argc>1) D = atof(argv[1]);
    do_test<float >(D);
    do_test<double>(D);

    do_tests<float >(D);
    do_tests<double>(D);


    if(argc>2&& 0==strcmp(argv[2],"true"))
    {
        ios::ocstream hdr("xbitrev-decl.hxx");
        ios::ocstream imp("xbitrev-impl.hxx");
        ios::ocstream im2("xbitrev-imp2.hxx");
        ios::ocstream src("xbitrev-data.cxx");
        ios::ocstream inc("xbitrev-for.hxx");

        generate(0,hdr,imp,src,im2,inc);
        for(size_t size=1;size<=8192;size<<=1)
        {
            generate(size,hdr,imp,src,im2,inc);
        }
    }

}
Y_UTEST_DONE()

