
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/timings.hpp"
#include "y/string.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/echostream.hpp"

using namespace upsylon;

namespace
{
    class Swaps : public object, public counted
    {
    public:
        typedef arc_ptr<Swaps> Ptr;
        vector<unsigned> I;
        vector<unsigned> J;
        virtual ~Swaps() throw() {}

        explicit Swaps(const size_t n)  : I(n,as_capacity), J(n,as_capacity) {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Swaps);
    };

    static inline void write_array(ios::ostream &fp, const accessible<unsigned> &arr)
    {
        fp("\t%u",arr[1]);
        for(size_t i=2;i<=arr.size();++i)
        {
            fp(",%u",arr[i]);
        }
        fp << '\n';
    }


}

#include "y/string/convert.hpp"

Y_UTEST(xbra_gen)
{
    unsigned pmax = 13;

    if(argc>1)
    {
        pmax = unsigned( string_convert::to<size_t>(argv[1],"pmax") );
    }

    // prepare output
    const string                header_name = "xbra.hpp";
    const string                source_name = "xbra.cpp";
    const arc_ptr<ios::ostream> output      = new ios::ocstream(ios::cstderr);
    ios::echostream             header;
    ios::echostream             source;
    {
        header.link( new ios::ocstream(header_name) );
        header.link(output);
    }
    {
        source.link( new ios::ocstream(source_name) );
        source.link(output);
    }

    // prolog
    {
        header << "//! \\file\n";
        header << "#ifndef Y_MK_XBRA_INCLUDED\n";
        header << "#define Y_MK_XBRA_INCLUDED 1\n";
        header << "#include \"y/os/platform.hpp\"\n";
        header << "namespace upsylon {\n";
        header << "\t//! eXtended Bit Reversal Algorithm\n";
        header << "\tstruct xbra {\n";
        {
            header << '\n';
            header << "\t\t//! cswap two words\n";
            header << "\t\ttemplate <typename T> static inline void cswap2(T *a, T *b) throw() {\n";
            header << "\t\t\tconst T tmp0 = a[0];\n";
            header << "\t\t\tconst T tmp1 = a[1];\n";
            header << "\t\t\ta[0] = b[0];\n";
            header << "\t\t\ta[1] = b[1];\n";
            header << "\t\t\tb[0] = tmp0;\n";
            header << "\t\t\tb[1] = tmp1;\n";
            header << "\t\t};\n";
        }

        {
            header << '\n';
            header << "//! implementing generic loop\n";
            header << "#define Y_XBRA_LOOP(CODE)   \\\n";
            header << "const size_t n = size << 1; \\\n";
            header << "size_t       j = 1;         \\\n";
            header << "for(size_t i=1; i<n; i+=2)  \\\n";
            header << "{                           \\\n";
            header << "  if(j>i)                   \\\n";
            header << "  {                         \\\n";
            header << "    CODE(i,j);              \\\n";
            header << "  }                         \\\n";
            header << "  size_t m = size;          \\\n";
            header << "  while( (m>=2) && (j>m) )  \\\n";
            header << "  {                         \\\n";
            header << "    j -=  m;                \\\n";
            header << "    m >>= 1;                \\\n";
            header << "  }                         \\\n";
            header << "  j += m;                   \\\n";
            header << "}\n";
        }

        {
            header << '\n';
            header << "//! code to run one array\n";
            header << "#define Y_XBRA_SWAP(I,J) xbra::cswap2( &arr[I], &arr[J] )\n";

            header << '\n';
            header << "\t\t//! apply to arr[1..2*size]\n";
            header << "\t\ttemplate <typename T> static inline void run_(T arr[], const size_t size) throw() {\n";
            header << "\t\t\tY_XBRA_LOOP(Y_XBRA_SWAP)\n";
            header << "\t\t}\n";
        }

        {
            header << '\n';
            header << "//! code to run two arrays\n";
            header << "#define Y_XBRA_SWAP2(I,J) const size_t _I = (I); const size_t _J =(J); xbra::cswap2( &arr[_I], &arr[_J] ); xbra::cswap2( &brr[_I], &brr[_J] )\n";

            header << '\n';
            header << "\t\t//! apply to arr[1..2*size] and brr[1..2*size] simultaneously\n";
            header << "\t\ttemplate <typename T> static inline void run_(T arr[], T brr[], const size_t size) throw() {\n";
            header << "\t\t\tY_XBRA_LOOP(Y_XBRA_SWAP2)\n";
            header << "\t\t}\n";
        }


    }

    {
        source << "#include \"xbra.hpp\"\n";
        source << "namespace upsylon {\n";
    }

    vector<Swaps::Ptr> code(pmax+1,as_capacity);

    for(unsigned p=0;p<=pmax;++p)
    {
        const unsigned size  = 1<<p;
        Swaps::Ptr     swaps  = new Swaps(size);
        const unsigned n     = size << 1;
        unsigned       j     = 1;
        code.push_back_(swaps);
        for(unsigned i=1; i<n; i+=2)
        {
            if(j>i)
            {
                //Y_XBITREV_SWAP(i,j);
                swaps->I.push_back_(i);
                swaps->J.push_back_(j);
            }
            unsigned m = size;
            while( (m>=2) && (j>m) )
            {
                j -=  m;
                m >>= 1;
            }
            j += m;
        }
        std::cerr << size << " => " << swaps->I.size() << " swaps" << std::endl;
    }


    for(unsigned p=0;p<=pmax;++p)
    {
        const unsigned size  = 1<<p;
        const Swaps   &swaps = *code[p+1];
        const unsigned count = unsigned(swaps.I.size());

        if(count>0)
        {
            header << '\n';
            header("\t\tstatic const unsigned size%u=%u;      //!< size\n", size, count);
            header("\t\tstatic const unsigned indx%u[size%u]; //!< indx\n", size, size);
            header("\t\tstatic const unsigned jndx%u[size%u]; //!< jndx\n", size, size);

            source << '\n';
            source("\tconst unsigned xbra::indx%u[size%u]={\n",size,size);
            write_array(source,swaps.I);
            source("\t};\n");
            source("\tconst unsigned xbra::jndx%u[size%u]={\n",size,size);
            write_array(source,swaps.J);
            source("\t};\n");
        }
        else
        {

        }
    }

    {
        header << '\n';
        header << "\t\t//! dispatching calls for one array\n";
        header << "\t\ttemplate <typename T> static inline void run(T arr[], const size_t size) throw() {\n";
        header << "\t\t\tswitch(size) {\n";
        header << "\t\t\t\tcase 0: break;\n";
        for(unsigned p=0;p<=pmax;++p)
        {
            const unsigned size  = 1<<p;
            const Swaps   &swaps = *code[p+1];
            const unsigned count = unsigned(swaps.I.size());
            header("\t\t\t\tcase %u: ",size);
            if(count>0)
            {
                header("for(size_t i=0;i<xbra::size%u;++i) Y_XBRA_SWAP(indx%u[i],jndx%u[i]); ",size,size,size);
            }
            header("break;\n");
        }
        header << "\t\t\t\tdefault: run_(arr,size);\n";
        header << "\t\t\t}\n";
        header << "\t\t}\n";
    }

    {
        header << '\n';
        header << "\t\t//! dispatching calls for two arrays\n";
        header << "\t\ttemplate <typename T> static inline void run(T arr[], T brr[], const size_t size) throw() {\n";
        header << "\t\t\tswitch(size) {\n";
        header << "\t\t\t\tcase 0: break;\n";
        for(unsigned p=0;p<=pmax;++p)
        {
            const unsigned size  = 1<<p;
            const Swaps   &swaps = *code[p+1];
            const unsigned count = unsigned(swaps.I.size());
            header("\t\t\t\tcase %u: ",size);
            if(count>0)
            {
                header("for(size_t i=0;i<xbra::size%u;++i) { Y_XBRA_SWAP2(indx%u[i],jndx%u[i]); }",size,size,size);
            }
            header("break;\n");
        }
        header << "\t\t\t\tdefault: run_(arr,brr,size);\n";
        header << "\t\t\t}\n";
        header << "\t\t}\n";
    }


    {
        // end of xbra
        header << "\t};\n";
    }

    {
        // dedicated
        header << '\n';
        header << "\t//! generic call\n";
        header << "\ttemplate <typename T, const size_t SIZE> struct xbra_for {\n";
        header << "\t\tstatic inline void run(T arr[])          throw() { xbra::run_(arr,SIZE);     } //!< one array\n";
        header << "\t\tstatic inline void run(T arr[], T brr[]) throw() { xbra::run_(arr,brr,SIZE); } //!< two arrays\n";
        header << "\t};\n";
    }

    {
        // 0 items
        header << '\n';
        header << "\t//! no items, safe-guard\n";
        header << "\ttemplate <typename T> struct xbra_for<T,0> {\n";
        header << "\t\tstatic inline void run(T [])       throw() { } //!< one array\n";
        header << "\t\tstatic inline void run(T [], T []) throw() { } //!< two arrays\n";
        header << "\t};\n";
    }

    for(unsigned p=0;p<=pmax;++p)
    {
        const unsigned size  = 1<<p;
        const Swaps   &swaps = *code[p+1];
        const unsigned count = unsigned(swaps.I.size());

        header << '\n';
        header("\t//! for SIZE=%u\n",size);
        header("\ttemplate <typename T> struct xbra_for<T,%u> {\n",size);

        if(count>0)
        {
            header << "\t\t//! one array\n";
            header << "\t\tstatic inline void run(T arr[])          throw() {\n";
            header("\t\t\tfor(size_t i=0;i<xbra::size%u;++i) Y_XBRA_SWAP(xbra::indx%u[i],xbra::jndx%u[i]);\n",size,size,size);
            header << "\t\t}\n";
            header << "\t\t//! two arrays\n";
            header << "\t\tstatic inline void run(T arr[], T brr[]) throw() {\n";
            header("\t\t\tfor(size_t i=0;i<xbra::size%u;++i) { Y_XBRA_SWAP2(xbra::indx%u[i],xbra::jndx%u[i]); }\n",size,size,size);
            header << "\t\t}\n";
        }
        else
        {
            header << "\t\tstatic inline void run(T [])       throw() { } //!< one array\n";
            header << "\t\tstatic inline void run(T [], T []) throw() { } //!< two arrays\n";
        }


        header << "\t};\n";

    }


    // epilog
    {
        header << "}\n";
        header << "#endif\n";
    }
    {
        source << "}\n";
    }

}
Y_UTEST_DONE()

#include "y/fft/xbra.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/type/spec.hpp"

namespace upsylon {


    template<typename T> static inline
    bool identical(const T *arr, const T *brr, const size_t SIZE) throw()
    {
        return 0 == memcmp(arr,brr,SIZE*2);
    }

    template <typename T,size_t SIZE>
    void do_test_xbra(const double D)
    {
        std::cerr << "SIZE=" << std::setw(8) << SIZE << " : ";

        typedef complex<T> cplx;
        vector<cplx> a0(SIZE,0);
        vector<cplx> a1(SIZE,0);
        vector<cplx> a2(SIZE,0);
        vector<cplx> a3(SIZE,0);
        vector<cplx> a4(SIZE,0);
        vector<cplx> a5(SIZE,0);
        vector<cplx> a6(SIZE,0);
        vector<cplx> a7(SIZE,0);
        vector<cplx> a8(SIZE,0);

        support::fill1D(a0);
        for(size_t i=SIZE;i>0;--i)
        {
            a1[i] = a2[i] = a3[i] = a4[i] = a5[i] = a6[i] = a7[i] = a8[i] = a0[i];
        }

        xbra::run_( &(a0[1].re) - 1,SIZE); // raw
        xbra::run(  &(a1[1].re) - 1,SIZE); // dispatched

        xbra::run_(&(a2[1].re)-1,&(a3[1].re)-1, SIZE); // raw2
        xbra::run( &(a4[1].re)-1,&(a5[1].re)-1, SIZE); // dispatched2

        xbra_for<T,SIZE>::run( &(a6[1].re)-1 );                //! specialized
        xbra_for<T,SIZE>::run( &(a7[1].re)-1, &(a8[1].re)-1);  //! specialized 2


        Y_ASSERT(identical(*a0,*a1,SIZE));
        Y_ASSERT(identical(*a0,*a2,SIZE));
        Y_ASSERT(identical(*a0,*a3,SIZE));
        Y_ASSERT(identical(*a0,*a4,SIZE));
        Y_ASSERT(identical(*a0,*a5,SIZE));
        Y_ASSERT(identical(*a0,*a6,SIZE));
        Y_ASSERT(identical(*a0,*a7,SIZE));
        Y_ASSERT(identical(*a0,*a8,SIZE));

        double speed0 = 0;
        Y_TIMINGS(speed0,D, xbra::run_( &(a0[1].re) - 1,SIZE) );

        double speedN = 0;
        Y_TIMINGS(speedN,D, (xbra_for<T,SIZE>::run( &(a1[1].re)-1 )) );
        const double speed_up = speedN/speed0;
        std::cerr << "x" << speed_up << std::endl;

    }

    template <typename T>
    void do_test_xbras(const double D)
    {
        std::cerr << "with " << type_name_of<T>() << std::endl;
        do_test_xbra<T,1>(D);
        do_test_xbra<T,2>(D);
        do_test_xbra<T,4>(D);
        do_test_xbra<T,8>(D);
        do_test_xbra<T,16>(D);
        do_test_xbra<T,32>(D);
        do_test_xbra<T,64>(D);
        do_test_xbra<T,128>(D);
        do_test_xbra<T,256>(D);
        do_test_xbra<T,512>(D);
        do_test_xbra<T,1024>(D);
        do_test_xbra<T,2048>(D);
        do_test_xbra<T,4096>(D);
        do_test_xbra<T,8192>(D);
        do_test_xbra<T,16384>(D);
        do_test_xbra<T,32768>(D);

    }

}

Y_UTEST(xbra)
{
    double D = 0.1;
    do_test_xbras<float>(D);
    do_test_xbras<double>(D);

}
Y_UTEST_DONE()
