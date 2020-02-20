#include "y/information/translator/shannon-fano.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            Tree::Node:: ~Node() throw()
            {

            }

            Tree:: Node:: Node(const Char *h, const Char *l) throw() :
            heavy(h),
            light(l),
            left(0),
            right(0),
            code(0),
            bits(0)
            {

            }

            Tree:: Tree() : Alphabet(),
            root(0),
            treeBytes( Nodes * sizeof(Node) ),
            treeNodes( static_cast<Node *>( memory::global::instance().acquire(treeBytes) ) )
            {
                build();
            }

            Tree:: ~Tree() throw()
            {

                void *addr = treeNodes;
                memset(addr,0,treeBytes);
                memory::global::location().release(addr,treeBytes);

                treeNodes = 0;
                root      = 0;

            }

            void Tree:: build() throw()
            {
                size_t inode = 0;
                root  = new (&treeNodes[inode++]) Node(chars.head,chars.tail);
                assert(0==root->code);
                assert(0==root->bits);
                while( !split(root,inode) )
                {
                    rescaleFrequencies();
                }
                for(Char *ch = chars.head; ch; ch=ch->next )
                {
                    assert(ch->priv);
                    const Node *node = static_cast<const Node *>(ch->priv);
                    ch->code = node->code;
                    ch->bits = node->bits;
                }
            }

            template <typename T> static inline
            T deltaFreq( const T lhs, const T rhs ) throw()
            {
                return (lhs<=rhs) ? rhs-lhs : lhs-rhs;
            }

            bool Tree:: split(Node *node, size_t &inode ) throw()
            {
                assert(node);
                assert(node->heavy);
                assert(node->light);
                assert(inode<Nodes);

                const size_t bits = node->bits+1;
                std::cerr << "\t[bits: " << bits << "]" << std::endl;

                if(bits>Bits)
                {
                    return false;
                }

                if(node->heavy==node->light)
                {
                    // only one node => leaf
                    std::cerr << "assign " << NameOf(node->heavy->symbol) << std::endl;
                    aliasing::_(node->heavy->priv) = node;
                    return true;
                }
                else
                {
                    // two or more nodes
                    std::cerr << "split from " << NameOf(node->heavy->symbol) << " to " << NameOf(node->light->symbol) << std::endl;

                    // heavy bracket
                    const Char *hIni  = node->heavy;
                    const Char *hEnd  = hIni;
                    size_t      hFreq = hIni->frequency;

                    // light bracket
                    const Char *lEnd  = node->light;
                    const Char *lIni  = hEnd->next;    assert(lIni!=NULL);
                    size_t      lFreq = lIni->frequency;
                    for(const Char *ch = lEnd; ch!=lIni; ch=ch->prev)
                    {
                        lFreq += ch->frequency;
                    }
                    FreqType dFreq = deltaFreq(hFreq,lFreq);
                    std::cerr << "\tfreqs: " << hFreq << " - " << lFreq << ": " << dFreq << std::endl;

                    // bracket until singulet or exceed balance
                    while(lEnd!=lIni)
                    {
                        assert(hEnd->next==lIni);
                        const FreqType f = lIni->frequency;
                        hFreq += f;
                        lFreq -= f;
                        const FreqType dTemp = deltaFreq(hFreq, lFreq);
                        if(dTemp<dFreq)
                        {
                            //accept and forward
                            dFreq = dTemp;
                            hEnd  = hEnd->next;
                            lIni  = lIni->next;
                        }
                        else
                        {
                            // done!
                            break;
                        }
                    }
                    std::cerr << "\t[" << NameOf(hIni->symbol) << ":" << NameOf(hEnd->symbol) << "] - [" << NameOf(lIni->symbol) << ":" << NameOf(lEnd->symbol) << "]" << std::endl;

                    {
                        assert(inode<Nodes);
                        Node *left  = node->left  = new ( &treeNodes[inode++] ) Node(hIni,hEnd);
                        left->bits  = bits;
                        if(!split(left,inode)) return false;
                    }

                    {
                        assert(inode<Nodes);
                        Node *right = node->right = new ( &treeNodes[inode++] ) Node(lIni,lEnd);
                        right->bits = bits;
                        if(!split(right,inode)) return false;
                    }

                    return true;
                }
            }

        }

    }

}

