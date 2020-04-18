//! \file
#ifndef Y_FIELD_ACTIVE_INCLUDED
#define Y_FIELD_ACTIVE_INCLUDED 1

#include "y/oxide/field/set.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        typedef sorted_vector<FieldPointer>   OrderedFields;     //!< base ordered class
        typedef ordered_single<OrderedFields> ActiveFieldsType;  //!< bases ordered policy class

        //! list of active fields for comms
        /**
         the policy is: silently discard multiple fields
         */
        class ActiveFields : public ActiveFieldsType
        {
        public:
            static const char Separator = ';'; //!< separator for fields names
            
            explicit ActiveFields() throw();    //!< setup
            virtual ~ActiveFields() throw();    //!< cleanup
            ActiveFields(const ActiveFields &); //!< copy
            
            ActiveFields  & operator<<( Field     &); //!< add a field
            ActiveFields  & operator()( Fields    &); //!< add all fields
            ActiveFields  & operator()( Fields &, const string IDs); //!< add some fields name, with separators

            comms::manner getCommMode() const throw();  //!< analyze global comm_mode
            size_t        getBlockSize() const throw(); //!< analyze blockSize, 0 for variable sized transfer

        private:
            Y_DISABLE_ASSIGN(ActiveFields);
        };
        
    }
    
}

#endif

