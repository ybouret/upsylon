
#include "y/information/translator/huffman.hpp"
#include "y/memory/io.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            const char Tree:: FMID[] = "Huffman";

            const char * Tree:: family() const throw() { return FMID; }

            Node:: ~Node() throw()
            {
            }

            int Node::Comparator::operator()(const Node &lhs, const Node &rhs) const throw()
            {
                const FreqType L = lhs.freq;
                const FreqType R = rhs.freq;
                return (L<R) ? 1 : ( (R<L) ? -1 : 0 );
            }

            Node:: Node(const Char   * _char,
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

            void  Node:: vizCore(ios::ostream &fp) const
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
            }

            Tree:: Tree() : TreeOf<Node>( Alive * sizeof(Node*) ),
            pq( getExtra<Node *>(), Alive )
            {
                buildTree();
            }

            static inline void UpdateCode( Node *node ) throw()
            {
                assert(node);
                Node *left = node->left;
                if(left)
                {
                    Node *right = node->right;
                    assert(right);
                    const size_t nbit = node->bits;
                    left->bits  = right->bits = nbit+1;
                    left->code  = right->code = node->code;
                    right->code |= (1<<nbit);
                    UpdateCode(left);
                    UpdateCode(right);
                }

            }


            void Tree:: buildTree() throw()
            {
            BUILD:
                //--------------------------------------------------------------
                //
                // enqueue leaves
                //
                //--------------------------------------------------------------
                pq.clear();
                size_t inode = 0;
                for(Char *chr = chars.head; chr; chr=chr->next)
                {
                    Node *node = new ( nextNode(inode) ) Node(chr,chr->frequency,1);
                    chr->priv  = node;
                    pq.enqueue(node);
                }

                //--------------------------------------------------------------
                //
                // build the tree
                //
                //--------------------------------------------------------------
                while( pq.count > 1 )
                {
                    assert(inode<Nodes);
                    Node        *right  = pq.extract();
                    Node        *left   = pq.extract();
                    const size_t bits   = max_of(right->bits,left->bits) + 1;
                    if(bits>Bits)
                    {
                        rescaleFrequencies();
                        goto BUILD;
                    }
                    Node *parent  = new ( nextNode(inode) ) Node(0,left->freq+right->freq,bits);
                    parent->left  = left;
                    parent->right = right;

                    pq.enqueue(parent);
                }

                assert(1==pq.count);
                root = pq.extract();

                //--------------------------------------------------------------
                //
                // build the codes
                //
                //--------------------------------------------------------------
                root->code = 0;
                root->bits = 0;
                UpdateCode(root);

                //--------------------------------------------------------------
                //
                // propagate the codes
                //
                //--------------------------------------------------------------
                for(Char *chr=chars.head;chr;chr=chr->next)
                {
                    assert(chr->priv);
                    const Node *node = static_cast<Node *>(chr->priv);
                    chr->code = node->code;
                    chr->bits = node->bits;
                }

            }

            

            

        }

    }
}
