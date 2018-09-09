//! \file
#ifndef Y_LANG_CHAR_INCLUDED
#define Y_LANG_CHAR_INCLUDED 1

#include "y/lang/char-info.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! full fledge class
        class Char : public CharInfo
        {
        public:
            Char   *next; //!< for list
            Char   *prev; //!< for list
            uint8_t code; //!< current char value

            //! constructor
            inline          Char(const uint8_t c, const CharInfo &info) throw() : CharInfo(info), next(0), prev(0), code(c) {}

            //! destructor
            inline virtual ~Char() throw() {}
            //! copy
            inline          Char(const Char &other) throw() : CharInfo(other), next(0), prev(0), code(other.code) {}


            typedef core::list_of_cpp<Char> List;      //!< list alias
            typedef core::pool_of_cpp<Char> PoolType;  //!< pool base type

            //! Pool for caches management
            class Pool : public PoolType
            {
            public:
                //! constructor
                inline explicit Pool() throw() : PoolType() {}
                //! destructor
                inline virtual ~Pool() throw() {}
                
                //! cached creation
                inline Char *make( const uint8_t code, const CharInfo &info )
                {
                    if(top)
                    {
                        Char *ch = query();
                        ch->code = code;
                        CharInfo tmp(info);
                        ch->swapWith(tmp);
                        return ch;
                    }
                    else
                    {
                        return new Char(code,info);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pool);
            };

        private:
            Y_DISABLE_ASSIGN(Char);
        };

        //! cache for objects
        class Manager : public singleton<Manager>, public Char::Pool
        {
        public:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Manager);
            explicit Manager() throw();
            virtual ~Manager() throw();
            friend class singleton<Manager>;
        public:
            static const at_exit::longevity life_time = Object::life_time - 12; //!< for singleton
        };

    }

}
#endif

