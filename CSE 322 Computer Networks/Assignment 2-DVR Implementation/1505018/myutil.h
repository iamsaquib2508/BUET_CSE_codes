#include<bits/stdc++.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;


class routing_table_entry
{
public:
    string nexthop;
    int cost;
    routing_table_entry(){}
    routing_table_entry(string s, int costt)
    {
        nexthop=s; cost=costt;
    }
    routing_table_entry(const routing_table_entry &obj);
    bool operator<(const routing_table_entry &b)
    {
        if(nexthop<b.nexthop) return true;
        if(nexthop==b.nexthop && cost<b.cost) return true;
        return false;
    }
};

routing_table_entry::routing_table_entry(const routing_table_entry &obj)
{
    nexthop=obj.nexthop;
    cost=obj.cost;
}


class neighbours
{
public:
    int cost;
    int TTL;
    neighbours(){}
    neighbours(int costt, int ttl)
    {
        cost=costt; TTL=ttl;
    }
};

string MakeIP(string str)
{

    string ret="";
    for(int i=0;i<4;i++)
    {
        string temp="";
        int a=(int)(unsigned char)str[i], x;

        if(a==0) temp="0";
        while(a>0)
        {
            x=a%10; a/=10; temp+=(x+'0');
        }
        reverse(temp.begin(),temp.end());
        ret+=temp;
        if(i<3) ret+=".";
    }
    return ret;
}


string IPtoChar(string ip)
{
    int sz=ip.size();
    int i=0;
    string ret="";
    unsigned char num=0;
    for(i=0;i<sz;i++)
    {
        if(ip[i]=='.')
        {
            ret+=(unsigned char)(num);
            num=0;
        }
        else
        {
            num*=10;
            num+=((ip[i]-'0'));
        }
    }
    ret+=(unsigned char)(num);
    return ret;
}

int String2Int(string ip)
{
    int num=0, i, sz=ip.size();
    for(i=0;i<sz;i++)
    {
        {
            num*=10;
            num+=((ip[i]-'0'));
        }
    }
    return num;
}

vector<string> StringSplit(string msg, char delim)
{
    string token;
    vector<string> ret; ret.clear();
    stringstream ss(msg);
    while(getline(ss,token,delim))
    {
        ret.push_back(token);
    }
    return ret;
}

string makestring(int a)
{
    string temp="";
    int x;
    if(a==0) temp="0";
    while(a>0)
    {
        x=a%10; a/=10; temp+=(x+'0');
    }
    reverse(temp.begin(),temp.end());
    return temp;
}

char* substrr(char* msg, int st, int len)
{
    char* ret = new char[len];
    for(int i=0;i<len;i++)
    {
        ret[i]=msg[st+i];
    }
    return ret;
}

string stringmaker(char* st)
{
    string ret="";
    int sz=150;
    for(int i=0;i<sz;i++)
    {
        ret+=(st[i]);
    }
    return ret;
}


bool SendMessage(string destination, string msg, int sockfd){
    struct sockaddr_in routerAddress;

    routerAddress.sin_family = AF_INET;
    routerAddress.sin_port = htons(4747);
    routerAddress.sin_addr.s_addr = inet_addr(destination.c_str());

    int sent_bytes = sendto(sockfd, msg.c_str(), 1024, 0, (struct sockaddr*) &routerAddress, sizeof(sockaddr_in));
    if(sent_bytes!=-1){
    return true;
    }
    return false;
}
