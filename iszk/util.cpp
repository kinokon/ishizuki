#include "util.h"

wstring iszkutil::s2w(string in,string cs){
	UINT csint;
	if(cs=="Shift_JIS"){
		csint=932;
	}else if(cs=="ISO-2022-JP"){
		csint=50221;
	}else if(cs=="EUC-JP"){
		csint=51932;
	}else if(cs=="UTF-8"){
		csint=65001;
	}else{
		csint=CP_ACP;
	}
	const int nSize = MultiByteToWideChar( csint, 0, (LPCSTR)in.c_str(), -1, NULL, 0 );
    TCHAR* buff = new TCHAR[ nSize +1 ];
    MultiByteToWideChar( csint, 0, (LPCSTR)in.c_str(), -1, (LPWSTR)buff, nSize );
	wstring ret(buff);
	delete[] buff;
	return ret;
}

string iszkutil::w2s(wstring in,string cs){
	UINT csint;
	if(cs=="Shift_JIS"){
		csint=932;
	}else if(cs=="ISO-2022-JP"){
		csint=50221;
	}else if(cs=="EUC-JP"){
		csint=51932;
	}else if(cs=="UTF-8"){
		csint=65001;
	}else{
		csint=CP_ACP;
	}
	const int nSize = WideCharToMultiByte( csint, 0, (LPCWSTR)in.c_str(), -1, NULL, 0, NULL, NULL );	 
    BYTE* buff = new BYTE[ nSize +1  ];
	WideCharToMultiByte( csint, 0, (LPCWSTR)in.c_str(), -1, (LPSTR)buff, nSize, NULL, NULL );
	string ret=string((char*)buff);
	delete[] buff;
	return ret;
}


vector<wstring> iszkutil::split( wstring s, wstring c ,int cnt)
{
	vector<wstring> ret;
	for( int i=0, n; i <= s.length(); i=n+1 ){

		n = s.find_first_of( c, i );
		if( n == wstring::npos ) n = s.length();
		wstring tmp = s.substr( i, n-i );
		ret.push_back(tmp);
	}
	return ret;
}

vector<string> iszkutil::split( string s, string c ,int cnt)
{
	vector<string> ret;
	for( int i=0, n; i <= s.length(); i=n+1 ){

		n = s.find_first_of( c, i );
		if( n == string::npos ) n = s.length();
		string tmp = s.substr( i, n-i );
		ret.push_back(tmp);
	}
	return ret;
}


long iszkutil::rand(long min,long max){
	return std::rand()%(max-min+1)+min;
}

wstring iszkutil::genmes(wstring in){
    wregex p(_T("\\(([^()]+,[0-9]+(?:\\|[^()]+,[0-9]+)*)\\)(L{0,1})(?:\\{([0-9,]+)\\}){0,1}"));
				
	vector<vector<wstring>> retval;
	wsmatch result;
	while(regex_search(in,result,p)){
			vector<wstring> group;
			for(int i=0;i<result.size();i++){
				group.push_back(result[i]);
			}
			int lp=1;
			if(group.size()>=4 && group[3].length()>0){
				vector<wstring> cnt=split(group[3],_T(","));
				lp=_wtoi(cnt[0].c_str());
				if(cnt.size()==2)lp=rand(lp,_wtoi(cnt[1].c_str()));
			}
			wstring rep;
			wstring rep_tmp;
			for(int i=0;i<lp;i++){
				if(i==0 || group[2]==_T("")){
					vector<wstring> intxt=split(group[1],_T("|"));
					int h=0;
                    int rnd_point=rand(1,100);
					for(int j=0;j<intxt.size();j++){
						vector<wstring> intxt2=split(intxt[j],_T(","));
                        int l=h+1;
                        h=l+_wtoi(intxt2[1].c_str())-1;
                        if(rnd_point>=l && rnd_point<=h){
							rep_tmp=intxt2[0];
                        }
					}
				}
				rep=rep+rep_tmp;
			}
			std::regex_constants::match_flag_type fonly = std::regex_constants::format_first_only; 
			in=regex_replace(in,p,rep,fonly);	
	}
	return in;			
}

wstring iszkutil::trim(wstring in)
{
wstring ret;
wregex p(_T("^[ \\t\\r\\n]*(.*)[ \\t\\r\\n]*$"));
ret=regex_replace(in,p,wstring(_T("$1")));
return ret;
}


vector<masterRecord> iszkutil::recordsInit(string fnameMask,string encoding){
	regex mask("@@");
	string fname=regex_replace(fnameMask,mask,string("list"));
    vector<masterRecord> core;
	ifstream ifs(fname);
	string s;
	while(ifs && getline(ifs,s)){
		masterRecord tmp(s2w(s,encoding));
        tmp.addMessageFromFile(encoding,regex_replace(fnameMask,mask,w2s(tmp.file,encoding)));
        core.push_back(tmp);
    }
	return core;
}

int iszkutil::query(vector<masterRecord> core,wstring id,wstring text,masterRecord* m){
                for(int i=0;i<core.size();i++){
                        if(core[i].checkUser(id) && core[i].matchText(text) && iszkutil::rand(1,100)<=core[i].rate){
                                if(core[i].chooseMessage()){
									
									*m= core[i];
									return 1;
						}

                        }
                }
				return 0;
        }
