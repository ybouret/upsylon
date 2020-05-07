#include "y/oxide/field/active.hpp"
#include "y/string/tokenizer.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        ActiveFields:: ActiveFields() throw()
        {
        }
        
        ActiveFields:: ActiveFields( const ActiveFields &other) : collection(), ActiveFieldsType(other)
        {
        }
        
        ActiveFields:: ~ActiveFields() throw()
        {
        }
        
        ActiveFields  &  ActiveFields::  operator<<( Field &ft )
        {
            if(ft.refcount()<=0) throw exception("ActiveFields: '%s' is not dynamic", *ft.name);
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


        comms::delivery ActiveFields:: getCommMode() const throw()
        {
            iterator  it = begin();
            size_t    n  = size();
            while(n-->0)
            {
                switch( (**it).transfer->mode )
                {
                    case comms:: flexible_block_size: return comms::flexible_block_size;
                    default: break;
                }
                ++it;
            }
            return comms::computed_block_size;
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
                    case comms::flexible_block_size: return 0;
                    default: break;
                }
                bs += F.sizeOfObject;
                ++it;
            }
            return bs;
        }


        
    }
    
}
