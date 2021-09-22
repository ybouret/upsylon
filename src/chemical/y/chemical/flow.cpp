
#include "y/chemical/flow.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Flow:: Flow(const State s) throw() : state(s)
        {
        }

        Flow:: ~Flow() throw()
        {
        }


        const char   * Flow:: StateText(const State t)   throw()
        {
            switch(t)
            {
                case Bounded: return "BOUNDED";
                case Endless: return "ENDLESS";
            }
            return unknown_text;
        }

        const char * Flow:: stateText() const throw()
        {
            return StateText(state);
        }
    }

}


