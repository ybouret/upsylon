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

            class supply
            {
            public:
                virtual ~supply() throw();

                size_t node_bytes() const throw();
                size_t block_size() const throw();
                size_t prefetched() const throw();

            protected:
                explicit supply(const size_t block_size);

                void prune() throw();
                void fetch(size_t n);

                void *query_block();
                void  store_block(void *) throw();


            private:
                struct node_type { node_type *next; };
                typedef core::pool_of<node_type> zcache;
                xcache xnodes;
                zcache znodes;
            };

            template <typename T>
            class supply_of : public supply
            {
            public:
                Y_DECL_ARGS(T,type);

                inline virtual ~supply_of() throw()
                {
                }

                inline explicit supply_of() : supply( sizeof(type) )
                {
                }
                

            protected:
                inline mutable_type *zquery() { return static_cast<mutable_type*>( query_block() ); }
                inline void          zstore(mutable_type *args) throw() { assert(args); store_block(args); }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(supply_of);
            };

            

        }
    }
}

#endif
