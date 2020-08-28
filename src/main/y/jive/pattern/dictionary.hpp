//! \file

#ifndef Y_JIVE_PATTERN_DICT_INCLUDED
#define Y_JIVE_PATTERN_DICT_INCLUDED 1

#include "y/jive/pattern.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Jive {

        //______________________________________________________________________
        //
        //
        //! base class for dictionary
        //
        //______________________________________________________________________
        typedef suffix_tree<const Motif> DictionaryType;

        //______________________________________________________________________
        //
        //
        //! dictionary of patterns
        //
        //______________________________________________________________________
        class Dictionary : public DictionaryType
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Dictionary();         //!< setup
            virtual ~Dictionary() throw(); //!< cleanup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
        };

    }

}

#endif
