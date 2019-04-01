#include "y/fs/vfs.hpp"
#include <cstring>

namespace upsylon {
	
    bool vfs::entry:: is_regular()   const throw()
    {
        return is_reg == attr;
    }
    
    bool vfs::entry:: is_directory()   const throw()
    {
        return is_dir == attr;
    }
    
    bool vfs:: entry:: is_dot()  const throw()
    {
        assert(base_name);
        return is_dir == attr && 0 == strcmp(".",base_name);
    }
    
    bool vfs:: entry:: is_ddot()  const throw()
    {
        assert(base_name);
        return is_dir == attr && 0 == strcmp("..",base_name);
    }
    
    bool vfs::entry:: is_dot_or_ddot() const throw()
    {
        assert(base_name);
        return is_dir == attr && ( 0 == strcmp(".",base_name) || 0 == strcmp("..",base_name));
    }
    
    bool vfs::entry::  is_subdir() const throw()
    {
        return (is_dir == attr) && (0 != strcmp(".",base_name)) && (0 != strcmp("..",base_name));
    }
    
    
    vfs::entry:: entry( const string &vfs_path, const vfs &vfs_from ) :
    path( vfs_path ),
    cstr( *path    ),
    base_name( vfs::get_base_name( path ) ),
    extension( vfs::get_extension( path ) ),
    link( false ),
    attr( vfs_from.query_attribute( path,  *(bool *)&link ) )
    {
    }
    
    vfs::entry:: ~entry() throw() {
    }
    
    
    vfs:: entry:: entry( const entry &other ) :
    path( other.path ),
    cstr( *path      ),
    base_name( vfs::get_base_name( path ) ),
    extension( vfs::get_extension( path ) ),
    link( other.link ),
    attr( other.attr )
    {
        
    }
    
    bool vfs::entry:: has_extension( const string &ext ) const throw()
    {
        return (extension!=0) && (strcmp( *ext, extension ) == 0);
    }
    
    bool vfs::entry:: has_extension( const char *ext ) const throw()
    {
        if( 0 == ext )
        {
            return (0 == extension);
        }
        else
        {
            return (extension!=0) && (strcmp(ext,extension) == 0);
        }
    }

    const char * vfs::entry:: get_attr_text(const attribute a) throw()
    {
        switch (a) {
            case no_ent: return "no  entry";
            case is_dir: return "directory";
            case is_reg: return " regular ";
            case is_unk: return " unknown ";
            default:
                break;
        }
        return "unexpected";
    }

    const char * vfs::entry:: attr2text() const throw()
    {
        return get_attr_text(attr);
    }

#if 0
    void vfs:: foreach_in( const string &dirname,  vfs::entry::callback &on_entry ) const
    {
        scanner *scan = new_scanner( dirname );
        try
        {
            const entry *ep = scan->next();
            while( NULL != ep)
            {
                if( !on_entry( *ep ) ) return;
                ep = scan->next();
            }
            delete scan;
        }
        catch(...)
        {
            delete scan;
            throw;
        }
        
    }
#endif
	
}
