

//! \file
#ifndef Y_CORE_RNODE_INCLUDED
#define Y_CORE_RNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/core/dnode.hpp"
#include "y/core/snode.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //! node to handle persistent reference
    template <typename T >
    class ref_dnode : public object, public dnode< ref_dnode<T> >
    {
    public:
        Y_DECL_ARGS(T,type);                     //!< aliases
        typedef dnode< ref_dnode<T> > node_type; //!< base class alias

        //! setup with persistent args
        inline explicit  ref_dnode(const_type &args) throw() :
        object(),
        node_type(),
        data( aliasing::_(args) )
        {
        }

        //! cleanup
        inline virtual ~ref_dnode() throw() {}
        
        //! access
        inline type & operator*()             throw() { return data; }

        //! access, const
        inline const_type & operator*() const throw() { return data; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ref_dnode);
        mutable_type &data;
    };


}

#endif

