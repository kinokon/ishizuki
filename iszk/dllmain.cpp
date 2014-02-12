#include "dllmain.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}




 BOOL __cdecl load(HGLOBAL h, long len){
	char* buf=new char[len+1];
	memcpy(buf,h,len);
	buf[len]=0;
	dllpath=string((char*)buf);
	GlobalFree(h);
	delete[] buf;

	 return true;
 }
 BOOL __cdecl unload(){
	 return true;
 }
 HGLOBAL __cdecl request(HGLOBAL h, long *len){
	char* tmpbuf=new char[*len+1];
	memcpy(tmpbuf,h,*len);
	tmpbuf[*len]=0;
	string temp((char*)tmpbuf);
	GlobalFree(h);
	delete[] tmpbuf;
	wstring ret(_T(""));
//SAORI仕様にあわせて分解する
	 vector<string> _temp=iszkutil::split(temp,"\r\n");
	vector<const char*> __args;
	 vector<string> _args;
	 string charset;
	 //一行目を判定
	 if(_temp[0].substr(0,7)=="EXECUTE"){
		 //基本的なパース
		 for(int i=1;i<_temp.size();i++){
			//Charset: を探す
			vector<string> _temp2=iszkutil::split(_temp[i],": ");
			if(_temp2[0]=="Charset")charset=_temp2[1];
			if(_temp2[0].substr(0,8)=="Argument"){
				int p=_temp[i].find(": ");
				_args.push_back(_temp[i].substr(p+2));
			}
		 }
	 }else if(_temp[0].substr(0,11)=="GET Version"){
		 string rtemp=string("SAORI/1.0 200 OK\r\n\r\n");
		 *len=rtemp.size()+1;
		 HGLOBAL buf=GlobalAlloc(GMEM_FIXED,*len);
		 memcpy(buf,rtemp.c_str(),*len);
		 return buf;
	 }else{
		 //知らない
		 string rtemp=string("SAORI/1.0 400 Bad Request\r\n\r\n");
		 *len=rtemp.size()+1;
		 HGLOBAL buf=GlobalAlloc(GMEM_FIXED,*len);
		 memcpy(buf,rtemp.c_str(),*len);
		 return buf;
	 }



	 /*ここから本題*/
	vector<wstring> params;
	//DLL method ID input
	//METHOD="GENMES"/"TALK"
	for(int i=0;i<_args.size();i++){
		params.push_back(iszkutil::s2w(_args[i],charset));
	}




	vector<masterRecord> core=TALKcore[params[0]];
	if(core.size()==0){
			vector<masterRecord> core=iszkutil::recordsInit(dllpath+_args[0]+"_@@.txt","UTF-8");
			TALKcore[params[0]]=core;
	}
		masterRecord m;

		if(iszkutil::query(core,params[1],params[2],&m)!=-1){
		if(m.Message!=_T("")){
			 m.Message=iszkutil::genmes(m.Message);
             m.replaceMessage(_T("hit"),m.hits);
             m.replaceMessage(_T("name"),params[1]);
             m.replaceMessage(_T("text"),params[2]);
			 ret=iszkutil::trim(m.Message);
		}
		}

	if(ret.empty()){
		temp=string("SAORI/1.0 204 No Content\r\nCharset: ")+charset+string("\r\n\r\n");
	}else{
		temp=string("SAORI/1.0 200 OK\r\nResult: ")+iszkutil::w2s(ret,charset)+string("\r\nCharset: ")+charset+string("\r\n\r\n");
	}
	*len = temp.size()+1;
	 HGLOBAL buf=GlobalAlloc(GMEM_FIXED,*len);
	 memcpy(buf,temp.c_str(),*len);
	 return buf;
	

 }