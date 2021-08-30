//! \file

#ifndef Y_CHEMICAL_SEEKING_INCLUDED
#define Y_CHEMICAL_SEEKING_INCLUDED 1

#include "y/chemical/equilibrium.hpp"


namespace upsylon
{
    namespace Chemical
    {

        class Seeking : public Object
        {
        public:
            typedef arc_ptr<const Seeking>             Pointer;  //!< alias
            typedef vector<Seeking::Pointer,Allocator> Array;    //!< alias
            
            explicit Seeking(const Species &, const iMatrix &);
            virtual ~Seeking() throw();

            const Species            &sp;
            const accessible<unit_t> &nu;

            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Seeking &s)
            {
                os << s.nu << ".xi";
                os << " >= -" << s.sp;
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Seeking);
        };

    }

}

#endif
