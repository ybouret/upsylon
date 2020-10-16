#include "y/jive/char.hpp"
#include "y/type/block/zset.hpp"


namespace upsylon {

    Y_MAGAZINE_IMPL(Jive::Char::Supply,object::life_time-1)

    namespace Jive
    {

        //======================================================================
        //
        // Char
        //
        //======================================================================
        Char:: Char(const Context &ctx, const uint8_t ch) throw() :
        Context(ctx),
        code(ch)
        {
        }

        Char:: Char(const Char &other) throw() :
        inode<Char>(),
        Context( static_cast<const Context&>(other) ),
        code(other.code)
        {
        }

        


        Char:: ~Char() throw()
        {
            _bzset(code);
        }

        Char::Supply & Char:: Instance()
        {
            static Supply &_ = Supply::instance();
            return _;
        }

        Char * Char:: Acquire(const Context &context,
                              const uint8_t  code)
        {
            static Supply &mgr = Supply::instance();
            return mgr.acquire<const Context &,uint8_t>(context,code);
        }

        void Char:: Release(Char *ch) throw()
        {
            static Supply &mgr = Supply::location();
            mgr.release(ch);
        }

        void Char:: Reserve(const size_t n)
        {
            static Supply &mgr = Supply::instance();
            mgr.reserve(n);
        }

        Char * Char:: Copycat(const Char *other)
        {
            static Supply &mgr = Supply::instance();
            assert(NULL!=other);
            return mgr.copycat(other);
        }

        Char * Char:: Copyset(const Char *other, const uint8_t C)
        {
            assert(NULL!=other);
            Char *ch = Copycat(other);
            aliasing::_(ch->code) = C;
            return ch;
        }

        
        std::ostream & operator<<(std::ostream &os, const Char &ch)
        {
            os << cchars::visible[ch.code];
            return os;
        }

        ios::ostream & operator<<(ios::ostream &fp, const Char &ch)
        {
            fp << char(ch.code);
            return fp;
        }



    }
}

