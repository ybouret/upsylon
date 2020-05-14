//! \file

#ifndef Y_SPADE_TOPOLOGY_INCLUDED
#define Y_SPADE_TOPOLOGY_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/counting/mloop.hpp"
#include "y/type/block/zset.hpp"
#include "y/sequence/slots.hpp"
#include "y/core/inode.hpp"
#include <iomanip>

namespace upsylon {
    
    namespace Spade
    {
        //----------------------------------------------------------------------
        //
        //! direction for each Level
        //
        //----------------------------------------------------------------------
        enum Direction
        {
            Forward, //!< forward at a given level
            Reverse  //!< reverse at a given level
        };

        //----------------------------------------------------------------------
        //
        //! connectivity with a link
        //
        //----------------------------------------------------------------------
        struct Connect
        {
            //! the very mode :)
            enum Mode
            {
                Zilch, //!< none, there is a wall
                Local, //!< auto-connected
                Async  //!< connected to another rank
            };
            
            //! compute mode according to status
            static Mode        For(const bool exists, const size_t src, const size_t tgt) throw();
            
            //! return a human readable mode
            static const char *Text(const Mode) throw();
        };
        
        
        namespace Kernel
        {
            //! Number of neighbours=3^DIM-1
            template <size_t DIM> struct Coordination;
            
            //! Coordination in 1D
            template <> struct Coordination<1>
            {
                static const unsigned Level1  = 1; //!< +X
                static const unsigned Level2  = 0; //!< none
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord1D  Probes[Levels];                   //!< unique probes
            };
            
            //! Coordination in 2D
            template <> struct Coordination<2>
            {
                static const unsigned Level1  = 2; //!< +X,+Y
                static const unsigned Level2  = 2; //!< +X+Y,+X-Y
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord2D  Probes[Levels];                   //!< unique probes
            };
            
            //! Coordination in 3D
            template <> struct Coordination<3>
            {
                static const unsigned Level1  = 3; //!< +X,+Y,+Z
                static const unsigned Level2  = 6; //!< ...
                static const unsigned Level3  = 4; //!< ...
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord3D  Probes[Levels];                   //!< unique probes
            };
            
            //------------------------------------------------------------------
            //
            //! base class for Topology computations
            /**
             answers to: who are my neighbours ?
             */
            //
            //------------------------------------------------------------------
            class Topology
            {
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
            public:
                virtual ~Topology() throw();     //!< cleanup
            protected:
                explicit Topology(const size_t); //!< set size, with checking
                
            public:
                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                
                //! reverse (previous) local rank w.r.t local size
                static Coord1D Prev(Coord1D localSize, Coord1D localRank) throw();
                
                //! forward(next) local rank w.r.t. local size
                static Coord1D Next(Coord1D localSize, Coord1D localRank) throw();
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const size_t size; //!< number of cores

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Topology);
            };
        };
        
        //----------------------------------------------------------------------
        //
        //! Topology computation for ranks w.r.t sizes
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Topology : public Kernel::Topology
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static  const unsigned Dimensions = Coord::Get<COORD>::Dimensions;      //!< alias
            typedef Kernel::Coordination<Dimensions>          Coordination;         //!< alias
            static  const unsigned Levels    =  Coordination::Levels;               //!< alias
            typedef typename type_traits<COORD>::mutable_type coord;                //!< alias
            typedef const coord                               const_coord;          //!< alias
            typedef typename Coord::Get<COORD>::Boolean       Boolean;              //!< alias
            typedef mloop<Coord1D,coord>                      Loop;                 //!< loop over ranks if neccessary
            
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Topology() throw()
            {
                _bzset(sizes);
                _bzset(pitch);
                _bzset(maxRanks);
                _bzset(parallel);
            }
            
            
            //! setup from a mapping
            explicit Topology(const_coord mapping) :
            Kernel::Topology( Coord::Product(mapping) ),
            sizes(mapping),
            pitch(1),
            maxRanks( sizes - Coord::Ones<coord>() ),
            parallel( Coord::ToParallel(sizes)     )
            {
                Coord1D       *p = (Coord1D *)&pitch;
                const Coord1D *s = (const Coord1D *)&sizes;
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    p[dim] = p[dim-1] * s[dim-1];
                }
            }
            
            //------------------------------------------------------------------
            //
            // ranks conversion
            //
            //------------------------------------------------------------------
            
            //! rank = ranks.x + sizes.x * ranks.y + sizes.x * sizes.y * ranks.z;
            inline coord getLocalRanks(const size_t rank) const throw()
            {
                assert(rank<size);
                coord   q(0);
                Coord1D rem = Coord1D(rank);
                for(unsigned dim=Dimensions-1;dim>0;--dim)
                {
                    const Coord1D den = Coord::Of(pitch,dim);
                    const Coord1D qot = rem / den;
                    Coord::Of(q,dim)  = qot;
                    rem -= qot * den;
                }
                Coord::Of(q,0) = rem;
                assert( Coord::LT(q,sizes) );
                return q;
            }
            
            //! rank = ranks.x + sizes.x * ranks.y + sizes.x * sizes.y * ranks.z;
            inline size_t getGlobalRank(const_coord ranks) const throw()
            {
                assert( Coord::LT(ranks,sizes) );
                assert( Coord::GEQ(ranks,Coord::Zero<coord>()) );
                size_t rank = Coord::Of(ranks,0);
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    rank += Coord::Of(pitch,dim) * Coord::Of(ranks,dim);
                }
                return rank;
            }
            
            //------------------------------------------------------------------
            //
            // findind ranks
            //
            //------------------------------------------------------------------
            
            //! compute probe ranks and linked satus for some given ranks and probe
            inline coord getProbeRanks(const_coord   &ranks,
                                       const_coord    probe,
                                       const Boolean &pbc,
                                       bool          &linked) const throw()
            {
                assert( Coord::LT(ranks,sizes) );
                assert( Coord::GEQ(ranks,Coord::Zero<coord>()) );
                assert( Coord::Norm1(probe)>=1 );
                assert( Coord::Norm1(probe)<=Dimensions);
                assert(true==linked);
                coord  where = ranks;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D localScan = Coord::Of(probe,dim);
                    const Coord1D localSize = Coord::Of(sizes,dim);
                    const Coord1D localRank = Coord::Of(ranks,dim);
                    const Coord1D localLast = Coord::Of(maxRanks,dim);
                    const bool    wall      = !Coord::Flag(pbc,dim);

                    switch(localScan)
                    {
                        case  1:
                            Coord::Of(where,dim) = Next(localSize,localRank);
                            if( (localRank>=localLast) && wall )
                            {
                                linked = false;
                            }
                            break;
                            
                        case -1:
                            Coord::Of(where,dim) = Prev(localSize,localRank);
                            if( (localRank<=0) && wall )
                            {
                                linked = false;
                            }
                            break;
                        default: assert(0==localScan); break;
                    }
                }
                return where;
            }
            
          
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const_coord   sizes;    //!< local sizes
            const_coord   pitch;    //!< pitch for ranks algebra
            const_coord   maxRanks; //!< sizes-1
            const Boolean parallel; //!< dimension wise parallel flag
            
            //------------------------------------------------------------------
            //
            //! base class to build topology : local and global rank(s)
            //
            //------------------------------------------------------------------
            class Hub
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! setup
                inline Hub(const_coord     localRanks,
                           const Topology &topology) throw() :
                ranks(localRanks),
                rank( topology.getGlobalRank(ranks) )
                {
                }
                
                //! cleanup
                inline virtual ~Hub() throw()
                {
                }
                
                //! copy
                inline Hub(const Hub &hub) throw() :
                ranks(hub.ranks),
                rank(hub.rank)
                {
                }
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const_coord  ranks; //!< local ranks
                const size_t rank;  //!< global rank
                
            private:
                Y_DISABLE_ASSIGN(Hub);
            };
            
            //------------------------------------------------------------------
            //
            //! a Link is a Hub with a connect mode
            //
            //------------------------------------------------------------------
            class Link : public Hub
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! setup
                inline Link(const size_t    peerRank,
                            const_coord    &peerProbe,
                            const_coord    &localRanks,
                            const Topology &topology,
                            const bool      exists) throw() :
                Hub(localRanks,topology),
                connectMode( Connect::For(exists,peerRank,this->rank) ),
                probe(peerProbe)
                {
                }
                
                //! copy
                inline Link(const Link &link) throw() :
                Hub(link), connectMode(link.connectMode), probe(link.probe)
                { }
                
                //!cleanup
                inline virtual ~Link() throw() { }
                
                //! display
                friend inline std::ostream & operator<<( std::ostream &os, const Link &link )
                {
                    Coord::Disp(os << '+',link.probe) << '=';
                    Coord::Disp(os,link.ranks)  << " (" << Connect::Text(link.connectMode) << "@" << link.rank << ")";
                    return os;
                }
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Connect::Mode connectMode; //!< connect mode
                const_coord         probe;       //!< probe

            private:
                Y_DISABLE_ASSIGN(Link);
            };
            
            //------------------------------------------------------------------
            //
            //! Links describe two-ways connections for a given Level
            //
            //------------------------------------------------------------------
            class Links
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! setup
                inline Links(const Link &fwd, const Link &rev) throw() :
                forward(fwd), reverse(rev)
                {
                    
                }
                
                //! copy
                inline Links(const Links &other) throw() : forward(other.forward), reverse(other.reverse) {}
                
                //! cleanup
                inline ~Links() throw() {}
                
                //! display
                friend inline std::ostream & operator<<( std::ostream &os, const Links &links )
                {
                    os << "->@" << links.forward << " | <-@" << links.reverse;
                    return os;
                }
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Link forward; //!< for forward wave
                const Link reverse; //!< for reverse wave
                
            private:
                Y_DISABLE_ASSIGN(Links);
            };
            
            //------------------------------------------------------------------
            //
            //! a Node is an Hub with its Links[Levels]
            //
            //------------------------------------------------------------------
            class Node : public Hub, public slots<Links>
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
               
                //! setup
                inline explicit Node(const_coord      localRanks,
                                     const Topology  &topology,
                                     const Boolean   &pbc) :
                Hub(localRanks,topology),
                slots<Links>(Levels)
                {
                    // scan the neighbourhood in each level
                    for(unsigned level=0;level<Levels;++level)
                    {
                        const coord  probe   = Coordination::Probes[level];
                        const Link   forward = getLink(topology,probe,pbc);
                        const Link   reverse = getLink(topology,-probe,pbc);
                        this->template build<const Link&,const Link&>(forward,reverse);
                    }
                }
                
                //! cleanup
                inline virtual ~Node() throw() {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
                inline Link getLink(const Topology &topology,
                                    const_coord     probe,
                                    const Boolean  &pbc) const throw()
                {
                    bool        exists = true;
                    const_coord lranks = topology.getProbeRanks(this->ranks,probe,pbc,exists);
                    return Link(this->rank,probe,lranks, topology, exists);
                }
            };
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        
        
    }
    
}

#endif

