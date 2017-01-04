#include "stdafx.h"
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

vector<string> split(const string &s, char delim) {
	vector<string> elems;
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
string to_uppercase(string fstr) {
	string outStr = "";
	for (int i = 0; i < fstr.length(); i++)outStr += toupper(fstr[i]);
	return outStr;
}
string EraseCharacterFromString(string fstr, char fchar) {
	string tStr = fstr;
	tStr.erase(std::remove(tStr.begin(), tStr.end(), fchar), tStr.end());	
	return tStr;
}
char* convertType(string fstr) {
	if (CASELESS_EQUAL(fstr, "BYTE")) { return "INT8"; }
	else if (CASELESS_EQUAL(fstr, "WORD")) { return "INT16"; }
	else if (CASELESS_EQUAL(fstr, "DWORD")) { return "INT32"; }
	else if (CASELESS_EQUAL(fstr, "QWORD")) { return "INT64"; }
	else if (CASELESS_EQUAL(fstr, "LPBYTE")) { return "POINTER"; }
	return "";
}
char* unconvertType(string fstr) {
	if (CASELESS_EQUAL(fstr, "INT8")) { return "BYTE"; }
	else if (CASELESS_EQUAL(fstr, "INT16")) { return "WORD"; }
	else if (CASELESS_EQUAL(fstr, "INT32")) { return "DWORD"; }
	else if (CASELESS_EQUAL(fstr, "INT64")) { return "QWORD"; }
	else if (CASELESS_EQUAL(fstr, "POINTER")) { return "LPBYTE"; }
	return "";
}
char* convertArrayType(string fstr) {
	if (CASELESS_EQUAL(fstr, "INT8")) { return "db"; }
	else if (CASELESS_EQUAL(fstr, "INT16")) { return "dw"; }
	else if (CASELESS_EQUAL(fstr, "INT32")) { return "dd"; }
	else if (CASELESS_EQUAL(fstr, "INT64")) { return "dq"; }
	else if (CASELESS_EQUAL(fstr, "POINTER")) { return "dp"; }
	return "";
}
string ProcessArray(vector<string> tokens) {
	string output = tokens[0];
	string num = output;
	num.erase(0, num.find("[")+1);
	num.erase(num.find("]"),num.size());
	output.erase(output.find("["),output.size());
	
	
	string data = tokens[2];
	if (data.find('{') != string::npos && data.find('}') != string::npos) {
		data = EraseCharacterFromString(data, '{');
		data = EraseCharacterFromString(data, '}');
		num = SSTR(split(data, ',').size());
		data = string("\n<") + convertArrayType(tokens[1]) + string("><") + data + string(">");
	}
	else {
		data = "";
	}
	output += " <BUFFER>";
	if (!num.empty()) {
		cout << num << endl;
		output += "<";
		output += num + "*sizeof(";
		output += unconvertType(tokens[1]);
		output += ")>";
	}
		
	output += data;
	return output;

}
string ProcessStrings(vector<string> tokens) {
	string output = "";
	if (tokens.size() < 3)return string("");

	output += tokens[0];
	if (output.find('[') != string::npos && output.find(']') != string::npos) {
		return ProcessArray(tokens);
	}
	output += " <";
	output += tokens[1] + "><";
	output += tokens[2] + ">";
	return output;
}

string ConverterRed(const char* filename) {
	ifstream filereader(filename);
	string line = "";
	string output = "";
	vector<string> mem;

	if (filereader.is_open()) {
		while (getline(filereader, line)) {

			output += line;

			if (output.find(';') == std::string::npos) {
				output += ' ';
				continue;
			}
			mem.push_back(string("//") + output);
			vector<string> stuff;
			vector<string> tokens = split(output, ';');
			if (!tokens.size())continue;
			output = tokens[0];
			tokens = split(output, ' ');
			tokens.erase(std::remove(tokens.begin(), tokens.end(), ""), tokens.end());
			stuff.push_back(tokens[1]);
			stuff.push_back(split(string(convertType(tokens[0])), '=')[0]);
			string outputwithoutSpace = output;
			outputwithoutSpace = EraseCharacterFromString(outputwithoutSpace,' ');
			//outputwithoutSpace.erase(std::remove(outputwithoutSpace.begin(), outputwithoutSpace.end(), ' '), outputwithoutSpace.end());
			tokens = split(outputwithoutSpace, '=');
			if (tokens.size() > 1) {
				stuff.push_back(tokens[1]);
			}
			else {
				stuff.push_back("0");
			}
			mem.push_back(ProcessStrings(stuff));
			output = "";
		}
		filereader.close();
	}
	line = "sasm_";
	ofstream fileout(line + filename);
	string outStr = "";
	for (int i = 0; i < mem.size(); i++) outStr += mem[i] + "\n";

	fileout << outStr << "buckchecker";
	fileout.close();
	return line + filename;
}