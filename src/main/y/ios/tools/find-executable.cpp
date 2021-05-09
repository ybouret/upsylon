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
    
    bool exe_paths:: ok(const string &fn)
    {
        static const vfs   &fs   = local_fs::instance();
        return fs.is_reg(fn);
    }

    
#if 0
    const exe_paths::ro_strings & exe_paths:: operator()(const string &name) const
    {
        static const vfs   &fs   = local_fs::instance();
        exes.free();
        const size_t ndir = dirs.size();
        for(size_t i=1;i<=ndir;++i)
        {
            const string fn = dirs[i] + name;
            if(fs.is_reg(fn))
            {
                //std::cerr << "found '" << fn << "'" << std::endl;
                exes << fn;
            }
        }
        return exes;
    }
    
    const exe_paths::ro_strings & exe_paths:: operator()(const char *name) const
    {
        const string _(name); return (*this)(_);
    }
#endif
    
}
