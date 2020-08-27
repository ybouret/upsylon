//! \file

#ifndef Y_JIVE_PATTERN_INCLUDED
#define Y_JIVE_PATTERN_INCLUDED 1

#include "y/jive/token.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/ios/serializable.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef ios::serializable Serializable;
        typedef ios::vizible      Vizible;

#define Y_PATTERN_CLID(CLASS) const char CLASS::CLID[] = Y_FOURCC_CHAR8(CLASS::UUID)
#define Y_PATTERN_ACCEPT_ARGS Token &token, Source &source
#define Y_PATTERN_SELF(CLASS) *(const void **)&self = static_cast<const void *>(static_cast<const CLASS *>(this))

        //______________________________________________________________________
        //
        //
        //! pattern recognition API
        //
        //______________________________________________________________________
        class Pattern : public CountedObject, public Serializable, public Vizible
        {
        public:
            enum Result
            {
                Accepted,
                Rejected,
                Finished
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Pattern() throw();


            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual Result   accept(Y_PATTERN_ACCEPT_ARGS) const = 0;
            virtual Pattern *clone() const                       = 0;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint32_t     uuid;
            const void * const self;

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static const Pattern *Load(ios::istream &);

        protected:
            explicit Pattern(const uint32_t) throw();  //!< setup uuid
            explicit Pattern(const Pattern &) throw(); //!< copy  uuid...

            size_t         id(ios::ostream&) const;
        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

        typedef arc_ptr<const Pattern> Motif;
    }

}

#endif
