//! \file

#ifndef Y_OXIDE_FIELD_DATA_INCLUDED
#define Y_OXIDE_FIELD_DATA_INCLUDED 1
#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        template <typename T>
        class FieldData
        {
        public:
            Y_DECL_ARGS(T,type);
            type      *entry;
            

            inline  ~FieldData() throw() { cleanup(); }
            
            inline explicit FieldData(void        *addr,
                                      const size_t size) :
            entry( static_cast<type*>(addr) ),
            built(0)
            {
                assert( !(NULL==addr && size>0 ) );

                try
                {
                    mutable_type *p = static_cast<mutable_type*>(addr);
                    while(built<size)
                    {
                        new (p+built) T();
                        ++built;
                    }
                }
                catch(...)
                {
                    cleanup();
                    throw;
                }
            }


        private:
            size_t     built;
            
            inline void cleanup() throw()
            {
                while(built>0)
                {
                    mutable_type *p = (mutable_type *) &entry[--built];
                    destruct(p);
                }
            }

            Y_DISABLE_COPY_AND_ASSIGN(FieldData);
        };

    }
}

#endif

