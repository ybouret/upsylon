
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

        topology:: ~topology() throw()
        {
            aliasing::_(size)=0;
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
            aliasing::_(size)   += cls->count;
        }

        


        topology:: topology() :
        clusters(),
        size(0)
        {
            string info;
            if( environment::get(info,Y_NUM_THREADS) )
            {
                std::cerr << "Y_NUM_THREADS=<" << info << ">" << std::endl;
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

            if(size<=0)
            {
                throw exception("empty topology!");
            }
        }

        std::ostream & operator<<(std::ostream &os, const topology &topo)
        {
            os << "<topology size='" << topo.size << "' clusters='" << topo.clusters.size << "'>" << std::endl;
            for(const nucleus::cluster *cls=topo.clusters.head;cls;cls=cls->next)
            {
                os << '\t' << *cls << std::endl;
            }
            os << "<topology/>";
            return os;
        }

        size_t topology:: core_of(const size_t rank) const throw()
        {
            assert(rank<size);
            assert(clusters.size>0);
            const nucleus::cluster *cls = clusters.head;
            size_t lo=0;
            size_t up=lo+cls->count;

            while(rank>=up)
            {
                cls  = cls->next;
                lo   = up;
                up  += cls->count;
            }

            //std::cerr << "found rank between " << lo << " and " << up << std::endl;

            return cls->core_of(rank-lo);
        }

    }

}

