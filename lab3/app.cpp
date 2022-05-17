#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#include "define.h"
#include "data.h"

#define step (numprocs - 1)

int main(int argc,char *argv[]){
    matrix M(m,n,matrownum,matindex,matvalues);
    vec V(n,vecvalues);
    int myid, numprocs;
    my_data tmp;

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if(myid == step){
        int i = 0;
        double * array = (double *)malloc(sizeof(double) * m);
        while (i < m){
            MPI_Recv(&tmp,sizeof(my_data),MPI_BYTE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
            array[tmp.n] = tmp.values;
            i++;
        }
        printf("result\n");
        for(int i = 0; i < m; ++i)
            printf("%lf\n",array[i]);
    }
    if(myid < step){
        for(int i = myid; i < m; i += step){
            tmp.values = 0;
            tmp.n = i;
            for(int j = 0; j < M.rownum[i]; ++j)
                tmp.values += M.values[i][j] * V.values[M.index[i][j]];

            MPI_Send(&tmp,sizeof(my_data),MPI_BYTE,numprocs-1,0,MPI_COMM_WORLD);
        }  
    }

    MPI_Finalize();

    return 0;
}


