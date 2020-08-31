//! \file

#ifndef Y_JIVE_PATTERN_DICT_INCLUDED
#define Y_JIVE_PATTERN_DICT_INCLUDED 1

#include "y/jive/regexp.hpp"
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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! generic secured insertion of a new pattern
            template <typename NAME> inline
            bool insert(const NAME &id, Pattern *p)
            {
                assert(p);
                const Motif m(p);
                return insert_by(id,m);
            }

            //! search const pattern
            template <typename NAME> inline
            const Pattern *search(const NAME &id) const throw()
            {
                const Motif *pMotif = search_by(id);
                return pMotif ? & (**pMotif) : NULL;
            }

            template <typename NAME,typename REGEXP> inline
            bool use(const NAME &id, const REGEXP &rx)
            {
                return insert(id, RegExp(rx,this) );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);

        };

    }

}

#endif
