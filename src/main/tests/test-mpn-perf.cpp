#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/os/rt-clock.hpp"
#include "y/sequence/vector.hpp"
#include "../../math/y/math/stat/metrics.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace
{


    static inline void __nope( mpn &, mpn & )
    {

    }


    static inline mpn __add(const mpn &l,
                            const mpn &r )
    {
        return l+r;
    }

    static inline void __sort( mpn &l, mpn &r )
    {
        if(l<r) l.xch(r);
        assert(r<=l);
    }

    static inline mpn __sub(const mpn &l,
                            const mpn &r )
    {
        return l-r;
    }

    static void inline __rgtz( mpn &, mpn &r )
    {
        if(r<=0) ++r;
    }

    static inline mpn __mul(const mpn &l,
                            const mpn &r )
    {
        return l*r;
    }

    static inline mpn __div(const mpn &l,
                            const mpn &r )
    {
        return l/r;
    }

    static inline mpn __mod(const mpn &l,
                            const mpn &r )
    {
        return l%r;
    }

    struct binary_operator
    {
        const char *name;
        mpn       (*proc)(const mpn &, const mpn &);
        void      (*prep)(mpn &,mpn &);

        inline string fileName() const
        {
            string ans = "mpn-";
            ans += name;
            ans += ".dat";
            return ans;
        }

    };


    static inline
    double test_op(const binary_operator &op,
                   const size_t           count,
                   const size_t           nbits,
                   const double           duration
                  )
    {
        vector<mpn>  lhs(count,as_capacity);
        vector<mpn>  rhs(count,as_capacity);
        vector<mpn>  ans(count,as_capacity);


        for(size_t i=count;i>0;--i)
        {
            mpn l(1+alea.lt(nbits),alea);
            mpn r(1+alea.lt(nbits),alea);
            op.prep(l,r);
            lhs.push_back_(l);
            rhs.push_back_(r);
        }
        //std::cerr << "lhs=" << lhs << std::endl;
        //std::cerr << "rhs=" << rhs << std::endl;

        rt_clock clk;
        uint64_t sum = 0;
        size_t   cycles = 0;
        while( clk( sum ) < duration )
        {
            ++cycles;
            ans.free();
            for(size_t i=count;i>0;--i)
            {
                const uint64_t mark = rt_clock::ticks();
                mpn      temp = op.proc(lhs[i],rhs[i]);
                sum += ( rt_clock::ticks() - mark );
                ans.push_back_(temp);
            }
        }

        const size_t num_ops  = count * cycles;
        const double ellapsed = clk(sum);
        const double speed    = num_ops/ellapsed;
        return speed/1e6;
    }

    static inline
    double collect(const binary_operator &op,
                   const size_t           count,
                   const size_t           nbits,
                   const double           duration,
                   const size_t           iterations,
                   double                &sig)
    {
        std::cerr << "-- measuring speed for <" << op.name << "> with #bits=" << nbits << std::endl;
        vector<double> speeds(iterations,as_capacity);
        for(size_t iter=0;iter<iterations;++iter)
        {
            const double speed = test_op(op, count, nbits, duration);
            speeds.push_back_(speed);
            std::cerr << "\tspeed = " <<  speed << " MOps" << std::endl;
        }
        const double ave = math::average_of(speeds,&sig);
        std::cerr << "<" << op.name << "> = " << ave << " +/- " << sig << " MOps" << std::endl;
        return ave;
    }



}

#include "y/string/convert.hpp"

Y_UTEST(mpn_perf)
{

    const size_t count = 1024;

    double delta      = 0.2;
    size_t iterations = 8;

    if(argc>1)
    {
        delta = string_convert::to<double>(argv[1],"delta");
    }
    if( argc>2 )
    {
        iterations = string_convert::to<size_t>(argv[2],"iterations");
    }

    const binary_operator doAdd = { "add", __add, __nope  };
    const binary_operator doSub = { "sub", __sub, __sort  };
    const binary_operator doMul = { "mul", __mul, __nope  };
    const binary_operator doDiv = { "div", __div, __rgtz  };
    const binary_operator doMod = { "mod", __mod, __rgtz  };


    const binary_operator *ops[] = { &doAdd, &doSub, &doMul, &doDiv, &doMod };
    const size_t           nop   = sizeof(ops)/sizeof(ops[0]);

    for(size_t i=0;i<nop;++i)
    {
        const string fileName = ops[i]->fileName();
        ios::ocstream::overwrite(fileName);
    }


    double sig = 0;
    double ave = 0;

    for(size_t i=0;i<nop;++i)
    {
        for(size_t nbits=32;nbits<=512;nbits <<= 1)
        {
            const binary_operator &op = *ops[i];
            ave = collect(op, count, nbits, delta, iterations, sig);
            const string fileName = op.fileName();
            ios::ocstream fp(fileName,true);
            fp("%u %g %g\n", unsigned(nbits), ave, sig);
        }
    }

    (void)ave;
}
Y_UTEST_DONE()

