#pragma once

#include "mvs/visitors/node_count.hpp"

namespace mvs{
    
    struct Expr;
    
    int node_count(const Expr &expr);
}