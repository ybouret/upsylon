//! \file
#ifndef Y_ARRAYS_INCLUDED
#define Y_ARRAYS_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon {

    //! multiple arrays of same length
    template <typename T,typename ALLOCATOR = memory::global>
    class arrays
    {
    public:
        Y_DECL_ARGS(T,type);                     //!< type alias
        typedef lightweight_array<T> array_type; //!< array interface alias

        const size_t count; //!< count of individual arrays
        
        //! prepare memory for num_arrays, with possible already num_items
        inline explicit arrays(const size_t num_arrays,
                               const size_t array_size=0) :
        count(num_arrays),
        hmem_( ALLOCATOR::instance() ),
        _count(count),
        _bytes(0),
        _array( hmem_. template acquire_as<array_type>(_count,_bytes) ),
        sizes(0),
        items(0),
        bytes(0),
        entry(0),
        indx(0)
        {
            try { acquire(array_size); }
            catch(...)
            {
                hmem_. template release_as<array_type>(_array,_count,_bytes);
                throw;
            }
        }

        //! destructor
        inline virtual ~arrays() throw()
        {
            __release();
            hmem_. template release_as<array_type>(_array,_count,_bytes);
        }

        //! release current memory, reset arrays
        inline void release() throw()
        {
            __release();
            format();
        }

        //! acquire new memory, reset arrays
        inline void acquire(const size_t array_size)
        {
            // release old memory
            release();

            // get new memory
            sizes = array_size;
            items = count * sizes;
            bytes = items * sizeof(T);
            try
            {
                entry = hmem_.acquire(bytes);
            }
            catch(...)
            {
                sizes = items = 0;
                throw;
            }

            //! build items
            size_t built=0;
            try
            {
                mutable_type *dat = static_cast<mutable_type *>(entry);
                while(built<items)
                {
                    new (dat+built) type();
                    ++built;
                }
            }
            catch(...)
            {
                __free(built);
                hmem_.release(entry,bytes);
                sizes=items=0;
                throw;
            }

            // format arrays
            format();
        }

        //! one time raw get new array
        inline array_type & next() throw()
        {
            assert(indx<count);
            return _array[indx++];
        }

        //! raw get array
        const array_type & raw_get( const size_t i ) const throw()
        {
            assert(i<count);
            return _array[i];
        }

        //! virtual get array, default is raw_get
        virtual const array_type &get_array( const size_t i) const throw()
        {
            return raw_get(i);
        }

        //! array access
        inline array_type & operator[](const size_t i) throw()
        {
            return (array_type&)get_array(i);
        }

        //! array access, const
        inline const array_type & operator[](const size_t i) const throw()
        {
            return get_array(i);
        }
        
        //! common size
        inline size_t size() const throw() { return sizes; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(arrays);
        memory::allocator& hmem_;
        size_t            _count;
        size_t            _bytes;
        array_type *      _array;
        size_t             sizes;
        size_t             items;
        size_t             bytes;
        void  *            entry;
        size_t             indx;

        //! free n built items
        inline void __free(size_t n) throw()
        {
            mutable_type *dat = static_cast<mutable_type *>(entry);
            while(n-->0)
            {
                self_destruct(dat[n]);
            }
        }

        //! release built items
        inline void __release() throw()
        {
            __free(items);
            hmem_. release(entry,bytes);
            sizes = items = 0;
        }

        inline void format() throw()
        {
            assert(_array);
            array_type   *arr = static_cast<array_type   *>(_array);
            if(sizes)
            {
                assert(entry);
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

