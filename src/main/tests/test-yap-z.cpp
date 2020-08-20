
#include "y/yap/integer.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"


using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {

    static inline
    void test_setup()
    {
        std::cerr << "---> test setup" << std::endl;
        vector<integer> Z;
        {
            const integer zero;         std::cerr << "zero  = "  << zero  << std::endl;
            const integer one = 1;      std::cerr << "one   = "  << one   << std::endl;
            const integer minus = -1;   std::cerr << "minus = "  << minus << std::endl;
            Z.push_back(zero);
            Z.push_back(one);
            Z.push_back(minus);
            integer tmp;
            tmp = one; Z.push_back(tmp);
            tmp =  5;   Z.push_back(tmp);
            tmp = -7;  Z.push_back(tmp);
        }
        std::cerr << Z << std::endl;
        for(size_t i=0;i<ITER;++i)
        {
            const integer z(alea,alea.leq(80));
            Z.push_back(z);
        }

        {
            size_t written = 0;
            {
                ios::ocstream fp("apz.dat");
                for(size_t i=1;i<=Z.size();++i)
                {
                    written += Z[i].serialize(fp);
                }
            }
            std::cerr << "written=" << written << std::endl;
            size_t read = 0;
            {
                ios::icstream fp("apz.dat");
                for(size_t i=1;i<=Z.size();++i)
                {
                    size_t delta = 0;
                    const integer z = integer::read(fp, delta, "some integer");
                    read += delta;
                    if(z!=Z[i])
                    {
                        std::cerr << "read " << z << " for " << Z[i] << std::endl;
                        throw exception("bad integer read");
                    }
                    Y_ASSERT(z==Z[i]);
                    Y_ASSERT(!(z!=Z[i]));
                }
            }
            Y_ASSERT(read==written);
        }
    }

    static inline
    void test_cmp()
    {
        std::cerr << "---> test cmp" << std::endl;
        static const int         data[] = { -7, -5, 0, 1, 12 };
        static const size_t      size = sizeof(data)/sizeof(data[0]);
        lightweight_array<int> a((int*)data,size);
        std::cerr << "a=" << a << std::endl;
        vector<integer>        A(size,as_capacity);
        for(size_t i=1;i<=size;++i) { const integer tmp(a[i]); A.push_back_(tmp); }
        std::cerr << "A=" << A << std::endl;

        for(size_t i=1;i<=size;++i)
        {
            Y_ASSERT(a[i]==A[i]);
            Y_ASSERT(A[i]==a[i]);
            Y_ASSERT(A[i]==A[i]);

            Y_ASSERT(!(a[i]!=A[i]));
            Y_ASSERT(!(A[i]!=a[i]));
            Y_ASSERT(!(A[i]!=A[i]));

            Y_ASSERT(a[i]<=A[i]);
            Y_ASSERT(A[i]<=a[i]);
            Y_ASSERT(A[i]<=A[i]);

            Y_ASSERT(a[i]>=A[i]);
            Y_ASSERT(A[i]>=a[i]);
            Y_ASSERT(A[i]>=A[i]);
        }

        for(size_t i=1;i<size;++i)
        {
            for(size_t j=i+1;j<=size;++j)
            {
                Y_ASSERT(A[i]<A[j]);
                Y_ASSERT(A[i]<=A[j]);
                Y_ASSERT(A[j]>A[i]);
                Y_ASSERT(A[j]>=A[i]);

                Y_ASSERT(a[i]<A[j]);
                Y_ASSERT(a[i]<=A[j]);
                Y_ASSERT(a[j]>A[i]);
                Y_ASSERT(a[j]>=A[i]);

                Y_ASSERT(A[i]<a[j]);
                Y_ASSERT(A[i]<=a[j]);
                Y_ASSERT(A[j]>a[i]);
                Y_ASSERT(A[j]>=a[i]);
            }
        }
    }

    static inline void test_add()
    {
        std::cerr << "---> test add" << std::endl;
        for(int i=-10;i<=10;++i)
        {
            const integer I = i;
            {
                const integer II = +i;
                Y_ASSERT(II==I);
            }
            for(int j=-10;j<=10;++j)
            {
                const integer J = j;
                const integer S = I+J;
                Y_ASSERT(S==i+j);
                Y_ASSERT(I+j==S);
                Y_ASSERT(j+I==S);
                Y_ASSERT(i+J==S);
                Y_ASSERT(J+i==S);
            }
        }
        {
            int value = -10;
            for(integer i=-10;i<=10;++i,++value)
            {
                Y_ASSERT(i==value);
            }
        }
        {
            int value = -10;
            for(integer i=-10;i<=10;i++,value++)
            {
                Y_ASSERT(i==value);
            }
        }
    }

    static inline void test_sub()
    {
        std::cerr << "---> test sub" << std::endl;
        for(int i=-10;i<=10;++i)
        {
            const integer I = i;
            {
                const integer II = -I;
                Y_ASSERT(II==-i);
            }
            for(int j=-10;j<=10;++j)
            {
                const integer J = j;
                const integer S = I-J;
                //std::cerr << "(" << i << ") - (" << j << ") -> " << i-j << std::endl;
                //std::cerr << "(" << I << ") - (" << J << ") -> " << S   << std::endl;
                Y_ASSERT(S==i-j);
                Y_ASSERT(I-j==S);
                Y_ASSERT(i-J==S);
            }
        }
        {
            int value = 10;
            for(integer i=10;i>=-10;--i,--value)
            {
                Y_ASSERT(i==value);
            }
        }
        {
            int value = 10;
            for(integer i=10;i>=-10;i--,value--)
            {
                Y_ASSERT(i==value);
            }
        }
    }

    static inline void test_addops()
    {
        std::cerr << "---> test addops" << std::endl;
        for(size_t i=0;i<ITER;++i)
        {
            const integer a(alea,alea.leq(5000));
            const integer b(alea,alea.leq(5000));
            const integer c = a+b;
            const integer d = c-b;
            const integer e = c-a;
            Y_ASSERT(d==a);
            Y_ASSERT(e==b);
        }
    }




}

Y_UTEST(yap_z)
{

    test_setup();
    test_cmp();
    test_add();
    test_sub();
    test_addops();

    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *natural::instance() << std::endl;

}
Y_UTEST_DONE()


