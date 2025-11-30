#ifndef MVS_NETLIST_EXTRACTOR_HPP
#define MVS_NETLIST_EXTRACTOR_HPP

#include "netlist_types.hpp"
#include "module.hpp"

namespace mvs
{
    class NetlistExtractor
    {
    public:
        /**
         * Extract a Netlist from a parsed Module (AST).
         * @param module The parsed Module containing assignments.
         * @return A Netlist representing the circuit logic.
         */
        static Netlist extract(const Module& module);
    };
}

#endif // MVS_NETLIST_EXTRACTOR_HPP
