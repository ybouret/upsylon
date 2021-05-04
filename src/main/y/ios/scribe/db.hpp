//! \file

#ifndef Y_IOS_SCRIBES_INCLUDED
#define Y_IOS_SCRIBES_INCLUDED 1

#include "y/ios/scribe/native.hpp"
#include "y/associative/suffix/set.hpp"

namespace upsylon
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! global database of scribes
        //
        //______________________________________________________________________
        class scribes : public singleton<scribes>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<rtti,scribe>          scribe_handle;   //!< alias
            typedef suffix_set<rtti,scribe_handle> scribes_store;   //!< alias
            typedef scribes_store::const_iterator  iterator;        //!< alias

            typedef intr_ptr<rtti,native_scribe>   native_handle;   //!< alias
            typedef suffix_set<rtti,native_handle> natives_store;   //!< alias
            typedef natives_store::const_iterator  native_iterator; //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void insert_common(scribe        *); //!< insert a generic scribe
            void insert_native(native_scribe *); //!< insert a native scribe

            //! get a native scribe
            const native_scribe &native(const std::type_info &) const;

            //! get a native scribe, wrapper
            template <typename T> inline
            const native_scribe &native() const { return native( typeid(T) ); }

            //! get any scribe
            const scribe &get(const std::type_info &) const;

            //! get any scribe, wrapper
            template <typename T> inline
            const scribe &get() const { return get( typeid(T) ); }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const scribes_store all; //!< database of all scribed
            const natives_store nat; //!< sub-database of native scribes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribes);
            Y_SINGLETON_DECL_WITH(0,scribes);
            explicit scribes();
            virtual ~scribes() throw();
        };
    }

}

#endif

