#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define DBG_INT(arg) fprintf(stderr,"*DEBUG* " #arg "...%d\n",arg)
#define DBG_DOUBLE(arg) fprintf(stderr,"*DEBUG* " #arg "...%f\n",arg)
#define DBG_CHAR(arg) fprintf(stderr,"*DEBUG* " #arg "...%c\n",arg)
#define DBG_STRING(arg) fprintf(stderr,"*DEBUG* " #arg "...%s\n",arg)
#define mal(type,size)  malloc(size*sizeof(type))
#define rep(i,N)    for(int i=0;i<N;i++)

typedef long long ll;

int value[256][2]; // int変数格納庫 [][0]=変数名 [][1]=変数
int valuemax = 0; //　総行数,変数個数
int vector[256][256]; //vec変数格納庫 [][0]=変数名 [][1]!=次元数 それ以外=変数
int vecmax=0; // vec変数個数
int sum=0;  //式分解再帰関数での合計
int resultvec[256]; //vec再帰関数内での合計
int tmpvec[256];

int cal(char statement[20][100][100],int i,int j,int pm){   
    // cal(char 解析文章,int 行番号,int 解析開始単語数,int 解析開始命令文文字数)
    int val;
    if('0'<=statement[i][j][0] && '9'>=statement[i][j][0]){
        val = statement[i][j][0]-'0';
    }else{
        rep(l,valuemax){
            if(value[l][0]==(int)statement[i][j][0]){
                val = value[l][1];
            }
        }
    }
    
    if(pm==0){  // + -> 0,- -> 1
        sum+=val;
    }else{
        sum-=val;
    }
    
    if(statement[i][j+1][0]=='+'){
        cal(statement,i,j+2,0);
    }
    else if(statement[i][j+1][0]=='-'){
        cal(statement,i,j+2,1);
    }
    else{
        int tmp=sum;
        sum=0;
        return tmp;
    }
}

void analyse_vec(char statement[20][100][100],int i,int j){   // "[]"部分を分解して一つの変数として代入処理を行う
    int k=0;
    rep(o,256)  tmpvec[o]=0;
    while(statement[i][j][0]!=']'){
        tmpvec[k]=statement[i][j][0]-'0';
        j++;
        k++;
    }
    tmpvec[k]=-1;
    return;
}

int cal_vec(char statement[20][100][100],int i,int j,int pm){
    int l=0,dim=0;
    if(statement[i][j][0]=='['){
        j++;
        analyse_vec(statement,i,j);
        while(tmpvec[l]!=-1){
            j++;
            dim++;
        }
        j++;    // ]の分

        if(pm==0){  // +の処理
            rep(k,tmpvec[dim]){
                resultvec[k]+=tmpvec[k];
            }
        }else{
            rep(k,tmpvec[dim]){
                resultvec[k]-=tmpvec[k];
            }
        }
    }
    
    else if(statement[i][j][0]>='a' && statement[i][j][0]<='z'){   //　変数の場合
        rep(k,vecmax){
            if(statement[i][j][0]==vector[k][0]){
                rep(l,vector[k][1]){
                    if(pm==0)    resultvec[l]+=vector[k][l];
                    else resultvec[l]-=vector[k][l];
                    dim=vector[k][1];
                    break;
                }
            }
        }
        j++;
    }

    else if(statement[i][j][0]=='+'){
        cal_vec(statement,i,j+1,0);
    }
    else if(statement[i][j][0]=='-'){
        cal_vec(statement,i,j+1,1);
    }
    if(statement[i][j][0]==';'){
        return dim;
    }
}

signed main(void){
    // 文章分け
    int N;
    int i = 0,j = 0,k = 0;
    scanf("%d",&N);
    char statement[20][100][100];   //　行番号,単語数,命令
    char ctemp;
    rep(i,N){
        j = 0;
        while((ctemp = getchar()) != EOF){
            if(ctemp==';'){
                statement[i][j][0] = ';';
                statement[i][j+1][0] = '\0';
                break;
            }
            else if(ctemp==' '){
                statement[i][j][k] = '\0';
                j++;
                k = 0;
                continue;
            }
            else if(ctemp=='\n'){
                continue;
            }
            else if(ctemp==','){
                continue;
            }
            statement[i][j][k] = ctemp;
            k++;
        }
    }

    rep(i,256)  vector[i][1]=0;
    int tmp=1,tmp1=1;
    //構文解析
    rep(i,N){
        // 変数宣言の場合
        if(strlen(statement[i][0])==3){
            // int
            if(statement[i][0][0]=='i'){
                rep(j,valuemax){
                    // 変数名が既に存在しているか線形探索
                    if(value[j][0]==statement[i][1][0]){
                        value[j][1]=cal(statement,i,3,0);
                        tmp=0;
                        // 存在していれば演算結果を格納
                    }
                }
                if(tmp!=0){
                    value[valuemax][0]=(int)statement[i][1][0];
                    value[valuemax][1]=cal(statement,i,3,0);
                    valuemax++;
                    tmp=1;
                    // 存在していなければ新規に変数宣言
                }
            }
            // vec
            else if(statement[i][0][0]=='v'){
                tmp=0;
                rep(j,vecmax){
                    if(vector[j][0]==statement[i][1][0]){
                        rep(o,256){
                            resultvec[o]=0;
                        }
                        cal_vec(statement,i,3,0);
                        rep(k,vector[j][1]){
                            vector[j][k+2]=resultvec[k];
                        }
                        tmp=1;
                    }
                }
                if(tmp==0){ // 変数が存在していない場合
                    j=0;
                    rep(o,256){
                        resultvec[o]=0;
                    }
                    int dim;
                    dim=cal_vec(statement,i,3,0);
                    vector[vecmax][0]=(int)statement[i][1][0];
                    vector[vecmax][1]=dim;
                    rep(o,dim){
                        vector[vecmax][o+2]=resultvec[o];
                    }
                    vecmax++;
                }
            }
        }

        //　変数出力の場合
        else if(strlen(statement[i][0])==9){
            // int
            if(statement[i][0][6]=='i'){
                printf("%d\n",cal(statement,i,1,0));
            }
            // vec
            if(statement[i][0][6]=='v'){
                rep(h,256)  resultvec[h]=0;
                int dim=0;
                dim=cal_vec(statement,i,1,0);
                printf("[");
                rep(h,dim){
                    printf("%d",resultvec[h]);
                    printf(" ");
                }
                printf("]\n");
            }
        }
    }
}
