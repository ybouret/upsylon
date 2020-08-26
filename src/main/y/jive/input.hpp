
//! \file

#ifndef Y_JIVE_INPUT_INCLUDED
#define Y_JIVE_INPUT_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace Jive
    {
        typedef arc_ptr<ios::istream> Input;

        struct MakeInput
        {
            static ios::istream * FromFile(const string &filename);
            static ios::istream * FromData(const void *data, const size_t size);
        };

    }

}

#endif
