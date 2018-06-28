#ifndef Y_CORE_POOL_INCLUDED
#define Y_CORE_POOL_INCLUDED 1


#include "y/type/bswap.hpp"

namespace upsylon
{
    namespace core
    {
        //! core pool
        template <typename NODE>
        class pool_of
        {
        public:
            //! default constructor
            inline explicit pool_of() throw() : top(NULL), size(0) {}

            //! destructor: pool must be empty
            inline virtual ~pool_of() throw() { assert(NULL==top); assert(0==size); }

            NODE  *top;  //!< top of pool
            size_t size; //!< nodes in pool


            //! push a valid node
            inline void store( NODE *node ) throw()
            {
                assert( NULL != node       );
                assert( NULL == node->next );
                //assert( ! owns(node)       );
                node->next = top;
                top        = node;
                ++size;
            }

            //! query is size>0
            inline NODE *query() throw()
            {
                assert(top != NULL);
                assert(size > 0   );
                NODE *node = top;
                top = top->next;
                node->next = NULL;
                --size;
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
            inline void reset() throw() { top = NULL; size = 0; }

            //! no-throw swap
            inline void swap_with( pool_of<NODE> &other ) throw()
            {
                cswap(size,other.size);
                cswap(top, other.top);
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


        template <typename NODE>
        class pool_of_cpp : public pool_of<NODE>
        {
        public:
            explicit pool_of_cpp() throw() : pool_of<NODE>() {}
            virtual ~pool_of_cpp() throw() { clear(); }

            inline void clear() throw()
            {
                while(this->size>0)
                {
                    delete this->query();
                }
            }

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
