//! \file
#ifndef Y_ARRAYS_INCLUDED
#define Y_ARRAYS_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon
{
    template <typename T,typename ALLOCATOR = memory::global>
    class arrays
    {
    public:
        Y_DECL_ARGS(T,type);                     //!< type alias
        typedef lightweight_array<T> array_type; //!< array alias

        const size_t count; //!< count of individual arrays

        //! prepare memory for num_arrays, with possible already num_items
        inline explicit arrays(const size_t num_arrays,
                               const size_t num_items=0) :
        count(num_arrays),
        hmem_( ALLOCATOR::instance() ),
        _count(count),
        _array( hmem_. acquire_as<array_type>(count) ),
        sizes(0),
        bytes(0),
        entry(0),
        indx(0)
        {
            try
            {
                acquire(num_items);
            }
            catch(...)
            {
                release_all(); throw;
            }
        }

        //! destructor
        inline virtual ~arrays() throw()
        {
            release_all();
        }

        //! release current memory, reset arrays
        inline void release() throw()
        {
            hmem_. release(entry,bytes);
            sizes = 0;
            reset();
        }

        //! acquire new memory, reset arrays
        inline void acquire(const size_t num_items)
        {
            release();
            sizes = num_items;
            bytes = num_items * count * sizeof(T);
            try
            {
                entry = hmem_.acquire(bytes);
                reset();
            }
            catch(...)
            {
                sizes=0; throw;
            }
        }

        //! one time get new array
        inline array_type & next() throw()
        {
            assert(indx<count);
            return _array[indx++];
        }

        //! array access
        inline array_type & operator[](const size_t i) throw()
        {
            assert(i<count); return _array[i];
        }

        //! array access, const
        inline const array_type & operator[](const size_t i) const throw()
        {
            assert(i<count); return _array[i];
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(arrays);
        memory::allocator& hmem_;
        size_t            _count;
        array_type *      _array;
        size_t             sizes;
        size_t             bytes;
        void  *            entry;
        size_t             indx;

        //! release all memory
        inline void release_all() throw()
        {
            hmem_. release(entry,bytes);
            hmem_. release_as<array_type>(_array,_count);
            sizes = 0;
        }

        inline void reset() throw()
        {
            array_type   *arr = static_cast<array_type   *>(_array);
            if(sizes)
            {
                mutable_type *dat = static_cast<mutable_type *>(entry);
                for(size_t i=0;i<count;++i,dat+=sizes)
                {
                    new ( &arr[i] ) array_type(dat,sizes);
                }
            }
            else
            {
                for(size_t i=0;i<count;++i)
                {
                    new ( &arr[i] ) array_type();
                }
            }
        }

    };
}

#endif

