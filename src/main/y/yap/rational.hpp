
//! \file

#ifndef Y_AP_RATIONAL_INCLUDED
#define Y_AP_RATIONAL_INCLUDED 1

#include "y/yap/integer.hpp"

namespace upsylon {


    namespace yap {

        //______________________________________________________________________
        //
        //
        //! rational class
        //
        //______________________________________________________________________

        class rational : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char    CLASS_NAME[]; //!< "apq"

            //__________________________________________________________________
            //
            // C++ and setup
            //__________________________________________________________________
            virtual ~rational() throw();               //!< cleanup
            rational();                                //!< 0/1
            rational(const rational &);                //!< copy
            rational(const integer &,const natural &); //!< setup
            rational(const itype    ,const utype    ); //!< setup
            rational(const itype    ,const natural &); //!< setup
            rational(const integer &,const utype    ); //!< setup
            rational & operator=(const rational &);    //!< assign
            rational(const itype);                     //!< setup/1
            rational(const integer &);                 //!< setup/1
            rational(const natural &);                 //!< setup/1

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();                    //!< CLASS_NAME
            virtual size_t      serialize(ios::ostream &) const;              //!< output
            static  rational    read(ios::istream &, size_t &, const char *); //!< reload

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void xch(rational &q) throw(); //!< no-throw exchange

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const integer num;
            const natural den;

        private:
            void update();
        };
    }

}

#endif
