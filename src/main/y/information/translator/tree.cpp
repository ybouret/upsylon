
#include "y/information/translator/tree.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace information {

        TranslatorTree:: ~TranslatorTree() throw()
        {
            memory::global::location().release(_nodes,_bytes);
            _extra = 0;
            aliasing::_(_shift) = 0;
        }

        TranslatorTree:: TranslatorTree(const size_t sizeof_node,
                                        const size_t extra_bytes) :
        _shift( memory::align(sizeof_node*Nodes) ),
        _bytes( memory::align(extra_bytes) + _shift ),
        _nodes( memory::global::instance().acquire(_bytes) ),
        _extra( _shift ? static_cast<char *>(_nodes) + _shift : 0)
        {

        }

        void TranslatorTree:: restart() throw()
        {
            initialize();
            buildTree();
        }


    }

}
