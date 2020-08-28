//! \file

#ifndef Y_PATTERN_BASIC_EXCLUDE_INCLUDED
#define Y_PATTERN_BASIC_EXCLUDE_INCLUDED 1

#include "y/jive/pattern/basic/basic.hpp"

namespace upsylon {

    namespace Jive
    {
        class Rework;

        //______________________________________________________________________
        //
        //
        //! Reject one matching char
        //
        //______________________________________________________________________
        class Exclude : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('E','X','C','L');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Exclude  *Create(const uint8_t);             //!< create
            virtual Pattern  *clone()            const;          //!< copy
            virtual void      start(FirstChars&) const;          //!< all but code
            virtual void      update(Entropy &)  const throw();  //!< with code

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
            virtual ~Exclude() throw();

            //! display
            friend  std::ostream & operator<<(std::ostream &, const Exclude &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code;
            const uint8_t priv;
            
        private:
            Y_DISABLE_ASSIGN(Exclude);
            friend class Rework;

            explicit     Exclude(const uint8_t ) throw();
            explicit     Exclude(const Exclude&) throw();
            virtual bool isValid(const uint8_t)  const throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }
}


#endif
