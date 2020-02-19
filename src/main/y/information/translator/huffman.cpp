
#include "y/information/translator/huffman.hpp"
#include "y/memory/io.hpp"
#include "y/type/utils.hpp"
//#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            Tree::Node:: ~Node() throw()
            {
            }

            int Tree::Node::Comparator::operator()(const Node &lhs, const Node &rhs) const throw()
            {
                const FreqType L = lhs.freq;
                const FreqType R = rhs.freq;
                return (L<R) ? 1 : ( (R<L) ? -1 : 0 );
            }

            Tree::Node:: Node(const Char   * _char,
                              const FreqType _freq,
                              const size_t   _bits) throw() :
            chr(_char),
            left(0),
            right(0),
            freq(_freq),
            code(0),
            bits(_bits)
            {
            }

            void Tree:: Node:: vizCore(ios::ostream &fp) const
            {
                fp << " [label=\"";
                if(chr) fp << Alphabet::NameOf(chr->symbol);
                fp("#%lu", static_cast<unsigned long>(freq) );
                fp << "\"]";
                endl(fp);
                if(left)
                {
                    left->vizSave(fp);
                    endl( vizJoin(fp,left) << " [label=\"0\"]" );
                }
                if(right)
                {
                    right->vizSave(fp);
                    endl( vizJoin(fp,right) << " [label=\"1\"]" );
                }
            }


            Tree:: ~Tree() throw()
            {
                memory::global::location().release(workspace,fullBytes);
            }

            Tree:: Tree() : Alphabet(),
            root(0),
            treeBytes( memory::align( Nodes * sizeof(Node) )  ),
            fullBytes( treeBytes + Alive * sizeof(Node*)  ),
            workspace( memory::global::instance().acquire(fullBytes) ),
            treeNodes( static_cast<Node *>(workspace) ),
            pq( memory::io::cast<Node *>( workspace, treeBytes ), Alive )
            {
                build();
            }

            static inline void UpdateCode( Tree::Node *node ) throw()
            {
                assert(node);
                Tree::Node *left = node->left;
                if(left)
                {
                    Tree::Node *right = node->right;
                    assert(right);
                    const size_t nbit = node->bits;
                    left->bits  = right->bits = nbit+1;
                    left->code  = right->code = node->code;
                    right->code |= (1<<nbit);
                    UpdateCode(left);
                    UpdateCode(right);
                }

            }


            void Tree:: build() throw()
            {
                // enqueue leaves
                pq.clear();
                size_t inode = 0;
                for(Char *chr = chars.head; chr; chr=chr->next, ++inode)
                {
                    Node *node = new (treeNodes+inode) Node(chr,chr->frequency,1);
                    chr->priv  = node;
                    pq.enqueue(node);
                }

                // build the tree
                while( pq.count > 1 )
                {
                    assert(inode<Nodes);
                    Node        *right  = pq.extract();
                    Node        *left   = pq.extract();
                    const size_t bits   = max_of(right->bits,left->bits) + 1;

                    Node *parent  = new ( &treeNodes[inode++] ) Node(0,left->freq+right->freq,bits);
                    parent->left  = left;
                    parent->right = right;

                    pq.enqueue(parent);
                }

                assert(1==pq.count);
                root = pq.extract();

                // build the codes
                root->code = 0;
                root->bits = 0;
                UpdateCode(root);
                for(Char *chr=chars.head;chr;chr=chr->next)
                {
                    assert(chr->priv);
                    const Node *node = static_cast<Node *>(chr->priv);
                    chr->code = node->code;
                    chr->bits = node->bits;
                }

            }

            const Tree::Node & Tree:: getRoot() const throw()
            {
                assert(root);
                return *root;
            }

            void Tree:: update(const uint8_t byte, qbits *io)
            {
                updateByte(byte,io);
                build();
            }

            void Tree:: restart() throw()
            {
                initialize();
                build();
            }

        }

    }
}
