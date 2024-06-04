#include "fd.h"
#include "scanner.h"
#include <iostream>
#include <string>

// Helper function to convert TokenType to string
std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case lx_identifier:
        return "Identifier";
    case lx_float:
        return "Float";
    case lx_integer:
        return "Integer";
    case lx_string:
        return "String";
    case lx_plus:
        return "Plus";
    case lx_minus:
        return "Minus";
    case lx_star:
        return "Star";
    case lx_slash:
        return "Slash";
    case lx_eq:
        return "Equals";
    case lx_neq:
        return "Not Equals";
    case lx_lt:
        return "Less Than";
    case lx_le:
        return "Less Equals";
    case lx_gt:
        return "Greater Than";
    case lx_ge:
        return "Greater Equals";
    case lx_lparen:
        return "Left Parenthesis";
    case lx_rparen:
        return "Right Parenthesis";
    case lx_lbracket:
        return "Left Bracket";
    case lx_rbracket:
        return "Right Bracket";
    case lx_colon:
        return "Colon";
    case lx_dot:
        return "Dot";
    case lx_semicolon:
        return "Semicolon";
    case lx_comma:
        return "Comma";
    case lx_colon_eq:
        return "Colon Equals";
    case kw_program:
        return "Keyword Program";
    case kw_var:
        return "Keyword Var";
    case kw_constant:
        return "Keyword Constant";
    case kw_integer:
        return "Keyword Integer";
    case kw_boolean:
        return "Keyword Boolean";
    case kw_string:
        return "Keyword String";
    case kw_float:
        return "Keyword Float";
    case kw_true:
        return "Keyword True";
    case kw_false:
        return "Keyword False";
    case kw_if:
        return "Keyword If";
    case kw_fi:
        return "Keyword Fi";
    case kw_then:
        return "Keyword Then";
    case kw_else:
        return "Keyword Else";
    case kw_while:
        return "Keyword While";
    case kw_do:
        return "Keyword Do";
    case kw_od:
        return "Keyword Od";
    case kw_and:
        return "Keyword And";
    case kw_or:
        return "Keyword Or";
    case kw_read:
        return "Keyword Read";
    case kw_write:
        return "Keyword Write";
    case kw_for:
        return "Keyword For";
    case kw_from:
        return "Keyword From";
    case kw_to:
        return "Keyword To";
    case kw_by:
        return "Keyword By";
    case kw_function:
        return "Keyword Function";
    case kw_procedure:
        return "Keyword Procedure";
    case kw_return:
        return "Keyword Return";
    case kw_not:
        return "Keyword Not";
    case kw_begin:
        return "Keyword Begin";
    case kw_end:
        return "Keyword End";
    case END_OF_FILE:
        return "End Of File";
    case UNKNOWN:
        return "Unknown";
    case COMMENT:
        return "Comment";
    case INTEGER_LITERAL:
        return "Integer Literal";
    case STRING_LITERAL:
        return "String Literal";
    case OPERATOR:
        return "Operator";
    case KEYWORD:
        return "Keyword";
    default:
        return "Undefined Token Type";
    }
}

int main(int argc, char **argv)
{
    // Open the file
    FileDescriptor *fd;
    if (argc > 1)
    {
        fd = new FileDescriptor(argv[1]);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << "test.txt" << std::endl;
        return 1;
    }

    if (!fd->IsOpen())
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    // Create a scanner object
    Scanner scanner(fd);
    TOKEN *token;

    // Scan tokens until end of file
    while ((token = scanner.scan())->type != END_OF_FILE)
    {
        std::cout << "Token: ";
        if (!token->str_ptr.empty())
        {
            std::cout << token->str_ptr;
        }
        else if (token->type == lx_integer || token->type == INTEGER_LITERAL)
        {
            std::cout << token->value;
        }
        else if (token->type == lx_float)
        {
            std::cout << token->float_value;
        }

        std::cout << " Type: " << tokenTypeToString(token->type) << std::endl;
        delete token;
    }

    // Delete the last token (END_OF_FILE token)
    delete token;
    delete fd;
    return 0;
}