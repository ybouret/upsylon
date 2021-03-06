#include "y/program.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/fs/local/fs.hpp"
#include "y/fs/disk/file.hpp"

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
            integer_("integer *\\( *4 *\\)");
            integer_("integer *\\( *int_code *\\)");
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
        vfs & fs = local_fs::instance();
        const string  destName = vfs::with_new_extension(fileName,"tmp");
        std::cerr << "<" << fileName << "> -> <" << destName << ">" << std::endl;

        // perform patch fileName -> destName
        {
            Source        source( Module::OpenFile(fileName) );
            {
                ios::ocstream target( destName );
                ed.reset();
                ed.run(target,source);
            }
        }
        std::cerr << "\tcount=" << ed.count << std::endl;
        if(ed.count)
        {
            const string fileDir = vfs::get_file_dir(fileName) + "backup/";
            fs.create_sub_dir(fileDir);
            const string fileSave = fileDir + vfs::get_base_name(fileName);
            std::cerr << "\tfileSave =" << fileSave << std::endl;
            if(fs.is_reg(fileSave))
            {
                throw exception("<%s> is already patched!",*fileName);
            }
            ios::disk_file::copy(fileSave,fileName,false);
            fs.remove_file(fileName);
            ios::disk_file::copy(fileName,destName,false);
            fs.remove_file(destName);
        }
        else
        {
            std::cerr << "\tclean!" << std::endl;
            fs.try_remove_file(destName);
        }
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

