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

            void Tree:: Node:: vizCore(ios::ostream &fp) const
            {
                fp << " [label=\"";
                if(heavy==light)
                {
                    fp << Alphabet::NameOf(heavy->symbol);
                }
                //fp("#%lu", static_cast<unsigned long>(freq) );
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

            const Tree::Node & Tree:: getRoot() const throw()
            {
                assert(root);
                return *root;
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

                //--------------------------------------------------------------
                //
                // increase bits
                //
                //--------------------------------------------------------------
                const size_t bits = node->bits+1;
                if(bits>Bits)
                {
                    // shall rescale
                    return false;
                }

                if(node->heavy==node->light)
                {
                    //----------------------------------------------------------
                    //
                    // only one node => leaf
                    //
                    //----------------------------------------------------------
                    aliasing::_(node->heavy->priv) = node;
                    return true;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // two or more nodes
                    //
                    //----------------------------------------------------------

                    //__________________________________________________________
                    //
                    // heavy bracket
                    //__________________________________________________________
                    const Char *hIni  = node->heavy;
                    const Char *hEnd  = hIni;
                    size_t      hFreq = hIni->frequency;

                    //__________________________________________________________
                    //
                    // light bracket
                    //__________________________________________________________
                    const Char *lEnd  = node->light;
                    const Char *lIni  = hEnd->next;    assert(lIni!=NULL);
                    size_t      lFreq = lIni->frequency;
                    for(const Char *ch = lEnd; ch!=lIni; ch=ch->prev)
                    {
                        lFreq += ch->frequency;
                    }
                    FreqType dFreq = deltaFreq(hFreq,lFreq);

                    //__________________________________________________________
                    //
                    // bracket until singulet or exceed balance
                    //__________________________________________________________
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

                    //__________________________________________________________
                    //
                    // make left
                    //__________________________________________________________
                    {
                        assert(inode<Nodes);
                        Node *left  = node->left  = new ( &treeNodes[inode++] ) Node(hIni,hEnd);
                        assert(0==left->bits);
                        assert(0==left->code);
                        left->bits  = bits;
                        left->code  = node->code;
                        if(!split(left,inode)) return false;
                    }

                    //__________________________________________________________
                    //
                    // make right
                    //__________________________________________________________
                    {
                        assert(inode<Nodes);
                        Node *right = node->right = new ( &treeNodes[inode++] ) Node(lIni,lEnd);
                        assert(0==right->bits);
                        assert(0==right->code);
                        right->bits = bits;
                        right->code = node->code | ( 1 << (bits-1) );
                        if(!split(right,inode)) return false;
                    }

                    return true;
                }
            }


            void Tree:: update(const uint8_t byte, qbits *io)
            {
                emitAndUpdateByte(byte,io);
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

