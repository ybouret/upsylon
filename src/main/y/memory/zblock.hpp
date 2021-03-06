//! \file

#ifndef Y_MEMORY_ZBLOCK_INCLUDED
#define Y_MEMORY_ZBLOCK_INCLUDED 1

#include "y/sequence/addressable.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/aliasing.hpp"
#include "y/os/oor.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! flexible local memory
    //
    //__________________________________________________________________________
    template <typename T,class ALLOCATOR>
    class zblock : public memory::rw_buffer, public addressable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type); //!< aliases
        
        //______________________________________________________________________
        //
        // C++ resources
        //______________________________________________________________________

        //! cleanup
        inline virtual ~zblock() throw()
        {
            static ALLOCATOR &mgr = ALLOCATOR::location();
            mgr.release( *(void **)&entry, owned);
            shift = 0;
            aliasing::_(count) = 0;
            aliasing::_(bytes) = 0;
        }

        //! setup
        inline explicit zblock(const size_t required) :
        count(required<1?1:required),
        bytes(count*sizeof(type)),
        owned(bytes),
        entry( static_cast<mutable_type*>(ALLOCATOR::instance().acquire(owned)) ),
        shift( aliasing::prev(entry) )
        {
            assert( aliasing::delta(shift,entry) == sizeof(type) );
        }

        //______________________________________________________________________
        //
        // addressable interface
        //______________________________________________________________________

        //! access[1..size()=count]
        inline virtual type & operator[](const size_t indx) throw()
        {
            assert(indx>0); assert(indx<=size());
            return shift[indx];
        }

        //! access[1..size()=count], const
        inline virtual const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>0); assert(indx<=size());
            return shift[indx];
        }

        //! size() = count
        inline virtual size_t size() const throw() { return count; }

        //______________________________________________________________________
        //
        // rw buffer public interface
        //______________________________________________________________________
        //! length = bytes
        inline virtual size_t       length() const throw() { return bytes; }

        //! ro() = entry
        inline virtual const void * ro()     const throw() { return entry; }

        //______________________________________________________________________
        //
        // specific interface
        //______________________________________________________________________

        //! first item address = entry
        inline type *       operator*() throw()       { return entry; }

        //! first item address = entry
        inline const_type * operator*() const throw() { return entry; }

        //! cleanup
        inline void clear() throw()
        {
            out_of_reach::fill(entry,0,bytes);
        }

        //! no-throw swap
        inline void swap_with(zblock &other) throw()
        {
            _cswap(count,other.count);
            _cswap(bytes,other.bytes);
            _cswap(owned,other.owned);
            _cswap(entry,other.entry);
            _cswap(shift,other.shift);
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t count; //!< number of zombie objects
        const size_t bytes; //!< bytes for those zombies

    private:
        Y_DISABLE_COPY_AND_ASSIGN(zblock);
        size_t        owned;
        mutable_type *entry;
        mutable_type *shift;
    };

}

#endif
