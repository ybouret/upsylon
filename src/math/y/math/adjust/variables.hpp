
//! \file

#ifndef Y_MATH_ADJUST_VARIABLES_INCLUDED
#define Y_MATH_ADJUST_VARIABLES_INCLUDED 1

#include "y/math/adjust/variable.hpp"


namespace upsylon {
    
    namespace Adjust {
        
        class Variables : public Variable::Set
        {
        public:
            explicit Variables() throw();
            virtual ~Variables() throw();
            Variables(const Variables &);
            Variables & operator=( const Variables &other );
            
            const Variable & operator[](const string &name) const;
            const Variable & operator[](const char   *name) const;

            Variables & decl(const string &name,size_t index=0);
            Variables & decl(const char   *name,size_t index=0);

            Variables & operator<<( const string &name );
            Variables & operator<<( const char   *name );

            Variables & operator()(const Variable &v, const string &name);
            Variables & operator()(const Variable &v, const char   *name);
            Variables & operator()(const Variable &v);
            
            
            size_t MaxLength() const throw();
            
        private:
            void update();
            
            
        };
        
    }
}

#endif

