#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/ptr/arc.hpp"
#include "y/memory/pooled.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/arr.hpp"
#include "y/ptr/shared.hpp"
#include "support.hpp"

using namespace upsylon;


template <typename BATCH>
static inline void doAddr( BATCH &batch )
{
    std::cerr << batch << std::endl;
}



template <typename SEQ>
static inline void fill( SEQ &seq )
{
     for(size_t n=2+alea.leq(10);n>0;--n)
    {
        typename SEQ::const_type tmp = support::get<typename SEQ::mutable_type>();
        seq.push_back(tmp);
    }
 }


template <typename T>
static inline void doCreate()
{


    typedef arr_ptr< sequence<T>, arc_ptr>    arc_seq;
    typedef arr_ptr< sequence<T>, shared_ptr> shared_seq;

    arc_seq     gv = new vector<T,memory::global>();
    shared_seq  pv = new vector<T,memory::pooled>();
    arc_seq     gl = new list<T>();

    fill( *gv );
    fill( *pv );
    fill( *gl );

    doAddr( gv );
    doAddr( pv );
    doAddr( gl );


}


Y_UTEST(addressable)
{

    
    doCreate<int>();
    doCreate<string>();
    doCreate<float>();
    doCreate<mpq>();

}
Y_UTEST_DONE()



