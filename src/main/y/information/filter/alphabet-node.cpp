
#include "y/information/filter/alphabet.hpp"
#include "y/code/utils.hpp"
#include <cstdio>

namespace upsylon {

    namespace information {

        Alphabet:: Node:: Node() throw() :
        symbol( Built ),
        frequency(0),
        code(0),
        bits(0),
        next(0),
        prev(0),
        left(0),
        right(0)
        {
        }

        Alphabet:: Node:: Node(const CodeType _code,
                               const size_t   _bits ) throw() :
        symbol( _code ),
        frequency(0),
        code( _code ),
        bits( _bits ),
        next(0),
        prev(0),
        left(0),
        right(0)
        {
        }

        Alphabet:: Node:: ~Node() throw()
        {
        }

        const char * Alphabet:: NameOf( const CodeType byte ) throw()
        {
            switch(byte)
            {
                case EOS: return "EOS";
                case NYT: return "NYT";
                default: if(byte<Chars) return printable_char[ byte & 0xff ];
            }
            return "";
        }

        void Alphabet:: Node::emit( qbits &io ) const
        {
            io.push(code,bits);
        }


        void Alphabet:: Node:: vizCore(ios::ostream &fp) const
        {
            endl( fp( " [label=\"%s@%lu:%u\"]", NameOf(symbol), (unsigned long)frequency, unsigned(bits)) );
            if(left)
            {
                left->vizSave(fp);
                endl(vizJoin(fp,left) << " [label=\"0\"]");
            }
            if(right)
            {
                right->vizSave(fp);
                endl(vizJoin(fp,right) << " [label=\"1\"]");
            }
        }

        void Alphabet::Node:: display() const
        {
            std::cerr.flush();
            fprintf( stderr, "%4s : 0x%08x : %2u : ", NameOf(symbol), unsigned(frequency), unsigned(bits) );
            fflush(stderr);

            for(int i=bits;i>0;--i)
            {
                if( 0 != (code & (1<< (i-1) ))) std::cerr << "1"; else std::cerr << "0";
            }


            std::cerr << std::endl;
        }



        Alphabet::Node::Priority:: Priority() throw()
        {
        }

        Alphabet::Node::Priority:: ~Priority() throw()
        {
        }


        int Alphabet::Node::Priority:: operator()(const Node &lhs, const Node &rhs) throw()
        {
            const FreqType L = lhs.frequency;
            const FreqType R = rhs.frequency;
            return (L<R) ? 1 : ( (R<L) ? -1 : 0 );
        }

    }

}


