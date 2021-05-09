#include "y/ios/tools/find-executable.hpp"
#include "y/string/env.hpp"
#include "y/string/tokenizer.hpp"
#include "y/fs/local/fs.hpp"

namespace upsylon
{
    size_t find_exe:: load_paths(strings    &paths,
                                 const char *path_env_name)
    {
        size_t       extra = 0;
        const string path_env_label = path_env_name;
        string       path_env_value;
        const vfs  &fs     = local_fs::instance();
        
        if( environment::get(path_env_value,path_env_label))
        {
            // ok, found someting
            std::cerr << "$" << path_env_label << "='" << path_env_value << "'" << std::endl;
            
            tokenizer<char> tknz(path_env_value);
            while( tknz.next_with(':') )
            {
                string str( tknz.token(), tknz.units() );
                //std::cerr << "-> '" << str << "'" << std::endl;
                if(fs.is_dir(str))
                {
                    //std::cerr << "\tvalid!" << std::endl;
                    vfs::as_directory(str);
                    paths << str;
                }
            }
            
        }
        
        return extra;
    }

}
