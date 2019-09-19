//! \file

#ifndef Y_BTREE_INCLUDED
#define Y_BTREE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/type/args.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/memory/io.hpp"
#include "y/type/self-destruct.hpp"
#include "y/ios/ostream.hpp"
#include "y/code/utils.hpp"

#include <cstring>
#include <iostream>

namespace upsylon
{

    //! btree for any data
    template <typename T>
    class btree :  public container
    {
    public:
        Y_DECL_ARGS(T,type);                                                        //!< alias
        class                            node_type;                                 //!< forward declaration
        typedef core::list_of<node_type> list_type;                                 //!< list for nodes
        typedef core::pool_of<node_type> pool_type;                                 //!< pool for nodes
        static const size_t              data_size = Y_MEMALIGN(sizeof(type));      //!< aligned memory
        static const size_t              list_size = Y_MEMALIGN(sizeof(list_type)); //!< aligned memory

        //! a node with data for type and list
        class node_type
        {
        public:
            node_type *parent;          //!< parent node for tree
            node_type *next;            //!< for children list and node pool
            node_type *prev;            //!< for children list
            uint8_t    code;            //!< encoding byte
            int        live;            //!< flag to check valid data
            size_t     freq;            //!< frequency to optimize the tree
            char       dbuf[data_size]; //!< space for data
            char       lbuf[list_size]; //!< space for list

            //! access data
            inline mutable_type &data() throw()
            {
                assert(live!=0);
                return *memory::io::__force<mutable_type>(dbuf);
            }

            //! access data, const
            inline const_type &data() const throw()
            {
                assert(live!=0);
                return *memory::io::__force<const_type>(dbuf);
            }

            //! access child
            inline list_type &chld() throw()
            {
                return *memory::io::__force<list_type>(lbuf);
            }

            //! acces child, const
            inline const list_type &chld() const throw()
            {
                return *memory::io::__force<const list_type>(lbuf);
            }

            //! GraphViz representation, mostly to debug
            void viz( ios::ostream &fp ) const
            {
                fp.viz(this);
                const char *info = (live!=0) ? "(*)" : "";
                if(parent)
                {
                    fp( " [label=\"%s#%u%s\"];\n", printable_char[code], unsigned(freq),info);
                }
                else
                {
                    fp( " [label=\"root%s\"];\n",info);
                }
                for(const node_type *node=chld().head;node;node=node->next)
                {
                    node->viz(fp);
                    fp.viz(this) << "->"; fp.viz(node) << ";\n";
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
            node_type(); ~node_type() throw();
        };

        //! setup an empty tree
        inline explicit btree() throw() : root(0), count(0), nodes(0), pool()
        {}

        //! setup an emptry tree with some nodes
        inline explicit btree(const size_t n, const as_capacity_t & ) :
        root(0), count(0), nodes(0), pool()
        {
            reserve__(n);
        }

#if 0
        //! hard copy
        inline btree(const btree &other ) : root(0), count(0), nodes(0), pool()
        {
            try
            {

            }
            catch(...)
            {
                release__();
                throw;
            }

        }
#endif


        //! cleanup
        inline virtual ~btree() throw()
        {
            release__();
        }
        

        inline virtual size_t size()     const throw() { return count; }
        inline virtual size_t capacity() const throw() { return nodes; }
        inline virtual void   free()           throw() { free__(); }
        inline virtual void   release()        throw() { release__(); }
        inline virtual void   reserve(const size_t n)  { reserve__(n); }

        

        //! insert args following key
        bool insert_(const void  *key_buffer,
                     const size_t key_length,
                     param_type   args)
        {
            assert( !(0==key_buffer&&key_length>0) );
            //------------------------------------------------------------------
            //
            // start from the root
            //
            //------------------------------------------------------------------
            if(!root)      root = query_(); assert(0==root->parent);
            node_type     *ptr  = root;
            const uint8_t *key  = static_cast<const uint8_t *>(key_buffer);

            //------------------------------------------------------------------
            //
            // walk down
            //
            //------------------------------------------------------------------
            std::cerr << args << "@<";
            {
                assert(0!=ptr);
                size_t num = key_length;
                while(num-->0)
                {
                    const uint8_t  code  = *(key++);
                    bool           found = false;
                    std::cerr << printable_char[code];
                    // look for the code in child(ren)
                    for(node_type *node = ptr->chld().head;node;node=node->next)
                    {
                        if(code==node->code)
                        {
                            ptr   = node;
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        ptr = ptr->chld().push_back( query(ptr,code) );
                        std::cerr << '+';
                    }
                    else std::cerr << '.';
                }
            } std::cerr << ">";

            //------------------------------------------------------------------
            //
            // where are we
            //
            //------------------------------------------------------------------
            assert(ptr);
            if(ptr->live)
            {
                //--------------------------------------------------------------
                //
                // this key already exists
                //
                //--------------------------------------------------------------
                std::cerr << "(-)" << std::endl;
                return false;
            }
            else
            {

                //--------------------------------------------------------------
                //
                // build object
                //
                //--------------------------------------------------------------
                std::cerr << "(+)" << std::endl;
                new (ptr->dbuf) mutable_type(args);
                ptr->live = 1;
                ++count;

                //--------------------------------------------------------------
                //
                // update frequencies and optimize
                //
                //--------------------------------------------------------------
                while(ptr->parent)
                {
                    ++(ptr->freq);
                    list_type &chld = ptr->parent->chld();
                    assert(chld.owns(ptr));
                    while(ptr->prev&&ptr->prev->freq<ptr->freq)
                    {
                        chld.towards_head(ptr);
                    }

                    ptr=ptr->parent;
                }

                return true;
            }
        }

        //! insert wrapper
        bool insert_(const memory::ro_buffer &key, param_type args)
        {
            return insert_( key.ro(), key.length(), args );
        }

        //! insert wrapper
        bool insert_( const char *text, param_type args )
        {
            return insert_( text, (text!=0) ? strlen(text) : 0, args);
        }

        //! graphviz into output stream
        void graphviz( ios::ostream &fp )
        {
            fp << "digraph G {\n";
            if(root)
            {
                root->viz(fp);
            }
            fp << "}\n";
        }

        //! search following key
        const_type *search_(const void  *key_buffer,
                            const size_t key_length) const throw()
        {
            const node_type *node = search_node(key_buffer,key_length);
            assert(node->live);
            return & node->data();
        }

        //! search wrapper
        const_type *search_(const memory::ro_buffer &key) const throw()
        {
            return search_(key.ro(),key.length());
        }

        //! search wrapper
        const_type *search_(const char *text) const throw()
        {
            return search_(text, (text!=0) ? strlen(text) : 0);
        }

    private:
        Y_DISABLE_COPY(btree);
        Y_DISABLE_ASSIGN(btree);

        node_type *root;  //!< root node for empty key
        size_t     count; //!< size
        size_t     nodes; //!< total number of handled nodes
        pool_type  pool;  //!< available nodes

        //! return a new node
        inline node_type *query(node_type *parent, const uint8_t code)
        {
            node_type *node = query_();
            assert(parent!=node);
            assert(0==node->code);
            assert(0==node->live);
            assert(0==node->freq);
            assert(0==node->parent);
            node->parent = parent;
            node->code   = code;
            return node;
        }

        //! query an empty node
        inline node_type *query_()
        {
            if( pool.size > 0 )
            {
                return pool.query();
            }
            else
            {
                node_type *node = object::acquire1<node_type>();
                ++nodes;
                return node;
            }
        }

        //! reserve some nodes
        inline void reserve__(size_t n)
        {
            const size_t current = pool.size;
            try
            {
                while(n-->0)
                {
                    pool.store(object::acquire1<node_type>());
                    ++nodes;
                    assert(0==pool.top->live);
                }
            }
            catch(...)
            {
                while(pool.size>current)
                {
                    node_type *node = pool.query();
                    assert(0==node->live);
                    object::release1(node);
                    --nodes;
                }
                throw;
            }
        }

        //! recursive free of node, keeping it or not
        void free__(node_type * node, const bool keep) throw()
        {
            assert(node);
            assert(0==node->prev);
            assert(0==node->next);
            if(node->live)
            {
                assert(count>0);
                self_destruct(node->data());
                node->live = 0;
                --count;
            }
            list_type &chld = node->chld();
            while(chld.size>0)
            {
                free__( chld.pop_back(), keep );
            }
            memset(memory::io::__addr(node),0,sizeof(node_type));
            if(keep)
            {
                pool.store(node);
            }
            else
            {
                object::release1(node);
                --nodes;
            }
        }

        //! free all data, keeping nodes
        inline void free__() throw()
        {
            if( root )
            {
                free__(root,true);
                assert(0==root->live);
                assert(0==root->chld().size);
            }
        }

        //! release all data
        void release__() throw()
        {
            if( root )
            {
                free__(root,false);
                root = 0;
            }

            while(pool.size)
            {
                node_type *node = pool.query();
                assert(0==node->live);
                object::release1(node);
            }
        }

        //! search live node
        const node_type *search_node(const void *key_buffer,
                                     size_t      key_length) const throw()
        {
            assert( !(0==key_buffer&&key_length>0) );
            if(root)
            {
                const node_type *ptr   = root;
                const uint8_t   *key   = static_cast<const uint8_t *>(key_buffer);
                bool             found = false;
                while(key_length-->0)
                {
                    const uint8_t        code = *(key++);
                    for(const node_type *node = ptr->chld().head;node;node=node->next)
                    {
                        if(code==node->code)
                        {
                            ptr   = node;
                            found = true;
                            break;
                        }
                    }
                    if(!found) return 0;
                }

                return (ptr->live != 0) ? ptr : 0;
            }
            else
            {
                return 0;
            }
        }


    };

}


#endif

