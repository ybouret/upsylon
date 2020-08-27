//! \file

#ifndef Y_PATTERN_LOGICAL_INCLUDED
#define Y_PATTERN_LOGICAL_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! logical operands
        //
        //______________________________________________________________________
        class Logical : public Pattern
        {
        public:
            typedef Pattern::List Operands;

            //__________________________________________________________________
            //
            // common serializable
            //__________________________________________________________________
            virtual size_t serialize(ios::ostream &fp) const; //!< uuid+operangs
            void           load(ios::istream &p);             //!< read operands from stream

            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Logical() throw();

            virtual void add( Pattern *p ) throw() = 0;
            virtual void harden()                  = 0;

        protected:
            explicit Logical(const uint32_t) throw(); //!< setup
            explicit Logical(const Logical &);        //!< copy
            void vizLink(ios::ostream&) const;        //!< write operands and links

            Operands operands;

        private:
            Y_DISABLE_ASSIGN(Logical);

        };

     


    }

}

#endif
