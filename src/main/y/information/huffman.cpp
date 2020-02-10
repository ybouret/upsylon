#include "y/information/huffman.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        Huffman:: Node:: ~Node() throw()
        {
        }

        Huffman:: Node:: Node() throw() :
        code(0),
        bits(0),
        freq(0),
        next(0),
        prev(0),
        left(0),
        right(0)
        {
        }

        void Huffman::Node:: vizCore( ios::ostream &fp ) const
        {
            if(code<256)
            {
                fp( " [label=\"%s@%u\"]", visible_char[code], unsigned(freq) );
            }
            else
            {
                switch(code)
                {
                    case NYT: fp(" [label=\"NYT@%u\"]",unsigned(freq)); break;
                    case EOS: fp(" [label=\"EOS@%u\"]",unsigned(freq)); break;
                    default:
                        break;
                }
            }
            endl(fp);
            if(left)
            {
                left->vizSave(fp);
                endl( this->vizJoin(fp, left) << " [label=\"0\"]" );
            }
            if(right)
            {
                right->vizSave(fp);
                endl( this->vizJoin(fp, right) << " [label=\"1\"]" );
            }
        }

        Huffman:: Tree:: Tree( const Mode mode) throw() :
        root(0),
        alpha(),
        pq( NumNodes, as_capacity ),
        nodeCount(NumNodes),
        nodeBytes(0),
        nodes( memory::global::instance().acquire_as<Node>(nodeCount,nodeBytes) )
        {
            reset(mode);
            std::cerr << "heap.allocated=" << pq.allocated() << std::endl;
            std::cerr << "nodes.bytes   =" << nodeBytes  << std::endl;
        }

        void Huffman::Tree:: clear() throw()
        {
            root = 0;
            alpha.reset();
            for(size_t i=0;i<NumNodes;++i)
            {
                new ( &nodes[i] ) Node();
            }
            pq.free();
        }


        Huffman:: Tree:: ~Tree() throw()
        {
            clear();
            memory::global::location().release_as(nodes, nodeCount, nodeBytes);
        }

        void Huffman:: Tree:: reset(const Mode mode) throw()
        {
            clear();

            for(size_t i=0;i<NumChars;++i)
            {
                Node &node = nodes[i];
                node.code = i;
                node.bits = 8;
            }

            {
                Node &node = nodes[NYT];
                node.code  = NYT;
                node.bits  = 9;
                node.freq  = 1;

                alpha.push_back( &node );
            }

            {
                Node &node = nodes[EOS];
                node.code  = EOS;
                node.bits  = 9;
                switch(mode)
                {
                    case StreamMode:
                        node.freq = 1;
                        alpha.push_back(&node);
                        break;

                    case BufferMode:
                        node.freq = 0;
                        break;
                }

            }

            build();
        }

        void Huffman:: Tree:: build() throw()
        {
            assert(alpha.size>0);
            assert(pq.size()<=0);

            for(Node *node=alpha.head;node;node=node->next)
            {
                pq.push_(node);
            }

            std::cerr << "pq.size=" << pq.size() << std::endl;
            size_t inode = AlphabetSize;
            while( pq.size() > 1 )
            {
                assert(inode<NumNodes);
                Node *left   = pq.pop();
                Node *right  = pq.pop();
                Node *parent = &nodes[inode++];
                parent->left  = left;
                parent->right = right;
                parent->freq  = left->freq + right->freq;
                pq.push_( parent );
            }
            std::cerr << "inode=" << inode << std::endl;
            root = pq.pop();
        }

        void Huffman:: Tree:: encode(qbits &io, const uint8_t byte)
        {
            Node *node = &nodes[byte];
            if(node->freq++<=0)
            {
                alpha.push_back(node);
            }
            build();

        }
    }

}
