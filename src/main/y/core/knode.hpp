
//! \file
#ifndef Y_CORE_KNODE_INCLUDED
#define Y_CORE_KNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"
#include "y/core/pool.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon {
    
    namespace core {
        
        template <typename T>
        class knode
        {
        public:
            typedef core::pool_of< knode<T> > pool_type;
            typedef core::list_of< knode<T> > list_type;
            Y_DECL_ARGS(T,type);  //!< alias
            knode *next;
            knode *prev;
            type   data;
            
            //! direct with default constructor
            static inline knode *create()
            {
                return new( object::acquire1<knode>() ) knode();
            }
            
            //! direct with 1-args constructor
            static inline knode *create(param_type args)
            {
                return new( object::acquire1<knode>() ) knode(args);
            }
            
            
            //! cached with default constructor
            static inline knode *create(pool_type &cache)
            {
                knode *node = query(cache);
                try { new (node) knode(); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached with 1-args constructor
            static inline knode *create(param_type args, pool_type &cache)
            {
                knode *node = query(cache);
                try { new (node) knode(args); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached (not empty) with default constructor
            static inline knode *create_(pool_type &cache)
            {
                assert(cache.size>0);
                knode *node = cache.query();
                try { new (node) knode(); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached (not empty) with default constructor
            static inline knode *create_(param_type args, pool_type &cache)
            {
                assert(cache.size>0);
                knode *node = cache.query();
                try { new (node) knode(args); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            
            
            //! prefetch dead nodes
            static inline void prefetch(pool_type &nodes,size_t n)
            {
                while(n-- > 0)
                {
                    nodes.store( object::acquire1<knode>() );
                }
            }
            
            //! direct destruction
            static inline void destruct(knode * &alive) throw()
            {
                assert(alive);
                self_destruct(alive->data);
                object::release1(alive);
                alive = 0;
            }
            
           
            
            //! destruct and put back into cache
            static inline void destruct(knode *     &alive,
                                        pool_type   &cache) throw()
            {
                assert(alive);
                self_destruct(alive->data);
                cache.store(alive);
                alive = 0;
            }
            
            //! destruct a list into a cache
            static inline void destruct_to( pool_type &cache, list_type &nodes ) throw()
            {
                while(nodes.size)
                {
                    knode *alive = nodes.pop_back();
                    destruct(alive,cache);
                }
            }
            
            static inline void destruct(pool_type &cache) throw()
            {
                while(cache.size)
                {
                    knode *node = cache.query();
                    destruct(node);
                }
            }
            
            static inline void destruct(list_type &nodes) throw()
            {
                while(nodes.size)
                {
                    knode *node = nodes.pop_back();
                    destruct(node);
                }
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(knode);
            ~knode() throw();
            inline knode() :                 next(0), prev(0), data()     {}
            inline knode(const_type &args) : next(0), prev(0), data(args) {}
           
            static inline knode *query(pool_type &cache)
            {
                return ( (cache.size>0) ? cache.query() : object::acquire1<knode>() );
            }
        };
        
    }
    
}

#endif
