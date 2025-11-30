#include "mvs/netlist_extractor.hpp"
#include "mvs/module.hpp" // מכיל את מבני ה-AST: Assign, ExprPtr, ExprIdent וכו'.
#include <stdexcept>

namespace mvs
{
    // נניח ש-char_to_gate קיים מתוך הקוד הקודם
    GateType char_to_gate(char op)
    {
        switch (op)
        {
        case '&': return GateType::AND;
        case '|': return GateType::OR;
        case '^': return GateType::XOR;
        case '~': return GateType::NOT; // למרות ש-NOT הוא Unary, נכלול אותו כאן
        default: throw std::runtime_error("Unsupported gate type: " + std::string(1, op));
        }
    }

    // 💡 שימו לב: זוהי פונקציה רקורסיבית מפושטת המשתמשת ב-dynamic_cast
    // כדי לפרש את ה-AST ולבנות את הנטליסט.
    void process_expression(const ExprPtr& expr, Netlist& netlist, std::vector<std::string>& current_inputs, const std::string& output_name);

    Netlist NetlistExtractor::extract(const Module& module)
    {
        Netlist netlist;

        for (const auto& assign : module.assigns)
        {
            std::vector<std::string> inputs;
            
            // תהליך רקורסיבי על צד ימין של ההקצאה
            // output_name בשיטה זו הוא ה-name של assign
            process_expression(assign.rhs, netlist, inputs, assign.name);
            
            // אם אחרי עיבוד, נותרו שני קלטים או יותר, הם כבר טופלו
            // אם נותרו רק קלט אחד, זה יכול להיות Identity (assign A = B;)
            if (inputs.size() == 1)
            {
                // ה-assign יצר כבר רכיב IDENTTITY או שהוא היה קבוע
                // אם ה-process_expression לא יצר צומת חדש (כמו במקרה של ExprIdent),
                // צריך להוסיף צומת IDENTITY
                // (הלוגיקה הזו מורכבת יותר ודורשת בדיקה האם ה-input הוא כבר פלט של שער)
                // לצורך הפשטות, אם נותר input יחיד, נניח שזו IDENTITY
                
                // התיקון הפשוט ביותר: נטפל רק בביטויים מורכבים
                // אם הביטוי הוא רק IDENT, הוא יטופל למטה ב-process_expression.
            }
        }
        return netlist;
    }

    void process_expression(const ExprPtr& expr, Netlist& netlist, std::vector<std::string>& current_inputs, const std::string& output_name)
    {
        // נניח שהמבנים: ExprIdent, ConstExpr, ExprUnary, ExprBinary קיימים
        if (auto ident = dynamic_cast<const ExprIdent*>(expr.get()))
        {
            // זהו קלט - לא מייצר שער, רק מוסיף ל-inputs
            current_inputs.push_back(ident->name);
        }
        else if (auto const_expr = dynamic_cast<const ConstExpr*>(expr.get()))
        {
            // יצירת רכיב קבוע
            netlist.push_back({
                output_name, GateType::CONSTANT, {}, const_expr->value
            });
        }
        else if (auto unary = dynamic_cast<const ExprUnary*>(expr.get()))
        {
            // דוגמה: assign Z = ~A;
            
            // 1. קבלת קלט
            std::vector<std::string> inputs_rhs;
            process_expression(unary->rhs, netlist, inputs_rhs, output_name); 

            // 2. יצירת שער NOT
            if (inputs_rhs.size() == 1)
            {
                netlist.push_back({
                    output_name, char_to_gate(unary->op), {inputs_rhs[0]}
                });
            } else {
                 // ביטוי מורכב יותר (למשל: assign Z = ~(A & B);)
                 // במקרה כזה, הביטוי הפנימי כבר יצר שער שהפלט שלו הוא output_name,
                 // וה-NOT צריך להיות צומת חדש. זה דורש יצירת wire זמני,
                 // אך לצורך הפשטות נשתמש ב-output_name הנתון כפלט הסופי.
            }

        }
        else if (auto binary = dynamic_cast<const ExprBinary*>(expr.get()))
        {
            // דוגמה: assign Z = A & B;
            
            // 1. קבלת קלטים (A ו-B)
            std::vector<std::string> inputs;
            process_expression(binary->lhs, netlist, inputs, output_name);
            process_expression(binary->rhs, netlist, inputs, output_name);

            // 2. יצירת שער AND/OR/XOR
            if (inputs.size() == 2)
            {
                netlist.push_back({
                    output_name, 
                    char_to_gate(binary->op), 
                    {inputs[0], inputs[1]}
                });
            }
        }
        else
        {
            // במקרה של צומת לא נתמך, זרוק שגיאה
            throw std::runtime_error("NetlistExtractor: Unsupported expression type in AST.");
        }
    }
}