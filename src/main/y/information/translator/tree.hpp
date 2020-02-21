//!\file

#ifndef Y_INFORMATION_TRANSLATOR_TREE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_TREE_INCLUDED 1

#include "y/information/translator/alphabet.hpp"

namespace upsylon {

    namespace information {

        //! base class to build trees from an alphabet
        class TranslatorTree : public Alphabet
        {
        public:
            virtual ~TranslatorTree() throw(); //!< cleanup
            
        protected:
            //! setup memory
            explicit TranslatorTree(const size_t sizeof_node,
                                    const size_t extra_bytes);

            //! helper
            template <typename NODE> inline
            NODE *getNodes() throw() { assert(_nodes); return static_cast<NODE *>(_nodes); }

            //! helper
            template <typename T> inline
            T *getExtra() throw() { assert(_extra); return static_cast<T *>(_extra); }

            virtual void buildTree() throw() = 0;          //!< the main effort
            void         setupTree() throw();              //!< initialize + buildTree
            void         inputByte(const uint8_t,qbits *); //!< emitAndUpdate, buildTree
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorTree);
            const size_t _shift;
            size_t       _bytes;
            void        *_nodes;
            void        *_extra;
        };

        //! generic tree class
        template <typename NODE>
        class TreeOf : public TranslatorTree
        {
        public:
            //! cleanup
            inline virtual ~TreeOf() throw() { root=0; _nodes=0; }

            //! access root
            inline const NODE & getRoot() const throw() { assert(root); return *root; }
            
        protected:
            NODE *root; //!< root of the tree

            //! initialize for nodes and extra bytes
            inline explicit TreeOf(const size_t extra_bytes) :
            TranslatorTree( sizeof(NODE), extra_bytes),
            root(0), _nodes( getNodes<NODE>() )
            {
            }

            //! address of next node
            inline NODE *nextNode( size_t &inode ) throw()
            {
                assert(inode<Nodes);
                return & _nodes[ inode++ ];
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TreeOf);
            NODE *_nodes;
        };
    }
}

#endif

