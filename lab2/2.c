#include <stdio.h>
#include <omp.h>
#include <Windows.h>

#define P 4

#define MAXN 4096
#define infinite 2147483648


unsigned int W[MAXN][MAXN] = {0};
unsigned int D[MAXN] = {0};

int n,s;            // 顶点数，应当小于MAXN
int VL[MAXN] = {0}; // VL[n] = 0 表示未取，1 表示已取    
int VLn = 0;        // VL中已取的个数

void initializeDist(){
    #pragma omp parallel
    {
        for(int i = omp_get_thread_num(); i < n; i+=P)
            D[i] = infinite;
    }
}

void Dijkstra() {
    
    initializeDist();
    D[s] = 0;

    while (VLn != n) {
        int u[P],u0 = 0;
        unsigned int minD[P];

        #pragma omp parallel
        {
            int p = omp_get_thread_num();
            minD[p] = infinite;
            for(int i = p; i < n; i+=P)
                if(!VL[i] && D[i] < minD[p]){
                    u[p] = i;
                    minD[p] = D[i];
                }
        }
        for(int i = 0; i < P; ++i)
            if(minD[i] < minD[u0])
                u0 = i;
        u0 = u[u0];

        #pragma omp parallel
        {
            for(int v = omp_get_thread_num(); v < n; v+=P)
                if(!VL[v] && W[u0][v] < infinite)
                    if(D[u0] + W[u0][v] < D[v])
                        D[v] = D[u0] + W[u0][v];
        }

        VL[u0] = 1;
        VLn++;
    }
}


int main(int argc, char* argv[]){
    omp_set_num_threads(P);
    FILE * fp;
    if(!(fp = fopen(argv[1],"r"))) return printf("can't open the file:%s",argv[1]);
    fscanf(fp,"n=%d,s=%d",&n,&s);
    for(int i = 0,tmp; i < n; ++i)
        for(int j = 0; j < n; ++j){
            fscanf(fp,"%d",&tmp);
            W[i][j] = tmp >= 0 ? tmp : infinite;
        }
    SYSTEMTIME t0,t1;
    GetSystemTime(&t0);
    
    Dijkstra();
    
    GetSystemTime(&t1);
    printf("time:%u ms\n",t1.wMilliseconds-t0.wMilliseconds);
    for(int i = 0; i < n; ++i)
        D[i] >= infinite ? printf("~") : printf("%u,",D[i]);
    return 0;
}