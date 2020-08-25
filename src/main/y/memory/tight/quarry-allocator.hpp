
//! \file

#ifndef Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED 1

#include "y/memory/tight/exp2-allocator.hpp"
#include "y/type/args.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {


            //__________________________________________________________________
            //
            //
            //! allocator with bytes=2^shift markers
            //
            //__________________________________________________________________
            class quarry_allocator : public exp2_allocator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                
                //! set up with permanent user's data
                explicit quarry_allocator(lockable &usr_access,
                                          quarry   &usr_quarry) throw();

                //! cleanup
                virtual ~quarry_allocator() throw();

                //______________________________________________________________
                //
                // exp2_allocator interface
                //______________________________________________________________
                
                //! acquire next power of two bytes=2^shift, clean memory
                virtual void *acquire(size_t &bytes, size_t &shift);
                
                //! release a previously allocated block
                virtual void  release(void *&addr, size_t &bytes, size_t &shift) throw();

                //! do nothing
                virtual void gc() throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry_allocator);
                void *fetch_locked(const size_t);
            };
            
            //__________________________________________________________________
            //
            //
            //! smart pointer for a field
            //
            //__________________________________________________________________
            template <typename T>
            class quarry_field
            {
            public:
                Y_DECL_ARGS(T,type); //!< aliasing
                
                //! acquire resource, keep track of allocator
                inline explicit quarry_field(quarry_allocator &mgr,const size_t required) :
                qhost(mgr),
                count(required),
                bytes(0),
                shift(0),
                field( qhost.acquire_field<mutable_type>(count, bytes, shift) )
                {
                }
                
                //! release resource
                inline virtual ~quarry_field() throw()
                {
                    qhost.release_field(field,count,bytes,shift);
                }
                
                //! access to memory field
                inline type *       operator*() throw()       { return field; }
                
                //! access to memory field, const
                inline const_type * operator*() const throw() { return field; }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry_field);
                quarry_allocator &qhost;
                size_t            count;
                size_t            bytes;
                size_t            shift;
                mutable_type     *field;
            };

        }

    }
}

#endif
