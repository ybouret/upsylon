#include "y/fs/vfs.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/list.hpp"

#include <iostream>

namespace upsylon {

    vfs::  vfs() throw() {}
    vfs:: ~vfs() throw() {}
    
    vfs::scanner * vfs::scan( const string &dirname ) const
    {
        return new_scanner(dirname);
    }

    vfs::scanner * vfs::scan( const char *dirname ) const
    {
        const string _(dirname);
        return scan(_);
    }


    
    void vfs:: create_sub_dir( const string &dirName )
    {
        string            org = dirName;
        const std::size_t len = vfs::as_directory( org ).size();
        string            sub( len, as_capacity, false );
        std::size_t       idx = 0;
        while( idx < len )
        {
            while( org[idx] != '/' && org[idx] != '\\' ) {
                sub +=  org[idx++];
            }
            sub += org[idx++];
            
            //std::cerr << "sub=[" << sub << "]" << std::endl;
            
            this->create_dir( sub, true );
        }
    }

    void vfs:: create_sub_dir( const char *dirName )
    {
        const string _(dirName);
        create_sub_dir(_);
    }
    
    bool vfs::is_reg( const string &path ) const throw() {
        bool link = false;
        return entry::is_reg == this->query_attribute( path, link );
    }

    bool vfs::is_reg( const char *path ) const throw() {
        const string _(path); return is_reg(_);
    }


    bool vfs::is_dir( const string &path ) const throw() {
        bool link = false;
        return entry::is_dir == this->query_attribute( path, link );
    }

    bool vfs::is_dir( const char *path ) const throw() {
        const string _(path); return is_dir(_);
    }


    namespace
        {
            struct rm_param
            {
                const string *pExt;
                inline bool operator()(const vfs::entry &ep)
                {
                    assert(pExt);
                    return  ep.is_regular() && ep.has_extension( *pExt );
                }
            };
        }

    void vfs:: remove_files_with_extension_in( const string &dirname, const string &extension)
    {
        rm_param cb = { &extension };
        remove_files(dirname,cb);
    }
    
    void vfs::try_remove_file( const string &path) throw()
    {
        try
        {
            remove_file(path);
        }
        catch(...)
        {
            
        }
    }

    void vfs::try_remove_file(const char *path) throw()
    {
        const string _(path);
        try_remove_file(_);
    }


    
}

#include "y/string/temporary-name.hpp"
#include "y/exception.hpp"

namespace upsylon {

    string vfs:: get_temporary_name(const size_t n) const
    {
        static const unsigned nmax = 32;
        unsigned ntry = 0;
        while(ntry<nmax)
        {
            ++ntry;
            const string     tmp    = temporary_name::create(n);
            bool             isLink = false;
            entry::attribute attr   = query_attribute(tmp,isLink);
            if(isLink||attr!=entry::no_ent) continue;
            return tmp;
        }
        throw exception("vfs::get_temporary_name(failure after %u trials)",nmax);
    }

}
