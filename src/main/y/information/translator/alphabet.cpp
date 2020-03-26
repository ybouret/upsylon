
#include "y/information/translator/alphabet.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"
#include <iomanip>

namespace upsylon {

    namespace Information {
        

        void Alphabet::Char::emit(IOBits &io) const
        {
            io.push(code,bits);
        }

        void Alphabet::Char:: display() const
        {
            std::cerr << std::setw(4) << NameOf(symbol) << " #" << std::setw(6) << frequency;
            std::cerr << " : " << std::setw(3) << bits << " [";
            for(size_t i=0;i<bits;++i)
            {
                const bool tst = ( 0 != (code & (1<<i) ) );
                std::cerr << (tst ? '1' : '0' );
            }
            std::cerr << "]" << std::endl;
        }


        const char * Alphabet::NameOf(const CodeType code) throw()
        {
            switch(code)
            {
                case EOS: return "EOS";
                case NYT: return "NYT";
                default: if(code<Bytes) return cchars::encoded[code&0xff];
            }
            return "BAD";
        }

        Alphabet:: ~Alphabet() throw()
        {
            usedBytes = 0;
            chars.reset();
            {
                void *addr = charNodes;
                memset(addr,0,charBytes);
                memory::global::location().release(addr,charBytes);
            }
            charNodes = 0;
            nyt       = 0;
            eos       = 0;
        }

        Alphabet:: Alphabet() :
        chars(),
        eos(0),
        nyt(0),
        usedBytes(0),
        charBytes( memory::align( Codes * sizeof(Char) ) ),
        charNodes( static_cast<Char *>(memory::global::instance().acquire(charBytes)) )
        {
            eos = charNodes + EOS;
            nyt = charNodes + NYT;
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
                Char &ch     = charNodes[i];
                ch.symbol    = ch.code = i;
                ch.bits      = 8;
                ch.frequency = 0;
                ch.next      = 0;
                ch.prev      = 0;
            }

            for(size_t i=Bytes;i<Codes;++i)
            {
                Char &ch     = charNodes[i];
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

        void Alphabet:: rescaleFrequencies() throw()
        {
            for(Char *chr = chars.head;chr;chr=chr->next)
            {
                FreqType &f = chr->frequency;
                if(f!=0)
                {
                    f >>= 1;
                    f |=  1;
                }
            }
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


        void Alphabet:: writeEOS(IOBits &io) const
        {
            if(usedBytes)
            {
                eos->emit(io);
            }
        }

       void Alphabet:: writeAndUpdateByte( const uint8_t byte, IOBits *io )
        {
            Char *chr = charNodes + byte;
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
                        if(io)
                        {
                            nyt->emit(*io);
                            chr->emit(*io);
                        }
                        assert(usedBytes>=1);
                        assert(usedBytes<255);
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
