
#include "y/codec/asf.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    const char *  ASF::Char:: text() const throw()
    {
        if(code<NUM_CHARS)
        {
            return visible_char[code];
        }
        else if(code==NYT)
        {
            return "nyt";
        }
        else if(code==EOS)
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
    }

    ASF::Alphabet:: Alphabet() throw() :
    chars(),
    active(),
    nyt( chars[NYT] ),
    eos( chars[EOS] ),
    nodes()
    {
        for(size_t i=0;i<ALPHABET_SIZE;++i)
        {
            Char *ch = &chars[i];
            ch->next = 0;
            ch->prev = 0;
            ch->code = i;
            ch->freq = 0;
        }
        init();
    }


    void ASF::Alphabet:: init() throw()
    {
        assert(0==active.size);
        nyt.freq = 1;
        eos.freq = 1;
        active.push_back(&nyt);
        active.push_back(&eos);
    }

    void ASF::Alphabet:: reset() throw()
    {
        active.reset();
        for(size_t i=0;i<ALPHABET_SIZE;++i)
        {
            Char *ch = &chars[i];
            ch->next = 0;
            ch->prev = 0;
            ch->freq = 0;
        }
        init();
    }

    void ASF::Alphabet:: display(std::ostream &os) const
    {
        for(const Char *ch = active.head; ch;ch=ch->next)
        {
            os << ch->text() << " : " << ch->freq << std::endl;
        }
    }


    void ASF:: Alphabet:: add(const char C) throw()
    {
        Char         *ch = &chars[ uint8_t(C) ]; assert(ch->code==uint8_t(C));
        if(ch->freq<=0)
        {
            assert(!active.owns(ch));
            ch->freq++;
            active.push_back(ch);
            while(ch->prev&&ch->prev->freq<=1)
            {
                assert(1==ch->freq);
                active.towards_head(ch);
            }
        }
        else
        {
            ch->freq++;
            while(ch->prev&&ch->prev->freq<=ch->freq)
            {
                assert(1<ch->freq);
                active.towards_head(ch);
            }
        }
    }

    void ASF:: Alphabet:: split(Node        *source,
                                const Char  *head,
                                const Char  *tail,
                                const size_t size,
                                size_t      &inode ) throw()
    {
        assert(source); assert(head); assert(tail); assert(size>0);
        if(size<=1)
        {
            assert(tail==head);
            source->ch = head;
        }
        else
        {
            source->left  = &nodes[inode++];
            source->right = &nodes[inode++];
            const Char *L_head = head;
            const Char *R_tail = tail;
            size_t      L_size = 1;
            size_t      R_size = 1;
            exit(0);
        }
    }

    void ASF::Alphabet:: build_tree() throw()
    {
        size_t inode = 0;
        Node  *root  = & nodes[inode++];
        split(root,active.head,active.tail,active.size,inode);
    }

}

