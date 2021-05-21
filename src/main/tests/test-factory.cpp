#include "y/container/workshop.hpp"
#include "y/utest/run.hpp"

#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"

#include "y/string.hpp"
#include "y/string/display.hpp"

using namespace upsylon;

typedef hashing::function * (*hfn_new)();
typedef workshop<hashing::function,hfn_new> hfn_factory;

#define H_DECL(NAME) do { const string key = hashing::NAME::CLID; f.declare(hashing::NAME::CLID,hashing::NAME::create); } while(false)

Y_UTEST(factory)
{
    hfn_factory f;

    H_DECL(md5);
    H_DECL(sha1);

    for( hfn_factory::iterator i = f.begin(); i != f.end(); ++i )
    {
        const string & key = i.get().key();
        string_display::align(std::cerr,key,8) << " "; Y_CHECK( f.search(key) );
    }

    hashing::function::pointer H = f( "md5" );
    std::cerr << "created = " << H->name() << std::endl;

}
Y_UTEST_DONE()
