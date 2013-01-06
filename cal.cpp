#include "data.h"
#include <cstdio>
#include <memory.h>
void ByteToBit(bool *out,const char *in,int bits)
{
    for(int i=0;i<bits;i++)
        out[i]=(in[i>>3]>>(i&7))&1;
}

void BitToByte(char *out,const bool* in,int bits )
{
    memset(out,0,bits>>3);
    for(int i=0;i<bits;i++)
        out[i>>3]|=in[i]<<(i&7);
}

void Transform(bool* out,bool *in,const int *Table,int len)
{
    static bool tmp[256];
    for(int i=0;i<len;i++)
        tmp[i]=in[Table[i]-1];
    memcpy(out,tmp,len);
}

void Xor(bool *InA,const bool *InB,int len)
{
    for(int i=0;i<len;i++)
        InA[i]^=InB[i];
}

void RotateL(bool *In,int len ,int loop)
{
    static bool tmp[256];
    memcpy(tmp,In,loop);
    memcpy(In,In+loop,len-loop);
    memcpy(In+len-loop,tmp,loop);
}

void Sfunc(bool out[32],const bool in[48])
{
    int j,m,n;
    for(j=0;j<8;j++,in+=6,out+=4)
    {
      m=(in[0]<<1)+in[5];
      n=(in[1]<<3)+(in[2]<<2)+(in[3]<<1)+in[4];
     ByteToBit(out,&S[j][16*m+n],4);
    }
}

void Ffunc(bool in[32],const bool ki[48])
{
    static bool MR[48];
    Transform(MR,in,EP,48);
    Xor(MR,ki,48);
    Sfunc(in,MR);
    Transform(in,in,FP,32);
}

void DesSetKey(const char Key[8])    //获得子密钥
{
 static bool K[64],*KL=&K[0],*KR=&K[28];
 ByteToBit(K,Key,64);
 Transform(K,K,PC1,56);
 for(int i=0;i<16;i++)
 {
   RotateL(KL,28,loopTable[i]);
   RotateL(KR,28,loopTable[i]);
   Transform(SubKey[i],K,PC2,48);    
 }
        
}

void Des(char out[8],char in[8],bool Type)
{
  static bool M[64],tmp[32],*Li=&M[0],*Ri=&M[32];
  ByteToBit(M,in,64);   //输入字符转换
  Transform(M,M,IP,64);//初始置换

  if(Type==false) //0为加密
     for(int i=0;i<16;i++)
     {
         memcpy(tmp,Ri,32);
         Ffunc(Ri,SubKey[i]); //R[i]=L[i-1]Xor f(R[i-1],K[i])
         Xor(Ri,Li,32);      //异或

         memcpy(Li,tmp,32);  //L[i]=R[i-1]
     }
  else       //解密，
      for(int i=15;i>=0;i--)
       {
           memcpy(tmp,Li,32);
           Ffunc(Li,SubKey[i]);
           Xor(Li,Ri,32);
           memcpy(Ri,tmp,32);
       }
  Transform(M,M,IPR,64);
  BitToByte(out,M,64);
}


int main()
{
    char key[8];
    char str[8];
    char str2[8];
	printf("输入密钥(8字节)\n");
       for(int i=0;i<8;i++)
           cin>>key[i];
     printf("输入明文(8字节)\n");
       for(int i=0;i<8;i++)
           cin>>str[i];
    printf("Before Encrypting:\n");
    puts(str);

    DesSetKey(key);
    memset(str2,0,sizeof(str2));
    Des(str2,str,0);

    printf("After Encrypting :\n");
    for(int i=0;i<8;i++)
        printf("%c",str2[i]);
    printf("\n");
   
    memset(str,0,sizeof(str));
    Des(str,str2,1);
    printf("After Decrypting:\n");
    puts(str);
   
    return 0;
}
