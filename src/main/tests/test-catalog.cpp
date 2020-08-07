#include "y/associative/suffix/batch.hpp"
#include "y/associative/set.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/intr.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/spec.hpp"
#include "y/type/aliasing.hpp"

using namespace upsylon;

namespace {
 
    template <typename KEY>
    class dummy : public counted_object
    {
    public:
        Y_DECL_ARGS(KEY,key_type);
        typedef intr_ptr<KEY,dummy> pointer;
        
        const_key_type id;
        
        const_key_type & key() const throw() { return id; }
        
        dummy(param_key_type k) : id(k)
        {
        }
        
        virtual ~dummy() throw()
        {
        }
        
        dummy( const dummy &other ) :object(),  counted_object(), id(other.id)
        {
        }
        
        
        
        
    private:
        Y_DISABLE_ASSIGN(dummy);
    };
    
    template <typename KEY>
    void doTest(const size_t nmax)
    {
        std::cerr << "-- Test<" << type_name_of<KEY>() << ">(" << nmax << ")" << std::endl;
        typedef dummy<KEY>              Dummy;
        typedef typename Dummy::pointer Pointer;
        
        vector<KEY>               ok(nmax, as_capacity);
        suffix_batch<KEY,Dummy>   st(nmax/4, as_capacity);
        suffix_batch<KEY,Pointer> pst(nmax/4,as_capacity);
        set<KEY,Dummy>            db(nmax/4, as_capacity);
        set<KEY,Pointer>          pdb(nmax/4,as_capacity);
        
        std::cerr << "\tgenerating data..." << std::endl;
        for(size_t i=0;i<nmax;++i)
        {
            typename Dummy::const_key_type k = support::get< typename Dummy::mutable_key_type>();
            const Dummy                    D(k);
            if( st.insert(D) )
            {
                ok << k;
                Y_ASSERT( db.insert(D) );
                const Pointer p = new Dummy(D);
                Y_ASSERT( pst.insert(p) );
                Y_ASSERT( pdb.insert(p) );

            }
        }
        
        Y_CHECK(ok.size()==st.size());
        Y_CHECK(ok.size()==db.size());
        Y_CHECK(ok.size()==pst.size());
        Y_CHECK(ok.size()==pdb.size());
        std::cerr << "[" << ok.size() << "/" << nmax << "]" << std::endl;
        
        std::cerr << "\ttesting search" << std::endl;
        {
            typename Dummy::const_key_type   *K = *ok;
            typename Dummy::mutable_key_type *k = (typename Dummy::mutable_key_type *)K;
            alea.shuffle( k, ok.size() );
        }
        
        for(size_t i=ok.size();i>0;--i)
        {
            Y_ASSERT( st.search( ok[i]) );
            Y_ASSERT( db.search( ok[i]) );
            Y_ASSERT( pst.search( ok[i]) );
            Y_ASSERT( pdb.search( ok[i]) );
        }
        vector<KEY> no;
        for(size_t i=ok.size()/2;i>0;--i)
        {
            no << ok.back();
            ok.pop_back();
        }
        
        std::cerr << "\ttesting remove" << std::endl;

        for(size_t i=no.size();i>0;--i)
        {
            Y_ASSERT( st. remove( ok[i]) );
            Y_ASSERT( db. remove( ok[i]) );
            Y_ASSERT( pst.remove( ok[i]) );
            Y_ASSERT( pdb.remove( ok[i]) );
        }
        for(size_t i=no.size();i>0;--i)
        {
            Y_ASSERT( !st.search( ok[i]) );
            Y_ASSERT( !db.search( ok[i]) );
            Y_ASSERT( !pst.search( ok[i]) );
            Y_ASSERT( !pdb.search( ok[i]) );
        }
        
    }
    
}

Y_UTEST(catalog)
{
    doTest<int>(1000);
    doTest<const short>(1000);
    doTest<const string>(1000);
    
}
Y_UTEST_DONE()

