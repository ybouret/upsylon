
//! \file

#ifndef Y_HASH_ZNODES_INCLUDED
#define Y_HASH_ZNODES_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"
#include "y/sort/merge-list.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! list of zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class hash_znodes : public core::list_of<NODE>
    {
    public:

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit hash_znodes() throw() : core::list_of<NODE>()
        {
        }

        //! cleanup
        inline virtual ~hash_znodes() throw()
        {
            release();
        }

        //! reserve one new node
        inline void push()        { this->push_back( object::acquire1<NODE>() ); }

        //! remove one old node
        inline void pop() throw() { NODE *node = this->pop_back(); object::release1(node); }

        //! release all nodes
        inline void release() throw() { while(this->size) pop(); }

        //! reorder nodes by increasing addresses
        inline void reorder() throw()
        {
            core::list_of<NODE> &self = *this;
            merge_list_of<NODE>::sort_by_increasing_address(self);
        }

        //! query a zombie node
        NODE *query() throw()
        {
            assert(this->size>0);
            return this->pop_front();
        }

        //! store a zombie node
        void store(NODE *node) throw()
        {
            assert(node);
            (void) this->push_front(node);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_znodes);
    };

}

#endif
