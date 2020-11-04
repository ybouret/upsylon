//! \file

#ifndef Y_JIVE_CHAR_INCLUDED
#define Y_JIVE_CHAR_INCLUDED 1

#include "y/jive/context.hpp"
#include "y/memory/magazine.hpp"
#include "y/core/inode.hpp"

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
            // types and definitions
            //__________________________________________________________________
            typedef memory::magazine<Char> Supply;  //!< alias
            typedef Supply::auto_ptr       Pointer; //!< alias
            typedef Supply::list_          List;    //!< alias
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static Char *  Acquire(const Context &, const uint8_t);           //!< create
            static void    Release(Char *) throw();                           //!< delete
            static Char *  Copycat(const Char *);                             //!< copy
            static Char *  Copyset(const Char *, const uint8_t);              //!< copy with another byte
            static void    Reserve(const size_t);                             //!< reserve chars
            static Supply &Instance();                                        //!< get instance

            friend std::ostream & operator<<(std::ostream &, const Char &); //!< display
            friend ios::ostream & operator<<(ios::ostream &, const Char &); //!< append to ostream
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
            friend class memory::magazine<Char>;
        };


    }

    Y_MAGAZINE_DECL(Jive::Char)
    

}
#endif

