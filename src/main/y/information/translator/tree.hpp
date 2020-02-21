
#ifndef Y_INFORMATION_TRANSLATOR_TREE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_TREE_INCLUDED 1

#include "y/information/translator/alphabet.hpp"

namespace upsylon {

    namespace information {

        class TranslatorTree : public Alphabet
        {
        public:
            virtual ~TranslatorTree() throw();


        protected:
            explicit TranslatorTree(const size_t sizeof_node,
                                    const size_t extra_bytes);

            template <typename NODE> inline NODE *getNodes() throw()
            {
                return static_cast<NODE *>(_nodes);
            }

            template <typename T> inline T *getExtra() throw()
            {
                assert(_extra); return static_cast<T *>(_extra);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorTree);
            const size_t _shift;
            size_t       _bytes;
            void        *_nodes;
            void        *_extra;
        };

        template <typename NODE>
        class TreeOf : public TranslatorTree
        {
        public:
            inline virtual ~TreeOf() throw() { root=0; _nodes=0; }


            inline const NODE & getRoot() const throw() { assert(root); return *root; }
            
        protected:
            NODE *root;
            
            inline explicit TreeOf(const size_t extra_bytes) :
            TranslatorTree( sizeof(NODE), extra_bytes),
            root(0),
            _nodes( getNodes<NODE>() )
            {
            }


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

