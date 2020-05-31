
//! \file

#ifndef Y_JARGON_PATTERN_VFS_INCLUDED
#define Y_JARGON_PATTERN_VFS_INCLUDED 1

#include "y/jargon/pattern/matcher.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        class VFS_Matcher : public Matcher
        {
        public:
            virtual ~VFS_Matcher() throw();
            
            template <typename REGEXP> inline
            VFS_Matcher(const REGEXP &rx, const Dictionary *dict = NULL ) :
            Matcher(rx,dict)
            {
            }
            
            VFS_Matcher(const VFS_Matcher &) throw();
            
            bool matchExtensionOf(const vfs::entry &entry);
            
            
        private:
            Y_DISABLE_ASSIGN(VFS_Matcher);
        };
        
    }
}

#endif

