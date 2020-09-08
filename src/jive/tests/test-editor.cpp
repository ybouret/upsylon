
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class Ed : public Lexical::Editor::Program
    {
    public:
        Ed()
        {
            on("[:lower:]",this, & Ed::OnLower);
        }

        virtual ~Ed() throw()
        {
        }

        void OnLower(ios::ostream &fp, const Token &t)
        {
            fp << '<' << t << '>';
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ed);
    };

}

Y_UTEST(editor)
{
    Ed prog;
    if(argc>1)
    {
        Source        source( Module::OpenFile(argv[1]) );
        ios::ocstream fp( ios::cstdout );

        prog.run(fp,source);
    }
}
Y_UTEST_DONE()

