#include "y/fs/vfs.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/list.hpp"

#include <iostream>

namespace upsylon
{
	
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
    
    bool vfs::is_reg( const string &path ) const throw() {
        bool link = false;
        return entry::is_reg == this->query_attribute( path, link );
    }
    
    bool vfs::is_dir( const string &path ) const throw() {
        bool link = false;
        return entry::is_dir == this->query_attribute( path, link );
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
    
}
