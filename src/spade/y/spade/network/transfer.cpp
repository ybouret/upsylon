#include "y/spade/network/transfer.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade {
        
        IOBlocks:: IOBlocks(const size_t levels) :
        IOBlocksType(2*levels)
        {
            while( size() < count )
            {
                build();
            }
        }
        
        IOBlocks::~IOBlocks() throw()
        {
        }
        
        
        Transfer:: ~Transfer() throw()
        {
        }
        
        Transfer:: Transfer( const comms::infrastructure where) :
        infra(where),
        style(comms::computed_block_size),
        chunk(0),
        IO( ios::conveyors::instance() )
        {
            IO.import(infra);
        }
        
        void Transfer:: asyncSetup(const Field &field) throw()
        {
            assert(field.io);
            switch( aliasing::_(style) = field.io->style )
            {
                case comms::flexible_block_size: aliasing::_(chunk) = 0; break;
                case comms::computed_block_size: aliasing::_(chunk) = field.objectSize; break;
            }
        }
        
        


        
        
        void Transfer:: asyncSetup(const accessible<_Field> &fields) throw()
        {
            aliasing::_(style) = comms::computed_block_size;
            aliasing::_(chunk) = 0;
            for(size_t i=fields.size();i>0;--i)
            {
                const Field &field = *fields[i];
                assert(field.io);
                switch(field.io->style)
                {
                    case comms::flexible_block_size:
                        aliasing::_(style) = comms::flexible_block_size;
                        aliasing::_(chunk) = 0;
                        return;
                        
                    case comms::computed_block_size:
                        aliasing::_(chunk) += field.objectSize;
                        break;
                }
            }
        }
        
        void Transfer:: asyncMake(IOBlock &block, const Ghost &ghost) const
        {
            switch(style)
            {
                case comms::flexible_block_size: block.free(); break;
                case comms::computed_block_size: block.set_fast(ghost.items * chunk); break;
            }
        }
        
        
        void Transfer:: setup( Field &F ) const
        {
            F.io             = IO.search(F.objectType,infra);
            const string &id = type_spec::declare(F.objectType).name();
            if(!F.io) throw exception("Spade::Field<%s> '%s': no register I/O", *id, *F.name);
        }
        
        void Transfer:: asyncSave(IOBlock     &block,
                                  const Field &field,
                                  const Ghost &ghost) const
        {
            assert(field.io);
            const ios::conveyor &io = *field.io;
            size_t               n  = ghost.items; assert(ghost.items==ghost.size());
            
            while(n>0)
            {
                io.save(block, field.objectAt( ghost[n] ) );
                --n;
            }
        }
        
        void Transfer:: asyncSave(IOBlock                  &block,
                                  const accessible<_Field> &fields,
                                  const Ghost              &ghost) const
        {
            const size_t f = fields.size();
            size_t       n = ghost.items;
            while(n>0)
            {
                const size_t i = ghost[n];
                size_t       j = f;
                while(j>0)
                {
                    const Field         &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io    = *field.io;
                    io.save(block,field.objectAt(i));
                    --j;
                }
                --n;
            }
        }
        
        void Transfer:: asyncLoad(Field         &field,
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
        
        void Transfer:: asyncLoad(Field         &field,
                                  const IOBlock &block,
                                  const Ghost   &ghost) const
        {
            ios::imstream source(block);
            asyncLoad(field,source,ghost);
        }
        
        void Transfer:: asyncLoad(addressable<_Field> &fields,
                                  ios::istream        &source,
                                  const Ghost         &ghost) const
        {
            const size_t f = fields.size();
            size_t       n  = ghost.items;
            while(n>0)
            {
                const size_t i = ghost[n];
                size_t       j = f;
                while(j>0)
                {
                    Field               &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io    = *field.io;
                    io.load(field.objectAt(i),source);
                    --j;
                }
                --n;
            }
        }
        
        void Transfer:: asyncLoad(addressable<_Field>  &fields,
                                  const IOBlock        &block,
                                  const Ghost          &ghost) const
        {
            ios::imstream source(block);
            asyncLoad(fields,source,ghost);
        }
        
        void Transfer:: localSwap(Field         &field,
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
        
        void Transfer:: localSwap(addressable<_Field> &fields,
                                  const Indices       &innerFwd,
                                  const Indices       &outerFwd,
                                  const Indices       &innerRev,
                                  const Indices       &outerRev) const
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
                    Field               &field = *fields[j]; assert(field.io);
                    const ios::conveyor &io    = *field.io;
                    io.copy( field.objectAt(outerFwd_), field.objectAt(innerRev_) );
                    io.copy( field.objectAt(outerRev_), field.objectAt(innerFwd_) );
                    --j;
                }
                --n;
            }
        }
        
        
        
    }
    
}
