
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
    eos( chars[EOS] )
    {
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
            ch->code = i;
            ch->freq = 0;
        }
        nyt.freq = 1;
        eos.freq = 1;
        active.push_back(&nyt);
        active.push_back(&eos);
    }

    void ASF::Alphabet:: display(std::ostream &os) const
    {
        for(const Char *ch = active.head; ch;ch=ch->next)
        {
            os << ch->text() << " : " << ch->freq << std::endl;
        }
    }


}

