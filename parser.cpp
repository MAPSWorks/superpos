#include "parser.h"
#include <iostream>

using namespace std;

Parser::Parser()
{
}

void Parser::openFile(const char* name) {
  file.open(name,

 // file.open("/media/byldin/a64c9c7d-9716-464d-9ee5-812df991874f/Эксперименты 18.12.16/RLS_3_fileRLS_FFT_001.b",
 // file.open("/home/byldin/work/polinom/RLS_4_fileRLS_FFT_001.b",
 // file.open("/media/byldin/204E-EA5F/rls_data/regs/RLS_3_fileRLS_FFT_001.b",
 // file.open("/windows/Work/IANS/polinom/meteo_interf/RLS_4_fileRLS_FFT_001.b",
 // file.open("/windows/Work/IANS/polinom/Эксперименты_10_6хРЛС/1_250316/RLS_1_fileRLS_FFT_001.b",
  ifstream::binary);

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
