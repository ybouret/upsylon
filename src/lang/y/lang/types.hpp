#ifndef Y_LANG_TYPES_INCLUDED
#define Y_LANG_TYPES_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    namespace Lang
    {
        
        //! Base Object
        class Object : public object
        {
        public:
            explicit Object() throw();
            virtual ~Object() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };

        //! full fledge class
        class Char : public Object
        {
        public:
            Char   *next; //!< for list
            Char   *prev; //!< for list
            uint8_t code;   //!< current char value

            //! constructor
            inline          Char(const uint8_t c) throw() : Object(), next(0), prev(0), code(c) {}
            //! desctructor
            inline virtual ~Char() throw() {}
            //! copy
            inline          Char(const Char &other) throw() : Object(), next(0), prev(0), code(other.code) {}
            //! assign
            inline Char  &  operator=(const Char &other) throw()
            {
                code = other.code; return *this;
            }

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

                inline Char *make( const uint8_t code )
                {
                    if(top)
                    {
                        Char *ch = query();
                        ch->code = code;
                        return ch;
                    }
                    else
                    {
                        return new Char(code);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pool);
            };
        };

        class Manager : public singleton<Manager>, public Char::Pool
        {
        public:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Manager);
            explicit Manager() throw();
            virtual ~Manager() throw();
            friend class singleton<Manager>;
        public:
            static const at_exit::longevity life_time = Object::life_time - 12;
        };

    }
}

#endif

