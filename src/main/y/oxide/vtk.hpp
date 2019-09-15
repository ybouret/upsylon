//! \file
#ifndef Y_OXIDE_VTK_INCLUDED
#define Y_OXIDE_VTK_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/ios/ostream.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/auto.hpp"
#include "y/string.hpp"
#include "y/hashing/type-info.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //======================================================================
        //
        //
        //! writing VTK data
        //
        //
        //======================================================================
        class vtk : public singleton<vtk>
        {
        public:
            //==================================================================
            //
            // types and defintions
            //
            //==================================================================


            class Writer : public counted_object
            {
            public:
                const std::type_info &tid; //!< type info, used as key
                auto_ptr<string>      fmt; //!< format
                
                virtual               ~Writer() throw();
                const std::type_info & key() const throw();

                virtual void           write( ios::ostream &, const void *) const = 0;

            protected:
                //! setup using type info a default format
                explicit Writer(const std::type_info &,
                                const char           *) ;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };

            typedef hashing::type_info_hasher<>      Hasher;        //!< hasher for database
            typedef intr_ptr<std::type_info,Writer>  SharedWriter;  //!< shared writer
            typedef set<std::type_info,SharedWriter> SharedWriters; //!< set of shared writers
            

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
            static const at_exit::longevity life_time = 0;
            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;
            
            SharedWriters writers;
            
        };
        
    }
}

#endif

