
//! \file

#ifndef Y_JIVE_INPUT_INCLUDED
#define Y_JIVE_INPUT_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! shared input definition
        //
        //______________________________________________________________________
        typedef arc_ptr<ios::istream> Input;

        //______________________________________________________________________
        //
        //
        //! returning an input stream
        //
        //______________________________________________________________________
        struct MakeInput
        {
            static ios::istream * FromFile(const string &filename);              //!< ios::icstream
            static ios::istream * FromData(const void *data, const size_t size); //!< ios::imstream
        };

    }

}

#endif
