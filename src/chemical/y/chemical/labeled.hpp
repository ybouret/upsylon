
//! \file

#ifndef Y_CHEMICAL_LABELED_INCLUDED
#define Y_CHEMICAL_LABELED_INCLUDED 1

#include "y/chemical/indexed.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! base type for labeled class
        //
        //______________________________________________________________________
        class Labeled : public Indexed
        {
        public:
            
            virtual ~Labeled() throw();
            
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os) const
            {
                for(size_t i=name.size();i<nmax;++i) os << ' ';
                return os;
            }
            
            
            const string name;
            const size_t nmax;
            
        protected:
            //! constructor
            template <typename ID> inline
            explicit Labeled(const ID &id) :
            Indexed(), name(id), nmax(0) {}
            
           
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Labeled);
        };
        
        
    }
    
}

#endif
