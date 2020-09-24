#include "y/program.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/fs/local/fs.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    static const char integer_text[] = "integer(KIND=int_code)";

    class Ed : public Lexical::Editor::Program
    {
    public:
        unsigned count;

        Ed() :
        count(0)
        {
            integer_("integer *::");
            integer_("integer *\\( *4 *\\)");
        }

        inline void integer_(const char *rx)
        {
            on(rx,this,&Ed::OnInteger);
        }


        virtual ~Ed() throw()
        {
        }

        void reset()
        {
            count=0;
        }

        void OnInteger(ios::ostream &fp, const Token &t)
        {
            std::cerr << "|_replacing '" << t << "'"  << std::endl;
            fp << integer_text;
            ++count;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ed);
    };

    static inline
    void Patch(const string &fileName,
               Ed           &ed)
    {
        std::cerr << "<" << fileName << ">" << std::endl;
        Source        source( Module::OpenFile(fileName) );
        const string  destName = vfs::with_new_extension(fileName,"tmp");
        {
            ios::ocstream target( destName );
            ed.reset();
            ed.run(target,source);
        }
        std::cerr << "\tcount=" << ed.count << std::endl;
        std::cerr << "<" << fileName << "/>" << std::endl;
        std::cerr << std::endl;
    }

}

Y_PROGRAM_START()
{
    Ed ed;
    for(int i=1;i<argc;++i)
    {
        const string fn = argv[i];
        Patch(fn,ed);
    }
}
Y_PROGRAM_END()

