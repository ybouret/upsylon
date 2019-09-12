//! \file
#ifndef Y_FIELD_ACTIVE_INCLUDED
#define Y_FIELD_ACTIVE_INCLUDED 1

#include "y/oxide/field/set.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        typedef sorted_vector<FieldPointer>   OrderedFields;
        typedef ordered_single<OrderedFields> ActiveFieldsType;
        
        class ActiveFields : public ActiveFieldsType
        {
        public:
            static const char Separator = ';';
            
            ActiveFields() throw();
            virtual ~ActiveFields() throw();
            ActiveFields(const ActiveFields &);
            
            ActiveFields  & operator<<( Field     &);
            ActiveFields  & operator()( Fields    &);
            ActiveFields  & operator()( Fields &, const string IDs);

            comm_mode getCommMode() const throw();

        private:
            Y_DISABLE_ASSIGN(ActiveFields);
        };
        
    }
    
}

#endif

