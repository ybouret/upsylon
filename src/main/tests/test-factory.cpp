#include "y/container/xfactory.hpp"
#include "y/utest/run.hpp"

#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"

#include "y/string.hpp"
#include "y/string/display.hpp"

using namespace upsylon;

typedef hashing::function * (*hfn_new)();
typedef xfactory<hashing::function,hfn_new> hfn_factory;

#define H_DECL(NAME) do { const string key = hashing::NAME::CLID; f.declare(key,hashing::NAME::create); } while(false)

Y_UTEST(factory)
{
    hfn_factory f;

    H_DECL(md5);
    H_DECL(sha1);

    for( hfn_factory::const_iterator i = f.begin(); i != f.end(); ++i )
    {
        const string & key = i.key();
        string_display::align(std::cerr,key,16) << " "; Y_CHECK( f.query(key) );
    }

    hashing::function::pointer H = f( "MD5" );
    std::cerr << "created = " << H->name() << std::endl;

}
Y_UTEST_DONE()
