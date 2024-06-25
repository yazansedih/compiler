#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <unordered_map>
#include "fd.h" // Assuming this header defines FileDescriptor

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

	// Helper function to skip comments
	void skip_comments();

	// Helper function to check if a string is a keyword
	TOKEN *check_keyword(const char *str);

	// Helper function to get an identifier token
	TOKEN *get_id(const char *first_char);

	// Helper function to get an operator token
	TOKEN *get_op(const char *op);

	// Helper function to get an integer token
	TOKEN *get_int(const char *first_char);

	// Helper function to get a floating-point token
	TOKEN *get_float(const char *first_char);

	// Helper function to get a string token
	TOKEN *get_string(const char *str);

public:
	Scanner(FileDescriptor *fd);
	TOKEN *scan();
	std::string tokenTypeToString(TokenType type);
};

int keywordss = 24; /* number of keywords */
const char *keywordd[] = {
	"program", "var", "constant", "integer", "bool", "string",
	"float", "true", "false", "if", "fi", "then", "else", "while",
	"do", "od", "and", "or", "read", "write", "for", "from", "to",
	"by", nullptr};

LEXEME_TYPE key_type[] = {
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
};

int operators = 22;
const char *operator_list[] = {
	"(",
	")",
	":",
	":=",
	"+",
	"-",
	"*",
	"/",
	"=",
	"!=",
	"<",
	"<=",
	">",
	">=",
	".",
	";",
	"[",
	"]",
	",",
	"",
	"{",
	"}",
};

LEXEME_TYPE operator_type[] = {
	lx_Iparen,
	lx_rparen,
	Ix_colon,
	x_colon_eq,
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
	lx_dot,
	lx_semicolon,
	lx_lbracket,
	lx_rbracket,
	lx_comma,
	lx_eof,
	lx_lcbracket,
	lx_rcbracket,
};

#endif /* SCANNER_H */
