//! \file

#ifndef Y_PATTERN_LOGICAL_INCLUDED
#define Y_PATTERN_LOGICAL_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef Pattern::List Operands;

        //______________________________________________________________________
        //
        //
        //! logical operands
        //
        //______________________________________________________________________
        class Logical : public Pattern, public Operands
        {
        public:

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



        protected:
            explicit Logical(const uint32_t) throw(); //!< setup
            explicit Logical(const Logical &);        //!< copy
            void vizLink(ios::ostream&) const;        //!< write operands and links


        private:
            Y_DISABLE_ASSIGN(Logical);

        };

     


    }

}

#endif
