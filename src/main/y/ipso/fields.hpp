//! \file
#ifndef Y_IPSO_FIELDS_INCLUDED
#define Y_IPSO_FIELDS_INCLUDED 1

#include "y/ipso/field.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace ipso
    {
        typedef intr_ptr<string,field_info> field_pointer; //!< alias
        typedef set<string,field_pointer>   fields_set;    //!< alias

        //! set a field_info
        class fields : public fields_set
        {
        public:
            explicit fields(const size_t n=0); //!< setup with n as capacity
            virtual ~fields() throw();         //!< destructor

            //! add a new field
            template <typename FIELD>
            FIELD & add( const string &name, typename FIELD::layout_type &L )
            {
                FIELD *F = new FIELD(name,L);
                __add(F);
                return *F;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(fields);
            void __add( field_info *F );
        };
    }
}

#endif

