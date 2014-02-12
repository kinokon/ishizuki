#include <Windows.h>
#include <vector>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>

#include <tchar.h>

using namespace std;

#if !defined(ISZKUTIL_H)
#include "util.h"
#endif

#if !defined(MRECORD_H)
#define MRECORD_H

class masterRecord{


private:
	vector<wstring> message;
	vector<int> messageRate;
	int messageRateMax;
	wstring key;
  	wregex pat;

public:
	int rate;
	vector<wstring> users;
	vector<wstring> ignore_users;
	wstring file;
	vector<wstring> hits;
	wstring Message;
	masterRecord(){};
	masterRecord(wstring s);
	~masterRecord(){};
	virtual int addMessage(wstring msg,int mrate=1);
	virtual int addMessageFromFile(string encoding,string fname,int mrate=1);
	virtual void clearMessage();
	virtual bool chooseMessage(int max);
	virtual bool chooseMessage();
	virtual bool matchText(wstring text);
	virtual bool checkUser(wstring user);
	virtual void replaceMessage(wstring key,vector<wstring> vals);
	virtual void replaceMessage(wstring key,wstring val);

};

#endif