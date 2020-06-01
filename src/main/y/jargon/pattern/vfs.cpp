
#include "y/jargon/pattern/vfs.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        VFS_Matcher:: ~VFS_Matcher() throw()
        {
        }
        
        VFS_Matcher:: VFS_Matcher( const VFS_Matcher &other ) throw() :
        Matcher(other)
        {
        }
        
        bool VFS_Matcher:: matchExtensionOf(const vfs::entry &entry)
        {
            Module *module = Module::OpenData(entry.extension);
            return (**this).exactly_matches(*this,module);
        }

        namespace {

            struct findInfo
            {
                size_t            num;
                sequence<string> *seq;
                VFS_Matcher      *self;

                void call( const vfs::entry &ent )
                {
                    assert(self);
                    if(self->matchExtensionOf(ent))
                    {
                        ++num;
                        if(seq) seq->push_back(ent.path);
                    }
                }

                static void Call( const vfs::entry &ent, void *args )
                {
                    assert(args);
                    static_cast<findInfo *>(args)->call(ent);
                }
            };

        }

        size_t VFS_Matcher:: matchExtensions(sequence<string> *seq,
                                             vfs              &fs,
                                             const string     &dirName,
                                             const int         maxDepth)
        {
            findInfo info = { 0, seq , this };
            fs.find(dirName, findInfo::Call, &info, maxDepth);
            return info.num;
        }
    }
}
