#include <iostream>
#include "fd.h"		 // Assuming this header defines FileDescriptor
#include "scanner.h" // Assuming this header defines SCANNER and TOKEN

using namespace std;

int main()
{
	FileDescriptor fd("test_scanner.txt");
	cout << "Tokens:" << endl
		 << endl;
	SCANNER scanner(&fd);
	TOKEN *token;

	while ((token = scanner.Scan())->type != lx_eof)
	{
		if (token->str_ptr != nullptr)
		{
			cout << "Token Type: " << token->type << ", Value: " << token->str_ptr << endl;
			delete[] token->str_ptr; // Free allocated memory for string tokens
		}
		else if (token->float_value != 0.0)
		{
			cout << "Token Type: " << token->type << ", Value: " << token->float_value << endl;
		}
		else
		{
			cout << "Token Type: " << token->type << ", Value: " << token->value << endl;
		}
		delete token; // Free allocated memory for the token
	}

	return 0;
}
