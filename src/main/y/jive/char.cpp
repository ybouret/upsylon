
#include "y/jive/char.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL_WITH(object::life_time-1,Jive::Char::Supply);

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
        Context(other),
        code(other.code)
        {
        }

        


        Char:: ~Char() throw()
        {
            _bzset(code);
        }


        Char * Char:: Acquire(const Context &context,
                              const uint8_t  code)
        {
            static Supply &mgr = Supply::instance();
            return mgr.acquire(context,code);
        }

        void Char:: Release(Char *ch) throw()
        {
            static Supply &mgr = Supply::location();
            mgr.release(ch);
        }

        Char * Char:: Copycat(const Char &other)
        {
            static Supply &mgr = Supply::location();
            return mgr.copycat(other);
        }

        std::ostream & operator<<(std::ostream &os, const Char &ch)
        {
            os << cchars::visible[ch.code];
            return os;
        }



        //======================================================================
        //
        // Char Supply
        //
        //======================================================================
        Char:: Supply:: Supply() :
        xchars(object::supply::instance()),
        zchars()
        {
        }

        Char:: Supply:: ~Supply() throw()
        {
            Y_LOCK(access);
            Y_LOCK(xchars.access);
            while(zchars.size)
            {
                xchars.release_unlocked( zchars.query() );
            }
        }

        Char * Char::Supply:: acquire(const Context &context,
                                      const uint8_t  code)
        {
            Y_LOCK(access);
            Char *ch = zchars.size ? zchars.query() : xchars.acquire();
            try {
                return new(ch) Char(context,code);
            }
            catch(...)
            {
                zchars.store(ch);
                throw;
            }
        }

        Char * Char::Supply:: copycat(const Char &other)
        {
            Y_LOCK(access);
            Char *ch = zchars.size ? zchars.query() : xchars.acquire();
            try {
                return new(ch) Char(other);
            }
            catch(...)
            {
                zchars.store(ch);
                throw;
            }
        }

        void  Char::Supply:: release(Char *ch) throw()
        {
            Y_LOCK(access);
            assert(ch);
            assert(ch->is_single());
            ch->~Char();
            bzset(*ch);
            zchars.store(ch);
        }

        void  Char:: Supply:: reserve(size_t n)
        {
            Y_LOCK(access);
            Y_LOCK(xchars.access);
            while(n-- > 0) zchars.store( xchars.acquire() );
        }

        const Char::XCache_ & Char::Supply:: xCache() const throw()
        {
            return xchars;
        }

        const Char::ZCache & Char::Supply:: zCache() const throw()
        {
            return zchars;
        }

        //======================================================================
        //
        // Char List
        //
        //======================================================================
        Char:: List:: List() throw() : ListType()
        {
        }

        Char:: List:: ~List() throw()
        {
            while(size)
            {
                static Supply &mgr = Supply::location();
                mgr.release( pop_back() );
            }
        }

        Char:: List:: List( const List &other ) :   ListType()
        {
            static Supply &mgr = Supply::location();
            List           tmp;
            for(const Char *ch=other.head;ch;ch=ch->next)
            {
                tmp.push_back( mgr.copycat(*ch) );
            }
            swap_with(tmp);
         }

        Char::List & Char::List:: operator=(const Char::List &other)
        {
            List tmp(other);
            swap_with(tmp);
            return *this;
        }

        Char::List & Char::List:: operator<<(Char *ch) throw()
        {
            assert(ch);
            (void)push_back(ch);
            return *this;
        }

        Char::List & Char::List:: add(const Context &context, const uint8_t code)
        {
            return (*this) << Char::Acquire(context,code);
        }


        std::ostream & operator<<(std::ostream &os, const Char::List &L)
        {
            for(const Char *ch=L.head;ch;ch=ch->next)
            {
                os << *ch;
            }
            return os;
        }

    }
}

