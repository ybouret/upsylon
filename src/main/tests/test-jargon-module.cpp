#include "y/jargon/module.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(jargon_module)
{
    const Jargon::Context ctx( "hello" );
    
    
    Jargon::Char::List       chList;
    Jargon::Char::SharedPool chPool = new Jargon::Char::Pool();
    
    for(unsigned i='a'; i <= 'z'; ++i )
    {
        chList.push_back( Jargon::Char::Create(ctx,i,chPool) );
    }
    
    chPool->store(chList);
    
    
}
Y_UTEST_DONE()

