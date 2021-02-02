#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
unordered_map<string,string> monthNameToNumber;
vector<int> messageIDs;
vector<string> keywords;
vector<string> froms;
vector<string> tos;
ll mndate, mxdate;
unordered_set<int> added;

void init()
{
	//build monthNameToNumber
	monthNameToNumber.insert(pair<string,string>("January","01"));
	monthNameToNumber.insert(pair<string,string>("February","02"));
	monthNameToNumber.insert(pair<string,string>("March","03"));
	monthNameToNumber.insert(pair<string,string>("April","04"));
	monthNameToNumber.insert(pair<string,string>("May","05"));
	monthNameToNumber.insert(pair<string,string>("June","06"));
	monthNameToNumber.insert(pair<string,string>("July","07"));
	monthNameToNumber.insert(pair<string,string>("August","08"));
	monthNameToNumber.insert(pair<string,string>("September","09"));
	monthNameToNumber.insert(pair<string,string>("October","10"));
	monthNameToNumber.insert(pair<string,string>("November","11"));
	monthNameToNumber.insert(pair<string,string>("December","12"));
}
void print_string(string s)
{
	for(char c:s) putchar(c);
	putchar('\n');
}
void process_keyword(string str)
{
	string keyword;
	int cur=0;
	for(int i=0;i<str.length();i++){
		if((str[i]>'z' || str[i]<'a') && (str[i]>'Z' || str[i]<'A') && (str[i]>'9' || str[i]<'0')){
			keyword=str.substr(cur,i-cur);
			if(keyword.size()){
				keywords.push_back(keyword);
				//cout << keyword << " " << keyword.length() << "\n";
			}
			cur=i+1;
		}
	}
	if(cur<str.length())
	{
		keyword=str.substr(cur,str.length()-cur);
		if(keyword.size()){
			keywords.push_back(keyword);
			//cout << keyword << " " << keyword.length() << "\n";
		}
	}
}
void read_mail(string filename)
{
	//cout << filename << "\n";
	ifstream myfile(filename);
	string redundant;

	myfile >> redundant;
	string from;
	myfile >> from;
	froms.push_back(from);

	myfile >> redundant;
	int day;
	myfile >> day;
	string sday;
	if(day<10){
		sday+='0';
		sday+=day+'0';
	} else {
		sday=to_string(day);
	}
	string month;
	myfile >> month;
	month = monthNameToNumber[month];
	string year;
	myfile >> year;
	myfile >> redundant;
	string time;
	myfile >> time;
	string hour;
	int pos=time.find(':');
	hour=time.substr(0,pos);
	string minute;
	minute=time.substr(pos+1,time.length()-pos);
	//cout << hour << ":" << minute << "\n";
	//cout << stoll(year+month+sday+hour+minute) << "\n";
	mndate=min(mndate,stoll(year+month+sday+hour+minute));
	mxdate=max(mxdate,stoll(year+month+sday+hour+minute));
	//cout << mndate << "," << mxdate << "\n";

	myfile >> redundant;
	int messageID;
	myfile >> messageID;

	myfile >> redundant;
	string subject;
	getline(myfile,subject);
	process_keyword(subject);

	myfile >> redundant;
	string to;
	myfile >> to;
	tos.push_back(to);

	myfile >> redundant;
	string content;
	getline(myfile, content);
	while(getline(myfile, content)){
		process_keyword(content);
	}

	myfile.close();
}
string generate_expression()
{
	if((rand()%100)/100.0<0.2){
		int r=rand()%keywords.size();
		return keywords[r];
	}
	int numberOfKeywords = rand()%20;
	numberOfKeywords++;
	stack<string> stk;
	for(int i=0;i<numberOfKeywords;i++){
		int r=rand()%keywords.size();
		stk.push(keywords[r]);
	}
	while(stk.size()>1){
		if(stk.top().length()>1000) break;
		int r=rand()%100;
		if(r/100.0<0.15){
			string s=stk.top(); stk.pop();
			stk.push(string("!")+s);
		} else if(r/100.0<0.3){
			string s=stk.top(); stk.pop();
			stk.push(string("(")+s+string(")"));
		} else if(r/100.0<0.65){
			string a=stk.top(); stk.pop();
			string b=stk.top(); stk.pop();
			stk.push(a+string("&")+b);
		} else {
			string a=stk.top(); stk.pop();
			string b=stk.top(); stk.pop();
			stk.push(a+string("|")+b);
		}
	}
	return stk.top();
}
int main()
{
	srand(time(NULL));
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	init();
	mxdate=0;
	mndate=999999999999ll;
	//cout << mndate << "," << mxdate << "\n";
	string filename_base("./test_data/mail");
	//cout << "yes\n";
	for(int i=1;i<10000;i++){
		//cout << filename_base+to_string(i) << "\n";
		read_mail(filename_base+to_string(i));
	}
	//for(string s: keywords) cout << s << endl;
	//cout << mndate << "," << mxdate << "\n";
	for(int kase=0;kase<1500;kase++){
		int op=rand()%100;
		if(op/100.0<0.25){
			int file=rand()%9999;
			file++;
			cout << "add ./test_data/mail" << file << "\n";
			added.insert(file);
		} else if(op/100.0<0.5){
			int r=rand()%100;
			if(r/100.0<0.8 && added.size()>0){
				int rmv=rand()%added.size();
				auto it=added.begin();
				for(int cnt=0;it!=added.end() && cnt<rmv;) it++;
				if(it==added.end()) cout << "remove " << (rand()%9999)+1 << "\n";
				else{
					cout << "remove " << *it << "\n";
					added.erase(it);
				}
			} else {
				int rmv=(rand()%9999)+1;
				cout << "remove " << rmv << "\n";
			}
		} else if(op/100.0<0.75){
			cout << "longest\n";
		} else {
			cout << "query ";
			int r=rand()%100;
			if(r/100.0<0.5){
				cout << "-f\"";
				int from=rand()%froms.size();
				cout << froms[from] << "\" ";
			}
			r=rand()%100;
			if(r/100<0.5){
				cout << "-t\"";
				int to=rand()%tos.size();
				cout << tos[to] << "\" ";
			}
			r=rand()%100;
			if(r/100<0.5){
				cout << "-d";
				int sz=(int)mxdate-mndate;
				int a=rand()%sz, b=rand()%sz;
				if(a>b) swap(a,b);
				int r=rand()%100;
				if(r>0.3){
					cout << 1ll*(mndate+a);
				}
				cout << "~";
				r=rand()%100;
				if(r>0.3){
					cout << 1ll*(mndate+b);
				}
				cout << " ";
			}
			cout << generate_expression() << "\n";
		}
	}
}
