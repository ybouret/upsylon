////////////////////////////////////////////////////////////////////////////////
//
// very crude resources assembler
//
////////////////////////////////////////////////////////////////////////////////

#include "y/program.hpp"
#include "y/ios/rc/io.hpp"
#include "y/fs/local.hpp"
#include "y/lang/pattern/matching.hpp"

using namespace upsylon;

////////////////////////////////////////////////////////////////////////////////
//
// make global path to local path using base name
//
////////////////////////////////////////////////////////////////////////////////
static inline
string make_resource_name(const string &parent,
                          const string &path )
{
    const string b_name = vfs::get_base_name(path);
    if(parent.size()>0)
    {
        return vfs::to_directory(parent) + b_name;
    }
    else
    {
        return b_name;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// recursive call
//
////////////////////////////////////////////////////////////////////////////////
static inline
void add_rc(ios::rc::writer &rc,
            const string    &parent,
            const string    &path,
            const vfs       &fs,
            Lang::Matching  &exclude)
{

    vfs::entry ep(path,fs);
    if(exclude.partly(path))
    {
        return;
    }

    if(ep.is_regular())
    {
        //======================================================================
        //
        // add the file
        //
        //======================================================================
        const string identifier = make_resource_name(parent,ep.path);
        rc.append_file(identifier,path);
        return;
    }

    if( ep.is_subdir() )
    {
        //======================================================================
        //
        // add recursively the directory
        //
        //======================================================================
        const string           child = make_resource_name(parent,ep.path);
        auto_ptr<vfs::scanner> scan  = fs.new_scanner(ep.path);
        for(const vfs::entry  *sub   = scan->next(); sub; sub=scan->next() )
        {
            add_rc(rc,child,sub->path,fs,exclude);
        }
        return;
    }


}

Y_PROGRAM_START()
{
    //==========================================================================
    //
    // check if a target is given
    //
    //==========================================================================
    if(argc<=1)
    {
        return 0;
    }
    else
    {
        Lang::Matching exclude( "[.]svn" );

        const string filename = argv[1];
        ios::rc::writer rc(filename,true);
        string parent = "";
        for(int iarg=2;iarg<argc;++iarg)
        {
            const string arg = argv[iarg];
            add_rc(rc,parent,arg,local_fs::instance(),exclude);
        }
        rc.finalize();
        return 0;
    }

}
Y_PROGRAM_END()

