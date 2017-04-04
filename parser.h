#ifndef DATA_DRAWER_H
#define DATA_DRAWER_H

#define DATA_OFFSET 0

#include <QObject>
#include <fstream>

#include "rls_header.h"

class Parser
{
public:
  Parser();
  Parser(const Parser& p) {*this = p;}
  Parser& operator=(const Parser&) {return *this;}
  ~Parser() {}

  void openFile(const char*);
  const DATA_PACKAGE_AD getData();

private:
  std::ifstream file;
  unsigned length;
};

#endif // DATA_DRAWER_H
