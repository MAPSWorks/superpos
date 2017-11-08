#include "parser.h"
#include <iostream>

using namespace std;

Parser::Parser()
{
}

void Parser::openFile(const char* name) {
  file.open(name, ifstream::binary);

  file.seekg (0, file.end);
  length = file.tellg();
  file.seekg (DATA_OFFSET * sizeof(DATA_PACKAGE_AD), file.beg);
}

const DATA_PACKAGE_AD Parser::getData()
{
  DATA_PACKAGE_AD rls_data;
  file.read((char*)&rls_data, sizeof(DATA_PACKAGE_AD));
  return rls_data;
}
