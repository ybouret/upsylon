//! \file
#ifndef Y_OXIDE_VTK_INCLUDED
#define Y_OXIDE_VTK_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/ios/ostream.hpp"
#include "y/ptr/intr.hpp"
#include "y/string.hpp"
#include "y/hashing/type-info.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        class vtk : public singleton<vtk>
        {
        public:
            typedef hashing::type_info_hasher<> Hasher;
            
            class Writer : public counted_object
            {
            public:
                const std::type_info &tid;
                string                fmt;
                
                virtual ~Writer() throw() {}
                
                virtual void write( ios::ostream &, const void *addr) const = 0;
                
                const std::type_info & key() const throw() { return tid; }
                
            protected:
                explicit Writer(const std::type_info &t,
                                const char           *f) :
                tid(t), fmt(f)
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };
            
            typedef intr_ptr<std::type_info,Writer>  SharedWriter;
            typedef set<std::type_info,SharedWriter> SharedWriters;
            
            static const at_exit::longevity life_time = 0;
            
            const Writer & get( const std::type_info &tid ) const;
            
            template <typename T>
            inline const Writer & get() const { return get( typeid(T) ); }
            
            template <typename T> inline
            ios::ostream &write( ios::ostream &fp, const T &args ) const
            {
                get<T>().write(fp,&args);
                return fp;
            }
            
        private:
            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;
            
            SharedWriters writers;
            
        };
        
    }
}

#endif

