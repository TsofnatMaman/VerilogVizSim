#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace mvs
{
    enum class PortDir
    {
        INPUT,
        OUTPUT,
        INOUT
    };

    struct ExprIdent;
    struct ConstExpr;
    struct ExprUnary;
    struct ExprBinary;

    // --- Visitor interface ---
    struct ExprVisitor
    {
        virtual ~ExprVisitor() = default;
        virtual int visit(const ExprIdent &) = 0;
        virtual int visit(const ConstExpr &) = 0;
        virtual int visit(const ExprUnary &) = 0;
        virtual int visit(const ExprBinary &) = 0;
    };

    struct Expr
    {
        virtual ~Expr() = default;
        virtual int accept(ExprVisitor &v) const = 0;
    };

    // --- Smart pointer type ---
    using ExprPtr = std::shared_ptr<Expr>;

    // --- AST node types ---
    struct ExprIdent : Expr
    {
        std::string name;
        std::optional<int> pos;
        int accept(ExprVisitor &v) const override { return v.visit(*this); }
    };

    struct ConstExpr : Expr
    {
        int value = 0;
        int accept(ExprVisitor &v) const override { return v.visit(*this); }
    };

    struct ExprUnary : Expr
    {
        char op = '~';
        ExprPtr rhs;
        int accept(ExprVisitor &v) const override { return v.visit(*this); }
    };

    struct ExprBinary : Expr
    {
        char op = '&';
        ExprPtr lhs;
        ExprPtr rhs;
        int accept(ExprVisitor &v) const override { return v.visit(*this); }
    };

    struct TargetBits
    {
        std::optional<int> msb; // Most Significant Bit
        std::optional<int> lsb; // Least Significant Bit
    };

    struct Assign
    {
        std::string name;

        TargetBits tb;
        ExprPtr rhs;
    };

    struct Port
    {
        PortDir dir = PortDir::INPUT;
        std::string name;
        int width = 32;
    };

    struct Wire
    {
        std::string name;
        int width = 32;
    };

    struct Module
    {
        std::string name;
        std::vector<Port> ports;
        std::vector<Wire> wires;
        std::vector<Assign> assigns;
    };

} // namespace mvs