#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <map>
#include <cstring>
#include <fstream>
#include <cctype>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
using namespace std;
class email_data{
    public:
        string from;
        string to;
        string date;    //YYYYMMDDhhmm
        int MID;
        unordered_set<string> subcon;
	int alph_num;
};
int month_index(string month){
    map<string,int>months{
        { "January", 1 },
        { "February", 2 },
        { "March", 3 },
        { "April", 4 },
        { "May", 5 },
        { "June", 6 },
        { "July", 7 },
        { "August", 8 },
        { "September", 9 },
        { "October", 10 },
        { "November", 11 },
        { "December", 12 }
    };
    const auto iter=months.find(month);
    if(iter!=months.cend())
        return iter->second;
    return -1;
}
string lowercase(string str){
    char *cstr=new char[str.size()+1];
    strcpy(cstr,str.c_str());
    for(int i=0;i<str.size();i++){
	if(cstr[i]<='Z' && 'A'<=cstr[i])
		cstr[i]=cstr[i]+32;
    }
    string lowerstr(cstr);
    delete [] cstr;
    return lowerstr;
}
void tokenize(string others,unordered_set<string>& subcon,int *cnt_len){
	int k=0;
	for(int si=0;si<others.size();si++){
		if(!isalnum(others[si])){
			string tmp;
			tmp.assign(others,k,si-k);
			if(tmp!=""){
				tmp=lowercase(tmp);
				subcon.insert(tmp);
				*cnt_len+=tmp.size();
			}
			k=si+1;
		}
	}
	string tmp;
	tmp.assign(others,k,others.size()-k);
	if(tmp!=""){
		tmp=lowercase(tmp);
		subcon.insert(tmp);
		*cnt_len+=tmp.size();
	}
}
void add(string abspath,unordered_map<int,email_data>& all_email,map<int,set<int>>& len_map,unordered_map<string,set<int>>& from_map,unordered_map<string,set<int>>& to_map,map<string,set<int>>& time_map){
	email_data this_email;
    ifstream file;
    char *cname=new char[abspath.length()+1];
    strcpy(cname,abspath.c_str());
    file.open(cname);
    if(!file.is_open())
	    cout<<"failed"<<endl;
    string others;
    file>>others;
    file>>this_email.from;
    this_email.from=lowercase(this_email.from);
    //cout<<this_email.from<<endl;
    file>>others;
    string date;
    int day;
    file>>day;
    if(day<10){
        date=to_string(day);
        date="0"+date;
    }
    else
        date=to_string(day);
    string month;
    file>>month;
    int month_num=month_index(month);
    date=to_string(month_num)+date;
    if(month_num<10)
        date="0"+date;
    int year;
    file>>year;
    date=to_string(year)+date;
    file>>others;
    string timing;
    string hour;
    string minute;
    file>>timing;
    hour.assign(timing,0,2);
    minute.assign(timing,3,2);
    date=date+hour+minute;
    //cout<<date<<endl;
    this_email.date=date;
    file>>others;
    int mid;
    file>>mid;
    this_email.MID=mid;
    unordered_map<int,email_data>::iterator mid_it;
    mid_it=all_email.find(mid);
    if(mid_it!=all_email.end()){
	cout<<"-"<<endl;
	return;
    }
    cout<<all_email.size()+1<<endl;

    //time_map
    map<string,set<int>>::iterator tmit;
    tmit=time_map.find(this_email.date);
    if(tmit==time_map.end()){
	set<int> newset;
	newset.insert(mid);
	time_map[this_email.date]=newset;
    }
    else{
    	set<int> curr_time=tmit->second;
    	curr_time.insert(mid);
    	tmit->second=curr_time;
    }

    file>>others;
    //subject can have space
    string subject;
    file>>subject;
    if(subject=="To:"){
	file>>this_email.to;
    this_email.to=lowercase(this_email.to);
    }
    else{
	int cnt_len=0;
    	while(subject!="To:"){	
		tokenize(subject,this_email.subcon,&cnt_len);
		file>>subject;
	}
    	file>>this_email.to;
        this_email.to=lowercase(this_email.to);	
    }

    //cout<<this_email.to<<endl;
    file>>others;
    file>>others;
    int cnt_len=0;
    while(!file.eof()){
	tokenize(others,this_email.subcon,&cnt_len);
	file>>others;
    }
    this_email.alph_num=cnt_len;
    
    /*cout<<"len:"<<this_email.alph_num<<endl;
    for(auto it=this_email.subcon.begin();it!=this_email.subcon.end();++it){
	cout<<*it<<endl;
    }*/
    
    //map mid-email
    all_email[mid]=this_email;
    
    //process len_map
    map<int,set<int>>::iterator lit;
    cnt_len=this_email.alph_num;
    mid=this_email.MID;
    lit=len_map.find(cnt_len);
    if(lit==len_map.end()){
	set<int> newset;
	newset.insert(mid);
	len_map[cnt_len]=newset;
    }
    else{
    	set<int> curr_len=lit->second;
    	curr_len.insert(mid);
    	lit->second=curr_len;
    }

    //from and to
    unordered_map<string,set<int>>::iterator fit;
    fit=from_map.find(this_email.from);
    if(fit==from_map.end()){
	set<int> newset;
	newset.insert(mid);
	from_map[this_email.from]=newset;
    }
    else{
    	set<int> curr_from=fit->second;
    	curr_from.insert(mid);
    	fit->second=curr_from;
    }
    unordered_map<string,set<int>>::iterator tit;
    tit=to_map.find(this_email.to);
    if(tit==to_map.end()){
	set<int> newset;
	newset.insert(mid);
	to_map[this_email.to]=newset;
    }
    else{
    	set<int> curr_to=tit->second;
    	curr_to.insert(mid);
    	tit->second=curr_to;
    }
    /*
    cout<<*(time_map[this_email.date].begin())<<endl;
    cout<<*(to_map[this_email.to].begin())<<endl;
    cout<<len_map.begin()->first<<endl;
    */

    file.close();
    delete [] cname;
}
void remove(int mid,unordered_map<int,email_data>& all_email,map<int,set<int>>& len_map,unordered_map<string,set<int>>& from_map,unordered_map<string,set<int>>& to_map,map<string,set<int>>& time_map){
	unordered_map<int,email_data>::iterator it;
	it=all_email.find(mid);
	if(it==all_email.end())
		cout<<"-"<<endl;
	else{	
		email_data this_email=all_email[mid];
		int rm_len=this_email.alph_num;
		set<int> len_set=len_map[rm_len];
		len_set.erase(mid);
		if(len_set.empty())
			len_map.erase(rm_len);
		else
			len_map[rm_len]=len_set;
		string rm_from=this_email.from;
		set<int> from_set=from_map[rm_from];
		from_set.erase(mid);
		if(from_set.empty())
			from_map.erase(rm_from);
		else
			from_map[rm_from]=from_set;

		string rm_to=this_email.to;
		set<int> to_set=to_map[rm_to];
		to_set.erase(mid);
		if(to_set.empty())
			to_map.erase(rm_to);
		else
			to_map[rm_to]=to_set;

		string rm_time=this_email.date;
		set<int> time_set=time_map[rm_time];
		time_set.erase(mid);
		if(time_set.empty())
			time_map.erase(rm_time);
		else
			time_map[rm_time]=time_set;

		all_email.erase(mid);
		cout<<all_email.size()<<endl;
	}	
}
void longest(map<int,set<int>>& len_map){
	if(len_map.empty()){
		cout<<"-"<<endl;
		return;
	}
	int max=prev(len_map.end())->first;
	int mid=*(prev(len_map.end())->second.begin());
	cout<<mid<<" "<<max<<endl;
}
int precedence(string op){
	if(op=="!")
		return 9;
	if(op=="&")
		return 4;
	if(op=="|")
		return 2;
	else return -1;
}
void in_to_postfix(string infix,vector<string>& postfix){
    int j=0;
    int n=infix.size();
    string tmp;    string tmpop;
    stack<string> op;
    for(int i=0;i<n;i+=j){
        j=0;
        if(isalnum(infix[i])){
            while( i+j<n && isalnum(infix[i+j]) )
				j++;
			tmp.assign(infix,i,j);
            tmp=lowercase(tmp);
			postfix.push_back(tmp);
        }
        else{
			j++;
			if(infix[i]=='('){
				tmpop.assign(infix,i,1);
				op.push(tmpop);
			}
			else if(infix[i]==')'){
				while(op.top()!="("){
					postfix.push_back(op.top());
					op.pop();
				}op.pop();			
			}
			else{
				tmpop.assign(infix,i,1);
				while(!op.empty() && precedence(op.top())>=precedence(tmpop) ){
					postfix.push_back(op.top());
					op.pop();
				}op.push(tmpop);
			}
		}
    }
    while(!op.empty()){
		postfix.push_back(op.top());
		op.pop();
	}
	
	/*for(int i=0;i<postfix.size();i++){
        cout<<postfix[i]<<" ";
	}cout<<endl;*/
    
}
/*vector<int> union_t(vector<int>& time_result,set<int>& curr){
	vector<int> result(time_result.size()+curr.size());
	vector<int>::iterator it;
	it=set_union(time_result.begin(),time_result.end(),curr.begin(),curr.end(),result.begin());
	return result;
}*/
void t_range(string stday,string enday,map<string,set<int>>& time_map,set<int>& time_result){
    map<string,set<int>>::iterator itlow,itup;
    itlow=time_map.lower_bound(stday);
    //cout<<"strtt"<<*((itlow->second).begin())<<endl;
    itup=time_map.upper_bound(enday);
    /*if(itup==time_map.end())
        cout<<"endtime"<<endl;*/
    for(auto it=itlow;it!=itup;++it){
        set<int> curr=it->second;
        time_result.insert(curr.begin(),curr.end());
    }
}
/*unordered_set<int> intersect(unordered_set<int>& a,unordered_set<int>& b){
    unordered_set<int> result;
    if(a.size()<b.size()){
        for(auto it=a.begin();it!=a.end();++it){
            if(b.find(*it)!=b.end()){
                result.insert(*it);
            }
        }
    }
    else{
        for(auto it=b.begin();it!=b.end();++it){
            if(a.find(*it)!=a.end()){
                result.insert(*it);
            }
        }
    }
    return result;
}*/
int whose_size(set<int>& f,set<int>& t,set<int>& time){
    if(f.size()>=t.size()&&f.size()>=time.size())
        return 1;
    if(t.size()>=f.size()&&t.size()>=time.size())
        return 2;
    return 3;
}
/*set<int>::iterator my_intersection(set<int>::iterator f1,set<int>::iterator l1,set<int>::iterator f2,set<int>::iterator l2,set<int>::iterator re){
	while(f1!=l1 && f2!=l2){
		if(*f1<*f2) ++f1;
		else if(*f2<*f1) ++f2;
		else{
			*re=*f1;
			++re;	++f1;	++f2;
		}
	}
	return re;
}*/
vector<int> intersect_f_t_time(set<int>& f_result,set<int>& t_result,set<int>& time_result,bool fti[3]){
	/*int size=whose_size(f_result,t_result,time_result);
    set<int>::iterator itf,ite;
    if(size==1){
        itf=f_result.begin();   ite=f_result.end();  
    }
    if(size==2){
        itf=t_result.begin();   ite=t_result.end();  
    }
    if(size==3){
        itf=time_result.begin();   ite=time_result.end();  
    }
	set<int> result(itf,ite);*/
    vector<int> result;
    if(fti[0]==true){
        for(auto it=f_result.begin();it!=f_result.end();++it)
	        result.push_back(*it);
        //cout<<"f_result intersect:"<<*(result.begin())<<endl;
    }
    if(fti[1]==true){
        if(fti[0]==true){
		vector<int>::iterator it;
		it=set_intersection(result.begin(),result.end(),t_result.begin(),t_result.end(),result.begin());
		result.resize(it-result.begin());
	    }
        else{
        for(auto it=t_result.begin();it!=t_result.end();++it)
	        result.push_back(*it);
	    }   
    }
    if(fti[2]==true){
        if(fti[0]==true || fti[1]==true){
		vector<int>::iterator it;
		//set<int> tmp=result;
		it=set_intersection(result.begin(),result.end(),time_result.begin(),time_result.end(),result.begin());
		//result=tmp;
		result.resize(it-result.begin());
        }
        else{
            for(auto it=time_result.begin();it!=time_result.end();++it)
	        result.push_back(*it);
        }
        //cout<<"time_result intersect:"<<*(result.begin())<<endl;
    }
    /*for(auto it=result.begin();it!=result.end();++it){
        cout<<*it<<" ";
    }cout<<endl;*/
    return result;
}
int cal1(int a,string op){
    int ans=(a==1)?0:1;
    return ans;
}
int cal2(int a,int b,string op){
    if(op=="|"){
	if(a==1||b==1)
		return 1;
    }
    if(op=="&"){
        if(a==1&&b==1)
	    return 1;
    }
    return 0;
}
void arithmetic(stack<int>& opnd,string op){
	int a,b;
	if(op=="!"){
		a=opnd.top(); opnd.pop();
		opnd.push(cal1(a,op));
	}
	else{
		b=opnd.top();	opnd.pop();
		a=opnd.top();	opnd.pop();
		opnd.push(cal2(a,b,op));
	}
}
int expression_fit(int mid,vector<string> postfix,email_data& this_email){
    for(int i=0;i<postfix.size();i++){
        if(isalnum(postfix[i][0])){
            if(this_email.subcon.find(postfix[i])!=this_email.subcon.end())
                postfix[i]="1";
            else
                postfix[i]="0";
        }
    }
    /*for(int i=0;i<postfix.size();i++)
        cout<<postfix[i]<<" ";
    cout<<endl;*/
    stack<int> opnd;
	int tmpopnd;
	for(int i=0;i<postfix.size();i++){
		if(isdigit(postfix[i][0])){
			tmpopnd=stoi(postfix[i]);
			opnd.push(tmpopnd);
		}
		else{
			arithmetic(opnd,postfix[i]);
		}
	}
    return opnd.top();
}
vector<int> collect_expression(vector<int>& result,vector<string>& postfix,unordered_map<int,email_data>& all_email){
    vector<int> ans;
    for(auto it=result.begin();it!=result.end();++it){
        int mid=*it;
        if(expression_fit(mid,postfix,all_email[mid])){
            ans.push_back(mid);
        }
    }
    return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
    string command;
    unordered_map<int,email_data> all_email;
    /*
    string path;
    cin>>path;
    */
    map<int,set<int>> len_map;
    unordered_map<string,set<int>> from_map;
    unordered_map<string,set<int>> to_map;
    map<string,set<int>> time_map;
    
    while(cin>>command){
        switch(command[0]){
            case 'a':{
                //cout<<"add"<<endl;
		string abspath;
                cin>>abspath;
                add(abspath,all_email,len_map,from_map,to_map,time_map);
                break;
            }
            case 'r':{
                //cout<<"remove"<<endl;
			     int m_id;
		        cin>>m_id;
                remove(m_id,all_email,len_map,from_map,to_map,time_map);
                break;
            }
            case 'l':{
                //cout<<"longest"<<endl;
			     longest(len_map);
                break;
            }
            case 'q':{
                //cout<<"query"<<endl;
			     string q;
                cin>>q;
                vector<int> result;
                set<int> f_result;
                set<int> t_result;
                set<int> time_result;
                bool fti[3];
                fti[0]=false;   fti[1]=false;   fti[2]=false;
                while(q[0]=='-'){
                    if(q[1]=='f'){
                        fti[0]=true;
                        string tarfrom;
                        int end=q.size();
                        tarfrom=q.substr(3,end-4);
                        tarfrom=lowercase(tarfrom);
                        //cout<<tarfrom<<endl;
			if(from_map.find(tarfrom)!=from_map.end())
                        	f_result=from_map[tarfrom];
                        //cout<<"f_result:"<<*(f_result.begin())<<endl;
                    }
                    else if(q[1]=='t'){
                        fti[1]=true;
                        string tarto;
                        int end=q.size();
                        tarto=q.substr(3,end-4);
                        tarto=lowercase(tarto);
                        //cout<<tarto<<endl;
			if(to_map.find(tarto)!=to_map.end())
                        	t_result=to_map[tarto];
                    }
                    else if(q[1]=='d'){
                        fti[2]=true;
                        string stday;
                        string enday;
                        int found=q.find("~");
                        if(found==2){
                            stday="000000000000";
                            if(found+1==q.size())
                                enday="999999999999";
                            else
                                enday=q.substr(found+1);
                        }
                        else if(found==q.size()-1){
                            enday="999999999999";
                            stday=q.substr(2,12);
                        }
                        else{
                            stday=q.substr(2,12);
                            enday=q.substr(found+1);
                        }
                        //cout<<stday<<" "<<enday<<endl;
                        t_range(stday,enday,time_map,time_result);
                    }
                    cin>>q;
                }
                result=intersect_f_t_time(f_result,t_result,time_result,fti);
                vector<string> postfix;
                in_to_postfix(q,postfix);
                //postfix process
                vector<int> ans;
                ans=collect_expression(result,postfix,all_email);
                if(ans.size()==0)
                    cout<<"-"<<endl;
                else{
                    for(int ai=0;ai<ans.size();ai++){
                        if(ai==ans.size()-1)
                            cout<<ans[ai]<<endl;
                        else
                            cout<<ans[ai]<<" ";
                    }
                }
                break;
            }
        }
    }
    
    return 0;
}
