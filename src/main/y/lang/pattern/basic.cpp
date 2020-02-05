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

        void Any1:: vizCore(ios::ostream &fp) const
        {
            endl(fp(" [shape=circle,label=\"any1\"]"));
        }


        bool Any1:: univocal() const throw()
        {
            return false;
        }

        bool Any1:: accept_byte( const uint8_t ) const throw()
        {
            return true;
        }

        size_t Any1:: serialize(ios::ostream &fp) const
        {
            size_t count = 0;
            (void)fp.emit_net<uint32_t>(uuid,&count);
            assert(4==count);
            return count;
        }

        const char *Any1:: className() const throw()
        {
            return CLID;
        }

        Y_LANG_PATTERN_CLID(Any1);

        bool Any1:: equals(const Pattern &p) const throw()
        {
            assert(UUID==uuid);
            return UUID == p.uuid;
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

        void Single:: vizCore(ios::ostream &fp) const
        {
            endl(fp(" [shape=square,label=\"%s\"]", printable_char[code] ));
        }
        
        bool Single:: univocal() const throw()
        {
            return true;
        }

        size_t Single:: serialize(ios::ostream &fp) const
        {
            size_t nUUID = 0, nCODE = 0;
            fp.emit_net<uint32_t>(UUID,&nUUID).emit_net<uint8_t>(code,&nCODE);
            assert(4==nUUID); assert(1==nCODE);
            return nUUID+nCODE;
        }

        const char *Single:: className() const throw()
        {
            return CLID;
        }

        Y_LANG_PATTERN_CLID(Single);

        bool Single:: equals(const Pattern &p) const throw()
        {
            if( UUID == p.uuid )
            {
                return code == static_cast<const Single *>(p.priv)->code;
            }
            else
            {
                return false;
            }
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

        void Range:: vizCore(ios::ostream &fp) const
        {
            endl(fp(" [shape=box,label=\"%s-%s\"]", printable_char[lower], printable_char[upper] ));
        }
        
        size_t Range:: serialize(ios::ostream &fp) const
        {
            size_t nUUID  = 0; (void)fp.emit_net<uint32_t>(uuid,&nUUID);  assert(4==nUUID);
            size_t nLOWER = 0; (void)fp.emit_net<uint8_t>(lower,&nLOWER); assert(1==nLOWER);
            size_t nUPPER = 0; (void)fp.emit_net<uint8_t>(upper,&nUPPER); assert(1==nUPPER);
            return nUUID+nLOWER+nUPPER;
        }

        const char *Range:: className() const throw()
        {
            return CLID;
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

        Y_LANG_PATTERN_CLID(Range);

        bool Range:: equals(const Pattern &p) const throw()
        {
            if( UUID ==  p.uuid )
            {
                const Range &r = *static_cast<const Range *>(p.priv);
                return (r.lower == lower) && (r.upper==upper);
            }
            else
            {
                return false;
            }
        }

    }
}
