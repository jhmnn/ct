#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "CoolLexer.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: coolc <source>\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream ifs(argv[1]);
  if (ifs.fail()) {
    std::cerr << "Error opening file `" << argv[1] << "`\n";
    std::exit(EXIT_FAILURE);
  }

  CoolLexer* lexer = new CoolLexer(ifs, std::cout);
  for (int token = lexer->yylex(); token > 0; token = lexer->yylex()) {
    std::cout << "Token: " << token << " '" << lexer->YYText() << "'\n";
  }

  return 0;
}
