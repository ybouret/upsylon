

//! \file
#ifndef Y_CORE_RNODE_INCLUDED
#define Y_CORE_RNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/core/dnode.hpp"
#include "y/core/snode.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //! node to handle persistent reference
    template <typename T,
    template <typename> class NODE_TYPE >
    class ref_node : public object, public NODE_TYPE< ref_node<T,NODE_TYPE> >
    {
    public:
        Y_DECL_ARGS(T,type);                                  //!< aliases
        typedef NODE_TYPE< ref_node<T,NODE_TYPE> > node_type; //!< base class alias

        //! setup withj persistent args
        inline explicit  ref_node(const_type &args) throw() :
        object(),
        node_type(),
        data( aliasing::_(args) )
        {
        }

        //! cleanup
        inline virtual ~ref_node() throw() {}
        
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

