
//! \file

#ifndef Y_PATTERN_BASIC_SINGLE_INCLUDED
#define Y_PATTERN_BASIC_SINGLE_INCLUDED 1

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
        class Single : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];                           //!< CLID
            static const uint32_t UUID = Y_FOURCC('S','N','G','L'); //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Single  *Create(const uint8_t);             //!< create
            virtual Pattern *clone() const;                     //!< copy
            virtual void     start(FirstChars &) const;         //!< only code
            virtual void     update(Entropy &)  const throw();  //!< with code

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();         //!< CLID
            virtual size_t      serialize(ios::ostream&fp) const;  //!< id+code

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Single() throw(); //!< cleanup

            //! display
            friend std::ostream & operator<<(std::ostream &, const Single &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< matching code
            const uint8_t priv; //!< private byte
            
        private:
            Y_DISABLE_ASSIGN(Single);
            friend class Rework;

            explicit Single(const uint8_t ) throw();
            explicit Single(const Single&) throw();
            virtual bool isValid(const uint8_t)  const throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }
}


#endif
