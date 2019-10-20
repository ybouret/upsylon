//! \file
#ifndef Y_SEQUENCE_ACCESSIBLE_INCLUDED
#define Y_SEQUENCE_ACCESSIBLE_INCLUDED 1

#include "y/collection.hpp"
#include "y/type/args.hpp"
#include <iostream>

namespace upsylon {

    //! a collection with some access
    template <typename T>
    class accessible : public virtual collection
    {
    public:
        Y_DECL_ARGS(T,type);                                     //!< aliases
        inline virtual ~accessible() throw() { }                 //!< cleanup
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


    protected:
        inline explicit accessible() throw() {}                 //!< setup
    private:
        Y_DISABLE_COPY_AND_ASSIGN(accessible);
    };


}

#endif

