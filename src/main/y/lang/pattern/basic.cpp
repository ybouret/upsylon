#include "y/lang/pattern/basic.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {

        bool Match1:: match( Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            Char *ch = src.get();
            if(!ch) return false;
            if( accept_byte(ch->code) )
            {
                tkn.push_back(ch);
                return true;
            }
            else
            {
                src.unget(ch);
                return false;
            }
        }

        void Any1:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=circle,label=\"any1\"];\n");
        }

        void Any1:: write(ios::ostream &fp) const
        {
            fp.emit(UUID);
        }

        void Single:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=square,label=\"%s\"];\n", printable_char[code] );
        }

        void Single:: write(ios::ostream &fp) const
        {
            fp.emit(UUID).emit(code);
        }

        void Range:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=box,label=\"%s-%s\"];\n", printable_char[lower], printable_char[upper] );
        }

        void Range:: write(ios::ostream &fp) const
        {
            fp.emit(UUID).emit(lower).emit(upper);
        }

    }
}
