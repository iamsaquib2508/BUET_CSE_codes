#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define ll long long int

using namespace std;

#define word_size 5
#define len 10000
#define N 10000

#define pp pair<bool,int>
#define mp make_pair

class data
{
	public:
	string s;
	int stat, val;
	data* next;
	
	static int free, taken, del;
	
	data()
	{
		s="", val=-1, stat=0, next=NULL;
	}
	
	data(string ss, int vv)
	{
		s=ss, val=vv, stat=0, next=NULL;
	}
	data(string ss, int vv, int status)
	{
		s=ss, val=vv, stat=status, next=NULL;
	}
	bool isavailable()
	{
		if(s.size()==0 && val==-1 && stat==free) return true;
		return false;
	}
	bool isdel()
	{
		if(s.size()==0 && val==-1 && stat==del) return true;
		return false;
	}
};

class hashing
{
public:
	int M;
	data lin[len];
	data * chin[len];
	int collision, seq, match;
	double time;
	
	hashing()
	{
		M=len;
		int i;
		ffr(i,0,M)
		{
			chin[i]=NULL;
		}
	}
	/*
	void insert(string s, int t)
	{
		
	}
	pp search(string s, int t)
	{
		
	}
	bool deleting(string s, int t)
	{
		
	}*/
	unsigned long long int hash1(string s)
	{
		unsigned long long int i, sz=s.size(), sum=0;
		int hf1=127, c=1;
		unsigned long long int mod=104729;
		ffr(i,0,sz)
		{
			sum+=(c*(s[i]-'a'));
			if(sum>mod) sum%=mod;
			c*=hf1;
		}
		return (unsigned int) ((sum%mod)% M );
	}
	int hash2(string s)
	{
		unsigned int h = 0;
		int i, sz=s.size();
        ffr(i,0,sz)
		{
            h += s[i];
            h += (h << 10);
            h ^= (h >> 6);
        }

        h += (h << 3);
        h ^= (h >> 11);
        h += (h << 15);

        return h % M;
	}
	int hash3(string s)
	{
		unsigned long long int h = 2166136261;

        for(int i = 0 ; i < s.size() ; i++){
            h = (h * 16777619) ^ s[i];
        }

        return h % N;
	}
	int findhash(string s, int ii)
	{
		if(ii==1) return hash1(s);
		else if(ii==2) return hash2(s);
		else return hash3(s);
	}
};


string gen(int sz)
{
	string s=""; int i;
	ffr(i,0,sz)
	{
		s+=('a'+(unsigned int)(rand())%26);
	}
	//cout << s << endl;
	return s;
}

class linear : public hashing
{
	public:
		
	
	linear(): hashing()
	{
		collision=0, seq=0, time=0 , match=0;
	}
	void insert(string s, int tech)
	{
		pp result = search(s,tech);
		if(!result.first)
		{
			unsigned int ind=findhash(s,tech);
			if(ind!=result.second)
			{
				collision++;
				/*
				cout << "aisi\n";
				while(!lin[ind].isavailable())
				{
					ind++; ind%=M;
				}
				lin[ind] = data(s,seq,data::taken);*/
			
			}
			
			lin[ind] = data(s,seq,data::taken);
			
            seq++;
		}
		//pf("%d ", result.second);
		//cout << s << endl;
		return;
	}
	pp search(string s, int tech)
	{
		clock_t start= clock();
		unsigned int ind=findhash(s,tech);
		unsigned int tmp=ind;
		while(!lin[ind].isavailable())
		{
			if(lin[ind].isdel()) return mp(0,ind);
			else if(lin[ind].s==s) return mp(1,ind);
			ind++; ind%=M;
			if(tmp==ind) break;
		}
		double dd=(clock()-start)/ (double) CLOCKS_PER_SEC;
		time+=dd;
		return mp(0,ind);
	}
	
	bool deleting(string s, int tech)
	{
		pp result = search(s,tech);
		if(!result.first) false;
		else lin[result.second]=data("", -1, data::del);
		return true;
	}
	void run()
	{
		int i, j;
		ffr(i,1,4)
		{
			int kj;
			ffr(kj,0,N)
                lin[kj] = data();
            
            seq = 1;
            collision = 0;
            time = 0.0;
            match=0;
			ffr(j,0,N)
			{
				string st=gen(word_size);
				insert(st,i);
			}
			
			ffr(j,0,N)
			{
				string st=gen(word_size);
				match+=(int) search(st,i).first;
			}
			pf("Hash Function %d:  Collision %d  Search Time %f Matching %d\n\n",i,collision,time, match);
			//int aa; cin >> aa;
		}
	}
};

class chaining : public hashing
{
	public:
	void insert(string s, int tech)
	{
		bool res = search(s,tech);
            if(!res)
			{
            
                unsigned int ind = findhash(s,tech);

                if(chin[ind] == NULL){
                    chin[ind] = new data(s,seq);
                }
                else{
                    data *hashOb = new data(s,seq);
                    hashOb -> next = chin[ind];
                    chin[ind] = hashOb;
                    collision++;
                }
                seq++;
            }
	}
	
	bool search(string s, int tech)
	{
		clock_t start = clock();

            unsigned int ind = findhash(s,ind);

            data* head = chin[ind];

            while(head != NULL){
                if(head -> s == s){
                    double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
                    time += diff;
                    
                    return true; // found
                }
                head = head -> next;
            }
            double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
            time += diff;
            return false;
	}
	
	bool deleting(string s, int tech)
	{
		unsigned int ind = findhash(s,tech);
            data* head = chin[ind];
            data* prev = NULL;

            while(head != NULL){
                if(head -> s == s) break;
                prev = head;
                head = head -> next;
            }

            if(head == NULL) return false;
                
            if(head == chin[ind]){
                
                chin[ind] = chin[ind] -> next;
                delete head;
                head = NULL;
            }
            else{
                prev -> next = head -> next;
                delete head;
                head = NULL;
            }

            return true;
	}
	void run()
	{
        int i, j;
		ffr(i,1,4)
		{
			for(int h = 0 ; h < N ; h++)
                chin[h] = NULL;
            
            seq = 1;
            collision = 0;
            time = 0.0;
            match=0;
            
			ffr(j,0,N)
			{
				string st=gen(word_size);
				insert(st,i);
			}
			
			ffr(j,0,N)
			{
				string st=gen(word_size);
				
				match+=(int)search(st,i);
			}
			pf("Hash Function %d:  Collision %d  Search Time %f Matchings %d\n\n",i,collision,time, match);
			//int a; cin >> a;
		}
	}
};

int data::free=0;
int data::taken=1;
int data::del=2;


int main()
{
	srand(time(NULL));
	pf("Chaining--------------\n");
	chaining B;
	B.run();
	pf("------------------------------------------\n");
	pf("Linear Probing----------\n");
	linear A;
	A.run();
	
	return 0;
}
