
//! \file

#ifndef Y_JARGON_TOKEN_INCLUDED
#define Y_JARGON_TOKEN_INCLUDED 1

#include "y/jargon/char.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! a dynamic Token of Chars
        class Token : public Char::List
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit Token(const Cache &) throw(); //!< setup
            virtual ~Token() throw();              //!< erase
            Token(const Token &);                  //!< copy

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            void append(const Context &, const uint8_t ); //!< append a new char
            void append(const Context &, const string &); //!< append a new string
            void erase() throw();                         //!< erase content into cache
            void skip(size_t n) throw();                  //!< skip n chars
            void trim(size_t n) throw();                  //!< trim n chars
            //! display all Chars
            friend std::ostream & operator<<( std::ostream &, const Token &);
           
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            mutable Cache cache; //!< shared cache for I/O

        private:
            Y_DISABLE_ASSIGN(Token);
        };
        
    }
    
}

#endif


