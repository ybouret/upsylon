//! \file

#ifndef Y_LANG_PATTERN_VFS_MATCHING_INCLUDED
#define Y_LANG_PATTERN_VFS_MATCHING_INCLUDED 1

#include "y/lang/pattern/matching.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! matchin a vfs::entry name or path
        class MatchVFS : public MatchString
        {
        public:
            //! operating mode
            enum Mode
            {
                ByName, //!< test base name
                ByPath  //!< test full path
            };

            virtual ~MatchVFS() throw();                         //!< destructor
            MatchVFS(const string &rx, const Mode m = ByName);   //!< setup
            MatchVFS(const char   *rx, const Mode m = ByName );  //!< setup
            MatchVFS(const MatchVFS &);                          //!< copy
            bool operator()( const vfs::entry &e );              //!< true is Name/Path is matching

            Mode mode;                                           //!< the current operating mode
            bool verbose;                                        //!< verbosity, default=off

        private:
            Y_DISABLE_ASSIGN(MatchVFS);
        };
    }
}

#endif

