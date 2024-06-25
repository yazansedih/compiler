#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <unordered_map>
#include "fd.h"

// Token types enumeration
typedef enum
{
	lx_identifier,
	lx_float,
	lx_integer,
	lx_string,
	lx_plus,
	lx_minus,
	lx_star,
	lx_slash,
	lx_eq,
	lx_neq,
	lx_lt,
	lx_le,
	lx_gt,
	lx_ge,
	lx_lparen,
	lx_rparen,
	lx_lbracket,
	lx_rbracket,
	lx_lbrace,
	lx_rbrace,
	lx_colon,
	lx_dot,
	lx_semicolon,
	lx_comma,
	lx_colon_eq,
	kw_program,
	kw_var,
	kw_constant,
	kw_integer,
	kw_boolean,
	kw_string,
	kw_float,
	kw_true,
	kw_false,
	kw_if,
	kw_fi,
	kw_then,
	kw_else,
	kw_while,
	kw_do,
	kw_od,
	kw_and,
	kw_or,
	kw_read,
	kw_write,
	kw_for,
	kw_from,
	kw_to,
	kw_by,
	kw_function,
	kw_procedure,
	kw_return,
	kw_not,
	kw_begin,
	kw_end,
	END_OF_FILE,
	UNKNOWN,
	COMMENT,
	INTEGER_LITERAL,
	STRING_LITERAL,
	OPERATOR,
	KEYWORD,
	IDENTIFIER
} TokenType;

// Token structure
typedef struct
{
	TokenType type;
	std::string str_ptr;
	int value;
	double float_value;
} TOKEN;

class Scanner
{
private:
	FileDescriptor *fd;
	std::unordered_map<std::string, TokenType> keywords;
	void initKeywords();

public:
	Scanner(FileDescriptor *fd);
	TOKEN *scan();
	TOKEN *getNextToken();
	std::string tokenTypeToString(TokenType type);
};

#endif /* SCANNER_H */
