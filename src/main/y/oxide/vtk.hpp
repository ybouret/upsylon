//! \file
#ifndef Y_OXIDE_VTK_INCLUDED
#define Y_OXIDE_VTK_INCLUDED 1

#include "y/oxide/layout.hpp"
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
                
                virtual               ~Writer() throw();                               //!< cleanup
                const std::type_info & key() const throw();                            //!< key for database
                virtual void           write( ios::ostream &, const void *) const = 0; //!< interface
                virtual const  char   *dataType()   const throw() = 0;                 //!< VTK data type
                virtual unsigned       components() const throw() = 0;

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
            
            //! access a registered writer
            const Writer & get( const std::type_info &tid ) const;

            //! access a registered writer for a type
            template <typename T>
            inline const Writer & get() const { return get( typeid(T) ); }

            //! write object to stream
            template <typename T> inline
            ios::ostream & operator()( ios::ostream &fp, const T &args ) const
            {
                get<T>().write(fp,&args);
                return fp;
            }

            //! write header
            void writeHeader(ios::ostream &fp,
                             const unsigned major=3,
                             const unsigned minor=0 ) const;

            //! write title
            void writeTitle(ios::ostream &fp,
                            const string &title) const;


            //! write layout as structured points, no physical data
            template <typename COORD> inline
            void writeLayout( ios::ostream &fp, const Layout<COORD> &L ) const
            {
                structuredPoints(fp, Layout<COORD>::Dimensions, (const Coord1D *)&L.width, (const Coord1D *)&L.lower );
            }

        private:
            static const at_exit::longevity life_time = 0;
            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;
            SharedWriters writers;

            void structuredPoints(ios::ostream  &fp,
                                  const size_t   dims,
                                  const Coord1D *width,
                                  const Coord1D *lower) const;

            ios::ostream & write3D(ios::ostream &fp, const Coord1D *C, const size_t dims) const;

        };
        
    }
}

#endif

