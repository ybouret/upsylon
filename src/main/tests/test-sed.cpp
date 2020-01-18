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

        bool Check( Token &token  )
        {
            assert(token.size>0);
            const CharInfo info = *token.head;
            token.push_front( new Char('<',info) );
            token.push_back( new Char('>',info) );
            return true;
        }

        bool UpperCase( Token &token )
        {
            assert(2==token.size);
            assert('_' == token.head->code);
            delete token.pop_front();
            assert(1==token.size);
            token.tail->code = toupper(token.tail->code);
            return true;
        }

        bool Zap( Token &token ) throw()
        {
            token.release();
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
        sed.on("[:digit:]",  todo, &ToDo::Check);
        sed.on("_[:alpha:]", todo, &ToDo::UpperCase);
        sed.on("(class *|struct *)", todo, &ToDo::Zap);
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


