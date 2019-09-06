//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/topology.hpp"
#include "y/oxide/connectivity.hpp"

#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! metrics information
        template <size_t DIM> struct Metrics;

        //! 1D Metrics
        template <> struct  Metrics<1>
        {
            static const size_t LocalNodes  = 3;            //!< [-1:0:1]
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 2;            //!< along 2 main axis
            static const size_t AtLevel2    = 0;            //!< N/A
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 2D Metrics
        template <> struct  Metrics<2>
        {
            static const size_t LocalNodes  = 9;            //!< [-1:0:1]^2
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 4;            //!< along 4 mains
            static const size_t AtLevel2    = 4;            //!< along 4 diagonals
            static const size_t AtLevel3    = 0;            //!< N/A
        };

        //! 3D Metrics
        template <> struct  Metrics<3>
        {
            static const size_t LocalNodes  = 27;           //!< [-1:0:1]^3
            static const size_t Neighbours  = LocalNodes-1; //!< exclude center=hub
            static const size_t AtLevel1    = 6;            //!< across 6  axis
            static const size_t AtLevel2    = 12;           //!< across 12 edges
            static const size_t AtLevel3    = 8;            //!< across 8  vertices
        };


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
            typedef Layout<COORD>                             LayoutType;                                   //!< alias
            typedef typename LayoutType::Loop                 Loop;                                         //!< alias
            typedef typename LayoutType::coord                coord;                                        //!< alias
            typedef typename LayoutType::const_coord          const_coord;                                  //!< alias
            typedef Topology::Hub<COORD>                      HubType;                                      //!< alias
            typedef Topology::Node<COORD>                     NodeType;                                     //!< alias
            static const size_t                               Dimensions = Coord::Get<COORD>::Dimensions;   //!< alias
            static const size_t                               Neighbours = Metrics<Dimensions>::Neighbours; //!< number of possible neighbours
            static const size_t                               Directions = Neighbours/2;                    //!< number of direction
            static const size_t                               AtLevel1   = Metrics<Dimensions>::AtLevel1;   //!< alias
            static const size_t                               AtLevel2   = Metrics<Dimensions>::AtLevel2;   //!< alias
            static const size_t                               AtLevel3   = Metrics<Dimensions>::AtLevel3;   //!< alias
            typedef Connectivity::Link<COORD>                 LinkType; //!< alias

            //! a ghost is a sub layout with indices
            class Ghost : public LayoutType
            {
            public:
                vector<Coord1D>  indices; //!< indices to pick up items later

                //! setup build collecting indices
                inline Ghost( const LayoutType &sub, const LayoutType &out ) :
                LayoutType(sub), indices( this->items, as_capacity )
                {
                    assert(out.contains(*this));
                    out.collect(indices, sub);
                    assert(this->items==indices.size());
                }

                //! cleanup, do nothing
                inline ~Ghost() throw()  {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghost);
            };


            //! base class for ghosts inner and outer sublayouts
            /**
             The positional NodeType will be used as peer information for I/O
            */
            class Ghosts_ : public NodeType, public counted_object
            {
            public:
                const LinkType          link;  //!< directionality of ghosts
                const Coord1D           host;   //!< global host rank
                const bool              local;  //!< is local
                const bool              async;  //!< is async

                //! setup
                inline explicit Ghosts_(const_coord           &localSizes,
                                        const Coord1D         &globalRank,
                                        const Coord1D         &globalHost,
                                        const LinkType        &localLink,
                                        const LayoutType      &innerArea,
                                        const LayoutType      &outerArea,
                                        const LayoutType      &outerLayout) throw() :
                NodeType(localSizes,globalRank), link(localLink),
                host(globalHost),
                local( (host==this->rank)  ),
                async( !local ),
                inner(innerArea,outerLayout),
                outer(outerArea,outerLayout)
                {
                    assert(inner.items==outer.items);
                }

                //! cleanup
                inline virtual ~Ghosts_() throw()
                {
                }

                const Ghost inner; //!< inner, to send
                const Ghost outer; //!< outer, to recv

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghosts_);
            };

            typedef arc_ptr<Ghosts_> Ghosts; //!< pointer for multiple locations, same data

            //! a pair of ghosts with same orientation
            class GhostsPair
            {
            public:
                const Ghosts first;  //!< first ghosts
                const Ghosts second; //!< second ghosts

                //! setup
                inline GhostsPair( const Ghosts &g1, const Ghosts &g2) throw() :
                first(g1),
                second(g2)
                {
                    assert( LinkType::ArePaired(g1->link,g2->link) );
                }

                //! cleanup
                inline ~GhostsPair() throw()
                {
                    
                }

                //! copy
                inline GhostsPair( const GhostsPair &other ) throw() :
                first(other.first), second(other.second)
                {
                }

                //! get common orientation
                inline const_coord & orientation() const throw()
                {
                    return first->orientation;
                }

                //! get common level
                inline Connectivity::Level & level() const throw()
                {
                    return first->level;
                }
                
            private:
                Y_DISABLE_ASSIGN(GhostsPair);
            };
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t            size;   //!< product of sizes
            const LayoutType        inner;  //!< inner layout
            const LayoutType        outer;  //!< outer layout
            vector<Ghosts>          async;  //!< async ghosts
            vector<GhostsPair>      local;  //!< local pairs
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            //! cleanup
            inline virtual ~Workspace() throw()
            {
                 bzset_(size);
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
            outer( expandInner(abs_of(ng)) ),
            async( Neighbours, as_capacity ),
            local( Directions, as_capacity )
            {
                std::cerr << "\ttile[";
                Coord::Disp(std::cerr,this->rank) << "]=" << inner << " -> " << outer << std::endl;
                if(ng>0)
                {
                    buildGhosts(ng-1);
                }
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
                    // Expand per dimension, using Hub properties
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


            //! try to create Ghosts, shift=ng-1
            inline bool tryCreateGhosts(const_coord   delta,
                                        const Coord1D shift)
            {
                assert(shift>=0);
                const LinkType link(delta);

                //--------------------------------------------------------------
                //
                // build the probe: set the center of layout and move
                // coodinates according to delta
                //
                //--------------------------------------------------------------
                coord probe = (inner.lower+inner.upper)/2;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    switch( Coord::Of(link.direction,dim)  )
                    {
                        case -1: Coord::Of(probe,dim) = Coord::Of(inner.lower,dim)-1; break;
                        case  1: Coord::Of(probe,dim) = Coord::Of(inner.upper,dim)+1; break;
                        default: break;
                    }
                }
                if(outer.has(probe))
                {
                    std::cerr << link << ':';
                    //----------------------------------------------------------
                    //
                    // get info
                    //
                    //----------------------------------------------------------
                    const_coord           granks = Coord::Regularized(this->sizes,this->ranks+link.direction);

                    //----------------------------------------------------------
                    //
                    // build recv/send layouts from delta
                    //
                    //----------------------------------------------------------
                    coord inner_lower = inner.lower;
                    coord inner_upper = inner.upper;
                    coord outer_lower = inner.lower;
                    coord outer_upper = inner.upper;

                    for(size_t dim=0;dim<Dimensions;++dim)
                    {
                        switch( Coord::Of(link.direction,dim)  )
                        {
                            case  1:
                                Coord::Of(outer_upper,dim) = Coord::Of(outer.upper,dim);
                                Coord::Of(outer_lower,dim) = Coord::Of(inner_upper,dim) + 1;
                                Coord::Of(inner_lower,dim) = Coord::Of(inner_upper,dim) - shift;
                                break;
                            case -1:
                                Coord::Of(outer_lower,dim) = Coord::Of(outer.lower,dim);
                                Coord::Of(outer_upper,dim) = Coord::Of(inner_lower,dim) - 1;
                                Coord::Of(inner_upper,dim) = Coord::Of(inner_lower,dim) + shift;
                                break;
                            default: break;
                        }
                    }

                    const LayoutType ghostInnerLayout(inner_lower,inner_upper);
                    const LayoutType ghostOuterLayout(outer_lower,outer_upper);
                    assert(outer.contains(ghostInnerLayout));
                    assert(outer.contains(ghostOuterLayout));

                    //----------------------------------------------------------
                    //
                    // create ghosts and push them in their positions
                    //
                    //----------------------------------------------------------
                    const Ghosts  g = new Ghosts_(this->sizes,
                                                  Coord::GlobalRank(this->sizes,granks),
                                                  this->rank,
                                                  link,
                                                  ghostInnerLayout,
                                                  ghostOuterLayout,
                                                  outer);
                    async.push_back(g);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            inline void builLocalGhostsPair()
            {
                assert(async.size()>=2);
                const Ghosts     g1 = async.back(); async.pop_back(); assert(g1->local);
                const Ghosts     g2 = async.back(); async.pop_back(); assert(g2->local);
                const GhostsPair gp(g1,g2);
                local.push_back(gp);
            }

            inline void buildGhosts(const Coord1D shift)
            {
                std::cerr << "\tlinks@ranks="<< this->ranks << std::endl;

                //--------------------------------------------------------------
                // half loop on [-1:1]^Dimensions, using symetry
                //--------------------------------------------------------------
                coord __lo(0); Coord::LD(__lo,-1);
                coord __up(0); Coord::LD(__up, 1);
                Loop loop(__lo,__up);
                loop.start();

                for( size_t j=0; j<Directions; ++j, loop.next() )
                {
                    const bool has_g1 = tryCreateGhosts( loop.value,shift);
                    const bool has_g2 = tryCreateGhosts(-loop.value,shift);
                    if( (has_g1 && has_g2) )
                    {
                        // a pair was generated
                        assert(async.size()>=2);
                        if( async.back()->local )
                        {
                            builLocalGhostsPair();
                        }
                        else
                        {
                            const size_t nn = async.size();
                            assert( LinkType::ArePaired(async[nn]->link,async[nn-1]->link) );
                        }
                    }
                }
                std::cerr << "#ghosts: " << async.size() << std::endl;
                std::cerr << "#gpairs: " << local.size() << std::endl;
            }

        };
        
    }
}



#endif

