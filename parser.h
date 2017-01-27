#ifndef DATA_DRAWER_H
#define DATA_DRAWER_H

#define DATA_OFFSET 64000

#include <QObject>
#include <fstream>

#include "rls_header.h"

class Parser
{
public:
  Parser();
  ~Parser() {}

  void openFile();
  const DATA_PACKAGE_AD getData();

private:
  std::ifstream file;
  unsigned length;
};

#endif // DATA_DRAWER_H
