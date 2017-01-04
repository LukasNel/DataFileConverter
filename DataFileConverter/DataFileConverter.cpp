// DataFileConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Converter.h"
using namespace std;
int main()
{
	cout << split("stuff;", ';').size() << endl;
	cout << ConverterRed("openred.txt") << endl;
	while (1);
    return 0;
}

