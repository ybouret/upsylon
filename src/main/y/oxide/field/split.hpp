
//! \file
#ifndef Y_OXIDE_FIELD_SPLIT_INCLUDED
#define Y_OXIDE_FIELD_SPLIT_INCLUDED 1

#include "y/oxide/field/divide.hpp"

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
                size_t     active=0;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord1D &c = Coord::Of(preferred,dim);
                    if(c!=0)
                    {
                        c=1;
                        ++active;
                    }
                }

                //______________________________________________________________
                //
                // try to match request
                //______________________________________________________________
                std::cerr << "preferred=" << preferred << std::endl;
                if(active>0&&active<Dimensions)
                {
                    // user requested a preferred layout
                    vector<size_t> good(mappings.size(),as_capacity);
                    {
                        const size_t n = mappings.size();
                        for(size_t i=1;i<=n;++i)
                        {
                            const_coord value = Coord::Decreased(mappings[i]);
                            if( Coord::Dot(preferred,value) )
                            {
                                std::cerr << "keep " << mappings[i] << std::endl;
                                good.push_back_(i);
                            }
                        }
                    }
                }
                else
                {
                    // keep the default (=optimal?) choice
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

