#include "y/information/filter/alphabet.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        Alphabet:: Node:: Node() throw() :
        byte( Built ),
        freq(0),
        code(0),
        bits(0)
        {
            left = right = 0;
        }

        Alphabet:: Node:: Node( const CodeType leaf ) throw() :
        byte( leaf ),
        freq(0),
        code( leaf ),
        bits( bits_for(leaf) )
        {
            prev = next = 0;
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
                default: if(byte<Chars) return visible_char[ byte & 0xff ];
            }
            return "";
        }


        void Alphabet:: Node:: vizCore(ios::ostream &fp) const
        {
            endl( fp( " [label=\"%s@%lu\"]", NameOf(byte), (unsigned long)freq ) );

        }

        void Alphabet:: Format( Node *nodes, const size_t count ) throw()
        {
            assert(nodes);
            assert(count>=Chars);
            for(CodeType i=0;i<Codes;++i)
            {
                new (nodes+i) Node(i);
            }

            for(CodeType i=Chars;i<count;++i)
            {
                new (nodes+i) Node();
            }

        }


    }
}
