
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
                default: if(code<Bytes) return visible_char[code&0xff];
            }
            return "BAD";
        }

        Alphabet:: ~Alphabet() throw()
        {
            usedBytes = 0;
            chars.reset();
            {
                void *addr = nodes;
                memset(addr,0,allocated);
                memory::global::location().release(addr,allocated);
            }
            nodes = 0;
            nyt   = 0;
            eos   = 0;
        }

        Alphabet:: Alphabet() :
        chars(),
        nyt(0),
        eos(0),
        usedBytes(0),
        allocated( memory::align( Codes * sizeof(Char) ) ),
        nodes( static_cast<Char *>(memory::global::instance().acquire(allocated)) )
        {
            nyt = nodes + NYT;
            eos = nodes + EOS;
            initialize();
        }

        void Alphabet:: initialize() throw()
        {
            usedBytes = 0;
            chars.reset();

            for(size_t i=0;i<Bytes;++i)
            {
                Char &ch     = nodes[i];
                ch.symbol    = ch.code = i;
                ch.bits      = 8;
                ch.frequency = 0;
                ch.next      = 0;
                ch.prev      = 0;
            }

            for(size_t i=Bytes;i<Codes;++i)
            {
                Char &ch     = nodes[i];
                ch.symbol    = ch.code = i;
                ch.bits      = 9; assert( ch.bits == bits_for(ch.code) );
                ch.frequency = 0;
                ch.next      = 0;
                ch.prev      = 0;
            }

            chars.push_back(nyt);
            chars.push_back(eos);

        }

        void Alphabet:: rank( Char *chr ) throw()
        {
            assert(chr);
            assert(chars.owns(chr));
            while( chr->prev && chr->prev->frequency<chr->frequency )
            {
                chars.towards_head(chr);
            }
#ifndef NDEBUG
            for(const Char *node=chars.head;node->next;node=node->next)
            {
                assert(node->frequency>=node->next->frequency);
            }
#endif
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


        Alphabet::Char * Alphabet:: updateByte( const uint8_t byte, qbits &io ) throw()
        {
            Char *chr = nodes + byte;
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
                        chr->emit(io);
                        chars.push_front(chr);
                        usedBytes = 1;
                        return chr;

                    case 255:
                        //------------------------------------------------------
                        // last node ever
                        //------------------------------------------------------
                        nyt->emit(io);
                        chr->emit(io);
                        usedBytes = Bytes;
                        chars.insert_before(nyt,chr);
                        (void) chars.unlink(nyt);
                        break;

                    default:
                        //------------------------------------------------------
                        // generic update
                        //------------------------------------------------------
                        assert(usedBytes>=1);
                        assert(usedBytes<255);
                        nyt->emit(io);
                        chr->emit(io);
                        chars.insert_before(nyt,chr);
                        ++usedBytes; assert(usedBytes<Bytes);
                        break;
                }
                assert(usedBytes>=2);
            }
            else
            {
                assert(usedBytes<=Bytes);
                chr->emit(io);
            }
            return chr;
        }

        Alphabet::Char * Alphabet:: updateByte(const uint8_t byte) throw()
        {
            Char *chr = nodes + byte;
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
                        //chr->emit(io);
                        chars.push_front(chr);
                        usedBytes = 1;
                        return chr;

                    case 255:
                        //------------------------------------------------------
                        // last node ever
                        //------------------------------------------------------
                        //nyt->emit(io);
                        //chr->emit(io);
                        usedBytes = Bytes;
                        chars.insert_before(nyt,chr);
                        (void) chars.unlink(nyt);
                        break;

                    default:
                        //------------------------------------------------------
                        // generic update
                        //------------------------------------------------------
                        assert(usedBytes>=1);
                        assert(usedBytes<255);
                        //nyt->emit(io);
                        //chr->emit(io);
                        chars.insert_before(nyt,chr);
                        ++usedBytes; assert(usedBytes<Bytes);
                        break;
                }
                assert(usedBytes>=2);
            }
            else
            {
                assert(usedBytes<=Bytes);
                //chr->emit(io);
            }
            return chr;
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
