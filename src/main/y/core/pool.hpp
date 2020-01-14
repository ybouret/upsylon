//! \file
#ifndef Y_CORE_POOL_INCLUDED
#define Y_CORE_POOL_INCLUDED 1


#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace core
    {
        //! sanity check for node
#define Y_CORE_CHECK_POOL_NODE(node) \
assert((node)!=NULL);                \
assert((node)->next==NULL)
        
        //! core pool
        template <typename NODE>
        class pool_of
        {
        public:
            //! default constructor
            inline explicit pool_of() throw() : top(NULL), size(0) {}

            //! destructor: pool must be empty
            inline virtual ~pool_of() throw() { assert(NULL==top); assert(0==size); }

            NODE        *top;  //!< top of pool
            const size_t size; //!< nodes in pool


            //! push a valid node
            NODE *store( NODE *node ) throw()
            {
                Y_CORE_CHECK_POOL_NODE(node);
                node->next = top;
                top        = node;
                ++(size_t&)size;
                return node;
            }

            //! query if size>0
            inline NODE *query() throw()
            {
                assert(top != NULL);
                assert(size > 0   );
                NODE *node = top;
                top = top->next;
                node->next = NULL;
                --(size_t&)size;
                Y_CORE_CHECK_POOL_NODE(node);
                return node;
            }

            //! linear search
            inline bool owns( const NODE *node ) const throw()
            {
                for( const NODE *scan = top; scan != NULL; scan = scan->next )
                {
                    if( scan == node ) return true;
                }
                return false;
            }


            //! hard reset
            inline void reset() throw() { top = NULL; (size_t&)size = 0; }

            //! no-throw swap
            inline void swap_with( pool_of<NODE> &other ) throw()
            {
                _cswap(size,other.size);
                _cswap(top, other.top);
            }

            //! reverse order
            inline void reverse() throw()
            {
                pool_of<NODE> tmp;
                while( size ) { tmp.store( query() ); }
                swap_with(tmp);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_of);
        };


    }

}

#include "y/type/releasable.hpp"

namespace upsylon
{
    namespace core
    {
        //! pool of C++ NODEs
        template <typename NODE>
        class pool_of_cpp : public pool_of<NODE>, public releasable
        {
        public:
            //! constructor
            explicit pool_of_cpp() throw() : pool_of<NODE>(), releasable() {}

            //! destructor, delete NODEs
            virtual ~pool_of_cpp() throw() { release(); }

            //! proper NODEs deleting
            inline virtual void release() throw()
            {
                while(this->size>0)
                {
                    delete this->query();
                }
            }

            //! delete one NODE
            inline void pop() throw()
            {
                assert(this->top);
                assert(this->size>0);
                delete this->query();
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool_of_cpp);
        };
        
    }
    
}

#endif
