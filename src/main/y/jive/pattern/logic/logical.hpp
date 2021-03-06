//! \file

#ifndef Y_PATTERN_LOGICAL_INCLUDED
#define Y_PATTERN_LOGICAL_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef Pattern::List Operands; //!< alias

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
            virtual void   update(Entropy &)  const throw();  //!< gather

            void add(const uint8_t code);                       //!< add single
            void add(const uint8_t lower, const uint8_t upper); //!< add range

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Logical() throw(); //!< cleanup

            static Pattern *Among(const char *, size_t); //!< Or(...)
            static Pattern *Among(const string &); //!< Or(...)
            static Pattern *Among(const char   *); //!< Or(...)

            static Pattern *Equal(const char *, size_t); //!< And(...)
            static Pattern *Equal(const string &);       //!< And(...)
            static Pattern *Equal(const char   *);       //!< And(...)

            static Pattern *Avoid(const char *, size_t); //!< None(...)
            static Pattern *Avoid(const string &);       //!< Non(...)
            static Pattern *Avoid(const char   *);       //!< None(...)


        protected:
            explicit Logical(const uint32_t) throw(); //!< setup
            explicit Logical(const Logical &);        //!< copy
            
            //__________________________________________________________________
            //
            // other methods
            //__________________________________________________________________
            void         vizLink(ios::ostream&) const;        //!< write operands and links
            void         applyOptimize() throw();             //!< optimize all operands
            void         mergeSameUUID() throw();             //!< merge operands with same UUID (And/Or)
            
        private:
            Y_DISABLE_ASSIGN(Logical);


        };

     


    }

}

#endif
