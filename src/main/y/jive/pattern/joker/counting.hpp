
//! \file

#ifndef Y_JIVE_PATTERN_JOKER_COUNTING_INCLUDED
#define Y_JIVE_PATTERN_JOKER_COUNTING_INCLUDED 1

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
        class Counting : public Joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];                           //!< CLID
            static const uint32_t UUID = Y_FOURCC('C','N','T','_'); //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Pattern   *Create(Pattern *p, size_t,  size_t); //!< new
            virtual Pattern   *clone()             const;           //!< copy
            virtual bool       feeble()    const throw();           //!< true if minCount<=0 or motif is feeble
            virtual bool       accept(Y_PATTERN_ACCEPT_ARGS) const; //!< count is ok
            virtual void       express(ios::ostream &) const;       //!< motif+somethong

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();         //!< CLID
            virtual size_t      serialize(ios::ostream&fp) const;  //!< id+count+motif

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t minCount; //!< minimal count to accept
            const size_t maxCount; //!< maximal count to accept

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Counting() throw(); //!< cleanup

        private:
            Y_DISABLE_ASSIGN(Counting);
            explicit Counting(const Pattern *, const size_t, const size_t) throw();
            explicit Counting(const Counting   &) throw();
            virtual void vizCore(ios::ostream  &) const;
        };
    }

}

#endif
