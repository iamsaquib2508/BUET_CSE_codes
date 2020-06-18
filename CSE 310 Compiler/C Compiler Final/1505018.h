#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define mm(a,b) memset((a),(b),sizeof(a))
#define pf printf
#define fpf fprintf
#define pb push_back
#define ll long long int
#define PI 3.141592

using namespace std;


class SymbolInfo
{
    string Type, Name; //type means token type
    string IDtype; //var ara func
    string Datatype; //int float void
    string Rettype; // func
    int arrlen;
    int arrind;
    bool def;


    public:
    SymbolInfo* next;
    string asmcode;
    string asmname;
    bool isarray;
    vector<int> ints; //intarr
    vector<double> floats; //floatarr
    string show;
    vector<string> funcparamlist;

    SymbolInfo(){ next=NULL; Datatype=""; isarray=false;
    }
    SymbolInfo(string name, string type)
    {
        Type=type; Name=name; next=NULL; show=name; isarray=false;
    }
    SymbolInfo(string datatype)
    {
        Datatype=datatype;
        if(datatype=="INT") ints.pb(0);
        else if(datatype=="FLOAT") floats.pb(0);
        next=NULL; isarray=false;
    }

    void setType(string type){ Type=type;}
    void setName(string name){ Name=name;}
    void setDatatype(string datatype){
        Datatype=datatype;
        if(datatype=="INT") ints.pb(0);
        else if(datatype=="FLOAT") floats.pb(0);
    }
    void setIDtype(string idtype){ IDtype=idtype;}
    void setRettype(string rettype){ Rettype=rettype;}
    void setarrlen (int len) {arrlen=len;}
    void setarrind(int ind) {arrind=ind;}
    void setdef() {def=true;}
    bool operator ==(const SymbolInfo b) const { return (Name==b.Name) && (Type==b.Type);
    }
    SymbolInfo operator = (const SymbolInfo &sym){
        this->Name = sym.Name;
        this->Type = sym.Type;
        return *this;
    }

    string getType(){ return Type;}
    string getName(){ return Name;}
    string getIDtype(){ return IDtype;}
    string getDatatype(){ return Datatype;}
    string getRettype(){ return Rettype;}
    int getarrlen() {return arrlen;}
    int getarrind() {return arrind;}
    bool getdef() {return def;}
    string getshow() {return show;}

    void copyfrom(SymbolInfo* orig, SymbolInfo* dest)
    {
        ///*
        if(orig==NULL)
        {
            dest=NULL;
            return;
        }
        //*/
        next=orig->next;
        Type=orig->getType();
        Name=orig->getName();
        IDtype=orig->getIDtype();
        Datatype=orig->getDatatype();
        arrlen=orig->getarrlen();
        arrind=orig->getarrind();
        Rettype=orig->getRettype();
        def=orig->getdef();
        isarray=orig->isarray;
        show=orig->show;
        asmname=orig->asmname;
        asmcode=orig->asmcode;
        int i, sz;

        sz=orig->funcparamlist.size();
        funcparamlist.clear();
        ffr(i,0,sz) funcparamlist.pb(orig->funcparamlist[i]);

        sz=orig->ints.size();
        ints.clear();
        ffr(i,0,sz) ints.pb(orig->ints[i]);

        sz=orig->floats.size();
        floats.clear();
        ffr(i,0,sz) floats.pb(orig->floats[i]);

        show=orig->getshow();
    }
    //78 to 98


};

class ScopeTable
{
    public:
    SymbolInfo **array;
    ScopeTable* ParentScope;
    int id, sz; //id is koto number scopetable, sz is mod
    ScopeTable(ScopeTable* pp, int n, int idd)
	{ sz=n; ParentScope=pp; id=idd; array=new SymbolInfo*[sz]; int i; ffr(i,0,sz) array[i]=NULL;
    //if(id>0) printf("New ScopeTable with id %d created\n", id);
    }
    ScopeTable(int n, int idd)
	{ sz=n; array=new SymbolInfo*[sz]; ParentScope=NULL; id=idd; int i; ffr(i,0,sz) array[i]=NULL;
    //if(id>0) printf("New ScopeTable with id %d created\n", id);
    }

    SymbolInfo* Lookup(string ss, string idtype)
    {
        int ptr=hash(ss), cnt=0;
        SymbolInfo* cur=array[ptr];

        while(1)
        {
            if(cur==NULL) return NULL;
            if(cur->getName()==ss && cur->getIDtype()==idtype) return cur;
            cur=cur->next; cnt++;
        }
    }
    int hash(string ss)
    {
    	ll prm=23, i=0, num=0, sze=ss.size(), mul;
    	mul=prm;
    	while(i<sze)
    	{
    		num+=mul * (i+1) * ss[i];
    		mul*=prm;
    		i++;
		}

        return num%sz;
    }
    bool Insert(SymbolInfo *ss)
    {
        //ensure ss next=null
        int ptr=hash(ss->getName()), cnt;
        SymbolInfo* cur=array[ptr], *prev;
        if(cur==NULL)
        {
            array[ptr]=ss;
			return true;
        }
        if((*cur)==(*ss)) return false;
        prev=cur; cur=cur->next; cnt=1;
        while(1)
        {
            if(cur==NULL)
            {
                prev->next=ss;
        		return true;
            }
            if((*cur)==(*ss)) return false;
            prev=cur; cur=cur->next; cnt++;
        }
    }
    bool Delete(string ss)
    {
        int ptr=hash(ss), cnt;
        SymbolInfo* cur=array[ptr], *prev;
        if(cur==NULL) return false;
        if(cur->getName()==ss)
        {
        	array[ptr]=cur->next; delete cur; return true;
		}
        prev=cur; cur=cur->next; cnt=1;
        while(1)
        {
            if(cur==NULL) return false;
            if(cur->getName()==ss)
            {
            	prev->next=cur->next; delete cur; return true;
			}
            prev=cur; cur=cur->next; cnt++;
        }
    }
    void printscopetable(FILE* logFile)
    {
    	int i;
        fpf(logFile,"ScopeTable #%d\n", id);
    	ffr(i,0,sz)
    	{
	        SymbolInfo* cur=array[i];
		    if(cur==NULL) continue;
		    fpf(logFile,"%d -->", i);
	        while(1)
	        {
	            if(cur==NULL) break;
		    fpf(logFile," <%s:%s> ", cur->getName().c_str(), cur->getIDtype().c_str());
	            cur=cur->next;
	        }
	        fpf(logFile,"\n");
		}
        fpf(logFile,"\n");
	}
	~ScopeTable()
	{
		//fprintf(lg, "ScopeTable with id %d removed\n", id);
        int i;
        ffr(i,0,sz) delete array[i];
		delete[] array;
	}

};

class SymbolTable
{
	public:
	vector<ScopeTable*> v;
	ScopeTable* cur;
	int hashsize, sz;
	SymbolTable(int n){ cur=NULL; sz=0; v.clear(); hashsize=n;
	}

	void EnterScope()
	{
		ScopeTable *niu = new ScopeTable(cur, hashsize, sz);
		cur= niu;
		v.pb(cur); sz++;
	}
	void ExitScope()
	{
		cur=v[sz-1]->ParentScope;

        //delete v[sz-1];
        //v[sz-1]=NULL;

		//sz--;
	}
	bool Insert(SymbolInfo *ss)
	{
		bool ok=cur->Insert(ss);
		if(ok)
        {
	        //PrintThisScopeTable();
        }
        return ok;
	}
	bool Remove(string ss)
	{
		return cur->Delete(ss);
	}
	SymbolInfo* Lookup(string ss, string idtype)
	{
		ScopeTable *now;
		SymbolInfo *te;
		now=cur;
		while(now!=NULL)
		{
			te=now->Lookup(ss,idtype);
			if(te!=NULL) return te;
            else return NULL;
			//now=now->ParentScope;
		}
		//printf("Not found\n");
		return NULL;
	}
    SymbolInfo* Lookupwithparent(string ss, string idtype)
	{
		ScopeTable *now;
		SymbolInfo *te;
		now=cur;
		while(now!=NULL)
		{
			te=now->Lookup(ss,idtype);
			if(te!=NULL) return te;
            //else return NULL;
			now=now->ParentScope;
		}
		//printf("Not found\n");
		return NULL;
	}
	void PrintThisScopeTable(FILE* fi)
	{
		cur->printscopetable(fi);
	}
	void PrintAllScopeTable(FILE* fi)
	{
		ScopeTable *now;
		now=cur;
		while(now!=NULL)
		{
			now->printscopetable(fi);
			now=now->ParentScope;
		}
	}

};
