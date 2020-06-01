
//! \file

#ifndef Y_JARGON_PATTERN_VFS_INCLUDED
#define Y_JARGON_PATTERN_VFS_INCLUDED 1

#include "y/jargon/pattern/matcher.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! tools for VFS manipulation
        //
        //----------------------------------------------------------------------
        class VFS_Matcher : public Matcher
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~VFS_Matcher() throw();
            
            //! setup
            template <typename REGEXP> inline
            VFS_Matcher(const REGEXP &rx, const Dictionary *dict = NULL ) :
            Matcher(rx,dict)
            {
            }
            
            //! copy
            VFS_Matcher(const VFS_Matcher &) throw();
            
            //! try to match extension
            bool matchExtensionOf(const vfs::entry &entry);
            
            
        private:
            Y_DISABLE_ASSIGN(VFS_Matcher);
        };
        
    }
}

#endif

