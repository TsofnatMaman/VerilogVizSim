#pragma once

#include "mvs/visitors/node_count.hpp"
#include "mvs/visitors/expression_evaluator.hpp"
#include "mvs/visitors/print.hpp"

namespace mvs{
    
    struct Expr;
    
    int node_count(const Expr &expr);
    int to_string(const Expr &expr);
    int expression_evaluator();
}