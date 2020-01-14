
//! \file
#ifndef Y_OXIDE_FIELD_SPLIT_INCLUDED
#define Y_OXIDE_FIELD_SPLIT_INCLUDED 1

#include "y/oxide/field/divide.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! split operations to find preferred partition
        struct __Split
        {
            //! get the cost, low-level
            static mpq    Cost(const Coord1D *P, const Coord1D *M, const size_t D );

            //! get the cost of chosing preferred w.r.t mapping
            template <typename COORD> static inline
            mpq Cost( const COORD &preferred, const COORD &mapping )
            {
                return Cost( (const Coord1D *) &preferred, (const Coord1D *)&mapping, Coord::Get<COORD>::Dimensions );
            }

            //! find the index of the best mapping against the preferred one
            static size_t Find(const array<mpq> &costs);
        };

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
            static const size_t                      Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef Layout<COORD>                    LayoutType;                                 //!< alias
            typedef typename LayoutType::coord       coord;                                      //!< alias
            typedef typename LayoutType::const_coord const_coord;                                //!< alias
            typedef vector<COORD>                    MappingsType;                               //!< will store mappings

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const LayoutType   layout;              //!< keep track of full layout
            const MappingsType mappings;            //!< all possible mappings
            const_coord        optimal;             //!< optimal mappings
            const_coord        favored;             //!< from preferred

            //! create possible mappings and pick optimal and favorite
            inline explicit Split(const size_t      cores,
                                  const LayoutType &fullLayout,
                                  const_coord       boundaries,
                                  coord             preferred) :
            layout( fullLayout ),
            mappings(),
            optimal( Divide::Find(layout,cores,boundaries, (MappingsType *)&mappings  ) ),
            favored(optimal)
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
                size_t active=0;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord1D &c = Coord::Of(preferred,dim);
                    if(c!=0)
                    {
                        c=abs_of(c);
                        ++active;
                    }
                }

                //______________________________________________________________
                //
                // cost = min(|preferred-alpha*mapping|^2), to look like
                // preferred!
                //______________________________________________________________
                if(active>0)
                {

                    //__________________________________________________________
                    //
                    // algorithm works with active==0, but result is optimal!
                    //__________________________________________________________
                    const size_t   nmaps = mappings.size();
                    vector<mpq>    costs(nmaps,as_capacity);

                    //__________________________________________________________
                    //
                    // First Pass: compute each cost
                    //__________________________________________________________
                    for(size_t i=1;i<=nmaps;++i)
                    {
                        const mpq cost = __Split::Cost(preferred,mappings[i]);
                        costs.push_back_(cost);
                    }

                    //__________________________________________________________
                    //
                    // Second pass: find the first minimal cost
                    //__________________________________________________________
                    const size_t min_indx = __Split::Find(costs);
                    aliasing::_(favored)  = mappings[min_indx];
                }

            }

            //! cleanup
            inline virtual ~Split() throw()
            {
                _bzset(optimal);
                _bzset(favored);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Split);
        };

    }

}

#endif

