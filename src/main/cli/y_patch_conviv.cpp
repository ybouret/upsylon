#include "y/program.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    class Ed : public Lexical::Editor::Program
    {
    public:
        Ed()
        {
            on("integer *::",this, & Ed::OnNakedInteger);
        }

        virtual ~Ed() throw()
        {
        }


        void OnNakedInteger(ios::ostream &fp, const Token &)
        {
            fp << "integer::";
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ed);
    };

    static inline
    void Patch(const string &fileName,
               Ed           &ed)
    {
        Source        source( Module::OpenFile(fileName) );
        ios::ocstream target( ios::cstdout );
        ed.run(target,source);
    }

}

Y_PROGRAM_START()
{
    Ed ed;
    if(argc>1)
    {
        const string fn = argv[1];
        Patch(fn,ed);
    }
}
Y_PROGRAM_END()

