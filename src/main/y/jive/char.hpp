//! \file

#ifndef Y_JIVE_CHAR_INCLUDED
#define Y_JIVE_CHAR_INCLUDED 1

#include "y/jive/context.hpp"
#include "y/memory/tight/xcache.hpp"

namespace upsylon {

    namespace Jive
    {
        
        //______________________________________________________________________
        //
        //
        //! a fully qualified char
        //
        //______________________________________________________________________
        class Char : public inode<Char>, public Context
        {
        public:

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static Char *Acquire(const Context &, const uint8_t);           //!< create
            static void  Release(Char *) throw();                           //!< delete
            static Char *Copycat(const Char &);                             //!< copy
            static void  Reserve(const size_t);                             //!< get memory from system
            friend std::ostream & operator<<(std::ostream &, const Char &); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< the code

        private:
            Y_DISABLE_ASSIGN(Char);
            explicit Char(const Context &, const uint8_t code) throw();
            explicit Char(const Char &) throw();
            virtual ~Char() throw();

        public:
            //__________________________________________________________________
            //
            // types for Char::Supply
            //__________________________________________________________________
            typedef memory::tight::xcache          XCache_; //!< base class for XCache
            typedef memory::tight::xcache_of<Char> XCache;  //!< memory cache of Char
            typedef core::pool_of<Char>            ZCache;  //!< local cache type

            //__________________________________________________________________
            //
            //! thread safe specialized Char::Supply
            //__________________________________________________________________
            class Supply : public singleton<Supply>
            {
            public:

                Char *acquire(const Context &context,
                              const uint8_t  code); //!< zeroed memory for Char
                void  release(Char *ch) throw();    //!< return memory
                void  reserve(size_t);              //!< query from system
                Char *copycat(const Char&);         //!< copy

                const XCache_ & xCache() const throw(); //!< access to xchars
                const ZCache  & zCache() const throw(); //!< access to zchars

            private:
                Y_SINGLETON_DECL(Supply);
                Y_DISABLE_COPY_AND_ASSIGN(Supply);
                explicit Supply();
                virtual ~Supply() throw();
                XCache   xchars; //!< global memory for zombi chars
                ZCache   zchars; //!< local pool of zombi chars
            };

            //__________________________________________________________________
            //
            //! base class for Char::List
            //__________________________________________________________________
            typedef core::list_of<Char> ListType;

            //__________________________________________________________________
            //
            //! list of Chars
            //__________________________________________________________________
            class List : public ListType
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                explicit List() throw();        //!< setup empty
                virtual ~List() throw();        //!< cleanup
                List( const List &);            //!< copy
                List & operator=(const List &); //!< copy/swap

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                List & operator<<(Char *) throw();         //!< helper
                List & add(const Context &,const uint8_t); //!< helper
                
                //! display
                friend std::ostream & operator<<(std::ostream &, const List &);

            };



        };


    }

}
#endif

