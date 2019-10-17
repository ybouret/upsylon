#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/container/shared.hpp"
#include "y/ptr/batch.hpp"
#include "y/memory/pooled.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

#include "support.hpp"

using namespace upsylon;


template <typename BATCH>
static inline void doAddr( BATCH &batch )
{
    std::cerr << "\t" << std::endl;
    support::fill1D(batch);
}

template <typename SEQ>
static inline SEQ *createSeq( SEQ  *src )
{
    auto_ptr<SEQ> keep = src;
    for(size_t n=2+alea.leq(10);n>0;--n)
    {
        typename SEQ::const_type tmp = support::get<typename SEQ::mutable_type>();
        src->push_back(tmp);
    }
    return keep.yield();
}


template <typename T>
static inline void doCreate()
{
    typedef shared< vector<T,memory::global> > sharedGlobalVector;
    typedef shared< vector<T,memory::pooled> > sharedPooledVector;
    typedef shared< list<T>                  > sharedList;

    

}


Y_UTEST(addressable)
{

    batch_ptr< addressable<int>, shared_ptr> L = createSeq(new list<int>());
    batch_ptr< addressable<int>, shared_ptr> G = createSeq(new vector<int,memory::global>());
    batch_ptr< addressable<int>, shared_ptr> P = createSeq(new vector<int,memory::pooled>());

    doAddr(L);
    doAddr(G);
    doAddr(P);

}
Y_UTEST_DONE()



