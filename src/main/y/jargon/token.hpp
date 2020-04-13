
//! \file

#ifndef Y_JARGON_TOKEN_INCLUDED
#define Y_JARGON_TOKEN_INCLUDED 1

#include "y/jargon/char.hpp"

namespace upsylon {
    
    class exception;
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! a dynamic Token of Chars
        //
        //----------------------------------------------------------------------
        class Token : public Char::List, public Serializable
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit Token() throw(); //!< setup
            virtual ~Token() throw(); //!< erase
            Token(const Token &);     //!< copy

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            void append(const Context &, const uint8_t ); //!< append a new char, using context
            void append(const Context &, const string &); //!< append a new string, using a context
            void skip(size_t n) throw();                  //!< skip n chars
            void trim(size_t n) throw();                  //!< trim n chars
            void cat(exception &) const throw();          //!< append content to exception
            
            //! display all Chars
            friend std::ostream & operator<<( std::ostream &, const Token &);
            
            //------------------------------------------------------------------
            //
            // interfaces
            //
            //------------------------------------------------------------------
            virtual size_t      serialize( ios::ostream &fp ) const;
            virtual const char *className()           const throw();
            
            
        private:
            Y_DISABLE_ASSIGN(Token);
        
        };
        
    }
    
}

#endif


