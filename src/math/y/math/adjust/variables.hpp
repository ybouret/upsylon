
//! \file

#ifndef Y_MATH_ADJUST_VARIABLES_INCLUDED
#define Y_MATH_ADJUST_VARIABLES_INCLUDED 1

#include "y/math/adjust/variable.hpp"
#include "y/string/display.hpp"

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

            template <typename T>
            inline T & operator()( addressable<T> &source, const string &id ) const
            {
                const  Variables &self = *this;
                return self[id].get(source);
            }

            template <typename T>
            inline const T & operator()( const accessible<T> &source, const string &id ) const
            {
                const  Variables &self = *this;
                return self[id].get(source);
            }

            template <typename T>
            void display( std::ostream &os, const accessible<T> &source ) const
            {
                const Variables &self = *this;
                const size_t     nmax = MaxLength();
                for( const_iterator it = begin(); it != end(); ++it )
                {
                    const string &name = (**it).name;
                    string_display::align(os,name,nmax) << " = " << self(source,name) << std::endl;
                }
            }




        private:
            void update();
            
            
        };
        
    }
}

#endif

