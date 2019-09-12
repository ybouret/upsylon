
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
        dynamic(), ActiveFieldsType(other)
        {
        }
        
        ActiveFields:: ~ActiveFields() throw()
        {
        }
        
        ActiveFields  &  ActiveFields::  operator<<( Field &ft )
        {
            const FieldPointer tmp = &ft;
            insert(tmp);
            return *this;
        }
        
        ActiveFields  &  ActiveFields::  operator()( Fields &fields )
        {
            free();
            ensure(fields.size());
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
                const string id( tkn.token(), tkn.units() );
                (*this) << fields[id];
            }
            return *this;
        }


        comm_mode ActiveFields:: getCommMode() const throw()
        {
            iterator  it = begin();
            size_t    n  = size();
            while(n-->0)
            {
                switch( (**it).transfer->mode )
                {
                    case comm_variable_size: return comm_variable_size;
                    default: break;
                }
                ++it;
            }
            return comm_constant_size;
        }

        size_t ActiveFields:: getBlockSize() const throw()
        {
            iterator  it = begin();
            size_t    n  = size();
            size_t    bs = 0;
            while(n-->0)
            {
                const Field &F = **it;
                switch( F.transfer->mode )
                {
                    case comm_variable_size: return 0;
                    default: break;
                }
                bs += F.sizeOfObject;
                ++it;
            }
            return bs;
        }


        
    }
    
}
