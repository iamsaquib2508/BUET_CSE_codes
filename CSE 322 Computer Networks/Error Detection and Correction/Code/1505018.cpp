#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define ffrr(i,a,b) for(i=a;i<=b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#include<random>

#define setRed(a) cout<<"\033[1;31m"<<a<<"\033[0m"
#define setGreen(a) cout<<"\033[1;32m"<<a<<"\033[0m"
#define setCyan(a) cout<<"\033[1;36m"<<a<<"\033[0m"
#define setDefault(a) cout<<"\033[1;30m"<<a<<"\033[0m"

using namespace std;

int hamming_parity_bits(int m)
{
    //let
    int r=0, pow=1;
    while((m+r+1)>pow)
    {
        pow+=pow; r++;
    }
    return r;
}


int main()
{
    //setRed('c');
    //setGreen
    //setCyan
    //setDefault

    string data;
    int m_bytes_in_row;
    double probability;
    string poly;
    {
        cout << "Data string: ";
        getline(cin,data);
        cout << "Bytes in a row: ";
        cin >> m_bytes_in_row;
        cout << "Probability: ";
        cin >> probability;
        cout << "Generator Polynomial: ";
        cin >> poly;
    }

    int size=data.size();
    int rem=size%m_bytes_in_row;
    if(rem>0)
    {
        rem=m_bytes_in_row-rem;
        while(rem--)
        data+="~", size++;
        cout << "Updated data after padding: " << data << endl;
    }

    int m=(m_bytes_in_row<<3);
    int r=hamming_parity_bits(m);
    int rows=size/m_bytes_in_row, i, j, k, bits=m+r;

    char datablock[rows][m];
    int dataIndex=0;
    cout << "\nCreating data block\n";
    ffr(i,0,rows)
    {
        ffr(j,0,m_bytes_in_row)
        {
            int put=j*8;
            int c=data[dataIndex++];
            //char* poin=fff(c);

            ffr(k,0,8)
            {
                if(c&1)
                datablock[i][put+7-k]='1';
                else datablock[i][put+7-k]='0';
                // if(j==0 && i==0)
                // cout << c << " c\n";
                c/=2;
            }
        }
    }

    cout << "\nPrinting data block (ASCII code of m char per row) :\n\n";
    ffr(i,0,rows)
    {
        ffr(j,0,m)
        {
            cout << datablock[i][j];
        }
        cout << "\n";
    }

    char Rblock[rows][bits+1];

    cout << "\nAdding check bits\n";
    ffr(i,0,rows)
    {
        ffrr(j,1,bits) Rblock[i][j]='0';
        //all 0
        int pow=1;
        int datacol=0;
        for(j=1;j<=bits;j++)
        {
            if(j==pow)
            {
                //r probability
                pow+=pow;
            }
            else
            {
                //m bit
                if(datablock[i][datacol]=='1')
                {
                    Rblock[i][j]='1';
                    int cur=j;
                    int ind=1;
                    //update all necessary r bits
                    while(cur>0)
                    {
                        if(cur&1)
                        Rblock[i][ind]=('0'+'1'-Rblock[i][ind]);
                        ind+=ind;
                        cur/=2;
                    }
                }
                datacol++;
            }
        }
    }
    cout << "\nPrinting data block after adding check bits\n\n";
    ffr(i,0,rows)
    {
        int pow=1;
        ffrr(j,1,bits)
        {
            if(j==pow)
            {
                pow+=pow;
                //setGreen
                setGreen(Rblock[i][j]);
                //cout << (char)(Rblock[i][j]-'0'+'a');
                setDefault("");
                //setDefault
            }
            else
            cout << Rblock[i][j];
        }
        cout << "\n";
    }

    cout << "\nColumn-wise serializing\n";

    // do serialize
    string serialized_data="";
    ffrr(j,1,bits)
    {
        ffr(i,0,rows)
        {
            serialized_data+=Rblock[i][j];
        }
    }

    cout << "\nDatastream after columnwise serialization\n\n";

    // do print
    cout << serialized_data << endl;

    cout << "\nAdding CRC checksum bits\n";


    //do add CRC
    string CRCdata=serialized_data;
    while(poly[0]=='0') poly.erase(poly.begin());
    int polysize=poly.size();
    ffr(i,1,polysize) CRCdata+="0";
    int head=0, tail=polysize-1;
    string match="";
    ffr(i,head,tail) match+=CRCdata[i];
    while(tail<CRCdata.size())
    {
        match+=CRCdata[tail++];
        if(match[0]=='0')
        {
            string match2="";
            ffr(i,1,polysize)
            {
                match2+=match[i];
            }
            match=match2;
        }
        else
        {
            string match2="";
            ffr(i,1,polysize)
            {
                if(match[i]!=poly[i]) match2+="1";
                else match2+="0";
            }
            match=match2;
        }
    }
    CRCdata=serialized_data+match;

    cout << "\nDatastream after adding CRC checksum bits (sent frame)\n\n";

    //do print
    cout << serialized_data ;
    ffr(i,1,polysize)
    {
        //setCyan
        setCyan(match[i-1]);
        setDefault("");
        //cout << (char)('A'-'0'+match[i-1]);
        //setDefault
    }
    cout << endl;

    cout << "\nAltering bits\n";

    //do alteration
    const int range_from  = 0;
    const int range_to    = 1000;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);


    string receivedData=CRCdata;
    ffr(i,0,CRCdata.size())
    {
        int t= distr(generator) ;

        double p=(t+1)*1.0/range_to;

        if(p<=probability)
        {
            //cout << "hit\n";
            receivedData[i]='1'-receivedData[i]+'0';
        }

    }
    //temp ends

    cout << "\nDatastream received\n\n";
    ffr(i,0,receivedData.size())
    {
        if(CRCdata[i]!=receivedData[i])
        {
            setRed((char)receivedData[i]);
            setDefault("");
            //cout << (char)receivedData[i]-'0'+'x';
            //setDefault
        }
        else
        cout << (char)receivedData[i];
    }

    cout << endl;
    //cout << receivedData << endl;

    head=0, tail=polysize-1;
    match="";
    ffr(i,head,tail) match+=receivedData[i];
    while(tail<CRCdata.size())
    {
        match+=receivedData[tail++];
        if(match[0]=='0')
        {
            string match2="";
            ffr(i,1,polysize)
            {
                match2+=match[i];
            }
            match=match2;
        }
        else
        {
            string match2="";
            ffr(i,1,polysize)
            {
                if(match[i]!=poly[i]) match2+="1";
                else match2+="0";
            }
            match=match2;
        }
    }
    cout << "remainder: " << match << endl;
    ffr(i,0,match.size())
    {
        if(match[i]!='0') break;
    }
    if(i==match.size())
    {
        //0 found, correct
        cout << "\nResult of CRC chechsum: No Error\n";
    }
    else
    {
        //err
        cout << "\nResult of CRC chechsum: Error Found\n";
    }

    char receivedRblock[rows][bits+1];
    int index=0;
    ffrr(j,1,bits)
    {
        ffr(i,0,rows)
        {
            receivedRblock[i][j]=receivedData[index++];
        }
    }

    cout << "\nDatablock after removing CRC checksum bits\n\n";
    ffr(i,0,rows)
    {
        ffrr(j,1,bits)
        {
            bool errr=(receivedRblock[i][j]!=Rblock[i][j]);
            if(errr)
            {
                //setRed
                setRed((char)receivedRblock[i][j]);
                setDefault("");
                //setDefault
            }
            else
            {
                cout << (char)receivedRblock[i][j];
            }

        }
        cout << endl;
    }

    cout << "\nRemoving r bits\n";

    char receivedblock[rows][m];
    string output="";

    ffr(i,0,rows)
    {
        char rbitchecker[bits+1];
        ffrr(j,1,bits) rbitchecker[j]=receivedRblock[i][j];

        //all 0
        int pow=1;
        ffrr(j,1,bits)
        {
            if(j==pow)
            {
                //r probability
                rbitchecker[j]='0';
                pow+=pow;
            }
            else
            {
                //m bit
                if(rbitchecker[j]=='1')
                {
                    int cur=j;
                    int ind=1;
                    //update all necessary r bits
                    while(cur>0)
                    {
                        if(cur&1)
                        rbitchecker[ind]=('0'+'1'-rbitchecker[ind]);
                        ind+=ind;
                        cur/=2;
                    }
                }
            }
        }
        int errind=0;
        for(pow=1; pow<=bits;pow+=pow)
        {
            if(rbitchecker[pow]!=receivedRblock[i][pow])
            {
                errind+=pow;
            }
        }
        if(errind>0 && errind<=bits)
        receivedRblock[i][errind]=('0'+'1'-receivedRblock[i][errind]);
        pow=1;
        int blockcol=0;
        int charbuild=0;

        ffrr(j,1,bits)
        {
            if(blockcol%8==0 && blockcol>0)
            {

                output+=(char)(charbuild);
                charbuild=0;

            }
            if(j!=pow)
            {
                char temp=receivedRblock[i][j];
                //cout << temp;
                receivedblock[i][blockcol++]=temp;
                charbuild+=charbuild;
                charbuild+=(temp-'0');
            }
            else pow+=pow;
            //cout << endl;
        }
        //cout << endl;
        output+=(char)(charbuild);
    }

    cout << "\nDatablock after removing check bits\n\n";
    ffr(i,0,rows)
    {
        ffr(j,0,m)
        {
            if(receivedblock[i][j]!=datablock[i][j])
            {
                setRed(receivedblock[i][j]);
                setDefault("");
            }
            else
            cout << receivedblock[i][j];
        }
        cout << endl;
    }

    cout << "\nOutput frame: " << output << endl;


    return 0;

}
