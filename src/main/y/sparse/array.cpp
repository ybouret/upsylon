#include "y/sparse/array.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace sparse
    {
        array_info:: ~array_info() throw()
        {
            size_ = 0;
        }

        array_info:: array_info(const size_t n)  :
        size_(n)
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

        void array_info:: create_failure(const size_t i) const
        {
            throw exception("%smultiple indices=%u", fn, unsigned(i));
        }
    }
}
