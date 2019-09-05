//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/topology.hpp"
#include "y/memory/static-slots.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        //! workspace metrics
        template <typename COORD>
        class Workspace : public Topology::Hub<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                             LayoutType; //!< alias
            typedef typename LayoutType::Loop                 Loop;       //!< alias
            typedef typename LayoutType::coord                coord;      //!< alias
            typedef typename LayoutType::const_coord          const_coord;//!< alias
            typedef Topology::Hub<COORD>                      HubType;
            static const size_t                               Dimensions = HubType::Dimensions;
            static const size_t                               Directions = HubType::Directions;

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t     size;  //!< product of sizes
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~Workspace() throw()
            {
                Coord::LDZ_(size);
            }
            
            //! setup
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &localSizes,
                                      const Coord1D     globalRank,
                                      const_coord      &PBC,
                                      const Coord1D     ng = 0
                                      ) :
            HubType(localSizes,globalRank,PBC),
            size(  Coord::Product(this->sizes) ),
            inner( full.split(this->sizes,this->ranks) ),
            outer( expandInner( abs_of(ng) ) )
            {
                std::cerr << "tile[" << this->rank << "]=" << inner << " -> " << outer << std::endl;
                buildLinks();
            }
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);

            inline LayoutType expandInner(const Coord1D ng)
            {
                if(ng<=0)
                {
                    return inner;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // get coordinates to modify
                    //
                    //----------------------------------------------------------
                    coord          lower = inner.lower;
                    coord          upper = inner.upper;

                    //----------------------------------------------------------
                    //
                    // Expand per dimensionm
                    //
                    //----------------------------------------------------------
                    Topology::Expand((Coord1D *)       & lower,
                                     (Coord1D *)       & upper,
                                     (const Coord1D *) & inner.width,
                                     (const bool *)    & this->head,
                                     (const bool *)    & this->tail,
                                     (const bool *)    & this->bulk,
                                     (const bool *)    & this->par,
                                     (const bool *)    & this->pbc,
                                     ng,
                                     Dimensions);
                    //----------------------------------------------------------
                    //
                    // return modified layout
                    //
                    //----------------------------------------------------------
                    return LayoutType(lower,upper);
                }
            }


            inline void createLink(const_coord delta,
                                   size_t     *levels)
            {
                coord probe = delta;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    switch( Coord::Of(delta,dim) )
                    {
                        case -1: Coord::Of(probe,dim) += Coord::Of(inner.lower,dim); break;
                        case  1: Coord::Of(probe,dim) += Coord::Of(inner.upper,dim); break;
                        default:
                            break;
                    }
                }
                std::cerr << "\tlink@" << delta << " -> " << probe;
                if(outer.has(probe))
                {
                    std::cerr << " exists!" << std::endl;
                    const Topology::Level level = Topology::LevelOf(delta);
                    switch (level) {
                        case Topology::Level1: ++levels[0]; break;
                        case Topology::Level2: ++levels[1]; break;
                        case Topology::Level3: ++levels[2]; break;
                    }
                }
                else
                {
                    std::cerr <<  " doesn't exists!" << std::endl;
                }
            }

            inline void buildLinks()
            {
                std::cerr << "links@ranks="<< this->ranks << std::endl;
                //! half loop on [-1:1]^Dimensions, using symetry
                coord __lo(0); Coord::LD(__lo,-1);
                coord __up(0); Coord::LD(__up, 1);
                Loop loop(__lo,__up);
                loop.start();

                size_t levels[3] = { 0,0,0 };
                for( size_t j=0; j<Directions; ++j, loop.next() )
                {
                    createLink( loop.value,levels);
                    createLink(-loop.value,levels);
                }
                display_int::to(std::cerr << "links={",levels,Dimensions) << "}" << std::endl;

            }

        };
        
    }
}



#endif

