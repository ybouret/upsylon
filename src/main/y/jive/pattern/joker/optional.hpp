
//! \file

#ifndef Y_JIVE_PATTERN_JOKER_OPTIONAL_INCLUDED
#define Y_JIVE_PATTERN_JOKER_OPTIONAL_INCLUDED 1

#include "y/jive/pattern/joker/joker.hpp"

namespace upsylon
{

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! Accept optional motif
        //
        //______________________________________________________________________
        class Optional : public Joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('O','P','T','_');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Optional *Create(const Pattern *p);            //!< new
            virtual Pattern  *clone()             const;           //!< copy
            virtual bool      feeble()    const throw();           //!< true
            virtual bool      accept(Y_PATTERN_ACCEPT_ARGS) const; //!< motif or true

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream&fp) const;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Optional() throw();
            friend std::ostream & operator<<(std::ostream &, const Optional&);

        private:
            Y_DISABLE_ASSIGN(Optional);
            explicit Optional(const Motif   &) throw();
            explicit Optional(const Optional&) throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }

}

#endif
