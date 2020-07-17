//! \file
#ifndef Y_SPADE_VTK_INCLUDED
#define Y_SPADE_VTK_INCLUDED 1

#include "y/spade/mesh/rectilinear.hpp"
#include "y/spade/mesh/curvilinear.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {
    
    namespace Spade
    {
        //----------------------------------------------------------------------
        //
        //
        //! vtk API to write fields
        /**
         - for 1D field, expanded x2 x2 => 4 times
         - for 2D field, expanded x2    => 2 times
         */
        //
        //
        //----------------------------------------------------------------------
        class vtk : public singleton<vtk>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const at_exit::longevity life_time = 0; //!< for singleton
            static const char     DATASET[];          //!< "DATASET"
            static const char     DIMENSIONS[];       //!< "DIMENSIONS"
            static const char     ORIGIN[];           //!< "ORIGIN"
            static const char     SPACING[];          //!< "SPACING"
            static const char     _COORDINATES[];     //!< "_COORDINATES"
            static const char     POINTS[];           //!< "POINTS"
            static const char     RECTILINEAR_GRID[]; //!< "RECTILINEAR_GRID"
            static const char     STRUCTURED_GRID[];  //!< "STRUCTURED_GRID"
            static const unsigned Repeat[4];          //!< [0,4,2,1] to validate ParaView
            static const char     TypeFloat[];        //!< "float"
            static const char     TypeDouble[];       //!< "double"
            static const char     TypeInt[];          //!< "int"
            static const char     TwoByTwoEndl[];     //!< " 2 2\n"
            static const char     TwoSkip[];          //!< " 2 "
            static const char     TwoEndl[];          //!< " 2\n";

            //------------------------------------------------------------------
            //
            //
            // Formated type writing
            //
            //
            //------------------------------------------------------------------
            
            //------------------------------------------------------------------
            //
            //! base class for writers
            //
            //------------------------------------------------------------------
            class Writer : public Object
            {
            public:
                //--------------------------------------------------------------
                //
                // virtual interface
                //
                //--------------------------------------------------------------
                virtual               ~Writer()                                  throw();     //!< cleanup
                virtual ios::ostream & write(ios::ostream &, const void *) const         = 0; //!< write a given object
                virtual unsigned       components()                        const throw() = 0; //!< number of components
                virtual const char    *dataType()                          const throw() = 0; //!< vtk data type
                
                //--------------------------------------------------------------
                //
                // non-virtual interface
                //
                //--------------------------------------------------------------
                bool isScalar() const throw(); //!< components() == 1
                
            protected:
                explicit Writer() throw(); //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };
            
            //! alias to use in database
            typedef arc_ptr<Writer> IWriter;
            
            //------------------------------------------------------------------
            //
            //! native types writer
            //
            //------------------------------------------------------------------
            class Native : public Writer
            {
            public:
                virtual ~Native() throw(); //!< cleanup
                string   format;           //!< C-style string format
                
                virtual unsigned components() const throw(); //!< 1
                
            protected:
                explicit Native(const char *); //!< setup Writer(1) and format
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Native);
            };
            
            //! alias to use in database
            typedef arc_ptr<Native> INative;
            
            //------------------------------------------------------------------
            //
            //! tuples writer
            //
            //------------------------------------------------------------------
            template <template <typename> class TUPLE, typename T>
            class Tuples : public Writer
            {
            public:
                //! global definition
                static const unsigned COMPONENTS = sizeof(TUPLE<T>)/sizeof(T);
                
                //! cleanup
                inline virtual ~Tuples() throw() {}
                
                //! setup from a Native reference
                inline explicit Tuples(const Writer &itemWriter) :
                Writer(),
                writer(itemWriter),
                allComponents(COMPONENTS*writer.components())
                {}
                
            private:
                const Writer  &writer;
                const unsigned allComponents;
                
                Y_DISABLE_COPY_AND_ASSIGN(Tuples);
                inline virtual ios::ostream & write(ios::ostream &fp, const void *addr) const
                {
                    const T *p = static_cast<const T *>(addr);
                    writer.write(fp,&p[0]);
                    for(unsigned c=1;c<COMPONENTS;++c)
                    {
                        writer.write(fp<<' ', &p[c]);
                    }
                    return fp;
                }
                inline virtual unsigned    components() const throw() { return allComponents;     }
                inline virtual const char *dataType()   const throw() { return writer.dataType(); }
            };
            
            //------------------------------------------------------------------
            //
            //
            // accessing writer and data formatting
            //
            //
            //------------------------------------------------------------------
            
            
            //! get matching native writer
            const Native & getNative(const std::type_info &) const;
            
            //! get matching native writer
            template <typename T> inline
            const Native  &getNative() const
            {
                static const Native &_ = getNative(typeid(T)); return _;
            }
            
            //! get native or defined writer
            const Writer & getWriter(const std::type_info &) const;
            
            //! get native or defined writer
            template <typename T> inline
            const Writer  &getWriter() const
            {
                static const Writer &_ = getWriter(typeid(T)); return _;
            }
            
            //! write formatted output of registered type
            template <typename T> inline
            ios::ostream & operator()(ios::ostream &fp, const T &args) const
            {
                static const Writer &_ = getWriter<T>();
                return _.write(fp,&args);
            }
            
            //! record a new writer
            void record( const std::type_info &tid, const IWriter &w );
            
            //! record a new writer
            template <typename T> inline
            void record(Writer *W)
            {
                const IWriter w = W; record(typeid(T),w);
            }
            
            //! record for a tuple
            template <template <typename> class TUPLE, typename T>
            void tuples()
            {
                const IWriter w = new Tuples<TUPLE,T>( getWriter<T>() );
                record(typeid( TUPLE<T> ),w);
            }
            
            //------------------------------------------------------------------
            //
            //
            // writing vtk files
            //
            //
            //------------------------------------------------------------------
            
            //! write header
            void writeHeader(ios::ostream  & fp,
                             const unsigned  major=3,
                             const unsigned  minor=0) const;
            
            //! write title
            void writeTitle(ios::ostream &fp,
                            const string &title) const;
            
            //! write title, wrapper
            void writeTitle(ios::ostream &fp,
                            const char   *title) const;
            
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
            void writePointData(ios::ostream        &fp,
                                const LayoutMetrics &L) const;
            
            
            
            //! write a field with repeats to be usable by ParaView
            template <typename FIELD,typename LAYOUT> inline
            void writeField(ios::ostream &fp,
                            const FIELD  &F,
                            const LAYOUT &L) const
            {
                //______________________________________________________________
                //
                // local type
                //______________________________________________________________
                typedef void (vtk::*method)( ios::ostream &, const Writer &, const void *) const;
                const Writer &writer = revealField(fp,F);
                method        write1 = ( writer.isScalar() ) ?  & vtk::writeScalar : & vtk::writeVector;
                const size_t  repeat = Repeat[ LAYOUT::Dimensions ];

                //______________________________________________________________
                //
                // loop over layout
                //______________________________________________________________
                typename LAYOUT::Loop loop(L.lower,L.upper);
                for(unsigned r=0;r<repeat;++r)
                {
                    for(loop.boot();loop.good();loop.next())
                    {
                        (*this.*write1)(fp,writer, & F[*loop] );
                    }
                }
            }
            
            //------------------------------------------------------------------
            //
            //
            // write RectilinearMesh
            //
            //
            //------------------------------------------------------------------
            
            //! write 1D mesh, using [automatic] scaling for 2D and 3D
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const RectilinearMesh<Coord1D,T> &mesh,
                           T                                 scaling=0) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();
                if(scaling<=0)
                {
                    scaling=mesh.scaling();
                }

                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << RECTILINEAR_GRID << '\n';

                //______________________________________________________________
                //
                // expanded dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << TwoByTwoEndl;

                //______________________________________________________________
                //
                // Axis
                //______________________________________________________________
                fp << 'X' << _COORDINATES << ' '; self(fp,mesh.width) << ' ' <<  dataType << '\n'; writeAxis1D(fp,mesh[0]);
                fp << 'Y' << _COORDINATES << TwoSkip << dataType << '\n'; self(WriteZeroPrefix(fp),scaling) << '\n';
                fp << 'Z' << _COORDINATES << TwoSkip << dataType << '\n'; self(WriteZeroPrefix(fp),scaling) << '\n';

            }
            
            //! write 2D mesh, using [automatic] scaling for 3D
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const RectilinearMesh<Coord2D,T> &mesh,
                           T                                 scaling=0) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();
                if(scaling<=0)
                {
                    const point2d<T> vscale = mesh.scaling();
                    scaling = vscale.norm2();
                }
                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << RECTILINEAR_GRID << '\n';

                //______________________________________________________________
                //
                // expanded dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << TwoEndl;

                //______________________________________________________________
                //
                // Axis
                //______________________________________________________________
                fp << 'X' << _COORDINATES << ' '; self(fp,mesh.width.x) << ' ' << dataType << '\n'; writeAxis1D(fp,mesh[0]);
                fp << 'Y' << _COORDINATES << ' '; self(fp,mesh.width.y) << ' ' << dataType << '\n'; writeAxis1D(fp,mesh[1]);
                fp << 'Z' << _COORDINATES << TwoSkip << dataType << '\n'; self(WriteZeroPrefix(fp),scaling) << '\n';
            }
            
            //! write 3D mesh
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const RectilinearMesh<Coord3D,T> &mesh) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();

                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << RECTILINEAR_GRID << '\n';

                //______________________________________________________________
                //
                // dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << '\n';

                //______________________________________________________________
                //
                // Axis
                //______________________________________________________________
                fp << 'X' << _COORDINATES << ' '; self(fp,mesh.width.x) << ' ' << dataType << '\n'; writeAxis1D(fp,mesh[0]);
                fp << 'Y' << _COORDINATES << ' '; self(fp,mesh.width.y) << ' ' << dataType << '\n'; writeAxis1D(fp,mesh[1]);
                fp << 'Z' << _COORDINATES << ' '; self(fp,mesh.width.z) << ' ' << dataType << '\n'; writeAxis1D(fp,mesh[2]);
            }

            //------------------------------------------------------------------
            //
            //
            // write CurviLinearMesh
            //
            //
            //------------------------------------------------------------------

            //! write 1D mesh
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const CurvilinearMesh<Coord1D,T> &mesh,
                           T                                 scaling=0) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();
                const  Coord1D       points   = Repeat[1] * mesh.items;
                if(scaling<=0)
                {
                    scaling = 1;
                }
                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << STRUCTURED_GRID << '\n';

                //______________________________________________________________
                //
                // dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << TwoByTwoEndl;

                //______________________________________________________________
                //
                // points
                //______________________________________________________________
                fp << POINTS << ' '; self(fp,points) << ' ' << dataType << '\n';

                const typename CurvilinearMesh<Coord1D,T>::Axis &X = mesh[0];
                const T        delta[2] = {0,scaling};
                for(unsigned k=0;k<2;++k)
                {
                    const T z = delta[k];
                    for(unsigned j=0;j<2;++j)
                    {
                        const T y = delta[j];
                        for(Coord1D i=mesh.lower;i<=mesh.upper;++i)
                        {
                            const point3d<T> pos( X[i], y, z );
                            self(fp,pos) << '\n';
                        }
                    }
                }


            }


            //! write 2D mesh
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const CurvilinearMesh<Coord2D,T> &mesh,
                           T                                 scaling=0) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();
                const  Coord1D       points   = Repeat[2] * mesh.items;
                if(scaling<=0)
                {
                    //const point2d<T> vscale = mesh.scaling();
                    //scaling = vscale.norm2();
                    scaling = 1;
                }
                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << STRUCTURED_GRID << '\n';

                //______________________________________________________________
                //
                // dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << TwoEndl;

                //______________________________________________________________
                //
                // points
                //______________________________________________________________
                fp << POINTS << ' '; self(fp,points) << ' ' << dataType << '\n';

                Layout2D::Loop loop(mesh.lower,mesh.upper);
                const T        Z[2] = { 0, scaling };
                for(unsigned slice=0;slice<2;++slice)
                {
                    const T z=Z[slice];
                    for(loop.boot();loop.good();loop.next())
                    {
                        const point2d<T> v2d = mesh( *loop );
                        const point3d<T> v3d(v2d.x,v2d.y,z);
                        self(fp,v3d) << '\n';
                    }
                }
            }

            //! write 3D mesh
            template <typename T> inline
            void writeMesh(ios::ostream                     &fp,
                           const CurvilinearMesh<Coord3D,T> &mesh) const
            {
                //______________________________________________________________
                //
                // setup
                //______________________________________________________________
                static const Writer &tw       = getWriter<T>();
                const        vtk    &self     = *this;
                const  char         *dataType = tw.dataType();
                //______________________________________________________________
                //
                // set rectilinear
                //______________________________________________________________
                fp << DATASET << ' ' << STRUCTURED_GRID << '\n';

                //______________________________________________________________
                //
                // dimensions
                //______________________________________________________________
                fp << DIMENSIONS << ' '; self(fp,mesh.width) << '\n';

                //______________________________________________________________
                //
                // points
                //______________________________________________________________
                fp << POINTS << ' '; self(fp,mesh.items) << ' ' << dataType << '\n';

                Layout3D::Loop loop(mesh.lower,mesh.upper);
                for(loop.boot();loop.good();loop.next())
                {
                    self(fp,mesh( *loop )) << '\n';
                }

            }

        private:
            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;
            Y_DISABLE_COPY_AND_ASSIGN(vtk);
            
            suffix_tree<INative> natives;
            suffix_tree<IWriter> writers;
            
            void registerNatives();
            void registerWriters();
            
            //! write as VTK dimensions with padding for Paraview
            template <typename COORD> inline
            ios::ostream & writeDimensions(ios::ostream &fp, const COORD &width) const
            {
                return composeAs3D(fp << DIMENSIONS << ' ', & Coord::Of(width,0), Coord::Get<COORD>::Dimensions, 2);
            }
            
            //! write structured points
            void structuredPoints_(ios::ostream &fp, const unsigned dimensions, const Coord1D *width, const Coord1D *lower) const;
            
            //! make a 3D vector from data
            ios::ostream &  composeAs3D(ios::ostream  &fp, const Coord1D *v, const unsigned dimensions, const Coord1D pad) const;
            
            //! write type as scalar at address
            void writeScalar(ios::ostream &fp, const Writer &W, const void *) const;
            
            //! write type as vector at address
            void writeVector(ios::ostream &fp, const Writer &W, const void *) const;
            
            //! prepare lookup table
            const Writer & revealField( ios::ostream &fp, const Field  &F ) const;

            //! write all coordinates
            template <typename T> inline
            void writeAxis1D( ios::ostream &fp, const Field1D<T> &a ) const
            {
                const  vtk          &self = *this;
                for(Coord1D i=a.lower;i<=a.upper;++i)
                {
                    self(fp,a[i]) << '\n';
                }
            }

            //! fp << '0' << ' '
            static ios::ostream & WriteZeroPrefix(ios::ostream &fp);
            
        };
        
    }
    
}

#endif
