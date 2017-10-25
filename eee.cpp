#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <omp.h>
#define PAGE_SIZE 6000
#define B_COUNT 6
#define MULT 10
using namespace std;
struct sol{
	int s[4];
};
struct test{
	string name;
	vector<sol> v;
};
vector<test> ana;
int limit;//limit
int control,t;//barrier control
int Pn;//Processor number
char buffer[B_COUNT][PAGE_SIZE];
bool bcheck[B_COUNT];//check buffer if processed
int dyn[10005][2];
int n,m;
int found[B_COUNT];//buffer sonu kontrolu
int bs[B_COUNT];//buffer size,buffer no
pair<string,string> R;//Reference pair
int lastbuffer=-1;//son okunan buffer
//ofstream f("ace.txt");
//	ofstream de("debug.txt");
bool comp(test i,test j)
{
	return i.name<j.name;
}
pair<string,string> read_inp(ifstream &r,test *x)
{
    char c;
    string name,ref;
    if (r.get()=='>')	getline(r,name);
    (*x).name=name;
    //cout<<name<<endl;
    while (!r.eof())
    {
        c=r.get();
	if(r.peek()=='>')	break;
        if (c=='A' || c=='T' || c=='C' || c=='G')
            ref.push_back(c);
    }
    m=ref.length();
    return make_pair<string,string>(name,ref);
}
void add_entry(int a,int b,int c,int d,test *x)
{
	sol temp;
	temp.s[0]=a;temp.s[1]=b;temp.s[2]=c;temp.s[3]=d;
	(*x).v.push_back(temp);
}
void read_buffer(ifstream &s)
{
    int i=0,j;
    char c;
    string name;
    if (s.get()=='>')
        getline(s,name);
    for (i=0;;i++)
    {
        while (s.peek()=='\n') s.get();
        if (s.eof())	{
            lastbuffer=i;
            break;
        }
        while (bcheck[i%B_COUNT]==true) {/*cout<<"waiting for: "<<i%B_COUNT<<endl;*/}
        j=0;
        while (!s.eof())
        {
            c=s.get();
            if (c=='A' || c=='T' || c=='C' || c=='G')
            {
                buffer[i%B_COUNT][j]=c;
                j++;
            }
            if (j==(Pn-1)*MULT)	break;
        }
        bs[i%B_COUNT]=j;
        if (s.eof())
            for (;j<(Pn-1)*MULT;j++)
                buffer[i%B_COUNT][j]=0;
        bcheck[i%B_COUNT]=true;
    }
}
void process(int p,test *x)
{
    int bn=0;//Buffer number
    int cn=0;//Control number
    while (lastbuffer!=cn)
    {
   /*    #pragma omp critical
        	cout<<"P:"<<p<<" bcheck:"<<bcheck[bn]<<" "<<cn<<endl;/*buffer[bn]<<" TKey:"<<t<<endl;*/
        while (bcheck[bn]==false || t!=0);
       /* #pragma omp critical
        {
        	cout<<"IN PROCESS:"<<p<<" "<<"BUFFER:"<<bn<<" PROCESSED BUFFERS:"<<cn<<endl;
        }*/
        for (int r=0;r<MULT;r++) {
            int i,j,s=0;
            i=0;
            j=(p-1)+r*(Pn-1);
	    
	    if(j>=bs[bn])	break;
            for (int k=0;k<m;k++)
            {    
                if (buffer[bn][j]==R.second[i])
                {		
                    s++;
                }
                else {
                    if (limit<=s)
                    {
			#pragma omp critical
			{add_entry(cn*(Pn-1)*MULT+j-s+1,cn*(Pn-1)*MULT+j,i-s+1,i,x);
			//cout<<"CN:"<<cn<<" "<<i<<" "<<j<<" "<<s<<endl;
                     //   cout<<cn*(Pn-1)*MULT+j-s+1<<" "<<cn*(Pn-1)*MULT+j<<" "<<i-s+1<<" "<<i<<endl;
			}
                    }
                    s=0;
                }
                if (j>=bs[bn]-1)
                {
                    dyn[i][(bn+1)%2]=s;
                    s=dyn[i][bn%2];
                    j=-1;
                }
                i++;
                j++;

            }
            if(s>=limit)
	    {
		#pragma omp critical
		add_entry(cn*(Pn-1)*MULT+j-s+1,cn*(Pn-1)*MULT+j,i-s+1,i,x);
               // cout<<cn*(Pn-1)*MULT+j-s+1<<" "<<cn*(Pn-1)*MULT+j<<" "<<i-s+1<<" "<<i<<endl;
	    }
        }
	/*for(int z=0;z<m;z++)
		cout<<dyn[z][(bn+1)%2]<<" ";*/
	//cout<<endl;
	#pragma omp atomic
        control++;
        while (control!=Pn-1);
	#pragma omp atomic
        t++;
	#pragma omp critical
        {
            if (t==Pn-1)
            {
                control=0;
                t=0;
                bcheck[bn]=false;
            }
        }
        bn++;
        bn%=B_COUNT;
        cn++;
    }
}
void read_buffer2(ifstream &s,test *x)
{
    int i=0,j;
    char c;
    string name;
    if (s.get()=='>')
        getline(s,name);
    for (i=0;;i++)
    {
        while (s.peek()=='\n') s.get();
        if (s.eof())	{
            lastbuffer=i;
            break;
        }
        j=0;
        while (!s.eof())
        {
            c=s.get();
            if (c=='A' || c=='T' || c=='C' || c=='G')
            {
                buffer[i%B_COUNT][j]=c;
                j++;
            }
            if (j==(Pn-1)*MULT)	break;
        }
        bs[i%B_COUNT]=j;
        if (s.eof())
            for (;j<(Pn-1)*MULT;j++)
                buffer[i%B_COUNT][j]=0;
        	process2(1,&x);
    }
}
void process2(int p,test *x)
{
    int bn=0;//Buffer number
    int cn=0;//Control number
        for (int r=0;r<MULT;r++) {
            int i,j,s=0;
            i=0;
            j=(p-1)+r*(Pn-1);

	    if(j>=bs[bn])	break;
            for (int k=0;k<m;k++)
            {
                if (buffer[bn][j]==R.second[i])
                {
                    s++;
                }
                else {
                    if (limit<=s)
                    {
			{add_entry(cn*(Pn-1)*MULT+j-s+1,cn*(Pn-1)*MULT+j,i-s+1,i,x);
			//cout<<"CN:"<<cn<<" "<<i<<" "<<j<<" "<<s<<endl;
                     //   cout<<cn*(Pn-1)*MULT+j-s+1<<" "<<cn*(Pn-1)*MULT+j<<" "<<i-s+1<<" "<<i<<endl;
			}
                    }
                    s=0;
                }
                if (j>=bs[bn]-1)
                {
                    dyn[i][(bn+1)%2]=s;
                    s=dyn[i][bn%2];
                    j=-1;
                }
                i++;
                j++;

            }
            if(s>=limit)
	    {
		add_entry(cn*(Pn-1)*MULT+j-s+1,cn*(Pn-1)*MULT+j,i-s+1,i,x);
               // cout<<cn*(Pn-1)*MULT+j-s+1<<" "<<cn*(Pn-1)*MULT+j<<" "<<i-s+1<<" "<<i<<endl;
	    }
        }
        bn++;
        bn%=B_COUNT;
        cn++;
}
void init_test(ifstream &ref)
{
	lastbuffer=-1;
	control=0;t=0;
	for(int i=0;i<B_COUNT;i++){	bcheck[i]=false;}
	for(int i=0;i<m;i++)dyn[i][0]=dyn[i][1]=0;
	ref.close();
	
}
void print_results(){
	sort(ana.begin(),ana.end(),comp);
	for(int i=0;i<ana.size();i++)
	{
		cout<<ana[i].name<<endl;
		for(int j=0;j<ana[i].v.size();j++)
		{
			cout<<ana[i].v[j].s[0]<<" "<<ana[i].v[j].s[1]<<" "<<ana[i].v[j].s[2]<<" "<<ana[i].v[j].s[3]<<endl;
		}
	}
}
int main(int argc,char* argv[]) {


    if (argc<5) {
        cout<<"ERROR"<<endl;
        return 0;
    }
    Pn=atol(argv[1]);
    limit=atol(argv[2]);
    ifstream ref(argv[3]);
    ifstream in(argv[4]);
    while(!in.eof()){
	 init_test(ref);
	 ifstream ref(argv[3]);
	test x;
	#pragma omp parallel for
	if(Pn==1)
	{
		read_buffer2(ref,&x);

	}
	else
	{
		for (int processor=0;processor<Pn;processor++)
		{
			//cout<<processor<<endl;
			if (!processor) {
			R=read_inp(in,&x);//inp oku
			read_buffer(ref);//bufferlara basla
			}
			else
			{
			process(processor,&x);
			}
		}
	}
	//cout<<ans<<" LIMIT: "<<limit<<" found:"<<db<<"pair number:"<<pr<<endl;
	for (int i=0;i<m;i++)
	{
		if (dyn[i][(lastbuffer)%2]>=limit)
		{
			add_entry(i-dyn[i][(lastbuffer)%2]+2,i+1,(lastbuffer-1)*MULT*(Pn-1)+bs[(lastbuffer-1)%B_COUNT]-dyn[i][(lastbuffer)%2]+1,(lastbuffer-1)*MULT*(Pn-1)+bs[(lastbuffer-1)%B_COUNT],&x);
		//cout<<i-dyn[i][(lastbuffer)%2]+2<<" "<<i+1<<" "<<(lastbuffer-1)*MULT*(Pn-1)+bs[(lastbuffer-1)%B_COUNT]-dyn[i][(lastbuffer)%2]+1<<" "<<(lastbuffer-1)*MULT*(Pn-1)+bs[(lastbuffer-1)%B_COUNT]<<endl;
		}
	}
	ana.push_back(x);
	/*for(int i=0;i<x.v.size();i++)
		cout<<x.v[i].s[0]<<" "<<x.v[i].s[1]<<" "<<x.v[i].s[2]<<" "<<x.v[i].s[3]<<endl;*/
    }
    print_results();
    return 0;
}
