
#include "y/codec/asf.hpp"
#include "y/code/utils.hpp"
#include "y/type/bzset.hpp"
#include "y/memory/io.hpp"

namespace upsylon
{

    const char *  ASF::Char:: text() const throw()
    {
        if(byte<NUM_CHARS)
        {
            return printable_char[byte];
        }
        else if(byte==NYT)
        {
            return "nyt";
        }
        else if(byte==EOS)
        {
            return "eos";
        }
        else
        {
            return "???";
        }
    }


    ASF::Alphabet:: ~Alphabet() throw()
    {
        active.reset();
        memory::global::location().release(wksp,wlen);
    }

    static const size_t __asf_bytes_for_chars_  = sizeof(ASF::Char) * ASF::ALPHABET_SIZE;
    static const size_t __asf_bytes_for_chars  = Y_MEMALIGN(__asf_bytes_for_chars_);
    static const size_t __asf_bytes_for_nodes_ = (2*ASF::ALPHABET_SIZE-1) * sizeof(ASF::Node);
    static const size_t __asf_bytes_for_nodes  = Y_MEMALIGN(__asf_bytes_for_nodes_);
    static const size_t __asf_bytes            = __asf_bytes_for_chars + __asf_bytes_for_nodes;

    ASF::Alphabet:: Alphabet():
    active(),
    in_use(0),
    chars(0),
    nodes(0),
    nyt(0),
    eos(0),
    wlen(__asf_bytes),
    wksp(memory::global::instance().acquire(wlen))
    {
        chars = memory::io::cast<Char>(wksp);
        nodes = memory::io::cast<Node>(wksp,__asf_bytes_for_chars);
        nyt   = chars+NYT;
        eos   = chars+EOS;
        reset();
    }




    void ASF::Alphabet:: reset() throw()
    {
        active.reset();
        for(size_t i=0;i<ALPHABET_SIZE;++i)
        {
            Char *ch = &chars[i];
            ch->next = 0;
            ch->prev = 0;
            ch->byte = i;
            ch->code = i;
            ch->bits = 8;
            ch->freq = 0;
        }
        nyt->freq = 1;
        eos->freq = 1;
        active.push_back(nyt);
        active.push_back(eos);
        in_use = 0;
        build_tree();
    }

    void ASF::Alphabet:: display(std::ostream &os) const
    {
        for(const Char *ch = active.head; ch;ch=ch->next)
        {
            os << ch->text() << " : " << ch->freq << " => #bits=" << ch->bits << " : ";
            iobits::display(os,ch->code,ch->bits) << std::endl;
        }
    }




    ASF::Node *ASF:: Alphabet::  getNode( size_t &inode )
    {
        Node *node = &nodes[inode++];
        bzset(*node);
        return node;
    }


    bool ASF:: Alphabet:: split(Node        *source,
                                const Char  *head,
                                const Char  *tail,
                                const size_t size,
                                size_t      &inode ) throw()
    {
        assert(source); assert(head); assert(tail); assert(size>0);
        if(size<=1)
        {
            assert(tail==head);
            source->ch  = head;
            head->bits  = source->bits;
            head->code  = source->code;
            return true;
        }
        else
        {
            const size_t new_bits = source->bits+1;
            const size_t new_code = source->code<<1;
            //------------------------------------------------------------------
            // first pass: total frequencies and partial frequencies
            //------------------------------------------------------------------
            {
                const Char *ch = head;
                source->freq   = ch->part = ch->freq;
                for(size_t i=size;i>1;--i)
                {
                    ch=ch->next;
                    source->freq += ch->freq;;
                    ch->part      = source->freq;
                }
            }

            //------------------------------------------------------------------
            // second pass: cut with total frequencies and partial frequencies
            //------------------------------------------------------------------
            const size_t fsum   = source->freq;
            const Char  *L_tail = head;
            size_t       L_size = 1;
            const Char  *R_head = L_tail->next;
            while(R_head->part<fsum-R_head->part)
            {
                ++L_size;
                L_tail = R_head;
                R_head = R_head->next;
            }

            {
                Node *node = source->left = getNode(inode);
                node->bits    = new_bits;
                node->code    = new_code;
                split(node,head,L_tail,L_size,inode);
            }

            {
                Node *node = source->right = getNode(inode);
                node->bits = new_bits;
                node->code = new_code|1;
                split(node,R_head,tail,size-L_size,inode);
            }

            return true;
        }
    }

    void ASF::Alphabet:: build_tree() throw()
    {
        size_t inode = 0;
        Node  *root  = getNode(inode);
        split(root,active.head,active.tail,active.size,inode);
    }


    void ASF:: Alphabet:: encode(iobits &io, const char C)
    {
        //----------------------------------------------------------------------
        // get the corresponding char
        //----------------------------------------------------------------------
        Char *ch = &chars[ uint8_t(C) ]; assert(ch->byte==uint8_t(C));
        if(ch->freq<=0)
        {
            
            assert(in_use<NUM_CHARS);

            //------------------------------------------------------------------
            // a new char to use
            //------------------------------------------------------------------
            if(in_use>0)
            {
                io.push(nyt->code,nyt->bits);
            }
            io.push(ch->code,ch->bits);

            //------------------------------------------------------------------
            // update list
            //------------------------------------------------------------------
            ch->freq++;
            active.push_back(ch);
            while(ch->prev&&ch->prev->freq<=1)
            {
                assert(1==ch->freq);
                active.towards_head(ch);
            }

            //------------------------------------------------------------------
            // check how many used char, to discard NYT
            //------------------------------------------------------------------
            ++in_use;
            if(in_use>=NUM_CHARS)
            {
                (void) active.unlink(nyt);
            }
        }
        else
        {
            assert(in_use>0);
            //------------------------------------------------------------------
            // a used char
            //------------------------------------------------------------------
            io.push(ch->code,ch->bits);

            //------------------------------------------------------------------
            // update list
            //------------------------------------------------------------------
            ch->freq++;
            while(ch->prev&&ch->prev->freq<=ch->freq)
            {
                assert(1<ch->freq);
                active.towards_head(ch);
            }
        }
        build_tree();
    }

    void ASF:: Alphabet:: update(const char C) throw()
    {
        //----------------------------------------------------------------------
        // get the corresponding Char
        //----------------------------------------------------------------------
        Char *ch = &chars[ uint8_t(C) ]; assert(ch->byte==uint8_t(C));
        if(ch->freq<=0)
        {
            assert(in_use<NUM_CHARS);
            //------------------------------------------------------------------
            // update list
            //------------------------------------------------------------------
            ch->freq++;
            active.push_back(ch);
            while(ch->prev&&ch->prev->freq<=1)
            {
                assert(1==ch->freq);
                active.towards_head(ch);
            }

            //------------------------------------------------------------------
            // check how many used char, to discard NYT
            //------------------------------------------------------------------
            ++in_use;
            if(in_use>=NUM_CHARS)
            {
                (void) active.unlink(nyt);
            }
        }
        else
        {
            //------------------------------------------------------------------
            // update list
            //------------------------------------------------------------------
            ch->freq++;
            while(ch->prev&&ch->prev->freq<=ch->freq)
            {
                assert(1<ch->freq);
                active.towards_head(ch);
            }
        }
        build_tree();
    }


    void ASF:: Alphabet:: flush(iobits &io) const
    {
        if(in_use>0)
        {
            io.push(eos->code,eos->bits);
            io.zpad();
        }
        assert(0==io.size%8);
    }

}

namespace upsylon
{

    ASF:: Encoder:: Encoder() :
    ios::q_codec(),
    io(),
    alpha()
    {
    }

    ASF:: Encoder:: ~Encoder() throw()
    {
    }

    void ASF:: Encoder:: reset() throw()
    {
        alpha.reset();
        io.free();
        Q.free();
    }

    void ASF:: Encoder:: write(char C)
    {
        alpha.encode(io,C);
        while( io.size >= 8 )
        {
            Q.push_back( io.pop_full<uint8_t>() );
        }
    }

    void ASF:: Encoder:: flush()
    {
        alpha.flush(io);
        while( io.size >= 8 )
        {
            Q.push_back( io.pop_full<uint8_t>() );
        }
        assert(0==io.size);
    }

}


namespace upsylon
{
    ASF::Decoder:: ~Decoder() throw()
    {
    }

    ASF::Decoder:: Decoder() :
    ios::q_codec(),
    status( wait_for8 ),
    current(0),
    io(),
    alpha()
    {
    }

    void ASF::Decoder:: reset() throw()
    {
        alpha.reset();
        io.free();
        Q.free();
        status  = wait_for8;
        current = 0;
        
    }

    void ASF:: Decoder:: flush()
    {
        // should do nothing
    }

    void ASF::Decoder:: write(char C)
    {
        io.push_full<uint8_t>(C);
        process();
    }

    
    void ASF:: Decoder:: process()
    {
    PROCESS:
        switch(status)
        {
                //--------------------------------------------------------------
            case wait_for8: // waiting for a new byte
                //--------------------------------------------------------------
                assert(current==NULL);
                if(io.size<8)
                {
                    return;
                }
                else
                {
                    const char C = io.pop_full<uint8_t>();
                    alpha.update(C);
                    Q.push_back(C);
                    status  = wait_for1;
                    current = alpha.nodes;
                    goto PROCESS;
                }

                //--------------------------------------------------------------
            case wait_for1: // waiting for a new bit to walk down the tree
                //--------------------------------------------------------------
                assert(current!=NULL);
                if(io.size<=0)
                {
                    return;
                }
                else
                {
                    current = io.pop() ? current->right : current->left;
                    if(current->ch)
                    {
                        const CodeType byte = current->ch->byte;
                        switch(byte)
                        {
                            case NYT:
                                // a new char is coming
                                current = NULL;
                                status  = wait_for8;
                                break;

                            case EOS:
                                // drop garbage and continue wait_for1
                                io.drop();
                                current = alpha.nodes;
                                break;

                            default:
                                assert(byte<NUM_CHARS);
                                // emit decoded and continue wait_for1
                                alpha.update(byte);
                                Q.push_back(byte);
                                current = alpha.nodes;
                                break;
                        }
                    }
                    goto PROCESS;
                }
        }
    }

}

#include "y/ios/ocstream.hpp"
#include "y/ios/graphviz.hpp"
namespace upsylon
{

    void ASF::Node:: viz( ios::ostream &fp ) const
    {
        if(ch)
        {
            assert(!left);
            assert(!right);
            fp.viz(this)("[label=\"%s#%u\"];\n", ch->text(), unsigned(ch->freq) );
        }
        else
        {
            assert(left);
            assert(right);
            fp.viz(this)("[label=\"#%u\"];\n",unsigned(freq));
            left->viz(fp);
            fp.viz(this) << "->"; fp.viz(left) << ";\n";
            right->viz(fp);
            fp.viz(this) << "->"; fp.viz(right) << ";\n";
        }

    }

    void ASF:: Alphabet:: GraphViz(const string &filename) const
    {
        {
            ios::ocstream fp(filename);
            fp << "digraph G {\n";
            nodes[0].viz(fp);
            fp << "}\n";
        }
        ios::GraphViz::Render(filename);
    }


}
