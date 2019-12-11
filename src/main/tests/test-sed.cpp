#include "y/lang/stream/processor.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Lang;

namespace {

    class ToDo
    {
    public:
        inline  ToDo() throw() {}
        inline ~ToDo() throw() {}

        bool Check( Token &token, Source &source )
        {
            return true;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ToDo);
    };

}

Y_UTEST(sed)
{
    ToDo todo;

    Stream::Processor sed;

    {
        Stream::Instruction I( &todo, & ToDo::Check );
        sed.on("[:digit:]", I );
        sed.on("_[:alpha:]", I);
    }

    if(argc>1)
    {
        const string  fileName = argv[1];
        Module       *module   = (fileName=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(fileName);
        ios::ocstream target( ios::cstdout );
        sed.run(target,module);
    }
}
Y_UTEST_DONE()


