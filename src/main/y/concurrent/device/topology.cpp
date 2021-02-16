
#include "y/concurrent/device/topology.hpp"
#include "y/type/aliasing.hpp"
#include "y/os/hw.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/exceptions.hpp"
#include <iostream>

namespace upsylon
{
    namespace concurrent
    {

        topology:: node:: ~node() throw()
        {
            aliasing::_(rank) = 0;
        }
        
        topology:: node:: node(const size_t r) throw() :
        next(0), prev(0), rank(r)
        {
        }
        
        
        
        topology:: ~topology() throw()
        {
        }

        
        void topology:: add(const size_t start, const size_t width, const size_t every)
        {
            add_( nucleus::cluster::create(start,width,every) );
        }

        void topology:: add(const string &description)
        {
            add_( nucleus::cluster::create(description) );
        }

        void topology:: add(const char *description)
        {
            const string _(description);
            add(_);
        }

        void topology:: add_(nucleus::cluster *cls) throw()
        {
            assert(cls);
            aliasing::_(clusters).push_back(cls);
            const size_t old_size = nodes.size;
            try
            {
                for(size_t rank=0;rank<cls->count;++rank)
                {
                    aliasing::_(nodes).push_back( new node( cls->core_of(rank) ) );
                }
            }
            catch(...)
            {
                while(nodes.size>old_size)
                {
                    delete aliasing::_(nodes).pop_back();
                }
                throw;
            }
        }

        size_t topology:: size() const throw()
        {
            return nodes.size;
        }
        
        topology:: topology(const size_t start,
                            const size_t width,
                            const size_t every)
        {
            add(start,width,every);
        }

        topology:: topology() :
        nodes(),
        clusters()
        {
            string info;
            if( environment::get(info,Y_NUM_THREADS) )
            {
                //std::cerr << "Y_NUM_THREADS=<" << info << ">" << std::endl;
                vector<string,memory::pooled> descriptions(4,as_capacity);
                tokenizer<char>::split_with(descriptions,info,',');
                for(size_t i=1;i<=descriptions.size();++i)
                {
                    add(descriptions[i]);
                }
            }
            else
            {
                add(0,hardware::nprocs(),1);
            }

            if(nodes.size<=0)
            {
                throw exception("empty topology!");
            }
        }

        std::ostream & operator<<(std::ostream &os, const topology &topo)
        {
            os << "<topology nodes='" << topo.nodes.size << "' clusters='" << topo.clusters.size << "'>" << std::endl;
            for(const nucleus::cluster *cls=topo.clusters.head;cls;cls=cls->next)
            {
                os << '\t' << *cls << std::endl;
            }
            os << "<topology/>";
            return os;
        }

       

    }

}

