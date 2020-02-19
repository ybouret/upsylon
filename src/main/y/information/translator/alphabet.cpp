
#include "y/information/translator/alphabet.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"
#include <iomanip>

namespace upsylon {

    namespace information {

        void Alphabet::Char::emit(qbits &io) const
        {
            io.push(code,bits);
        }

        void Alphabet::Char:: display() const
        {
            std::cerr << std::setw(4) << NameOf(symbol) << " #" << std::setw(6) << frequency << std::endl;
        }


        const char * Alphabet::NameOf(const CodeType code) throw()
        {
            switch(code)
            {
                case EOS: return "EOS";
                case NYT: return "NYT";
                default: if(code<Bytes) return printable_char[code&0xff];
            }
            return "BAD";
        }

        Alphabet:: ~Alphabet() throw()
        {
            usedBytes = 0;
            chars.reset();
            {
                void *addr = character;
                memset(addr,0,allocated);
                memory::global::location().release(addr,allocated);
            }
            character = 0;
            nyt       = 0;
            eos       = 0;
        }

        Alphabet:: Alphabet() :
        chars(),
        eos(0),
        nyt(0),
        usedBytes(0),
        allocated( memory::align( Codes * sizeof(Char) ) ),
        character( static_cast<Char *>(memory::global::instance().acquire(allocated)) )
        {
            eos = character + EOS;
            nyt = character + NYT;
            initialize();
        }

#ifndef NDEBUG
        static inline bool areOrdered( const Alphabet::Chars &chars )
        {
            for(const Alphabet::Char *ch=chars.head;ch&&ch->next;ch=ch->next)
            {
                if(ch->frequency<ch->next->frequency) return false;
            }
            return true;
        }
#endif

        void Alphabet:: initialize() throw()
        {
            usedBytes = 0;
            chars.reset();

            for(size_t i=0;i<Bytes;++i)
            {
                Char &ch     = character[i];
                ch.symbol    = ch.code = i;
                ch.bits      = 8;
                ch.frequency = 0;
                ch.next      = 0;
                ch.prev      = 0;
            }

            for(size_t i=Bytes;i<Codes;++i)
            {
                Char &ch     = character[i];
                ch.symbol    = ch.code = i;
                ch.bits      = 9; assert( ch.bits == bits_for(ch.code) );
                ch.frequency = 0;
                ch.next      = 0;
                ch.prev      = 0;
            }

            chars.push_back(eos);
            chars.push_back(nyt);

            assert( areOrdered(chars) );

        }



        bool Alphabet:: sameThan( const Alphabet &other ) const throw()
        {
            if(usedBytes!=other.usedBytes)
            {
                std::cerr << "different usedBytes" << std::endl;
                return false;
            }

            if( chars.size != other.chars.size )
            {
                std::cerr << "different #chars" << std::endl;
                return false;
            }

            for(const Char *lhs=chars.head, *rhs=other.chars.head; lhs; lhs=lhs->next, rhs=rhs->next )
            {
                if( lhs->symbol != rhs->symbol )
                {
                    std::cerr << "mismatch symbol" << std::endl;
                    return false;
                }
                if( lhs->code != rhs->code )
                {
                    std::cerr << "mismatch code" << std::endl;
                    return false;
                }
                if( lhs->bits != rhs->bits )
                {
                    std::cerr << "mismatch bits" << std::endl;
                    return false;
                }
                if(lhs->frequency!=rhs->frequency)
                {
                    std::cerr << "mismatch frequency" << std::endl;
                    return false;
                }
            }

            return true;
        }


       void Alphabet:: updateByte( const uint8_t byte, qbits *io )
        {
            Char *chr = character + byte;
            if( chr->frequency++ <= 0 )
            {
                //--------------------------------------------------------------
                //
                // this is a new char
                //
                //--------------------------------------------------------------
                assert(usedBytes<Bytes);
                assert( chars.owns(nyt) );

                switch( usedBytes )
                {

                    case 0:
                        //------------------------------------------------------
                        // first node ever
                        //------------------------------------------------------
                        if(io)
                        {
                            chr->emit(*io);
                        }
                        chars.push_front(chr);
                        usedBytes = 1;
                        assert(areOrdered(chars));
                        return;

                    case 255:
                        //------------------------------------------------------
                        // last node ever
                        //------------------------------------------------------
                        if(io)
                        {
                            nyt->emit(*io);
                            chr->emit(*io);
                        }
                        usedBytes = Bytes;
                        chars.insert_before(eos,chr);
                        (void) chars.unlink(nyt);
                        break;

                    default:
                        //------------------------------------------------------
                        // generic update
                        //------------------------------------------------------
                        assert(usedBytes>=1);
                        assert(usedBytes<255);
                        if(io)
                        {
                            nyt->emit(*io);
                            chr->emit(*io);
                        }
                        chars.insert_before(eos,chr);
                        ++usedBytes; assert(usedBytes<Bytes);
                        break;
                }
                assert(usedBytes>=2);
            }
            else
            {
                assert(usedBytes<=Bytes);
                if(io)
                {
                    chr->emit(*io);
                }
            }
            while(chr->prev && chr->prev->frequency<chr->frequency)
            {
                chars.towards_head(chr);
            }
            assert( areOrdered(chars) );
        }



        void Alphabet:: displayChars() const
        {
            std::cerr << "<Alphabet size=" << chars.size << ">" << std::endl;
            for(const Char *chr = chars.head; chr; chr=chr->next )
            {
                std::cerr << "\t"; chr->display();
            }
            std::cerr << "<Alphabet/>" << std::endl;
        }


    }

}
