//! \file

#ifndef Y_IOS_SCRIBES_INCLUDED
#define Y_IOS_SCRIBES_INCLUDED 1

#include "y/ios/scribe/native.hpp"
#include "y/associative/suffix/set.hpp"

namespace upsylon
{
    namespace ios
    {
        class scribes : public singleton<scribes>
        {
        public:
            typedef intr_ptr<rtti,scribe>          scribe_handle;
            typedef suffix_set<rtti,scribe_handle> scribes_store;
            typedef scribes_store::const_iterator  iterator;

            typedef intr_ptr<rtti,native_scribe>   native_handle;
            typedef suffix_set<rtti,native_handle> natives_store;
            typedef natives_store::const_iterator  native_iterator;


            const scribes_store all;
            const natives_store nat;

            void insert_common(scribe        *);
            void insert_native(native_scribe *);

            const native_scribe &native(const std::type_info &) const;
            template <typename T> inline
            const native_scribe &native() const { return native( typeid(T) ); }

            const scribe &get(const std::type_info &) const;

            template <typename T> inline
            const scribe &get() const { return get( typeid(T) ); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribes);
            Y_SINGLETON_DECL_WITH(0,scribes);
            explicit scribes();
            virtual ~scribes() throw();
            
        };
    }

}

#endif

