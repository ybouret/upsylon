#include "y/sparse/array.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace sparse
    {
        array_info:: ~array_info() throw()
        {
            object:: operator delete(item_,itsz_);
            item_ = 0;
            itsz_ = 0;
            size_ = 0;
        }

        array_info:: array_info(const size_t n, const size_t item_size)  :
        size_(n),
        itsz_( item_size ),
        item_( object:: operator new(itsz_) )
        {
        }

        static const char fn[] = "sparse_array: ";

        void array_info:: check_index(const size_t n) const
        {
            if(n<=0||n>size_) throw libc::exception(ERANGE,"%s%u not in [1:%u]", fn, unsigned(n), unsigned(size_));
        }

        void array_info:: insert_failure(const size_t i) const
        {
            throw exception("%sunexpected failure while inserting item@%u", fn, unsigned(i));
        }
    }
}
