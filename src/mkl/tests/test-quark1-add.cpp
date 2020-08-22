
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/type/spec.hpp"
#include "y/code/utils.hpp"
#include "y/type/binary.hpp"

using namespace upsylon;
using namespace mkl;

namespace {
    



    template <typename TARGET, typename LHS, typename RHS>
    static inline
    void check1D(const char   *name,
                 const TARGET &seq, const TARGET &par,
                 const LHS    &lhs, const RHS    &rhs)
    {
        const size_t n = seq.size();
        for(size_t i=1;i<=seq.size();++i)
        {
            if(mod2_of(seq[i]-par[i])>0)
            {
                std::cerr << name << "@" << i << "/" << n << ":" << std::endl;
                std::cerr << "lhs=" << lhs[i] << " (" << binary(lhs[i]) << ") rhs=" << rhs[i] << " (" << binary(rhs[i]) << ")" << std::endl;
                std::cerr << "seq=" << seq[i] << " (" << binary(seq[i]) << ") par=" << par[i] << " (" << binary(par[i]) << ")" << std::endl;
                throw exception("error in %s", name);
            }

        }

    }

    template <typename T,
    typename U,
    typename V>
    void doOPS(concurrent::for_each *loop)
    {
        std::cerr << "<OPS " << type_name_of<T>() << "," << type_name_of<U>()<< "," << type_name_of<V>() << ">" << std::endl;
        const T zt = 0;
        const U zu = 0;
        const V zv = 0;
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t n = 2000 + alea.leq(2000);

            vector<T>    seq(n,zt);
            vector<T>    par(n,zt);


            vector<U>    u(n,zu); support::fill1D(u);
            vector<V>    v(n,zv); support::fill1D(v);

            std::cerr << "#" << n << "\t";


            // add
            std::cerr << "[add]";
            support::reset1D(seq);
            support::reset1D(par);
            quark::add(seq,u,v);
            if(loop)
            {
                quark::add(par,u,v,*loop);
                check1D("add", seq, par, u, v);
            }

            // sub
            std::cerr << "[sub]";
            support::reset1D(seq);
            support::reset1D(par);
            quark::sub(seq,u,v);
            if(loop)
            {
                quark::sub(par,u,v,*loop);
                check1D("sub", seq, par, u, v);
            }

            // subp
            std::cerr << "[subp]";
            support::reset1D(seq);
            support::reset1D(par);
            quark::subp(seq,u,v);
            if(loop)
            {
                quark::subp(par,u,v,*loop);
                check1D("subp", seq, par, u, v);
            }



            // muladd
            std::cerr << "[muladd1]";
            support::reset1D(seq);
            support::reset1D(par);
            {
                const T x = support::get<T>();
                quark::muladd(seq,u,x,v);
                if(loop)
                {
                    quark::muladd(par,u,x,v,*loop);
                    std::cerr << "x=" << x << "(" << binary(x) << ")";
                    check1D("muladd_V1", seq, par, u, v);
                }
            }

            // muladd
            std::cerr << "[muladd2]";
            support::reset1D(seq);
            support::reset1D(par);
            {
                const T x = support::get<T>();
                quark::muladd(seq,x,v);
                if(loop)
                {
                    quark::muladd(par,x,v,*loop);
                    std::cerr << "x=" << x << "(" << binary(x) << ")";
                    check1D("muladd_V2", seq, par, u, v);
                }
            }

            std::cerr << std::endl;



#if 0
            vector<T>    t(n,zt);
            vector<T>    tb(n,zt);
            vector<U>    u(n,zu);
            vector<V>    v(n,zv);

            __PROC(add); __MPROC(muladd);
            __PROC(sub); __MPROC(mulsub);
            __PROC(subp);
#endif

        }
        std::cerr << "<OPS/>" << std::endl;

    }

}

Y_UTEST(quark1_add)
{
    concurrent::simd loop;
    std::cerr << "With #Threads=" << loop.number() << std::endl;
    doOPS<double,double,double>( &loop );
    doOPS<float,float,float>( &loop );
    doOPS<float,int,float>( &loop );
    doOPS<apz,int,apz>(NULL);

}
Y_UTEST_DONE()

