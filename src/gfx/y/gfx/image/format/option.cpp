
#include "y/gfx/image/format/option.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            option:: ~option() throw()
            {
            }

            const string & option:: key() const throw()
            {
                return name;
            }

            std::ostream & operator<<(std::ostream &os, const option &opt)
            {
                os << '[' << opt.name << '=' << opt.value << ']';
                return os;
            }

        }

    }

}
