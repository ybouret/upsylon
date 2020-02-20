#include "y/information/translator/shannon-fano.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            Tree:: Tree() : Alphabet(),
            treeBytes( Nodes * sizeof(Node) ),
            treeNodes( static_cast<Node *>( memory::global::instance().acquire(treeBytes) ) )
            {

            }

            Tree:: ~Tree() throw()
            {
                
            }

        }

    }

}

