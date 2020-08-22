
#include "y/ordered/sorted-list.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/memory/allocator/pooled.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

#include "support.hpp"

#include "y/sequence/vector.hpp"

using namespace upsylon;


namespace
{

    template <typename LHS> static inline
    void test_ownership(const LHS &lhs)
    {
        for(typename LHS::const_iterator i=lhs.begin();i!=lhs.end();++i)
        {
            Y_ASSERT( lhs.owns( *i ) );
        }
    }

    template <typename LHS,typename RHS> static inline
    void test_no_ownership(const LHS &lhs,const RHS &rhs)
    {
        for(typename LHS::const_iterator i=lhs.begin();i!=lhs.end();++i)
        {
            Y_ASSERT( !rhs.owns( *i ) );
        }

        for(typename RHS::const_iterator i=rhs.begin();i!=rhs.end();++i)
        {
            Y_ASSERT( !lhs.owns( *i ) );
        }
    }


    template <typename ORDERED>
    static inline void checkOrdered(const size_t maxCycles)
    {
        ordered_multiple<ORDERED> M;
        ordered_single<ORDERED>   S;
        ordered_unique<ORDERED>   U;

        std::cerr << "checkOrdered<" << typeid(ORDERED).name() << ">" << std::endl;


        typedef typename ORDERED::mutable_type type;

        vector<type> data;

        for(size_t cycle=0;cycle<maxCycles;++cycle)
        {
            std::cerr << "+";
            M.release();
            S.release();
            U.release();
            data.free();

            size_t       all_count = 0;
            size_t       one_count = 0;
            for(size_t iter=10+alea.leq(100);iter>0;--iter)
            {
                const type tmp = support::get<type>();
                for(size_t trials=1+alea.leq(2);trials>0;--trials)
                {
                    ++all_count;
                    M.insert(tmp);
                    S.insert(tmp);
                    if(U.insert(tmp))
                    {
                        ++one_count;
                        data.push_back(tmp);
                    }
                }
            }
            Y_ASSERT(M.size()==all_count);
            Y_ASSERT(S.size()==one_count);
            Y_ASSERT(U.size()==one_count);

            for(typename ORDERED::iterator i=S.begin(),j=U.begin(); i !=S.end(); ++i,++j)
            {
                Y_ASSERT( *i == *j );
            }

            for(typename ORDERED::reverse_iterator i=S.rbegin(),j=U.rbegin(); i !=S.rend(); ++i,++j)
            {
                Y_ASSERT( *i == *j );
            }

            std::cerr << "*";
            test_ownership(S);
            test_ownership(U);
            test_ownership(M);

            test_no_ownership(S,U);
            test_no_ownership(S,M);
            test_no_ownership(U,M);

            std::cerr << "-";
            alea.shuffle(*data,data.size());
            for(size_t k=data.size();k>0;--k)
            {
                M.no( data[k] );
                S.no( data[k] );
                U.no( data[k] );
            }


        } std::cerr << std::endl;

    }

    template <typename T>
    static inline void doOrdered(const size_t maxCycles)
    {

        checkOrdered< sorted_list<T,increasing_comparator<T> > >(maxCycles);
        checkOrdered< sorted_list<T,decreasing_comparator<T> > >(maxCycles);

        checkOrdered< sorted_vector<T,increasing_comparator<T>,memory::global> >(maxCycles);
        checkOrdered< sorted_vector<T,decreasing_comparator<T>,memory::global> >(maxCycles);

        checkOrdered< sorted_vector<T,increasing_comparator<T>,memory::pooled> >(maxCycles);
        checkOrdered< sorted_vector<T,decreasing_comparator<T>,memory::pooled> >(maxCycles);


    }

}

Y_UTEST(ordered)
{
    doOrdered<short>(128);
    doOrdered<int>(128);
    doOrdered<double>(128);
    doOrdered<string>(32);
    doOrdered<apq>(8);

}
Y_UTEST_DONE()




