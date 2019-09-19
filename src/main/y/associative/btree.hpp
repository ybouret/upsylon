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



    template <typename T>
    class btree :  public container
    {
    public:
        Y_DECL_ARGS(T,type);

        class                            node_type;
        typedef core::list_of<node_type> list_type;
        typedef core::pool_of<node_type> pool_type;
        static const size_t              data_size = Y_ROUND8(sizeof(type));
        static const size_t              list_size = Y_ROUND8(sizeof(list_type));

        class node_type
        {
        public:
            node_type *parent;
            node_type *next;
            node_type *prev;
            uint8_t    code;
            int        live;
            size_t     freq;
            char       dbuf[data_size];
            char       lbuf[list_size];

            inline mutable_type &data() throw()
            {
                assert(live!=0);
                return *memory::io::__force<mutable_type>(dbuf);
            }

            inline const_type &data() const throw()
            {
                assert(live!=0);
                return *memory::io::__force<const_type>(dbuf);
            }

            inline list_type &chld() throw()
            {
                return *memory::io::__force<list_type>(lbuf);
            }

            inline const list_type &chld() const throw()
            {
                return *memory::io::__force<const list_type>(lbuf);
            }

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

        inline explicit btree() throw() : root(0), count(0), nodes(0), pool()
        {}

        inline explicit btree(const size_t n, const as_capacity_t & ) :
        root(0), count(0), nodes(0), pool()
        {
            reserve__(n);
        }


        inline virtual ~btree() throw()
        {
            release__();
        }
        

        inline virtual size_t size()     const throw() { return count; }
        inline virtual size_t capacity() const throw() { return count+pool.size; }
        inline virtual void   free()           throw() { free__(); }
        inline virtual void   release()        throw() { release__(); }
        inline virtual void   reserve(const size_t n)  { reserve__(n); }
        inline size_t         internal() const throw() { return nodes; }


        bool insert(const void  *key_buffer,
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

        bool insert(const memory::ro_buffer &key, param_type args)
        {
            return insert( key.ro(), key.length(), args );
        }

        bool insert( const char *text, param_type args )
        {
            return insert( text, (text!=0) ? strlen(text) : 0, args);
        }


        void graphviz( ios::ostream &fp )
        {
            fp << "digraph G {\n";
            if(root)
            {
                root->viz(fp);
            }
            fp << "}\n";
        }

        const_type *search(const void  *key_buffer,
                           const size_t key_length) const throw()
        {
            const node_type *node = search_node(key_buffer,key_length);
            assert(node->live);
            return & node->data();
        }

        const_type *search(const memory::ro_buffer &key) const throw()
        {
            return search(key.ro(),key.length());
        }

        const_type *search(const char *text) const throw()
        {
            return search(text, (text!=0) ? strlen(text) : 0);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(btree);

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
            memset(node,0,sizeof(node_type));
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


        inline void free__() throw()
        {
            if( root )
            {
                free__(root,true);
                assert(0==root->live);
                assert(0==root->chld().size);
            }
        }

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

