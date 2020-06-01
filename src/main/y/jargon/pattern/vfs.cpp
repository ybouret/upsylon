
#include "y/jargon/pattern/vfs.hpp"
#include "y/fs/find.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        VFS_Matcher:: ~VFS_Matcher() throw()
        {
        }
        
        VFS_Matcher:: VFS_Matcher( const VFS_Matcher &other ) throw() :
        Matcher(other)
        {
        }
        

        namespace {

            struct findOps
            {
                size_t            num;
                sequence<string> *seq;
                VFS_Matcher      *self;
                
                
                inline void operator()(const vfs::entry &ent)
                {
                    assert(self);
                    if( ent.is_regular() && self->matches_exactly(ent.extension))
                    {
                        ++num;
                        if(seq) seq->push_back(ent.path);
                    }
                }
            };

        }

        size_t VFS_Matcher:: extensions(sequence<string> *seq,
                                        vfs              &fs,
                                        const string     &dirName,
                                        const int         maxDepth)
        {
            findOps ops = { 0, seq , this };
            fs_find::in(fs,dirName,ops,maxDepth);
            return ops.num;
        }
    }
}
