//! \file
#ifndef Y_OXIDE_VTK_INCLUDED
#define Y_OXIDE_VTK_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/oxide/field/type.hpp"
#include "y/ios/ostream.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/auto.hpp"
#include "y/string.hpp"
#include "y/hashing/type-info.hpp"
#include "y/associative/set.hpp"

namespace upsylon {

    namespace Oxide {

        //======================================================================
        //
        //
        //! writing VTK data
        /**
         - for 1D field, expanded x2 x2 => 4 times
         - for 2D field, expanded x2    => 2 times
         */
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
            static const char DATASET[];      //!< "DATASET"
            static const char DIMENSIONS[];   //!< "DIMENSIONS"
            static const char ORIGIN[];       //!< "ORIGIN"
            static const char SPACING[];      //!< "SPACING"
            static const char _COORDINATES[]; //!< "_COORDINATES"
            static const char POINTS[];       //!< "POINTS"

            //------------------------------------------------------------------
            //
            //! writer for a registered type
            //
            //------------------------------------------------------------------
            class Writer : public counted_object
            {
            public:
                const std::type_info &tid; //!< type info, used as key
                auto_ptr<string>      fmt; //!< format

                //--------------------------------------------------------------
                // virtual interface
                //--------------------------------------------------------------
                virtual               ~Writer() throw();                               //!< cleanup
                virtual void           write( ios::ostream &, const void *) const = 0; //!< interface
                virtual const  char   *dataType()   const throw() = 0;                 //!< VTK data type
                virtual unsigned       components() const throw() = 0;                 //!< number of components

                //--------------------------------------------------------------
                // non-virtual interface
                //--------------------------------------------------------------
                bool                   isScalar() const throw();                       //!< 1 == components()
                const std::type_info & key() const throw();                            //!< key for database

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

            //==================================================================
            //
            // methods
            //
            //==================================================================

            //! access a registered writer
            const Writer & get( const std::type_info &tid ) const;

            //! cached access to a registered writer for a type
            template <typename T> inline const Writer & get() const
            {
                static const Writer &_ = get( typeid(typename type_traits<T>::mutable_type) );
                return _;

            }

            //! write object to stream
            template <typename T> inline
            ios::ostream & operator()( ios::ostream &fp, const T &args ) const
            {
                static const Writer &_ = get<T>();
                _.write(fp,&args);
                return fp;
            }

            //! write header
            void writeHeader(ios::ostream  & fp,
                             const unsigned  major=3,
                             const unsigned  minor=0 ) const;

            //! write title
            void writeTitle(ios::ostream &fp,
                            const string &title) const;


            //! write layout as structured points, no physical data
            /**
             data are expanded to work with ParaView
             */
            template <typename COORD> inline
            void writeLayout( ios::ostream &fp, const Layout<COORD> &L ) const
            {
                structuredPoints_(fp, Layout<COORD>::Dimensions, (const Coord1D *)&L.width, (const Coord1D *)&L.lower );
            }
            
            //! write layout as structured points, no physical data
            /**
             write global POINT_DATA for this layout, expanded for ParaView
             */
            void writePointData(ios::ostream     &fp,
                                const LayoutInfo &L ) const;


            //! write a field with repeats to be usable by ParaView
            template <typename FIELD,typename LAYOUT> inline
            void writeField( ios::ostream &fp, const FIELD &F, const LAYOUT &L ) const
            {
                // local type
                typedef void (vtk::*method)( ios::ostream &, const Writer &, const void *) const;
                const Writer &writer = revealField(fp,F);
                method        write1 = ( writer.isScalar() ) ?  & vtk::writeScalar : & vtk::writeVector;
                const size_t  repeat = Repeat[ LAYOUT::Dimensions ];
                
                // loop over layout
                typename LAYOUT::Loop loop(L.lower,L.upper);
                for(size_t r=0;r<repeat;++r)
                {
                    for( loop.start(); loop.valid(); loop.next() )
                    {
                        (*this.*write1)(fp,writer, & F(loop.value) );
                    }
                }
            }


            ios::ostream & composeAs3D( ios::ostream &fp, const Coord1D *C, const size_t dims, const Coord1D pad) const; //!< write C[0..dims-1], and 'pad' [dims:3]
            const Writer & revealField( ios::ostream &fp, const Field  &F ) const;                                       //!< prepare lookup table
            void           writeScalar( ios::ostream &fp, const Writer &W, const void *addr ) const;                     //!< write type scalar@addr
            void           writeVector( ios::ostream &fp, const Writer &W, const void *addr ) const;                     //!< write type vector@addr


            template <typename COORD> inline
            ios::ostream & writeDimensions( ios::ostream &fp, const COORD &width) const
            {
                return composeAs3D(fp << DIMENSIONS << ' ', & Coord::Of(width,0), Coord::Get<COORD>::Dimensions, 2);
            }

        private:
            static const size_t             Repeat[4];
            static const at_exit::longevity life_time = 0;

            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;

            SharedWriters writers;

            void structuredPoints_(ios::ostream  &fp,
                                   const size_t   dims,
                                   const Coord1D *width,
                                   const Coord1D *lower) const;



        };

    }
}

#endif

