
#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>
#include "masterRecord.h"
#include "util.h"
#include <map>
//#include "lua.hpp"

string dllpath;
//vector<masterRecord> RANDOMcore;
//vector<masterRecord> TALKcore;
map<wstring,vector<masterRecord>> TALKcore;
extern "C" __declspec(dllexport) BOOL __cdecl load(HGLOBAL h, long len);
extern "C" __declspec(dllexport) BOOL __cdecl unload();
extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len);

