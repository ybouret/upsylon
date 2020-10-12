

//! \file
#ifndef Y_CORE_RNODE_INCLUDED
#define Y_CORE_RNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/core/inode.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //! node to handle persistent reference
    template <typename T>
    class ref_node : public object, public inode< ref_node<T> >
    {
    public:
        Y_DECL_ARGS(T,type);

        inline explicit  ref_node(const_type &args) throw() :
        object(),
        inode< ref_node<T> >(),
        data( aliasing::_(args) )
        {
        }

        inline virtual ~ref_node() throw()
        {

        }

        inline type & operator*()             throw() { return data; }
        inline const_type & operator*() const throw() { return data; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ref_node);
        mutable_type &data;
    };


}

#endif

