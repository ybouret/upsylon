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
        bits( (leaf < 256) ? 8 : 0 )
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

        void Alphabet:: Node::emit( qbits &io ) const
        {
            io.push(code,bits);
        }


        void Alphabet:: Node:: vizCore(ios::ostream &fp) const
        {
            endl( fp( " [label=\"%s@%lu\"]", NameOf(symbol), (unsigned long)frequency ) );
            if(left)
            {
                left->vizSave(fp);
                endl(vizJoin(fp,left) << " [label=\"0\"]");
            }
            if(right)
            {
                right->vizSave(fp);
                endl(vizJoin(fp,left) << " [label=\"1\"]");
            }
        }

        void Alphabet:: initialize() throw()
        {
            assert(nodes);
            assert(count>=Chars);

            level = 0;

            for(CodeType i=0;i<Codes;++i)
            {
                new (nodes+i) Node(i);
            }

            for(CodeType i=Codes;i<count;++i)
            {
                new (nodes+i) Node();
            }

            {
                assert(nyt->symbol==NYT);
                assert(0==nyt->frequency);
                nyt->frequency = 1;
                alpha.push_back(nyt);
            }
            
            {
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

        Alphabet::Node::FrequencyComparator:: FrequencyComparator() throw()
        {
        }

        Alphabet::Node::FrequencyComparator:: ~FrequencyComparator() throw()
        {
        }


        int Alphabet::Node::FrequencyComparator:: operator()(const Node &lhs, const Node &rhs) throw()
        {
            const FreqType L = lhs.frequency;
            const FreqType R = rhs.frequency;
            return (L<R) ? -1 : ( (R<L) ? 1 : 0 );
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
        nyt(0),
        eos(0),
        level(0),
        count(numNodes),
        shift( bytesFor(numNodes) ),
        bytes( shift+extraBytes   ),
        nodes( 0 ),
        extra( 0 )
        {
            void *wksp = memory::global::instance().acquire(bytes);
            nodes = static_cast<Node*>(wksp);
            extra = static_cast<uint8_t*>(wksp) + shift;
            nyt = nodes+NYT;
            eos = nodes+EOS;
            initialize();
        }


        void Alphabet:: emit(qbits &io, const uint8_t u)
        {
            Node *node = nodes+u; assert(u==node->symbol);
            if( node->frequency++ <= 0 )
            {
                assert(level<256);
                switch(level++)
                {
                    case 0:    // very first char, no prolog
                        break;
                    case 255: // emit nyt and remove it
                        nyt->emit(io);
                        nyt->frequency = 0;
                        (void) alpha.unlink(nyt);
                        break;
                    default:
                        nyt->emit(io);
                        break;
                }
                alpha.push_back(node);
            }
            node->emit(io);

        }

    }
}
