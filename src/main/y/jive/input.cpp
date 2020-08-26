
#include "y/jive/input.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {

        ios::istream * MakeInput:: FromFile(const string &filename)
        {
            return new ios::icstream(filename);
        }

        ios::istream * MakeInput:: FromData(const void *data, const size_t size)
        {
            if(NULL==data&&size>0) throw exception("Jive::MakeInput::FromData(invalid)");
            return new ios::imstream(data,size);
        }

    }

}
