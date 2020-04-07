#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define ll long long int
#define ffrr(i,a,b) for(i=a;i<=b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#define mpp(a,b) make_pair(a,b)
#define pb push_back
#define bug(X) cerr<< #X << " = " << X << endl

using namespace std;

vector<string> words;
vector<char> letters;

string Cin, letter, word, CCopy, Cout;
map<char,char> mapping;

void normalize()
{
	char cur, cur2;
	int i, j;

	map<char,char>::iterator it;

	for(it=mapping.begin();it!=mapping.end();it++)
	{
		cur=it->first;
		cur2=it->second;
		ffr(j,0,Cin.size())
		{
			if(Cin[j]==cur)
			{
				Cin[j]=cur2;
			}
		}
	}
}

void fit(string s)
{
	int len=s.size(), sz=Cin.size(), i;
	i=len;
	map<char,char> temp;
	map<char,char>::iterator itt;
	while(i<sz)
	{
		int j=i-len, k;
		//cout << j << ' ';
		temp.clear();
		ffr(k,0,len)
		{
			if(Cin[j+k]<='Z')
			{

				if( temp.find(Cin[j+k]) == temp.end() )
				{
					temp[Cin[j+k]]=s[k];
				}
				else
				{
					if(temp[Cin[j+k]] != s[k]) break;
				}
			}
			else
			if(Cin[j+k]!=s[k]) break;

		}
		if(k==len)
		{
			//satisfied
			ffr(k,0,len)
			{
				if(Cin[j+k]>'Z') continue;
				mapping[Cin[j+k]]=s[k];
			}

			i+=len;
			normalize();
		}
		else i++;
	}
}

void word_match()
{
	int i;
	ffr(i,0,words.size())
	{
		fit(words[i]);
		//cout << words[i] << ".\n";
	}
}

map<char,char> map_inverse;

void mapping_show()
{
	cout << "Mapping:\n";
	map<char,char>::iterator it;
	vector< pair<char, char> > vv;
	for(it=mapping.begin();it!=mapping.end();it++)
	{
		//cout << it->first << " -> " << it->second << endl;
		vv.pb(mpp(it->second,it->first));
		map_inverse[it->second]=it->first;
	}

	sort(vv.begin(),vv.end());
	int i;
	ffr(i,0,vv.size())
	{
		cout << vv[i].first << " = " << vv[i].second << endl;
	}
}

bool cmp(string a, string b)
{
	return a.size()>b.size();
}

int nf=0, er=0;
void encode()
{
	int i;
	ffr(i,0,Cin.size())
	{
		if(Cin[i]<='Z') {nf++; Cout[i]=Cin[i]; continue;}
		Cout[i]=map_inverse[Cin[i]];

		if(Cout[i]!=CCopy[i]) er++;
	}
}

int main()
{
	//freopen("substitution-18.txt", "r", stdin);

	getline(cin,Cin);
	CCopy=Cin;
	Cout=Cin;
	letter="";
	while(letter=="")
	getline(cin,letter);
	word="";
	while(word=="")
	getline(cin,word);

	int i;

	char cur;
	ffr(i,0,letter.size())
	{
		if(letter[i]==' ') continue;
		if(letter[i]==',')
		{
			letters.pb(cur); cur=0;
		}
		cur=letter[i];
	}
	letters.pb(cur);

	string curs="";
	ffr(i,0,word.size())
	{
		if(word[i]==' ') continue;
		if(word[i]==',')
		{
			words.pb(curs); curs="";
		}
		else
		curs+=word[i];
	}
	words.pb(curs);
	sort(words.begin(),words.end(),cmp);

	int textlen=Cin.size() ;
	map<char,int> mp;
	ffr(i,0,textlen)
	{
		mp[Cin[i]]++;
	}
	vector< pair<int,char> > v;
	map<char,int>::iterator it;
	int j;
	for(it=mp.begin();it!=mp.end();it++)
	{
		v.pb( mpp(it->second,it->first) );
	}

	sort(v.begin(),v.end());
	reverse(v.begin(),v.end());


	ffr(i,0,letters.size())
	{
		cur=v[i].second;
		mapping[cur]=letters[i];
	}

	normalize();

	word_match();


	cout << "\nPlaintext:\n" << Cin << endl;

	mapping_show();

	encode();
	cout << "\nRe-encoded:\n" << Cout << endl;
	cout << "\nNot found: " << nf << ", Error: " << er << ", Accuracy: " << 1.0-er/1.0/Cin.size() << endl;
	//cout << Cin.size() << endl;
	//cout << map_inverse['t'];

	return 0;
}
