#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define ll long long int
#define pb push_back
#define ffrr(i,a,b) for(i=a;i<=b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#define bug(X) cout << #X << " = " << X << endl

using namespace std;

int PI[64] = {58, 50, 42, 34, 26, 18, 10, 2,
      60, 52, 44, 36, 28, 20, 12, 4,
      62, 54, 46, 38, 30, 22, 14, 6,
      64, 56, 48, 40, 32, 24, 16, 8,
      57, 49, 41, 33, 25, 17, 9, 1,
      59, 51, 43, 35, 27, 19, 11, 3,
      61, 53, 45, 37, 29, 21, 13, 5,
      63, 55, 47, 39, 31, 23, 15, 7};

int CP_1[56] = {57, 49, 41, 33, 25, 17, 9,
      1, 58, 50, 42, 34, 26, 18,
      10, 2, 59, 51, 43, 35, 27,
      19, 11, 3, 60, 52, 44, 36,
      63, 55, 47, 39, 31, 23, 15,
      7, 62, 54, 46, 38, 30, 22,
      14, 6, 61, 53, 45, 37, 29,
      21, 13, 5, 28, 20, 12, 4};

int CP_2[48]= {14, 17, 11, 24, 1, 5, 3, 28,
      15, 6, 21, 10, 23, 19, 12, 4,
      26, 8, 16, 7, 27, 20, 13, 2,
      41, 52, 31, 37, 47, 55, 30, 40,
      51, 45, 33, 48, 44, 49, 39, 56,
      34, 53, 46, 42, 50, 36, 29, 32};

int E[48] = {32, 1, 2, 3, 4, 5,
           4, 5, 6, 7, 8, 9,
           8, 9, 10, 11, 12, 13,
           12, 13, 14, 15, 16, 17,
           16, 17, 18, 19, 20, 21,
           20, 21, 22, 23, 24, 25,
           24, 25, 26, 27, 28, 29,
           28, 29, 30, 31, 32, 1};

int PI_2[32] = {35, 38, 46, 6, 43, 40, 14, 45,
           		33, 19, 26, 15, 23, 8, 22, 10,
           		12, 11, 5, 25, 27, 21, 16, 31,
           		28, 32, 34, 24, 9, 37, 2, 1};

int P[32] = {16, 7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26, 5, 18, 31, 10,
     2, 8, 24, 14, 32, 27, 3, 9,
     19, 13, 30, 6, 22, 11, 4, 25};


int PI_1[64] = {40, 8, 48, 16, 56, 24, 64, 32,
             39, 7, 47, 15, 55, 23, 63, 31,
             38, 6, 46, 14, 54, 22, 62, 30,
             37, 5, 45, 13, 53, 21, 61, 29,
             36, 4, 44, 12, 52, 20, 60, 28,
             35, 3, 43, 11, 51, 19, 59, 27,
             34, 2, 42, 10, 50, 18, 58, 26,
             33, 1, 41, 9, 49, 17, 57, 25};

int SHIFT[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

FILE * pFile;

template<class xx>
void print_array(xx* arr, int len)
{
    int i, j=0;
    ffr(i,0,len)
    {
        if(i-j==8){j=i; cout << ' ';}
        //cout << arr[i];
    }
    //cout << endl;
}

void vector_print(vector<unsigned char> v)
{
    int i;
    //cout << "In vector print ";
    ffr(i,0,v.size())
    {
        if(v[i]=='~') break;
        //cout << v[i];
        unsigned char* ap=new unsigned char[1];
        //sprintf(ap,"%x", v[i]);
        //printf("%s", ap);
        fprintf(pFile,"%c", v[i]);
    }
    fprintf(pFile,"\n");
}

template<class xx>
xx* replace(xx* a, int* b, int lenb) //replaces a by b and keeps it in ret
{
    xx* ret=new xx[lenb];
    int i;
    ffr(i,0,lenb)
    {
        ret[i]=a[b[i]-1];
    }
    //print_array(*ret,56);
    return ret;
}


template<class xx>
xx* shift_half(xx* arr, int len, int shift)
{
    int i=0, mid=len/2, j;
    xx* ret=new xx[len];
    ffr(i,shift,mid) ret[i-shift]=arr[i];

    j=0;
    ffr(i,i-shift,mid)
    {
        ret[i]=arr[j];
        j++;
    }
    ffr(i,mid+shift,len) ret[i-shift]=arr[i];

    j=mid;
    ffr(i,i-shift,len)
    {
        ret[i]=arr[j];
        j++;
    }
    return ret;
}


void string_to_bits(vector<unsigned char> s, bool* ret)
{
    int i, sz=s.size(), cur=97, j;
    //vector_print(s);
    for(i=0,j=7;j<64;j+=8,i++)
    {
        int x=s[i], k=0;
        for(int kk=0;kk<8;kk++)
        {
            if(x&1) ret[j-k]=true;
            else ret[j-k]=false;
            x>>=1;
            k++;
        }
    }
}

vector<unsigned char> bits_to_string(bool* arr, int len)
{
    int i=0, j, ans, k=0;
    vector<unsigned char> uc;
    //print_array(arr,64);

    while(i<len)
    {
        ans=0;
        ffr(j,0,8)
        {
            ans+=ans; if(arr[i]) ans++; i++;
        }
        uc.pb((unsigned char)(ans)); k++;

        //cout << ((ans)) << ' ';

    }
    //cout << endl;
    //vector_print(uc);
    return uc;
}

//void print_array(xx* arr, int len)
//xx* replace(xx* a, int* b, int lenb) //replaces a by b and keeps it in ret
//xx* shift_half(xx** arr, int len, int shift)
//void string_to_bits(string s, bool* ret)

vector<unsigned char> func(vector<unsigned char> plain, vector<unsigned char> keyy, bool dec)
{
    int i;
    bool* plaintext=new bool[64];
    string_to_bits(plain,plaintext);

    bool* key=new bool[64];
    string_to_bits(keyy,key);

    bool* modified_key= new bool[56];
    modified_key=replace(key,CP_1,56);
    //print_array(modified_key,56);

    bool* transposed_data=new bool[64];
    transposed_data=replace(plaintext,PI,64);
    bool* rotated_key = new bool[56];

    int iter;
    ffr(iter,0,16)
    {
        int shiftcur=SHIFT[iter];
        if(dec) shiftcur=SHIFT[15-iter];
        rotated_key=shift_half(modified_key,56,shiftcur);
        //print_array(rotated_key,56);
        bool* K_i = new bool [48];
        K_i=replace(rotated_key,CP_2,48);
        //print_array(K_i,48);

        bool* temp = new bool[32];
        ffr(i,0,32) temp[i]=transposed_data[i+32];
        bool* e = new bool[48];
        e=replace(transposed_data+32,E,48);

        ffr(i,0,48) e[i]^=K_i[i];

        bool* pp = new bool[32];
        pp=replace(e,PI_2,32);
        //print_array(pp,32); cout << "\n\n";
        pp=replace(pp,P,32);

        ffr(i,0,32) pp[i]^=transposed_data[i];

        ffr(i,0,32) transposed_data[i]=temp[i];
        ffr(i,32,64) transposed_data[i]=pp[i-32];
    }

    bool* after_iteration_data = new bool[64];
    ffr(i,0,32) after_iteration_data[i]=transposed_data[i+32];
    ffr(i,32,64) after_iteration_data[i]=transposed_data[i-32];

    after_iteration_data=replace(after_iteration_data,PI_1,64);
    //print_array(after_iteration_data,64);

    return bits_to_string(after_iteration_data,64);

}



vector<unsigned char> encryption_decryption(vector<unsigned char> ss, vector<unsigned char> key, bool dec)
{
    int sz=ss.size(), i, j, kl;
    int mod=sz%8;
    if(mod>0) mod=8-mod;
    ffr(i,0,mod) {ss.pb('~'); sz++;}
    i=0;
    vector<unsigned char> ret, temp;
    vector<unsigned char>::iterator itb, ite;
    ret.clear();
    int retid=0;
    while(i<sz)
    {
        vector<unsigned char> s;
        s.clear();
        ffr(j,0,8) s.pb(ss[i+j]);
        i+=j;
        temp = func(s,key,dec) ;
        itb=temp.begin(); ite=temp.end();

        for(; itb!=ite; itb++) {ret.pb((*itb));}
        //vector_print(ret);
    }

    return ret;
}


int main()
{
    int i;
    string plain, keyy;
    std::vector<unsigned char> pp, kk;

    //freopen("DES_input.txt", "r", stdin);

    pFile = fopen ("DES.txt","w");

    while(1)
    {
        getline(cin,keyy);
        getline(cin,plain);


        if(plain=="quit") break;
        pp.clear(); kk.clear();
        fprintf(pFile,"\n");
        ffr(i,0,plain.size()) pp.pb(plain[i]);
        ffr(i,0,keyy.size()) kk.pb(keyy[i]);

        //vector_print(pp);

        vector<unsigned char> encrypted=(encryption_decryption(pp,kk,0));
        fprintf(pFile,"Encrypted: "); vector_print(encrypted);
        vector<unsigned char> decrypted=(encryption_decryption(encrypted,kk,1));
        fprintf(pFile,"Decrypted: "); vector_print(decrypted);

        cout << endl;
    }

	return 0;
}
