#include <fstream>
#define x default:
#define y break
#define z case
#define w char
using namespace std;int main(int a,w**b){ifstream g;g.open(b[1],_S_in);g.seekg(0,_S_end);a=g.tellg();g.seekg(0,_S_beg);w*c=new w[a];g.read(c,a);g.close();w*p=new w[1<<15];int i,d;for(;i<a-1;i++){switch(c[i]){z'>':++p;printf("++p:%d",p);y;z'<':--p;printf("--p:%d",p);y;z'+':z'-':*p+=44-c[i];printf("%c:%d",44-c[i],*p);y;z'.':putchar('.');putchar(*p);y;z',':putchar(',');*p=getchar();y;z'[':putchar('[');if(!*p)for(i++,d=1;d;i++){switch(c[i]){z'[':d++;y;z']':d--;y;x y;}}y;z']':putchar(']');if(*p)for(i--,d=1;d;i--){switch(c[i]){z'[':d--;y;z']':d++;y;x y;}}y;x y;}}}