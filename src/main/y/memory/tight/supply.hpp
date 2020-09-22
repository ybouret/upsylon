//! \file

#ifndef Y_MEMORY_TIGHT_SUPPLY_INCLUDED
#define Y_MEMORY_TIGHT_SUPPLY_INCLUDED 1

#include "y/memory/tight/xcache.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {
            //__________________________________________________________________
            //
            //
            //! interface to create local supplies
            //
            //__________________________________________________________________
            class supply
            {
            public:
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                size_t   node_bytes() const throw(); //!< internal sizeof(node_type)
                size_t   block_size() const throw(); //!< effective xcache block size
                size_t   prefetched() const throw(); //!< local avalaible blocks
                virtual ~supply()           throw(); //!< cleanup

            protected:
                //! setup from object::proto and block_size
                explicit supply(const size_t block_size);
                void  prune() throw();             //!< return local blocks to system cache
                void  fetch(size_t n);             //!< prefetch local blocks
                void *query_block();               //!< query a zeroed block
                void  store_block(void *) throw(); //!< release a block

            private:
                struct node_type { node_type *next; };
                typedef core::pool_of<node_type> zcache;
                xcache xnodes;
                zcache znodes;
            };


            //__________________________________________________________________
            //
            //
            //! generic supply to provide zquery/zstore
            //
            //__________________________________________________________________
            template <typename T>
            class supply_of : public supply
            {
            public:
                Y_DECL_ARGS(T,type); //!< aliases

                //! cleanup
                inline virtual ~supply_of() throw() {}

                //! setup from sizeof(type)
                inline explicit supply_of() : supply( sizeof(type) )
                {
                }
                

            protected:
                //! return a zeroed object
                inline mutable_type *zquery() { return static_cast<mutable_type*>( query_block() ); }
                //! store a previously allocated object
                inline void          zstore(mutable_type *args) throw() { assert(args); store_block(args); }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(supply_of);
            };

            

        }
    }
}

#endif
