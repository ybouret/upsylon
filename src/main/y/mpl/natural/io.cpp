
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace mpl
    {
        static inline bool is0(const char C) throw()
        {
            return '0' == C;
        }


        void natural:: display( std::ostream &os ) const
        {
            std::ios_base::fmtflags flags = os.flags();
            if( flags &  std::ios::hex)
            {
                //______________________________________________________________
                //
                // hexadecimal
                //______________________________________________________________
                if(bytes<=0)
                {
                    os << "0x0";
                }
                else
                {
                    string s(2+(bytes<<1),as_capacity);
                    for(size_t i=0;i<bytes;++i)
                    {
                        const char *h = hexadecimal::lowercase[byte[i]];
                        s << h[1] << h[0];
                    }
                    (s.trim(is0) << "x0").reverse();
                    os << s;
                }
            }
            else if( flags & std::ios::dec )
            {
                //______________________________________________________________
                //
                // decimal
                //______________________________________________________________
                if(bytes<=0)
                {
                    os << '0';
                }
                else
                {
                    const natural ten(10);
                    natural q,r;

                }

            }
            else
            {
                throw exception("natural.display: unsupported format");
            }
        }

    }

}

