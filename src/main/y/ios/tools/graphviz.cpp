#include "y/ios/tools/graphviz.hpp"
#include "y/fs/local/fs.hpp"
#include "y/exception.hpp"
#include <cstdlib>

namespace upsylon {

    namespace ios {

        bool GraphViz:: Render(const string &filename, bool keepFile )
        {

            local_fs &fs = local_fs::instance();
            if( !fs.is_reg(filename) )
            {
                throw exception("GraphViz: %s does not exists", *filename );
            }

            const string *pdot = fs.query_path_handle("dot");
            if(pdot)
            {
                string pngfile = filename;
                vfs::change_extension(pngfile,"png");
                fs.try_remove_file(pngfile);

                const string cmd = *pdot + " -Tpng " + filename + " -o " + pngfile;
                std::cerr << "[" << cmd << "]" << std::endl;

                if( 0 != system(*cmd) )
                {
                    //std::cerr << "[failure]" << std::endl;
                    return false;
                }
                else
                {
                    //std::cerr << "[success]" << std::endl;
                    if(!keepFile)
                    {
                        fs.try_remove_file(filename);
                    }
                    return true;
                }
            }
            else
            {
                std::cerr << "[ GrapViz not detected ]" << std::endl;
                return false;
            }
        }
    }
}

