#include "y/mpi++/mpi.hpp"
#include "y/code/human-readable.hpp"

namespace upsylon
{

    static inline
    string u2s(const uint64_t c)
    {
        return human_readable(c).to_string();
    }

    string mpi:: report() const
    {
        static const size_t ws = nodeName.size()+2;

        string ans = "<I/O report>\n";
        ans.fill(' ',ws) << "Ticks: send=" << u2s(commTicks.send.full) << " | recv= "<< u2s(commTicks.recv.full) << " |\n";
        ans.fill(' ',ws) << "Bytes: send=" << u2s(commBytes.send.full) << " | recv= "<< u2s(commBytes.recv.full) << " |\n";

        for(size_t i=ioFluxes.size();i>0;--i)
        {
            const commFlux &fx = ioFluxes[i];
            if(fx.send.full||fx.recv.full)
            {
                const string tag = vformat("%u",unsigned(fx.info.size));
                ans.fill(' ',ws) << "Data : send=" << u2s(fx.send.full) << " | recv= "<< u2s(fx.recv.full) << " | sizeof(" << fx.info.text() << ")=" << tag << "\n";
            }
        }



        ans.fill(' ',ws) << "<I/O report/>";
        return ans;
    }

}
