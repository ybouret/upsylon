
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

        
    }
}
