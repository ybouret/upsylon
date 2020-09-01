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
        //! mathing APU
        //
        //______________________________________________________________________
        class Matching : public Motif
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
            bool   exactly_(Module *) const;          //!< full module content must be accepted
            bool   somehow_(Token &, Module *) const; //!< first accepted token
            size_t collect_(sequence<Token::Handle> &, Module *) const; //!< collect all tokens

            //! wrapper
            template <typename DATANAME, typename DATATYPE> inline
            bool exactly( const DATANAME &dataName, const DATATYPE &data ) const
            {
                return exactly_( Module::OpenData(dataName,data) );
            }

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            bool somehow(Token &token, const DATANAME &dataName, const DATATYPE &data ) const
            {
                 return somehow_(token,Module::OpenData(dataName,data));
            }

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            size_t collect(sequence<Token::Handle> &tokens, const DATANAME &dataName, const DATATYPE &data ) const
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
