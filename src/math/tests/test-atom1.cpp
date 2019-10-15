#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "support.hpp"
#include <cstring>
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    struct LD
    {

        template <typename ARRAY> static inline
        void test( ARRAY &arr, concurrent::for_each &loop)
        {
            typedef typename ARRAY::mutable_type type;
            const type   value = support::get<type>();
            support::fill1D(arr);
            Y_SUPPORT_TICKS(fullTicks,atom::ld(arr,value));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,value) <= 0 );
            support::fill1D(arr);
            Y_SUPPORT_TICKS(loopTicks,atom::ld(arr,value,loop));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,value) <= 0 );
            rt_clock clk;
            std::cerr << clk.speedup(fullTicks,loopTicks) << '/';
        }

        template <typename T> static inline
        void testAll(concurrent::for_each &loop)
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048};
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter LD for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";
                { vector<T,memory::global> gv(nn); test(gv,loop); }
                { vector<T,memory::pooled> pv(nn); test(pv,loop); }
                { list<T>                  gl(nn); test(gl,loop); }
                if(2==nn)
                {
                    point2d<T> p; test(p,loop);
                }
                if(3==nn)
                {
                    point3d<T> p; test(p,loop);
                }
                std::cerr << std::endl;

            }
            std::cerr << "[Leave LD for <" << tid << ">]" << std::endl;

        }
    };

    struct NEG
    {

        template <typename ARRAY> static inline
        void test( ARRAY &arr, concurrent::for_each &loop)
        {
            typedef typename ARRAY::mutable_type type;
            {
                const type   value = support::get<type>();
                const type   minus = -value;
                atom::ld(arr,value);
                Y_SUPPORT_TICKS(fullTicks,atom::neg(arr));
                Y_ASSERT( atom::tool::deltaSquaredTo(arr,minus) <= 0 );

                atom::ld(arr,value);
                Y_SUPPORT_TICKS(loopTicks,atom::neg(arr,loop));
                Y_ASSERT( atom::tool::deltaSquaredTo(arr,minus) <= 0 );
                rt_clock clk;
                std::cerr << clk.speedup(fullTicks,loopTicks) << '/';
            }
        }

        template <typename T> static inline
        void testAll(concurrent::for_each &loop)
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048};
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter NEG for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";
                { vector<T,memory::global> gv(nn); test(gv,loop); }
                { vector<T,memory::pooled> pv(nn); test(pv,loop); }
                { list<T>                  gl(nn); test(gl,loop); }
                if(2==nn)
                {
                    point2d<T> p; test(p,loop);
                }
                if(3==nn)
                {
                    point3d<T> p; test(p,loop);
                }
                std::cerr << std::endl;

            }
            std::cerr << "[Leave NEG for <" << tid << ">]" << std::endl;
        }
    };


    struct UNARY
    {
        template <typename TARGET, typename SOURCE> static inline
        void test_set( TARGET &target, SOURCE &source, concurrent::for_each &loop )
        {
            Y_ASSERT( target.size() <= source.size() );

            support::fill1D(source);
            Y_SUPPORT_TICKS(fullTicks,atom::set(target,source));
            Y_ASSERT(atom::tool::deltaSquared1D(target,source)<=0);
            Y_SUPPORT_TICKS(loopTicks,atom::set(target,source,loop));
            Y_ASSERT(atom::tool::deltaSquared1D(target,source)<=0);
            rt_clock clk;
            std::cerr << '@' << clk.speedup(fullTicks,loopTicks) << '/';
        }

        template <typename TARGET, typename SOURCE> static inline
        void test_add( TARGET &target, SOURCE &source, concurrent::for_each &loop )
        {
            Y_ASSERT( target.size() <= source.size() );

            const size_t n = target.size();
            vector<typename TARGET::mutable_type> org(n), res(n);

            support::fill1D(source);
            support::fill1D(target);
            atom::tool::copy1D(org,target);
            Y_SUPPORT_TICKS(fullTicks,atom::add(target,source));
            atom::tool::copy1D(res,target);
            atom::tool::copy1D(target,org);
            Y_SUPPORT_TICKS(loopTicks,atom::add(target,source,loop));
            Y_ASSERT(atom::tool::deltaSquared1D(res,target)<=0);
            rt_clock clk;
            std::cerr << '+' << clk.speedup(fullTicks,loopTicks) << '/';
        }


        template <typename TARGET, typename SOURCE> static inline
        void test_sub( TARGET &target, SOURCE &source, concurrent::for_each &loop )
        {
            Y_ASSERT( target.size() <= source.size() );

            const size_t n = target.size();
            vector<typename TARGET::mutable_type> org(n), res(n);

            support::fill1D(source);
            support::fill1D(target);
            atom::tool::copy1D(org,target);
            Y_SUPPORT_TICKS(fullTicks,atom::sub(target,source));
            atom::tool::copy1D(res,target);
            atom::tool::copy1D(target,org);
            Y_SUPPORT_TICKS(loopTicks,atom::sub(target,source,loop));
            Y_ASSERT(atom::tool::deltaSquared1D(res,target)<=0);
            rt_clock clk;
            std::cerr << '-' << clk.speedup(fullTicks,loopTicks) << '/';
        }

        template <typename TARGET, typename SOURCE> static inline
        void test_subp( TARGET &target, SOURCE &source, concurrent::for_each &loop )
        {
            Y_ASSERT( target.size() <= source.size() );

            const size_t n = target.size();
            vector<typename TARGET::mutable_type> org(n), res(n);

            support::fill1D(source);
            support::fill1D(target);
            atom::tool::copy1D(org,target);
            Y_SUPPORT_TICKS(fullTicks,atom::subp(target,source));
            atom::tool::copy1D(res,target);
            atom::tool::copy1D(target,org);
            Y_SUPPORT_TICKS(loopTicks,atom::subp(target,source,loop));
            Y_ASSERT(atom::tool::deltaSquared1D(res,target)<=0);
            rt_clock clk;
            std::cerr << '_' << clk.speedup(fullTicks,loopTicks) << '/';
        }


        template <typename TARGET, typename SOURCE> static inline
        void test_dot( TARGET &target, SOURCE &source, concurrent::for_each &loop )
        {
            rt_clock clk;

            Y_ASSERT( target.size() <= source.size() );

            {
                support::fill1D(source);
                support::fill1D(target);
                typename TARGET::mutable_type seq(0);
                Y_SUPPORT_TICKS(fullTicks,seq=atom::dot(target,source));

                typename TARGET::mutable_type par(0);
                Y_SUPPORT_TICKS(loopTicks,par=atom::dot(target,source,loop));

                typename TARGET::mutable_type d2=seq-par;
                d2*=d2;

                std::cerr << '(' << d2 << ')' << clk.speedup(fullTicks,loopTicks) << '/';
            }

            {
                support::fill1D(target);
                typename TARGET::mutable_type seq(0);
                Y_SUPPORT_TICKS(fullTicks,seq=atom::norm2(target));


                typename TARGET::mutable_type par(0);
                Y_SUPPORT_TICKS(loopTicks,par=atom::norm2(target,loop));
                typename TARGET::mutable_type d2=seq-par;
                d2*=d2;

                std::cerr << '[' << d2 << ']' << clk.speedup(fullTicks,loopTicks) << '/';
            }

        }






#define TEST_PERM(u,v)                    \
test_set(u,v,loop);  test_set(v,u,loop);  \
test_add(u,v,loop);  test_add(v,u,loop);  \
test_sub(u,v,loop);  test_sub(v,u,loop);  \
test_subp(u,v,loop); test_subp(v,u,loop); \
test_dot(u,v,loop);  test_dot(v,u,loop)


        template <typename T> static inline
        void testAll(concurrent::for_each &loop)
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048 };
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter SET/ADD for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";

                vector<T,memory::global> v(nn);
                vector<T,memory::pooled> s(nn);
                list<T>                  l(nn);

                TEST_PERM(v,s); TEST_PERM(v,l); TEST_PERM(s,l);
                if( 2 == nn )
                {
                    point2d<T> p; TEST_PERM(v,p); TEST_PERM(s,p); TEST_PERM(l,p);
                }

                if( 3 == nn )
                {
                    point3d<T> p; TEST_PERM(v,p); TEST_PERM(s,p); TEST_PERM(l,p);
                }

                std::cerr << std::endl;

            }
            std::cerr << "[Leave SET/ADD for <" << tid << ">]" << std::endl;
        }

    };

    struct BINARY
    {
        /*
         abc
         acb
         bac
         bca
         cab
         cba
         */
#define TEST_PERM2(a,b,c) \
test(a,b,c,loop);         \
test(a,c,b,loop);         \
test(b,a,c,loop);         \
test(b,c,a,loop);         \
test(c,a,b,loop);         \
test(c,b,a,loop)

        template <typename TARGET, typename LHS, typename RHS> static inline
        void test( TARGET &target, LHS &lhs, RHS &rhs, concurrent::for_each &loop )
        {
            rt_clock clk;

            Y_ASSERT( target.size() <= lhs.size() );
            Y_ASSERT( target.size() <= rhs.size() );

            const size_t n = target.size();

            vector<typename TARGET::mutable_type> seq(n);

            {
                support::fill1D(lhs);
                support::fill1D(rhs);
                support::fill1D(target);
                Y_SUPPORT_TICKS(fullTicks,atom::add(target,lhs,rhs));
                atom::tool::copy1D(seq,target);

                support::fill1D(target);
                Y_SUPPORT_TICKS(loopTicks,atom::add(target,lhs,rhs,loop));
                Y_ASSERT(atom::tool::deltaSquared1D(seq,target)<=0);
                std::cerr << '+' << clk.speedup(fullTicks,loopTicks) << '/';
            }

            {
                support::fill1D(lhs);
                support::fill1D(rhs);
                support::fill1D(target);
                Y_SUPPORT_TICKS(fullTicks,atom::sub(target,lhs,rhs));
                atom::tool::copy1D(seq,target);

                support::fill1D(target);
                Y_SUPPORT_TICKS(loopTicks,atom::sub(target,lhs,rhs,loop));
                Y_ASSERT(atom::tool::deltaSquared1D(seq,target)<=0);
                std::cerr << '-' << clk.speedup(fullTicks,loopTicks) << '/';
            }


        }


        template <typename T>
        static inline void testAll(concurrent::for_each &loop )
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048};
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter BINARY ADD/SUB  for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";

                vector<T,memory::global> v(nn);
                vector<T,memory::pooled> s(nn);
                list<T>                  l(nn);

                TEST_PERM2(v,s,l);
                std::cerr << std::endl;
            }

            std::cerr << "[LEAVE BINARY ADD/SUB for <" << tid << ">]" << std::endl;

        }

    };



    struct Level1
    {
        template <typename T> static inline
        void Test( concurrent::for_each &loop )
        {
            LD    ::testAll<T>(loop);
            NEG   ::testAll<T>(loop);
            UNARY ::testAll<T>(loop);
            BINARY::testAll<T>(loop);
        }
    };


}

#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
//#include "y/counting/mloop.hpp"


Y_UTEST(atom1)
{
    concurrent::simd loop;
    {
        point2d<double> P;
        combination     C(5,2);
        for( C.start(); C.valid(); C.next() )
        {
            support::fill1D(P);
            const double d = atom::dot(P,C);
            std::cerr << P << "*" << C << "=" << d << std::endl;
        }
    }

    {
        point3d<float> V;
        permutation     P(V.size());
        for( P.start(); P.valid(); P.next() )
        {
            support::fill1D(V);
            const float d = atom::dot(V,P);
            std::cerr << V << "*" << P << "=" << d << std::endl;

        }
    }
    
    Level1::Test<float>(loop);
    Level1::Test<double>(loop);
    Level1::Test<short>(loop);
    Level1::Test<mpz>(loop);



    



}
Y_UTEST_DONE()

