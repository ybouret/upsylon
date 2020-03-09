
//! \file
#ifndef Y_CORE_KNODE_INCLUDED
#define Y_CORE_KNODE_INCLUDED 1

#include "y/object.hpp"
#include "y/type/args.hpp"
#include "y/core/pool.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon {
    
    namespace core {

        //! kernel kind node
        template <typename T>
        class knode
        {
        public:
            typedef core::pool_of< knode<T> > pool_type; //!< associated core pool
            typedef core::list_of< knode<T> > list_type; //!< associated core list
            Y_DECL_ARGS(T,type);                         //!< alias
            knode *next; //!< for list/pool
            knode *prev; //!< for list
            type   data; //!< the data

            //! access
            inline type & operator*() throw() { return data; }

            //! const acces
            inline const_type & operator*() const throw() { return data; }

            //! acquire empty node
            static inline knode *acquire_empty()
            {
                return object::acquire1<knode>();
            }

            //! acquire empty node from cache or fallback on acquire
            static inline knode *acquire_empty_with(pool_type &cache)
            {
                return (cache.size>0) ? cache.query() : acquire_empty();
            }

            //! release an empty node memory
            static inline void release_empty( knode * &node ) throw()
            {
                assert(NULL!=node);
                object::release1(node);
                assert(NULL==node);
            }

            //! direct with default constructor
            static inline knode *create_alive()
            {
                return new( acquire_empty() ) knode();
            }
            
            //! direct with 1-args constructor
            template <typename U>
            static inline knode *create_alive(const U &args)
            {
                return new( acquire_empty() ) knode(args);
            }

            //! direct with 2-args constructor
            template <typename U, typename V>
            static inline knode *create_alive(const U &u, const V &v)
            {
                return new( acquire_empty() ) knode(u,v);
            }


            //! cached with default constructor
            static inline knode *create_alive_with(pool_type &cache)
            {
                knode *node = acquire_empty_with(cache);
                try { new (node) knode(); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached with 1-args constructor
            template <typename U>
            static inline knode *create_alive_with(pool_type &cache,const U &args)
            {
                knode *node = acquire_empty_with(cache);
                try { new (node) knode(args); } catch(...) { cache.store(node); throw; }
                return node;
            }

            //! cached with 2-args constructor
            template <typename U,typename V>
            static inline knode *create_alive_with(pool_type &cache,const U &u, const V &v)
            {
                knode *node = acquire_empty_with(cache);
                try { new (node) knode(u,v); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached (not empty) with default constructor
            static inline knode *create_alive_with_(pool_type &cache)
            {
                assert(cache.size>0);
                knode *node = cache.query();
                try { new (node) knode(); } catch(...) { cache.store(node); throw; }
                return node;
            }
            
            //! cached (not empty) with 1-args constructor
            template <typename U>
            static inline knode *create_alive_with_(pool_type &cache,const U & args)
            {
                assert(cache.size>0);
                knode *node = cache.query();
                try { new (node) knode(args); } catch(...) { cache.store(node); throw; }
                return node;
            }

            //! cached (not empty) with 2-args constructor
            template <typename U,typename V>
            static inline knode *create_alive_with_(pool_type &cache,const U & u, const V &v)
            {
                assert(cache.size>0);
                knode *node = cache.query();
                try { new (node) knode(u,v); } catch(...) { cache.store(node); throw; }
                return node;
            }

            
            
            //! prefetch empty nodes
            static inline void prefetch(pool_type &cache,size_t n)
            {
                while(n-- > 0)
                {
                    cache.store( acquire_empty() );
                }
            }

            
            //! direct destruction
            static inline void destruct(knode * &alive) throw()
            {
                assert(NULL!=alive);
                self_destruct(alive->data);
                release_empty(alive);
                assert(NULL==alive);
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
            static inline void destruct_to(pool_type &cache, list_type &nodes) throw()
            {
                while(nodes.size)
                {
                    knode *alive = nodes.pop_back();
                    destruct(alive,cache);
                }
            }

            //! destruct a cache
            static inline void destruct(pool_type &cache) throw()
            {
                while(cache.size)
                {
                    knode *node = cache.query();
                    release_empty(node);
                }
            }

            //! destruct a list
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
            inline knode()              : next(0), prev(0), data()     {}
            template <typename U>
            inline knode(const U &args) : next(0), prev(0), data(args) {}

            template <typename U, typename V>
            inline knode(const U &u, const V &v) : next(0), prev(0), data(u,v) {}

        };
        
    }
    
}

#endif
