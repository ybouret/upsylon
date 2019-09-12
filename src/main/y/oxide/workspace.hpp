//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/oxide/field/active.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Oxide
    {
        typedef ios::ovstream IOBlock; //!< alias for a variable-length I/O block

        //! common workspaces operations
        struct __Workspace
        {
            //! check all coordinates are greater than zero
            static void CheckLocalSizes( const Coord1D *sizes, const unsigned dim );

            //! check all local sizes are greater than zero
            template <typename COORD> static inline
            const COORD & CheckLocalSizes( const COORD &localSizes )
            {
                CheckLocalSizes((const Coord1D *)&localSizes, Coord::Get<COORD>::Dimensions);
                return localSizes;
            }

            //! check matching total used bytes agains block size
            static void CheckBlockTotal( const IOBlock &block, const size_t total );

        };


        //! a workspace is some layouts and some fields
        template <typename COORD>
        class Workspace : public Layouts<COORD>, public Fields
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definition
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                    LayoutType;                               //!< alias
            typedef typename LayoutType::coord       coord;                                    //!< alias
            typedef typename LayoutType::const_coord const_coord;                              //!< alias
            typedef Layouts<COORD>                   LayoutsType;                              //!< alias
            typedef typename LayoutsType::GIO        GIO;                                      //!< alias
            static const size_t                      Dimensions   = LayoutType::Dimensions;    //!< alias
            static const size_t                      Orientations = LayoutsType::Orientations; //!< alias
            typedef typename LayoutsType::GhostsType GhostsType;                               //!< alias
            typedef typename LayoutsType::Peer       Peer;                                     //!< alias

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            IOBlock sendBlock; //!< memory to store inner ghosts data, to be sent
            IOBlock recvBlock; //!< memory where outer ghosts data is received

            //------------------------------------------------------------------
            //
            // C++ methods
            //
            //------------------------------------------------------------------
            //! cleaunp
            inline virtual ~Workspace() throw() {}

            //! setup
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &localSizes,
                                      const Coord1D     globalRank,
                                      const_coord      &PBC,
                                      const Coord1D     ng) :
            LayoutsType(full,
                        __Workspace::CheckLocalSizes(localSizes),
                        globalRank,
                        PBC,
                        ng),
            Fields(), sendBlock(), recvBlock()
            {

            }

            //------------------------------------------------------------------
            //
            // Fields creation/access
            //
            //------------------------------------------------------------------

            //! create and register a field
            template <typename FIELD> inline FIELD & create( const string &name )
            {
                FIELD *F = new FIELD(name,this->outer);
                enroll(F);
                return *F;
            }

            //! create and register a field, wrapper
            template <typename FIELD> inline FIELD & create( const char *name )
            {
                const string _(name); return create<FIELD>(_);
            }

            //! access a created field, const
            template <typename FIELD>
            const FIELD & as( const string &name ) const
            {
                return lookUp<FIELD>(name);
            }

            //! access a created field, const, wrapper
            template <typename FIELD>
            const FIELD & as( const char *name ) const
            {
                const string _(name);
                return as<FIELD>(_);
            }

            //! access a created field
            template <typename FIELD>
            FIELD & as( const string &name )
            {
                return (FIELD &)(lookUp<FIELD>(name));
            }

            //! access a created field, wrapper
            template <typename FIELD>
            FIELD & as( const char  *name )
            {
                const string _(name);
                return as<FIELD>(_);
            }


            //------------------------------------------------------------------
            //
            // local exchanges
            //
            //------------------------------------------------------------------


            //!  exchange of local ghosts pairs
            inline void localExchange( Field &F  )
            {
                assert(owns(F));
                for(const typename LayoutsType::gNode *g = this->localGhosts.head;g;g=g->next)
                {
                    const GIO &gio = g->gio;
                    assert(gio.local   == true);
                    assert(gio.status  == GhostsInfo::Both);
                    assert(gio.forward != 0);
                    assert(gio.reverse != 0);
                    const array<Coord1D> &fwd_inner = gio.forward->inner.indices;
                    const array<Coord1D> &fwd_outer = gio.forward->outer.indices;
                    const array<Coord1D> &rev_inner = gio.reverse->inner.indices;
                    const array<Coord1D> &rev_outer = gio.reverse->outer.indices;

                    assert( fwd_inner.size() == fwd_outer.size());
                    assert( fwd_inner.size() == rev_inner.size());
                    assert( fwd_inner.size() == rev_outer.size());

                    for(size_t j=fwd_inner.size();j>0;--j)
                    {
                        F.copyObject(fwd_outer[j],rev_inner[j]);
                        F.copyObject(rev_outer[j],fwd_inner[j]);
                    }
                }

            }



            //! exchange a full SEQUENCE<FieldPointer>
            inline void localExchange( const ActiveFields &fields )
            {
                for(size_t i=fields.size();i>0;--i)
                {
                    localExchange( (Field &)(*fields[i]) );
                }
            }

            //------------------------------------------------------------------
            //
            // asynchronous exchange
            //
            //------------------------------------------------------------------
            
            //! lighweight asynchonous information for one-way transfer
            struct AsyncIO
            {
                Peer      send; //!< whom to send to
                Peer      recv; //!< from whom to recv
                comm_mode mode; //!< constant/variable
                unsigned  comm; //!<  GhostsComm info
            };

            //! prolog to send in sendingWay

            void asyncProlog(AsyncIO            &aio,
                             const ActiveFields &fields,
                             const Conn::Way     sendingWay,
                             const size_t        orientation)
            {
                const GIO &gio = this->ghosts[orientation];

                sendBlock.free();
                recvBlock.free();
                aio.send = 0;
                aio.recv = 0;
                aio.comm = GhostsComm::None;
                if( gio.async )
                {
                    assert(gio.status!=0);

                    aio.mode = fields.getCommMode();
                    switch(sendingWay)
                    {
                        case Conn::Forward: aio.send = gio.forward; aio.recv = gio.reverse; break;
                        case Conn::Reverse: aio.send = gio.reverse; aio.recv = gio.forward; break;
                    }

                    if(aio.send)
                    {
                        // load sendBlock with inner layout
                        aio.comm |= GhostsComm::Send;
                        const GhostIndices &indices = aio.send->inner.indices;
                        size_t              total   = 0;
                        for(size_t i=fields.size();i>0;--i)
                        {
                            Field &F = (Field &)(*fields[i]);
                            total   += F.save(indices,sendBlock);
                        }
                        __Workspace::CheckBlockTotal(sendBlock,total);
                    }

                    if(aio.recv)
                    {
                        // prepare recvBlock from outer layout
                        aio.comm |= GhostsComm::Recv;
                        switch(aio.mode)
                        {
                            case comm_constant_size:
                                recvBlock.set_fast(fields.getBlockSize() * aio.recv->outer.indices.size());
                                break;

                            case comm_variable_size:
                                assert(0==recvBlock.size());
                                assert(0==fields.getBlockSize());
                                break;
                        }
                    }

                }
                else
                {
                    // no async in this orientation
                    aio.mode = comm_constant_size;
                }
            }


            //! epilog after recv operations in one way
            inline void asyncEpilog(const AsyncIO      &aio,
                                    const ActiveFields &fields)
            {
                
                if(aio.recv)
                {
                    assert(0!=(aio.comm&GhostsComm::Recv));
                    assert(fields.getCommMode()==aio.mode);
                    const GhostIndices &indices = aio.recv->inner.indices;
                    size_t              total   = 0;
                    {
                        ios::imstream input(recvBlock);
                        for(size_t i=fields.size();i>0;--i)
                        {
                            Field &F = (Field &)(*fields[i]);
                            total   += F.load(indices,input);
                        }
                    }
                    __Workspace::CheckBlockTotal(recvBlock,total);
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);


        };
    }
}

#endif

