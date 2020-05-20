#include "y/spade/network/transfer.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade {
        
        Transfer:: ~Transfer() throw()
        {
        }
        
        Transfer:: Transfer( const comms::infrastructure where) :
        infra(where),
        style(comms::computed_block_size),
        IO( ios::conveyors::instance() )
        {
            IO.import(infra);
        }
        
        void Transfer:: asyncInitialize(IOBlock &block) throw()
        {
            block.free();
            aliasing::_(style) = comms::computed_block_size;
        }

        void Transfer:: asyncAdjustment(IOBlock &recv, const IOBlock &send) const
        {
            switch(style)
            {
                case comms::flexible_block_size: recv.free(); break;
                case comms::computed_block_size: recv.adjust( send.size(), 0); break;
            }
        }

        
        void Transfer:: updateStyleFrom(const ios::conveyor &io) const throw()
        {
            switch(io.style)
            {
                case comms::flexible_block_size: aliasing::_(style) = comms::flexible_block_size; break;
                case comms::computed_block_size: break;
            }
        }
        
        void Transfer:: activate( Kernel::Field &F ) const
        {
            F.io = IO.search(F.objectType,infra);
            const string &id = type_spec::declare(F.objectType).name();
            if(!F.io) throw exception("Spade::Field<%s> '%s': no register I/O", *id, *F.name);
        }
        
        void Transfer:: asyncSave(IOBlock             &block,
                                  const Kernel::Field &field,
                                  const Ghost         &ghost) const
        {
            assert(field.io);
            const ios::conveyor &io = *field.io;
            size_t               n  = ghost.items; assert(ghost.items==ghost.size());
            
            updateStyleFrom(io);
            while(n>0)
            {
                io.save(block, field.objectAt( ghost[n] ) );
                --n;
            }
        }
        
        void Transfer:: asyncSave(IOBlock       &block,
                                  Fields        &fields,
                                  const Ghost   &ghost) const
        {
            const size_t f = fields.size();
            
            {
                size_t j = f;
                while(j>0)
                {
                    assert( fields[j]->io );
                    updateStyleFrom( *(fields[j]->io) );
                    --j;
                }
            }
            
            size_t       n = ghost.items;
            while(n>0)
            {
                const size_t i = ghost[n];
                size_t       j = f;
                while(j>0)
                {
                    const Kernel::Field &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io    = *field.io;
                    io.save(block,field.objectAt(i));
                    --j;
                }
                --n;
            }
        }
        
        void Transfer:: asyncLoad(Kernel::Field &field,
                                  ios::istream  &source,
                                  const Ghost   &ghost) const
        {
            assert(field.io);
            const ios::conveyor &io = *field.io;
            size_t               n  = ghost.items; assert(ghost.items==ghost.size());
            while(n>0)
            {
                io.load( field.objectAt( ghost[n] ), source);
                --n;
            }
        }
        
        void Transfer:: asyncLoad(Fields        &fields,
                                  ios::istream  &source,
                                  const Ghost   &ghost) const
        {
            const size_t f = fields.size();
            size_t       n  = ghost.items;
            while(n>0)
            {
                const size_t i = ghost[n];
                size_t       j = f;
                while(j>0)
                {
                    Kernel::Field       &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io    = *field.io;
                    io.load(field.objectAt(i),source);
                    --j;
                }
                --n;
            }
        }
        
        
        void Transfer:: localSwap(Kernel::Field &field,
                                  const Indices &innerFwd,
                                  const Indices &outerFwd,
                                  const Indices &innerRev,
                                  const Indices &outerRev) const
        {
            assert(innerFwd.size() == outerFwd.size() );
            assert(innerFwd.size() == outerRev.size() );
            assert(innerFwd.size() == innerRev.size() );
            assert(field.io!=NULL);
            
            size_t               n  = innerFwd.size();
            const ios::conveyor &io = *field.io;
            while(n>0)
            {
                io.copy( field.objectAt(outerFwd[n]), field.objectAt(innerRev[n]));
                io.copy( field.objectAt(outerRev[n]), field.objectAt(innerFwd[n]));
                --n;
            }
        }
        
        void Transfer:: localSwap(Fields        &fields,
                                  const Indices &innerFwd,
                                  const Indices &outerFwd,
                                  const Indices &innerRev,
                                  const Indices &outerRev) const
        {
            assert(innerFwd.size() == outerFwd.size() );
            assert(innerFwd.size() == outerRev.size() );
            assert(innerFwd.size() == innerRev.size() );
            
            const size_t         f  = fields.size();
            size_t               n  = innerFwd.size();
            while(n>0)
            {
                const size_t outerFwd_ = outerFwd[n];
                const size_t innerFwd_ = innerFwd[n];
                const size_t outerRev_ = outerRev[n];
                const size_t innerRev_ = innerRev[n];
                size_t j = f;
                while(j>0)
                {
                    Kernel::Field       &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io = *field.io;
                    io.copy( field.objectAt(outerFwd_), field.objectAt(innerRev_) );
                    io.copy( field.objectAt(outerRev_), field.objectAt(innerFwd_) );
                    --j;
                }
                --n;
            }
        }
        
        
        
    }
    
}
