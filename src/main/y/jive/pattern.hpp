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

    namespace Information
    {
        class Entropy; //!< forward declaration
    }

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        // types used in first chars
        //
        //______________________________________________________________________
        typedef ios::serializable Serializable; //!< alias
        typedef ios::vizible      Vizible;      //!< alias

        //! make a name from an UUID
#define Y_PATTERN_CLID(CLASS) const char CLASS::CLID[] = Y_FOURCC_CHAR8(CLASS::UUID)

        //! accept proto
#define Y_PATTERN_ACCEPT_ARGS Token &token, Source &source

        //! internal info for operations
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
            typedef Information::Entropy             Entropy; //!< alias
            typedef core::list_of_cloneable<Pattern> List;    //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Pattern() throw();


            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual bool     accept(Y_PATTERN_ACCEPT_ARGS) const = 0; //!< accept a token (empty on input)
            virtual bool     feeble() const throw()              = 0; //!< may accept an empty token ?
            virtual Pattern *clone() const                       = 0; //!< clone API
            virtual void     start(FirstChars &) const           = 0; //!< collect first chars
            virtual void     update(Entropy &)   const throw()   = 0; //!< update entropy
            virtual void     optimize()                 throw()  = 0; //!< sub-optimize
            
            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            bool   strong()  const throw(); //!< !feeble()
            double entropy() const throw(); //!< compute internal entropy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint32_t     uuid; //!< Unique User IDentrifier
            const void * const self; //!< derived class pointer
            Pattern *          next; //!< for list
            Pattern *          prev; //!< for list
        
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static Pattern *Load(ios::istream &);         //!< load pattern from stream
            static Pattern *Optimize(Pattern  *) throw(); //!< optimize pattern
            void   test(Source &source) const;            //!< perform test on source

            //! cast
            template <typename T> inline T *as() throw()
            {
                assert(uuid==T::UUID); assert(self); return (T*)self;
            }

            //! cast
            template <typename T> inline const T *as() const throw()
            {
                assert(uuid==T::UUID); assert(self); return static_cast<const T*>(self);
            }

            //! check type
            template <typename T> inline bool is() const throw() { return T::UUID == uuid; }


            static bool Eq(const Pattern &, const Pattern &);        //!< equality (TODO: optimize!!!)
            friend bool operator==(const Pattern &, const Pattern&); //!< uses Eq
            friend bool operator!=(const Pattern &, const Pattern&); //!< uses Eq

        protected:
            explicit Pattern(const uint32_t) throw();  //!< setup uuid
            explicit Pattern(const Pattern &) throw(); //!< copy  uuid...
            size_t         id(ios::ostream&) const;    //!< emit default uuid

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

        typedef arc_ptr<const Pattern> Motif; //!< alias
    }

}

#endif
