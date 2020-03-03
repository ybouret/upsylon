#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/comparison.hpp"
#include "y/sort/merge.hpp"
#include "y/sort/heap.hpp"
#include "y/sort/index.hpp"
#include "y/sort/ysort.hpp"

#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include <cstdlib>
#include "y/ios/ocstream.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{


    template<typename T>
    static inline bool check_sorted( array<T> &arr )
    {
        const size_t n = arr.size();
        for(size_t i=1;i<n;++i)
        {
            if(arr[i]>arr[i+1])
            {
                std::cerr << "!!! arr[" << i << "/ " << arr.size() << "]=" << arr[i] << ">arr[" << i+1 << "]=" << arr[i+1] << std::endl;
                return false;
            }
        }
        return true;
    }

    template <typename NODE>
    int compare_node( const NODE *lhs, const NODE *rhs, void *)
    {
        return comparison::increasing(lhs->data,rhs->data);
    }

    template <typename T> static inline
    void perform()
    {
        std::cerr << "Sorting <" << typeid(T).name() << ">" << std::endl;

        for(size_t iter=0;iter<16;++iter)
        {
            const size_t n = 1+alea.leq(100);
            std::cerr << "\tMerge" << std::endl;
            typedef core::node_of<T> Node;
            core::list_of_cpp<Node>  L;
            for(size_t i=1;i<=n;++i)
            {
                const T args = support::get<T>();
                L.push_back( new Node(args) );
            }
            std::cerr << "L1=";
            for(const Node *node=L.head;node;node=node->next)
            {
                std::cerr << node->data << " ";
            }
            std::cerr << std::endl;
            merging<Node>::sort(L,compare_node<Node>,NULL);
            std::cerr << "L2=" ;
            for(const Node *node=L.head;node;node=node->next)
            {
                std::cerr << node->data << " ";
            }
            std::cerr << std::endl;

            std::cerr << "\tHeap" << std::endl;
            vector<T> V;
            while(L.size)
            {
                if( alea.choice() )
                {
                    V.push_back(L.head->data);
                    delete L.pop_front();
                }
                else
                {
                    V.push_back(L.tail->data);
                    delete L.pop_back();
                }
            }
            std::cerr << "V1=" << V << std::endl;
            hsort(V, comparison::increasing<T> );
            std::cerr << "V2=" << V << std::endl;
            Y_ASSERT(check_sorted(V));

            std::cerr << "\tIndex" << std::endl;
            vector<size_t> indx(V.size());
            for(size_t i=1;i<=indx.size();++i)
            {
                indx[i] = i;
            }
            alea.shuffle(*indx,indx.size());
            hsort(indx,V,comparison::increasing<size_t>);

            alea.shuffle(*V,V.size());
            indexing::make(indx, comparison::increasing<T>, V);
            std::cerr << "V=" << V << std::endl;
            std::cerr << "I=" << indx << std::endl;

            for(size_t i=1;i<n;++i)
            {
                Y_ASSERT(V[indx[i]]<=V[indx[i+1]]);
            }

            std::cerr << "\tysort" << std::endl;
            alea.shuffle(*V,V.size());
            std::cerr << "V1=" << V << std::endl;
            ysort(V,comparison::increasing<T>);
            std::cerr << "V2=" << V << std::endl;
            Y_ASSERT(check_sorted(V));
        }
    }

    
    template <typename T>
    void do_perf(sequence<unsigned> &count,
                 sequence<double>   &hspd,
                 sequence<double>   &qspd,
                 sequence<double>   &yspd)
    {
        const size_t ITER = 1024;
        rt_clock clk;
        count.free();
        hspd.free();
        qspd.free();
        yspd.free();
        for(unsigned n=1;n<=2048;n*=2)
        {
            count.push_back(n);
            vector<T> arr(n,as_capacity);
            uint64_t  htmx = 0;
            uint64_t  qtmx = 0;
            uint64_t  ytmx = 0;
            for(size_t iter=0;iter<ITER;++iter)
            {
                arr.free();
                for(size_t i=n;i>0;--i)
                {
                    const T tmp = support::get<T>();
                    arr.push_back(tmp);
                }

                {
                    const uint64_t mark = rt_clock::ticks();
                    hsort(arr,comparison::increasing<T>);
                    htmx += rt_clock::ticks() - mark;
                }

                alea.shuffle(*arr,n);
                {
                    const uint64_t mark = rt_clock::ticks();
                    qsort(*arr,n,sizeof(T),comparison::__increasing<T>);
                    qtmx += rt_clock::ticks() - mark;
                }

                alea.shuffle(*arr,n);
                {
                    const uint64_t mark = rt_clock::ticks();
                    ysort(arr,comparison::increasing<T>);
                    ytmx += rt_clock::ticks() - mark;
                }


            }
            const size_t count = n*ITER;
            const double hell  = clk(htmx);
            hspd.push_back(1e-6*(count/hell));

            const double qell = clk(qtmx);
            qspd.push_back(1e-6*(count/qell));

            const double yell = clk(ytmx);
            yspd.push_back(1e-6*(count/yell));

            if( 0 == (n&15) )
            {
                std::cerr.flush();
                fprintf( stderr, "spead@%6u | heap: %7.3f | quick: %7.3f | y: %7.3f\n",  unsigned(n), hspd.back(), qspd.back(), yspd.back() );
                /*
                std::cerr
                << n
                << " | heap : " << hspd.back()
                << " | quick: " << qspd.back()
                << " | y    : " << yspd.back() << std::endl;
                 */
            }
        }
    }
}

Y_UTEST(sort)
{
    perform<int>();
    perform<float>();
    perform<double>();
    perform<string>();

    vector<unsigned> count;
    vector<double>   hspd;
    vector<double>   qspd;
    vector<double>   yspd;
#define DO_PERF(CLASS) do {\
std::cerr << "Speeds for " << #CLASS << std::endl;\
do_perf<CLASS>(count,hspd,qspd,yspd);\
ios::ocstream fp( #CLASS ".dat");\
for(size_t i=1;i<=count.size();++i) {\
fp("%u %g %g %g\n", count[i], hspd[i], qspd[i], yspd[i]);\
}\
} while(false)

    DO_PERF(int);
    DO_PERF(double);
    DO_PERF(string);
    std::cerr << "sizeof(string)=" << sizeof(string) << std::endl;
}
Y_UTEST_DONE()

