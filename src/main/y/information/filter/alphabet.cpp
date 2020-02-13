#include "y/information/filter/alphabet.hpp"
#include "y/memory/global.hpp"
#include "y/code/round.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace information {


        void Alphabet:: initialize() throw()
        {
            static const FreqType ControlFreq = 0;

            //! restart level
            aliasing::_(level) = 0;

            //! clean alphabet
            alpha.reset();

            for(CodeType i=0;i<Chars;++i)
            {
                new (nodes+i) Node(i,8);
                assert(0==nodes[i].next);
                assert(0==nodes[i].prev);
                assert(i==nodes[i].symbol);
            }

            for(CodeType i=Chars;i<Codes;++i)
            {
                assert( 9 == bits_for(i) );
                new (nodes+i) Node(i,9);
                assert(0==nodes[i].next);
                assert(0==nodes[i].prev);
                assert(i==nodes[i].symbol);
            }

            for(CodeType i=Codes;i<Nodes;++i)
            {
                new (nodes+i) Node();
                assert(0==nodes[i].next);
                assert(0==nodes[i].prev);
                assert(Built==nodes[i].symbol);
            }

            {
                assert(nyt->symbol==NYT);
                assert(0==nyt->frequency);
                nyt->frequency = ControlFreq;
                alpha.push_back(nyt);
            }

            {
                assert(eos->symbol==EOS);
                assert(0==eos->frequency);
                switch(mode)
                {
                    case StreamMode:
                        eos->frequency = ControlFreq;
                        alpha.push_back(eos);
                        break;
                    case BufferMode: break;
                }
            }


        }

        Alphabet:: ~Alphabet() throw()
        {
            alpha.reset();
            void *addr = nodes;
            memset(addr,0,bytes);
            memory::global::location().release( addr, aliasing::_(bytes) );
            nodes = 0;
            aliasing::_(level) = 0;
            aliasing::_(shift) = 0;
            aliasing::_(added) = 0;
            nyt = 0;
            eos = 0;
        }

        void * Alphabet:: extra() throw()
        {
            void *addr = static_cast<void *>(nodes);
            char *buff = static_cast<char *>(addr);
            return buff+shift;
        }

        Alphabet:: Alphabet(const Mode   operating,
                            const size_t extraBytes) :
        mode(operating),
        level(0),
        shift( memory::align(Nodes*sizeof(Node)) ),
        bytes( shift+extraBytes   ),
        alpha(),
        nodes( static_cast<Node*>(memory::global::instance().acquire( aliasing::_(bytes) ) ) ),
        nyt(nodes+NYT),
        eos(nodes+EOS),
        added( bytes - shift )
        {
            initialize();
        }


        Alphabet::Node *Alphabet:: emit(qbits &io, const uint8_t u)
        {
            Node *node = nodes+u; assert(u==node->symbol);
            if( node->frequency++ <= 0 )
            {
                assert(level<256);
                assert(8==node->bits);
                switch(aliasing::_(level)++)
                {
                    case 0:    // very first char, no prolog
                        break;
                    case 255: // emit nyt and remove it
                        nyt->emit(io);
                        nyt->frequency = 0;
                        (void) alpha.unlink(nyt);
                        break;
                    default:
                        assert(level<256);
                        nyt->emit(io);
                        break;
                }
                alpha.push_back(node);
                // always keep the control chars at the end
                while(node->prev&&node->prev->symbol>=Chars)
                {
                    alpha.towards_head(node);
                }
            }
            node->emit(io);
            return node;
        }


        void Alphabet:: displayAlpha() const
        {
            std::cerr << "<alphabet size=" << alpha.size << ", level=" << level << ">" << std::endl;
            for(const Node *node=alpha.head;node;node=node->next)
            {
                std::cerr << "\t"; node->display();
            }
            std::cerr << "<alphabet/>" << std::endl;
        }


    }
}
