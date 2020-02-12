#include "y/information/filter/alphabet.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        Alphabet:: Node:: Node() throw() :
        symbol( Built ),
        frequency(0),
        code(0),
        bits(0)
        {
            left = right = 0;
        }

        Alphabet:: Node:: Node( const CodeType leaf ) throw() :
        symbol( leaf ),
        frequency(0),
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
            endl( fp( " [label=\"%s@%lu\"]", NameOf(symbol), (unsigned long)frequency ) );

        }

        void Alphabet:: format() throw()
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

            {
                Node  *nyt = nodes+NYT;
                assert(nyt->symbol==NYT);
                assert(0==nyt->frequency);
                nyt->frequency = 1;
                alpha.push_back(nyt);
            }
            
            {
                Node *eos = nodes+EOS;
                assert(eos->symbol==EOS);
                assert(0==eos->frequency);
                switch(mode)
                {
                    case StreamMode:
                        eos->frequency = 1;
                        alpha.push_back(eos);
                        break;
                    case BufferMode: break;
                }
            }

        }

    }

}

#include "y/memory/global.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace information {

        Alphabet:: ~Alphabet() throw()
        {
        }

        static  size_t bytesFor( const size_t numNodes )
        {
            return memory::align(numNodes * sizeof(Alphabet::Node));
        }

        Alphabet:: Alphabet(const size_t numNodes,
                            const Mode   operating,
                            const size_t extraBytes) :
        mode(operating),
        alpha(),
        count(numNodes),
        shift( bytesFor(numNodes) ),
        bytes( shift+extraBytes   ),
        nodes( 0 ),
        extra( 0 )
        {
            void *wksp = memory::global::instance().acquire(bytes);
            nodes = static_cast<Node*>(wksp);
            extra = static_cast<uint8_t*>(wksp) + shift;
            format();

        }

    }
}
