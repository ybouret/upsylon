#include "y/lang/pattern/basic.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace Lang {

        Match1:: ~Match1() throw() {}

        Match1:: Match1(const uint32_t id) throw() : Pattern(id) {}

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

        
        bool Match1:: weak() const throw() { return false; }

    }

}

namespace upsylon {

    namespace Lang {

        Any1:: ~Any1() throw() {}

        Any1:: Any1() throw() : Match1(UUID) { Y_LANG_PATTERN_IS(Any1); }

        Any1 * Any1:: clone() const { return new Any1(); }   

        void Any1:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=circle,label=\"any1\"];\n");
        }

        void Any1:: write(ios::ostream &fp) const
        {
            fp.emit_net(UUID);
        }

        bool Any1:: univocal() const throw()
        {
            return false;
        }

        bool Any1:: accept_byte( const uint8_t ) const throw()
        {
            return true;
        }

    }

}

namespace upsylon {

    namespace Lang {

        Single:: Single(const uint8_t c) throw() : Match1(UUID), code(c)  { Y_LANG_PATTERN_IS(Single); }

        Single:: ~Single() throw() {}

        Single * Single:: clone() const { return new Single(code); }

        bool Single:: accept_byte( const uint8_t c) const throw()
        {
            return c==code;
        }

        void Single:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=square,label=\"%s\"];\n", printable_char[code] );
        }

        void Single:: write(ios::ostream &fp) const
        {
            fp.emit_net(UUID).emit_net(code);
        }

        bool Single:: univocal() const throw()
        {
            return true;
        }
        
    }

}

namespace upsylon {

    namespace Lang {

        Range:: ~Range() throw() {}

        Range:: Range(const uint8_t lo, const uint8_t up) throw() : Match1(UUID), lower(lo), upper(up)
        {
            Y_LANG_PATTERN_IS(Range);
            if(upper<lower) _cswap(lower,upper);
        }

        void Range:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=box,label=\"%s-%s\"];\n", printable_char[lower], printable_char[upper] );
        }

        void Range:: write(ios::ostream &fp) const
        {
            fp.emit_net(UUID).emit_net(lower).emit_net(upper);
        }

        bool Range:: univocal() const throw()
        {
            return (lower==upper);
        }

        Range * Range:: clone() const { return new Range(lower,upper); }


        bool Range:: accept_byte(const uint8_t c) const throw()
        {
            return (c>=lower) && (c<=upper);
        }
    }
}
