#pragma once

#include "mvs/visitors/node_count.h"

namespace mvs{
    
    struct Expr;
    
    int node_count(const Expr &expr);
}