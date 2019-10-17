//! \file

#ifndef Y_SEQUENCE_ADDRESSABLE_INCLUDED
#define Y_SEQUENCE_ADDRESSABLE_INCLUDED 1

#include "y/collection.hpp"
#include "y/type/args.hpp"
#include <iostream>

namespace upsylon {


    //! collection of addressable items
    template <typename T>
    class addressable : public virtual collection
    {
    public:
        Y_DECL_ARGS(T,type);                                     //!< aliases
        inline virtual ~addressable() throw() {}                 //!< cleanup
        virtual type &       operator[](const size_t)       = 0; //!< access
        virtual const_type & operator[](const size_t) const = 0; //!< const access

        //! default display function
        inline std::ostream & display( std::ostream &os ) const
        {
            os << '[';
            for(size_t i=1;i<=size();++i)
            {
                if(i>1) os << ' ';
                os << (*this)[i];
            }
            os << ']' << '\'';
            return os;
        }

    protected:                                                   //|
        inline explicit addressable() throw() {}                 //!< setup
    private:
        Y_DISABLE_COPY_AND_ASSIGN(addressable);
    };

}

#endif
