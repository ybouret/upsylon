
//! \file
#ifndef Y_MEMORY_TIGHT_WEDGE_INCLUDED
#define Y_MEMORY_TIGHT_WEDGE_INCLUDED 1

#include "y/code/round.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {

        namespace tight {

            //__________________________________________________________________
            //
            //
            //! common ops for wedge<>
            //
            //__________________________________________________________________
            struct __wedge
            {
                //! display run_time_warning
                static void leak(const size_t class_size, const size_t count) throw();
            };

            //__________________________________________________________________
            //
            //
            //! wedge: flexible provider of same object memory
            //
            //__________________________________________________________________
            template <typename T>
            class wedge
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const size_t class_size = sizeof(T);                           //!< class size
                static const size_t block_size = Y_ALIGN_FOR_ITEM(size_t,class_size); //!< size_t boundary
                static const size_t block_step = block_size/sizeof(size_t);           //!< number of size_t to store class


                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup from given, persistent memory
                inline wedge(void *       chunk_data,
                             const size_t chunk_size) throw() :
                first_available(0),
                data( static_cast<size_t*>(chunk_data) ),
                still_available(chunk_size/block_size),
                provided_number(still_available),
                last(data+provided_number*block_step)
                {
                    for(size_t q=0,*p=data; q!=provided_number; p += block_step)
                    {
                        assert(owns(p));
                        *p = ++q;
                    }
                }

                //! cleanup with warning
                inline ~wedge() throw()
                {
                    if(!is_empty()) __wedge::leak(class_size,provided_number-still_available);
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! detect ownership
                inline bool owns(const void *addr) const throw()
                {
                    const size_t *p = static_cast<const size_t *>(addr);
                    return (p>=data) && (p<last);
                }

                //! check is no block is allocated
                inline bool is_empty() const throw()
                {
                    return still_available>=provided_number;
                }

                //! get a block
                inline T *acquire() throw()
                {
                    // sanity check
                    assert(still_available>0);
                    assert(still_available<=provided_number);

                    // return object
                    size_t *p = &data[ first_available*block_step ];  // get address
                    first_available = *p;                             // read next available address
                    --still_available;                                // bookeeping
                    memset(p,0,block_size);                           // zero memory
                    return (T*)p;                                     // done
                }

                //! restore a block
                inline void release(T *addr) throw()
                {
                    assert(addr);
                    assert(owns(addr));
                    assert(still_available<provided_number);

                    // restore linking
                    size_t *to_release = (size_t *)addr;
                    *to_release = first_available;
                    const size_t indx = static_cast<size_t>(to_release-data)/block_step;
                    first_available   = indx;
                    ++still_available;
                }

                

                size_t               first_available; //!< for internal status
                size_t              *data;            //!< first entry
                size_t               still_available; //!< blocks ready for use
                const size_t         provided_number; //!< initial number of blocks
                const size_t * const last;            //!< invalid address

            private:
                Y_DISABLE_COPY_AND_ASSIGN(wedge);
            };

        }
    }
}


#endif
