#include "y/fs/vfs.hpp"

#include <cstring>
#include <new>

namespace upsylon
{
	
    
    vfs:: scanner:: ~scanner() throw()
    {
    }
    
    
    vfs:: scanner:: scanner( const string &dirname, const vfs &owner ) :
    fs( owner   ),
    folder( dirname ),
    current(0)
    {
        vfs::as_directory((string&)folder);
    }
    
    
    
    const vfs::entry * vfs:: scanner:: make_entry(const char *entry_name)
    {
        assert( length_of(entry_name)>0 );
        
        string      full_path = folder + entry_name;
        vfs::as_directory( full_path ).trim( 1 );

        current = new vfs::entry(full_path,fs);
        return & *current;
        //data.build<vfs::entry,string,vfs>(full_path,fs);
        //return & data.as<vfs::entry>();
    }
   
	
}
