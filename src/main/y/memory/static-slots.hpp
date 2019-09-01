// !\file
#ifndef Y_MEMORY_STATIC_SLOTS_INCLUDED
#define Y_MEMORY_STATIC_SLOTS_INCLUDED 1

#include "y/memory/io.hpp"
#include "y/type/args.hpp"
#include "y/code/round.hpp"
#include "y/type/self-destruct.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        
        //! static slots to build local objects
        template <
        typename T,
        const size_t LENGTH,
        const size_t OFFSET = 0
        >
        class static_slots
        {
        public:
            Y_DECL_ARGS(T,type);                    //!< alias
            static const size_t capacity = LENGTH;  //!< maximum items
            static const size_t offset   = OFFSET;  //!< initial index for access
            static const size_t bytes    = LENGTH * sizeof(type); //!< minimal memory bytes

            //! setup
            explicit static_slots() throw() :
            wksp(),
            addr( io::__force<mutable_type>(wksp)-OFFSET ),
            size(0)
            {
                clr();
            }
            
            //! cleanup
            inline ~static_slots() throw()
            {
                free();
                addr = 0;
            }
            
            //! push back new object
            inline void push_back( param_type args )
            {
                assert(size<capacity);
                new ( &addr[OFFSET+size] ) mutable_type(args);
                ++(size_t&)size;
            }
            
            //! access
            inline type & operator[](const size_t indx) throw()
            {
                assert(indx>=OFFSET);
                assert(indx<OFFSET+size);
                return addr[indx];
            }
            
            //! access
            inline const_type & operator[](const size_t indx) const throw()
            {
                assert(indx>=OFFSET);
                assert(indx<OFFSET+size);
                return addr[indx];
            }
            
            //! free all objects
            inline void free() throw()
            {
                mutable_type *temp = io::__force<mutable_type>(wksp);
                while(size>0)
                {
                    self_destruct( temp[--(size_t&)size ] );
                }
                clr();
            }
            
        private:
            uint64_t      wksp[ Y_U64_FOR_SIZE(bytes) ];
            mutable_type *addr;
            Y_DISABLE_COPY_AND_ASSIGN(static_slots);
            inline void clr() throw() { memset(wksp,0,sizeof(wksp) ); }
            
        public:
            const size_t size; //!< current number of items
            
        };
        
    }
}

#endif

