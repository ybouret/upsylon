#ifndef Y_SEQUENCE_ARRAY_INCLUDED
#define Y_SEQUENCE_ARRAY_INCLUDED 1

#include "y/dynamic.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    //! array of contiguous objects
    template <typename T>
    class array : virtual public dynamic
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! destructor
        inline virtual ~array() throw() {}

        //! dynamic interface: size()
        inline virtual size_t size() const throw() { return size_; }

        //! inline access
        inline type       & operator[](const size_t i) throw() { assert(i>0);assert(i<=size()); assert(item_); return item_[i]; }

        //! inline const access
        inline const_type & operator[](const size_t i) const throw() { assert(i>0);assert(i<=size()); assert(item_); return item_[i]; }

    protected:
        mutable_type *item_; //!< item_[1..size()]
        size_t        size_; //!< size()
        inline array() throw() : item_(0), size_(0) {}
        inline array(T *user_item,const size_t user_size) throw() :
        item_(user_item),
        size_(user_size)
        {

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(array);
    };
}

#endif

