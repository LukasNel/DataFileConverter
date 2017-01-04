#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <sstream>
using namespace std;
enum Type : int {
	BYTE,
	WORD,
	DWORD,
	QWORD,
	LPBYTE,
	NONE
};
#define CASELESS_EQUAL(a, b) ((!to_uppercase(a).compare(to_uppercase(string(b)))) ? 1 : 0)
#define STR_EQUAL(a, b) (((!a.compare(b))) ? 1 : 0)
#include "Converter.cpp"