#include "y/ios/tools/find-executable.hpp"
#include "y/string/env.hpp"
#include "y/string/tokenizer.hpp"
#include "y/fs/local/fs.hpp"

namespace upsylon
{
    
    exe_paths:: exe_paths() throw() :
    ro_strings(),
    dirs() 
    {
    }
    
    exe_paths:: ~exe_paths() throw()
    {
    }
    
    size_t exe_paths:: size() const throw()
    {
        return dirs.size();
    }
    
    const string & exe_paths:: operator[](const size_t i) const throw()
    {
        assert(i>0);
        assert(i<=size());
        return dirs[i];
    }
    
    bool exe_paths:: add(const string &d)
    {
        static const vfs   &fs   = local_fs::instance();
        const        string temp = vfs::to_directory(d);
        
        // check is_dir
        if(!fs.is_dir(temp)) return false;
        
        // check no multiple
        for(size_t i=dirs.size();i>0;--i)
        {
            if(temp==dirs[i]) return false;
        }
        dirs << temp;
        return true;
    }
    
    
    size_t  exe_paths:: load(const char *path_env_name)
    {
        
#if Y_WIN
        const char sep = ';';
#endif
        
#if Y_BSD
        const char sep = ':';
#endif
        size_t              extra = 0;
        string              path_env_value;
        const string        path_env_label = path_env_name;
        
        if( environment::get(path_env_value,path_env_label))
        {
            
            tokenizer<char> tknz(path_env_value);
            while( tknz.next_with(sep) )
            {
                const string str = tknz.to_string();
                if( add(str) )
                {
                    ++extra;
                }
            }
            
        }
        
        return extra;
    }
    
    bool exe_paths:: ok(string &fn)
    {
        static const vfs   &fs   = local_fs::instance();
        const char         *ext  = vfs::get_extension(fn);
        std::cerr << "Passing [" << fn << "]" << std::endl;
        if(!ext)
        {
            static const char *xx[] = { "", ".exe" };
            for(size_t i=0;i<sizeof(xx)/sizeof(xx[0]);++i)
            {
                const string tmp = fn + xx[i];
                std::cerr << "Testing [" << tmp << "]" << std::endl;
                if(fs.is_reg(tmp))
                {
                    fn = tmp;
                    return true;
                }
            }
            return false;
        }
        else
        {
            return fs.is_reg(fn);
        }
    }

    
    
}
