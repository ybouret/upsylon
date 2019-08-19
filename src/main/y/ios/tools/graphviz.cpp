#include "y/ios/tools/graphviz.hpp"
#include "y/fs/local/fs.hpp"
#include "y/exception.hpp"
#include <cstdlib>

namespace upsylon
{
    namespace ios
    {
        bool GraphViz:: Render(const string &filename, bool keepFile )
        {

            vfs &fs = local_fs::instance();
            if( !fs.is_reg(filename) )
            {
                throw exception("GraphViz: %s does not exists", *filename );
            }
            string pngfile = filename;
            vfs::change_extension(pngfile,"png");
            fs.try_remove_file(pngfile);

            const string cmd = "dot -Tpng " + filename + " -o " + pngfile;
            std::cerr << "[" << cmd << "]" << std::endl;

            if( 0 != system(*cmd) )
            {
                std::cerr << "[failure]" << std::endl;
                return false;
            }
            else
            {
                std::cerr << "[success]" << std::endl;
                if(!keepFile)
                {
                    fs.try_remove_file(filename);
                }
                return true;
            }
        }
    }
}
