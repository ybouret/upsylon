//! \file

#ifndef Y_JIVE_PATTERN_MATCHING_INCLUDED
#define Y_JIVE_PATTERN_MATCHING_INCLUDED 1

#include "y/jive/regexp.hpp"

namespace upsylon
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! mathing API
        //
        //______________________________________________________________________
        class Matching : public Motif, public Token
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Matching() throw();             //!< cleanup
            Matching(const Matching &);              //!< copy

            //! build from an acceptable regular expression
            template <typename REGEXP> inline
            Matching(const REGEXP &rx, const Dictionary *dict =0 ) :
            Motif( RegExp(rx,dict) ), firstChars()
            {
                setup();
            }

            

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const Token *exactly_(Module *);          //!< full module content must be accepted
            const Token *foundIn_(Module *); //!< first accepted token
            size_t       collect_(sequence<Token::Handle> &, Module *); //!< collect all tokens

            //! wrapper
            template <typename DATANAME, typename DATATYPE> inline
            const Token * isExactly( const DATANAME &dataName, const DATATYPE &data )
            {
                return exactly_( Module::OpenData(dataName,data) );
            }

            //! wrapper
            template <typename DATANAME>
            const Token *isExactly( const DATANAME &dataName )
            {
                return exactly_( Module::OpenData(dataName) );
            }

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            const Token *isFoundIn(const DATANAME &dataName, const DATATYPE &data )
            {
                 return foundIn_(Module::OpenData(dataName,data));
            }

            //! wrapper
            template <typename DATANAME> inline
            const Token *isFoundIn(const DATANAME &dataName )
            {
                return foundIn_(Module::OpenData(dataName));
            }

            

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            size_t collect(sequence<Token::Handle> &tokens, const DATANAME &dataName, const DATATYPE &data )
            {
                return collect_(tokens,Module::OpenData(dataName,data));
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Leading firstChars; //!< compiled firstChars

        private:
            Y_DISABLE_ASSIGN(Matching);
            void setup();
        };

    }
}

#endif
