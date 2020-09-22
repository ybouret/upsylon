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
        Context( static_cast<const Context&>(other) ),
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


        Char * Char:: Copyset(const Char &other, const uint8_t C)
        {
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

        void Char:: Reserve(const size_t n)
        {
            static Supply &mgr = Supply::instance();
            mgr.reserve(n);
        }


        //======================================================================
        //
        // Char Supply
        //
        //======================================================================
        Char:: Supply:: Supply() : SupplyType()
        {
        }

        Char:: Supply:: ~Supply() throw() 
        {

        }

        Char * Char::Supply:: acquire(const Context &context,
                                      const uint8_t  code)
        {
            Y_LOCK(access);
            Char *ch = zquery();
            try {
                return new(ch) Char(context,code);
            }
            catch(...)
            {
                zstore(ch);
                throw;
            }
        }

        Char * Char::Supply:: copycat(const Char &other)
        {
            Y_LOCK(access);
            Char *ch = zquery();
            try {
                return new(ch) Char(other);
            }
            catch(...)
            {
                zstore(ch);
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
            zstore(ch);
        }

        void  Char:: Supply:: reserve(size_t n)
        {
            Y_LOCK(access);
            fetch(n);
        }



        //======================================================================
        //
        // Char List
        //
        //======================================================================
        Char:: List:: List() throw() : ListType(), releasable()
        {
        }

        Char:: List:: ~List() throw()
        {
            release();
        }

        void Char:: List:: release() throw()
        {
            while(size)
            {
                static Supply &mgr = Supply::location();
                mgr.release( pop_back() );
            }
        }

        Char:: List:: List( const List &other ) :   ListType(), releasable()
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

        Char::List & Char::List:: operator<<(const List &other)
        {
            List tmp(other);
            merge_back(tmp);
            return *this;
        }

        const Char &  Char::List :: head_char() const throw()
        {
            assert(size>0);
            return *head;
        }

        const Char &  Char::List :: tail_char() const throw()
        {
            assert(size>0);
            return *tail;
        }
        

        std::ostream & operator<<(std::ostream &os, const Char::List &L)
        {
            for(const Char *ch=L.head;ch;ch=ch->next)
            {
                os << *ch;
            }
            return os;
        }

        ios::ostream & operator<<(ios::ostream &fp, const Char::List &L)
        {
            for(const Char *ch=L.head;ch;ch=ch->next)
            {
                fp << *ch;
            }
            return fp;
        }

    }
}

