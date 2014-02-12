#include <Windows.h>
#include <vector>
#include <regex>
#include <string>
#include <tchar.h>



using namespace std;

#if !defined(ISZKUTIL_H)
#define ISZKUTIL_H

#if !defined(MRECORD_H)
#include "masterRecord.h"
#endif

class iszkutil{
public:
	static long rand(long min,long max);
	static string w2s(wstring in,string cs="UTF-8");
	static wstring s2w(string in,string cs="UTF-8");
	static vector<wstring> split(wstring str, wstring delim,int cnt=0);
	static vector<string> split(string str, string delim,int cnt=0);
	static wstring genmes(wstring pat);
	static wstring trim(wstring);
	static vector<masterRecord> recordsInit(string fnameMask,string encoding);
	static int query(vector<masterRecord> core,wstring id,wstring text,masterRecord* m);
};

#endif