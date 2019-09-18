
//! \file
#ifndef Y_OXIDE_FIELD_SPLIT_INCLUDED
#define Y_OXIDE_FIELD_SPLIT_INCLUDED 1

#include "y/oxide/field/divide.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //======================================================================
        //
        //
        //! information about splitting layouts
        //
        //
        //======================================================================
        template <typename COORD> class Split
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const size_t                             Dimensions = Coord::Get<COORD>::Dimensions;
            typedef Layout<COORD>                           LayoutType;       //!< alias
            typedef typename LayoutType::coord              coord;            //!< alias
            typedef typename LayoutType::const_coord        const_coord;      //!< alias
            typedef vector<COORD>                           MappingsType;     //!< will store mappings

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const MappingsType mappings;            //!< all possible mappings
            const_coord        optimal;             //!< optimal mappings
            const_coord        favorite;            //!< from preferred

            //! create possible mappings and pick optimal
            inline explicit Split(const size_t      cores,
                                  const LayoutType &fullLayout,
                                  const_coord       boundaries,
                                  coord             preferred) :
            mappings(),
            optimal( Divide::Find(fullLayout,cores,boundaries, (MappingsType *)&mappings  ) ),
            favorite(optimal)
            {
                //______________________________________________________________
                //
                // check that everything is in place
                //______________________________________________________________
                if( Coord::Product(optimal) <= 0 )
                    Divide::ErrorNoMappingFor(cores);

                //______________________________________________________________
                //
                // analyze preferred request
                //______________________________________________________________
                size_t active = 0;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord1D &c = Coord::Of(preferred,dim);
                    if(c!=0)
                    {
                        c=abs_of(c);
                        ++active;
                    }
                }
                const size_t n = mappings.size();
                vector<mpq>  costs(n,as_capacity);
                for(size_t i=1;i<=n;++i)
                {
                    const_coord  &M   = mappings[i];
                    const_coord  &P   = preferred;
                    const mpq     alpha(Coord::Dot(P,M),Coord::Dot(M,M));
                    mpq cost = 0;
                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        const mpq p( Coord::Of(P,dim) );
                        const mpq m( Coord::Of(M,dim) );
                        const mpq d = p - alpha * m;
                        cost += d*d;
                    }
                    std::cerr << M << "->" <<alpha<< "-> " << cost << " ~ " << cost.to_real() << std::endl;
                    costs.push_back_(cost);
                }



            }

            //! cleanup
            inline virtual ~Split() throw()
            {
                bzset_(optimal);
                bzset_(favorite);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Split);
        };

    }

}

#endif

