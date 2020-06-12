
#include "y/spade/vtk.hpp"

namespace upsylon {

    namespace Spade
    {

        const char     vtk::DATASET[]      = "DATASET";
        const char     vtk::DIMENSIONS[]   = "DIMENSIONS";
        const char     vtk::ORIGIN[]       = "ORIGIN";
        const char     vtk::SPACING[]      = "SPACING";
        const char     vtk::_COORDINATES[] = "_COORDINATES";
        const char     vtk::POINTS[]       = "POINTS";
        const unsigned vtk::Repeat[4]      = { 0, 4, 2, 1 };
        const char     vtk::TypeFloat[]    = "float";
        const char     vtk::TypeInt[]      = "int";
        const char     vtk::TypeDouble[]   = "double";

        vtk:: vtk() :
        natives(),
        writers()
        {
            registerNatives();
            registerWriters();
        }

        vtk:: ~vtk() throw()
        {
        }

        void vtk:: writeHeader(ios::ostream & fp,
                               const unsigned major,
                               const unsigned minor) const
        {
            fp << "# vtk DataFile Version ";
            fp("%u.%u\n",major,minor);
        }

        void vtk:: writeTitle(ios::ostream &fp,
                              const string &title) const
        {
            for(size_t i=0;i<title.size();++i)
            {
                char C = title[i];
                switch(C)
                {
                    case '\n':
                    case '\r':
                        C = '_';
                        break;

                    default:
                        break;
                }
                fp << C;
            }
            fp << '\n';
            fp << "ASCII\n";
        }


        ios::ostream & vtk:: composeAs3D(ios::ostream  &fp,
                                         const Coord1D   *v,
                                         const unsigned   dimensions,
                                         const Coord1D    pad) const
        {
            assert(dimensions>=1); assert(dimensions<=3);
            assert(v!=NULL);

            const vtk           &VTK = *this;

            VTK(fp,v[0]);
            for(size_t dim=1;dim<dimensions;++dim) VTK(fp << ' ',v[dim]);
            for(size_t dim=dimensions;dim<3;++dim) VTK(fp << ' ',pad);
            return fp;
        }


        void vtk:: structuredPoints_(ios::ostream  &fp,
                                     const unsigned dims,
                                     const Coord1D *width,
                                     const Coord1D *lower) const
        {
            assert(dims>=1); assert(dims<=3); assert(width); assert(lower);
            fp << DATASET << " STRUCTURED_POINTS\n";
            composeAs3D(fp << DIMENSIONS << ' ',width,dims,2) << '\n';
            composeAs3D(fp << ORIGIN     << ' ',lower,dims,0) << '\n';
            fp << SPACING << " 1 1 1\n";
        }


        void vtk:: writePointData(ios::ostream        &fp,
                                  const LayoutMetrics &L) const
        {
            assert(L.dimensions>=1);
            assert(L.dimensions<=3);
            fp << "POINT_DATA ";
            (*this)(fp, L.items * Repeat[L.dimensions] );
            fp << '\n';
        }

        const vtk::Writer & vtk:: revealField(ios::ostream &fp,
                                              const Field  &F ) const
        {
            const Writer &W = getWriter(  F.objectType );

            if( W.components() == 1)
            {
                fp << "SCALARS " << F.name << ' ' << W.dataType() << '\n';
                fp << "LOOKUP_TABLE " << F.name << '\n';
            }
            else
            {
                fp << "VECTORS " << F.name << ' ' << W.dataType() << '\n';
            }

            return W;
        }

        void vtk:: writeScalar( ios::ostream &fp, const Writer &W, const void *addr ) const
        {
            W.write(fp,addr);
            fp << '\n';
        }


        void vtk:: writeVector(ios::ostream &fp,
                               const Writer &W,
                               const void   *addr ) const
        {
            W.write(fp,addr);
            for(size_t dim=W.components();dim<3;++dim)
            {
                fp << ' ' << '0';
            }
            fp << '\n';
        }
    }

}
