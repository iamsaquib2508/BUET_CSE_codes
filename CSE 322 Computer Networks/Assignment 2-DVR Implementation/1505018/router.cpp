
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include "myutil.h"
#define INF 10000000
#define MAX_TTL 3
#define UNDEFINED "___.___._._"
#define ffr(i,a,b) for(i=a;i<b;i++)


using namespace std;


map<string,neighbours> neighbours_list;
map<string,routing_table_entry> routing_table;
set<string> ips;
int numofIPs;
string MyIP;
int sockfd;

int EdgeFixer(int a)
{
    if(a>=INF) return INF;
    else return a;
}

void PrintTable()
{
    cout << "Destination    Next Hop      Cost  \n";
    map<string,routing_table_entry>::iterator it;
    it=routing_table.begin();
    while(it!=routing_table.end())
    {
        cout << it->first << "   " << it->second.nexthop << "   " << it->second.cost << endl;
        it++;
    }
}

void InitializeTable(string ip, string file)
{
    ips.clear();
    ifstream fin(file,ifstream::in);
    string uu, vv;
    int u, v, w;
    int totalnodes=0;
    MyIP=ip;

    routing_table.clear();
    routing_table[MyIP]=routing_table_entry(MyIP,0);
    while(fin >> uu >> vv >> w)
    {
        if(uu==MyIP)
        {
            routing_table[vv]=routing_table_entry(vv,w);
            ips.insert(vv);
            neighbours_list[vv]=neighbours(w,MAX_TTL);
        }
        else if(vv==MyIP)
        {
            routing_table[uu]=routing_table_entry(uu,w);
            ips.insert(uu);
            neighbours_list[uu]=neighbours(w,MAX_TTL);
        }
        else
        {
            if(ips.find(uu)==ips.end())
            {
                ips.insert(uu);
                routing_table[uu]=routing_table_entry(UNDEFINED,INF);
            }
            if(ips.find(vv)==ips.end())
            {
                ips.insert(vv);
                routing_table[vv]=routing_table_entry(UNDEFINED,INF);
            }
        }
    }
    fin.close();
    numofIPs=ips.size()+1;
    PrintTable();
}


char* ReceiveMessage()
{

    struct sockaddr_in router;
    socklen_t addrlen;
    char* buffer = new char[1024];

	int bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &router, &addrlen);
		//printf("[%s:%d]: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
	return buffer;
}



void SendProcess(string command)
{
    //string myip=MakeIP(command.substr(4,4));
    string Receiver=MakeIP(command.substr(8,4));
    //cout << Receiver << " rcvr this is\n";
    string RestCommand="frwd"+command.substr(8,command.size()-8);
    int len=command[12]+command[13]*256, i;
    //cout << len << endl;
    char message1[len+1];
    ffr(i,0,len)
    {
        message1[i]=(command[14+i]);
    }
    message1[len]='\0';
    string message(message1);
    //cout << message << endl;
    routing_table_entry temp=routing_table[Receiver];
    string next=temp.nexthop;
    int tempcost=temp.cost;
    bool succ=false;
    if(tempcost<INF)
    succ=SendMessage(next,RestCommand,sockfd);
    if(succ)
    {
        cout << message << " packet sent to " << next << " (printed by " << MyIP << ").\n";
    }
    return;
}

void ForwardProcess(string command)
{
    //string myip=MakeIP(command.substr(4,4));
    string Receiver=MakeIP(command.substr(4,4));

    int len=command[8]+command[9]*256, i;
    char message1[len+1];
    ffr(i,0,len)
    {
        message1[i]=(command[10+i]);
    }
    message1[len]='\0';
    string message(message1);
    if(Receiver==MyIP)
    {
        cout << message << " packet reached destination (printed by " << MyIP << ").\n";
    }
    else
    {
        routing_table_entry temp=routing_table[Receiver];
        string next=temp.nexthop;
        int tempcost=temp.cost;
        if(tempcost<INF)
        {
            bool succ=true;
            succ=SendMessage(next,command,sockfd); //todo
            if(succ)
            {
                cout << message << " packet forwarded to " << next << " (printed by " << MyIP << ").\n";
            }
            else
            {
                cout << "Message couldn't be forwarded to " << next << ".\n";
            }
        }
        else
        {
            cout << "Link with " << next << " is down.\n";
        }
    }
    return;
}

void CostProcess(string command)
{
    string oneip=MakeIP(command.substr(4,4));
    string otherip=MakeIP(command.substr(8,4));
    int costt=command[12]+command[13]*256;
    if(MyIP==oneip)
    {
        if(neighbours_list[otherip].TTL!=0)
        routing_table[otherip]=routing_table_entry(otherip,costt);
        neighbours_list[otherip].cost=costt;
        map<string,routing_table_entry>::iterator itt;
        for(itt=routing_table.begin();itt!=routing_table.end();itt++)
        {
            if(itt->second.nexthop==otherip)
            {
                itt->second = routing_table_entry(UNDEFINED, INF);
            }
        }
    }
    else if(MyIP==otherip)
    {
        if(neighbours_list[oneip].TTL!=0)
        routing_table[oneip]=routing_table_entry(oneip,costt);
        neighbours_list[oneip].cost=costt;
        map<string,routing_table_entry>::iterator itt;
        for(itt=routing_table.begin();itt!=routing_table.end();itt++)
        {
            if(itt->second.nexthop==oneip)
            {
                itt->second = routing_table_entry(UNDEFINED, INF);
            }
        }
    }

    //todo
}


void Broadcast()
{
    string ret="tbls "+MyIP+" ";
    map<string,routing_table_entry>::iterator it;
    it=routing_table.begin();
    while(it!=routing_table.end())
    {
        ret+= (it->first + "/" + it->second.nexthop + "/" + makestring(it->second.cost) + " ");
        it++;
    }

    map<string,neighbours>::iterator it1;
    for(it1=neighbours_list.begin(); it1!=neighbours_list.end(); it1++)
    {
        if(it1->second.cost<INF)
        {
            bool succ;
            succ=SendMessage(it1->first,ret,sockfd);
        }
    }

    return;
}

void UpdateTable(string msg)
{
    vector<string> messages=StringSplit(msg,' ');
    string NeighbourCurrent=messages[1];
    if(neighbours_list[NeighbourCurrent].TTL==0)
    {
        //was dead, now alive again
        routing_table[NeighbourCurrent] = routing_table_entry(NeighbourCurrent, neighbours_list[NeighbourCurrent].cost);
    }
    neighbours_list[NeighbourCurrent].TTL=MAX_TTL;
    for(int i=2;i<2+numofIPs;i++)
    {
        vector<string> row= StringSplit(messages[i],'/');
        string destination=row[0];
        string NextHopForNeighbour=row[1];


        if(destination==MyIP || NextHopForNeighbour== MyIP) continue;

        int CostForNeighbour=String2Int(row[2]);
        int CostToNeighbour=min( routing_table[NeighbourCurrent].cost, neighbours_list[NeighbourCurrent].cost);
        //if(MyIP=="192.169.0.3" && NeighbourCurrent=="192.168.0.2" && i==5)
            //printf("Special %d %d %d\n", CostForNeighbour, CostToNeighbour, routing_table[destination].cost);
        if(EdgeFixer(CostForNeighbour+CostToNeighbour)<routing_table[destination].cost)
        {
            string NextHopToNeighbour = (CostForNeighbour==INF)?UNDEFINED:(CostToNeighbour==neighbours_list[NeighbourCurrent].cost)?NeighbourCurrent:routing_table[NeighbourCurrent].nexthop;
            routing_table[destination]=routing_table_entry(NextHopToNeighbour,EdgeFixer(CostForNeighbour+CostToNeighbour));
        }
        else if((routing_table[destination].nexthop == routing_table[NeighbourCurrent].nexthop) && (routing_table[destination].cost>routing_table[NeighbourCurrent].cost))
        {
            string NextHopToNeighbour = (CostForNeighbour==INF)?UNDEFINED:routing_table[NeighbourCurrent].nexthop;
            routing_table[destination]=routing_table_entry(NextHopToNeighbour,EdgeFixer(CostForNeighbour+CostToNeighbour));
        }
    }
}

void TTLStatusUpdate()
{
    map<string, neighbours>::iterator it;
    for(it=neighbours_list.begin(); it!=neighbours_list.end(); it++)
    {
        if ((it->second).TTL != 0)
		{
			(it->second).TTL--;
		}
		else
		{
            map<string,routing_table_entry>::iterator itt;
            for(itt=routing_table.begin();itt!=routing_table.end();itt++)
            {
                if(itt->second.nexthop==it->first)
                {
                    itt->second = routing_table_entry(UNDEFINED, INF);
                }
            }
		}
    }
}

int main(int argc, char *argv[]) //ip, topologyfile
{
    if (argc != 3)
	{
		cout << "router : " << argv[1] << "<ip address>\n";
		exit(1);
	}
    InitializeTable(argv[1],argv[2]);
    cout << numofIPs << endl;

    //setup connectivity
	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET;
	client_address.sin_port = htons(4747);
	inet_pton(AF_INET, MyIP.c_str(), &client_address.sin_addr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bind(sockfd, (struct sockaddr *)&client_address, sizeof(sockaddr_in));


    //receive commands
    int time=0;
    while(true)
    {
        char* msg=ReceiveMessage();

        string message;
        message=stringmaker(msg);

        string command=message.substr(0,4);
        //cout << message << endl;
        if(command=="send")
        {
            SendProcess(message);
        }
        else if(command=="frwd")
        {

        }
        else if(command=="cost")
        {
            CostProcess(message);
        }
        else if(command=="tbls")
        {
            UpdateTable(message);

            //cout << "----------\n" << message << "\nPrintTable\n";
            //PrintTable();
        }
        else if(command=="show")
        {
            PrintTable();
        }
        else if(command.find("clk") != string::npos)
        {
            TTLStatusUpdate();
            Broadcast();
            if(time%3==0)
                PrintTable();
            time++;
        }
    }
	return 0;

}
