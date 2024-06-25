#include "fd.h"
#include "scanner.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	// Create a FileDescriptor object with the given file name
	FileDescriptor fd(argv[1]);

	// Create a Scanner object that operates on the FileDescriptor
	Scanner scanner(&fd);

	// Loop to scan and print tokens until EOF
	TOKEN *token;
	do
	{
		token = scanner.scan();
		std::cout << "Token type: " << scanner.tokenTypeToString(token->type) << ", Value: " << token->str_ptr << "\n";
		delete token; // Free allocated memory for the token
	} while (token->type != END_OF_FILE);

	return 0;
}
