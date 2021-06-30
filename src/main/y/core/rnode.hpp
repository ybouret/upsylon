

//! \file
#ifndef Y_CORE_RNODE_INCLUDED
#define Y_CORE_RNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/core/dnode.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //! node to handle persistent reference
    template <typename T>
    class ref_node : public object, public dnode< ref_node<T> >
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! setup withj persistent args
        inline explicit  ref_node(const_type &args) throw() :
        object(),
        dnode< ref_node<T> >(),
        data( aliasing::_(args) )
        {
        }

        //! cleanup
        inline virtual ~ref_node() throw()
        {

        }

        //! access
        inline type * operator->()             throw() { return &data; }

        //! access
        inline const_type * operator->() const throw() { return &data; }

        //! access
        inline type & operator*()             throw() { return data; }

        //! access, const
        inline const_type & operator*() const throw() { return data; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ref_node);
        mutable_type &data;
    };


}

#endif

