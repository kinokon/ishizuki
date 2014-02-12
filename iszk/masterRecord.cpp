#include "masterRecord.h"





masterRecord::masterRecord(wstring s){
                messageRateMax=0;
				
                vector<wstring>k=iszkutil::split(s,_T("\t"));

				if(k.size()==5){
                        key=k[0];
                        pat=wregex(key);
                        rate=_wtoi(k[1].c_str());
                        users=iszkutil::split(k[2],_T(","));
                        ignore_users=iszkutil::split(k[3],_T(","));
                        file=iszkutil::trim(k[4]);
                }

        }

int masterRecord::addMessage(wstring msg,int mrate){
                if(mrate>0){
                        message.push_back(msg);
                        if(messageRate.size()>0){
                                messageRateMax=messageRate[messageRate.size()-1]+mrate;
                        }else{
                                messageRateMax=mrate;
                        }
                        messageRate.push_back(messageRateMax);
                }
                return messageRateMax;
        }

int masterRecord::addMessageFromFile(string encoding,string fname,int mrate){
                int ret=0;
				ifstream ifs(fname);
				string buf;
				while(ifs && getline(ifs,buf)){
                   ret=addMessage(iszkutil::s2w(buf,encoding),mrate);
                }
		return ret;
}


void masterRecord::clearMessage(){
                message.clear();
                messageRate.clear();
        }
bool masterRecord::chooseMessage(int max){
                int p=iszkutil::rand(1,messageRateMax);

                for(int i=0;i<message.size();i++){
                        if(messageRate[i]>=p){
                                Message = message[i];
                                return true;
                        }
                }
                return false;
        }

bool masterRecord::chooseMessage(){
	return chooseMessage(message.size());
}

void masterRecord::replaceMessage(wstring key,vector<wstring> vals){
	for(int i=0;i<vals.size();i++){
		TCHAR buf[16];
		_itow_s(i,buf,16,10);
		wregex rep(wstring(_T("\\{"))+key+wstring(_T(":"))+wstring(buf)+wstring(_T("\\}")));
		Message=regex_replace(Message,rep,vals[i]);
	}
	replaceMessage(key,vals[0]);
}
void masterRecord::replaceMessage(wstring key,wstring val){
	wregex rep(wstring(_T("\\{"))+key+wstring(_T("\\}")));
	Message=regex_replace(Message,rep,val);
}

bool masterRecord::checkUser(wstring username){
                int i;
                boolean hitflag=false;
                for(i=0;i<users.size();i++){
                        if(users[i]==_T("*") || username==users[i]){
                                hitflag=true;
                                break;
                        }
                }
                for(i=0;i<ignore_users.size();i++){
                        if(username==ignore_users[i]){
                                hitflag=false;
                                break;
                        }
                }
                return hitflag;
	
}

bool masterRecord::matchText(wstring in){
	hits.clear();
	wsmatch result;
	if(regex_search(in,result,pat)){
		for(int i=0;i<result.size();i++){
			hits.push_back(result[i]);
		}
		return true;
	}
	return false;
}

