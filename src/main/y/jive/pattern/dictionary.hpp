//! \file

#ifndef Y_JIVE_PATTERN_DICT_INCLUDED
#define Y_JIVE_PATTERN_DICT_INCLUDED 1

#include "y/jive/regexp.hpp"
#include "y/suffix/storage.hpp"

namespace upsylon {

    namespace Jive {

        //______________________________________________________________________
        //
        //
        //! base class for dictionary
        //
        //______________________________________________________________________
        typedef suffix_storage<const Motif> DictionaryType;

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
            bool store(const NAME &id, Pattern *p)
            {
                assert(p);
                const Motif m(p);
                return insert(id,m);
            }

            //! search const pattern
            template <typename NAME> inline
            const Pattern *query(const NAME &id) const throw()
            {
                const Motif *pMotif = search(id);
                return pMotif ? & (**pMotif) : NULL;
            }

            //! auto generation of patterns
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
