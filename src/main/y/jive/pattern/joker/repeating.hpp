//! \file

#ifndef Y_JIVE_PATTERN_JOKER_REPEATING_INCLUDED
#define Y_JIVE_PATTERN_JOKER_REPEATING_INCLUDED 1

#include "y/jive/pattern/joker/joker.hpp"

namespace upsylon
{

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! Accept a repeating motif
        //
        //______________________________________________________________________
        class Repeating : public Joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('R','E','P','_');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Repeating *Create(const Pattern *p, const size_t); //!< new
            virtual Pattern   *clone()             const;              //!< copy
            virtual bool       feeble()    const throw();              //!< true if at least 0 or motif is feeble
            virtual bool       accept(Y_PATTERN_ACCEPT_ARGS) const;    //!< at least motif

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream&fp) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t minCount;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Repeating() throw();

        private:
            Y_DISABLE_ASSIGN(Repeating);
            explicit Repeating(const Motif     &, const size_t) throw();
            explicit Repeating(const Repeating &) throw();
            virtual void vizCore(ios::ostream  &) const;
        };
    }

}

#endif
