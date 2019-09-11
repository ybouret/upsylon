
#include "y/oxide/field/active.hpp"
#include "y/string/tokenizer.hpp"

namespace upsylon
{
    namespace Oxide
    {
        ActiveFields:: ActiveFields() throw()
        {
        }
        
        ActiveFields:: ActiveFields( const ActiveFields &other) :
        ActiveFieldsType(other)
        {
        }
        
        ActiveFields:: ~ActiveFields() throw()
        {
        }
        
        ActiveFields  &  ActiveFields::  operator<<( FieldType &ft )
        {
            const FieldPointer tmp = &ft;
            insert(tmp);
            return *this;
        }
        
        ActiveFields  &  ActiveFields::  operator<<( Fields &fields )
        {
            for( Fields::iterator it=fields.begin(); it != fields.end(); ++it)
            {
                insert( (*it).field );
            }
            return *this;
        }
        
        ActiveFields  & ActiveFields:: operator()( Fields &fields, const string IDs)
        {
            tokenizer<char> tkn(IDs);
            while( tkn.next_with(Separator) )
            {
                const string id( tkn.token(), tkn.count() );
                (*this) << fields[id];
            }
            return *this;
        }

        
    }
    
}
