//! \file
#ifndef Y_SEQUENCE_ARRAY_INCLUDED
#define Y_SEQUENCE_ARRAY_INCLUDED 1

#include "y/dynamic.hpp"
#include <iostream>

namespace upsylon
{
    //! array of contiguous objects
    template <typename T>
    class array : public virtual dynamic
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! destructor
        inline virtual ~array() throw() {}
        
        //! inline access
        inline  type       & operator[](const size_t i) throw() { assert(i>0);assert(i<=size()); assert(item_); return item_[i]; }

        //! inline const access
        inline  const_type & operator[](const size_t i) const throw() { assert(i>0);assert(i<=size()); assert(item_); return item_[i]; }

        //! output octave/julia style
        inline friend std::ostream & operator<<( std::ostream &os, const array &arr )
        {
            os << '[';
            for(size_t i=1;i<=arr.size();++i)
            {
                os << arr[i]; if(i<arr.size()) os << ' ';
            }
            os << ']' << '\'';
            return os;
        }

       
    protected:
        mutable_type *item_; //!< item_[1..size()]
        size_t        size_; //!< size()

        //! default initialisation
        inline array() throw() : item_(0), size_(0) {}

        //! intialize from precomputed data
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

