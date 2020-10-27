//! \file
#ifndef Y_AFFIX_INCLUDED
#define Y_AFFIX_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"
#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"
#include "y/type/args.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon
{
    namespace hashing
    {
        class function; //!< forward declaration
    }

    class exception; //!< forward declaration
    
    //__________________________________________________________________________
    //
    //
    //! basic operations on an affix tree
    //
    //__________________________________________________________________________
    class affix
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        class                                tree_node; //!< forward declaration
        typedef core::list_of_cpp<tree_node> tree_list; //!< alias
        typedef const tree_node             *tree_mark;    //!< alias

        //______________________________________________________________________
        //
        //! internal tree node
        //______________________________________________________________________
        class tree_node : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit tree_node(tree_node *, const uint8_t) throw(); //!< setup with parent and code
            virtual ~tree_node() throw();                           //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   optimize()                           throw(); //!< locally optimize with univocal sorting
            void   graphViz(ios::ostream &)               const; //!< for GraphViz
            void   leaves_to(tree_list &)               throw(); //!< return leaves to pool
            void   return_to(tree_list &)               throw(); //!< leaves and this to pool
            void   run(hashing::function &)       const throw(); //!< in-order hashing of codes
            size_t children()                     const throw(); //!< leaves.size
            size_t encode(addressable<uint8_t> &) const throw(); //!< path[0..deep-1]
            void   format(exception &)            const throw(); //!< 'path'

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            void         *addr;   //!< any data
            tree_node    *next;   //!< for list
            tree_node    *prev;   //!< for list
            tree_node    *parent; //!< parent node
            tree_list     leaves; //!< leaves nodes
            size_t        freq;   //!< frequency
            size_t        deep;   //!< from root
            const uint8_t code;   //!< code
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree_node);
        };

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~affix() throw(); //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! check if path exists and is occupied
        template <typename ITERATOR> inline
        bool has(ITERATOR curr, const size_t size) const throw()
        {
            const tree_node *node = find(curr,size);
            if(node)
            {
                return NULL!=node->addr;
            }
            else
            {
                return false;
            }
        }

        void        graphViz(const upsylon::string &)        const; //!< render
        void        graphViz(const char            *)        const; //!< render
        void        hash_with(hashing::function &)   const throw(); //!< H.set(); root->run(H)
        size_t      tell()      const throw();                      //!< root->freq
        size_t      cached()    const throw();                      //!< tree_pool.size
        void        limit(size_t)     throw();                      //!< sort and keep no more than nmax in pool
        void        prune()           throw();                      //!< limit(0)
        void        extra(size_t);                                  //!< reserve extra tree nodes
        static void throw_multiple(const tree_node *node);          //!< raise exception

    protected:
        explicit affix();         //!< setup with root
        
        //______________________________________________________________________
        //
        //! insert address following a path
        /**
         on the fly nodes creation
         */
        //______________________________________________________________________
        template <typename ITERATOR> inline
        tree_node *grow(ITERATOR     curr,
                        size_t       size,
                        void        *addr,
                        tree_mark   *mark)
        {
            assert(addr!=NULL);
            assert(root!=NULL);
            assert(root->code==0);
            assert(root->deep==0);
            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            tree_node *node = root;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const uint8_t  code = *(curr++);
                for(tree_node *chld = node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        //------------------------------------------------------
                        // take this path
                        //------------------------------------------------------
                        node = chld;
                        goto FOUND;
                    }
                }
                //--------------------------------------------------------------
                // need a new leaf
                //--------------------------------------------------------------
                node = node->leaves.push_back( new_tree_node(node,code) );
            FOUND:;
            }
            assert(node!=NULL);
            if(mark) { *mark = node; }
            if(node->addr)
            {
                //--------------------------------------------------------------
                // busy!
                //--------------------------------------------------------------
                return NULL;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                increase_path_to(node);
                return node;
            }
        }

        //______________________________________________________________________
        //
        //! search a node by path
        //______________________________________________________________________
        template <typename ITERATOR> inline
        const tree_node *find(ITERATOR     curr,
                              size_t       size) const throw()
        {
            const tree_node *node = root;
            while(size-- > 0)
            {
                const uint8_t code = *(curr++);
                for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        node = chld;
                        goto FOUND;
                    }
                }
                return NULL;
            FOUND:;
                assert(node);
            }

            assert(node);
            return node;
        }


        void cut(tree_node *) throw(); //!< cut a used node
        void reset()          throw(); //!< return all nodes (but root) to pool


    private:
        tree_node *root;
        tree_list  tree_pool;
        tree_node *new_tree_node(tree_node *, const uint8_t);
        void       increase_path_to(tree_node *node) throw();
        Y_DISABLE_COPY_AND_ASSIGN(affix);
    };
    
    
    
    
}

#endif
