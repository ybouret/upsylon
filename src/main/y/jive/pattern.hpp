//! \file

#ifndef Y_JIVE_PATTERN_INCLUDED
#define Y_JIVE_PATTERN_INCLUDED 1

#include "y/jive/token.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/ios/serializable.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/type/fourcc.hpp"
#include "y/core/list.hpp"

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

            typedef core::list_of_cpp<Pattern> List;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Pattern() throw();


            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const = 0;
            virtual bool     feeble() const throw()              = 0; //!< may accept an empty token
            virtual Pattern *clone() const                       = 0;
            virtual void     start(FirstChars &) const           = 0;

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            bool strong() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint32_t     uuid;
            const void * const self;
            Pattern *          next;
            Pattern *          prev;

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static Pattern *Load(ios::istream &);
            void   test(Source &source) const;

            template <typename T> inline T *as() throw()
            {
                assert(uuid==T::UUID); assert(self); return (T*)self;
            }

            template <typename T> inline const T *as() const throw()
            {
                assert(uuid==T::UUID); assert(self); return static_cast<const T*>(self);
            }

            template <typename T> inline bool is() const throw() { return T::UUID == uuid; }

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
