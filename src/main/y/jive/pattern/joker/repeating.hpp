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
            static const char     CLID[];                             //!< CLID minCount>1
            static const uint32_t UUID = Y_FOURCC('R','E','P','_');   //!< UUID minCount>1

            static const char     CLID_ZOM[];                           //!< CLID minCount=0
            static const uint32_t UUID_ZOM = Y_FOURCC('Z','O','M','_'); //!< UUID minCount=0

            static const char     CLID_OOM[];                           //!< CLID minCount=1
            static const uint32_t UUID_OOM = Y_FOURCC('O','O','M','_'); //!< CLID minCount=1

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Repeating *Create(Pattern *p, const size_t);       //!< new
            virtual Pattern   *clone()             const;              //!< copy
            virtual bool       feeble()    const throw();              //!< true if at least 0 or motif is feeble
            virtual bool       accept(Y_PATTERN_ACCEPT_ARGS) const;    //!< at least minCount motif
            virtual void       express(ios::ostream &) const;           //!< motif+something

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();          //!< depends on minCount
            virtual size_t      serialize(ios::ostream&fp) const;   //!< depends on minCount

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t minCount; //!< minimal count to accept

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Repeating() throw(); //!< cleanup

        private:
            Y_DISABLE_ASSIGN(Repeating);
            explicit Repeating(const Motif     &, const size_t) throw();
            explicit Repeating(const Repeating &) throw();
            virtual void vizCore(ios::ostream  &) const;
        };
    }

}

#endif
