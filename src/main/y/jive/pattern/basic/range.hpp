//! \file

#ifndef Y_PATTERN_BASIC_RANGE_INCLUDED
#define Y_PATTERN_BASIC_RANGE_INCLUDED 1

#include "y/jive/pattern/basic/basic.hpp"

namespace upsylon {

    namespace Jive
    {

        class Rework;

        //______________________________________________________________________
        //
        //
        //! Accept one matching char
        //
        //______________________________________________________________________
        class Range : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('R','N','G','_');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Range   *Create(const uint8_t,const uint8_t);
            virtual Pattern *clone() const;
            virtual void     start(FirstChars &) const;         //!< insert range
            virtual void     update(Entropy &)  const throw();  //!< with boundaries

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
            virtual ~Range() throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const Range &);  


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t lower;
            const uint8_t upper;

        private:
            Y_DISABLE_ASSIGN(Range);
            friend class Rework;

            explicit Range(const uint8_t, const uint8_t) throw();
            explicit Range(const Range&) throw();
            virtual bool isValid(const uint8_t)  const throw();
            virtual void vizCore(ios::ostream &) const;

        };
    }
}


#endif
