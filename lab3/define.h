#ifndef __DEFINE_H__
#define __DEFINE_H__

class matrix{// 采用ELL存储稀疏矩阵
public:
    int m,n;
    int *rownum;    // 行元素个数
    int **index;    // 存储列号
    double **values;   // 值存放处
    matrix(const char *FileName);
    matrix(int m,int n, int *rownum, int **index,double **values);

};

class vec{// 向量
public:
    int n;
    double *values;
    vec(const char *FileName);
    vec(int n,double *values);
};

typedef struct my_data{// MPI通信数据集
    double values;
    int n;
}my_data;

// 初始化：从文件读取创建一个稀疏矩阵
matrix::matrix(const char *FileName){
    FILE *fp;
    if(!(fp = fopen(FileName,"r"))) return;

    int id[1000],rn;
    double v[1000],tmp;

    fscanf(fp,"m=%d,n=%d",&m,&n);
    index = (int **)malloc(sizeof(int*) * m);
    values = (double **)malloc(sizeof(double*) * m);
    rownum = (int *)malloc(sizeof(int) * m);

    for(int i = 0; i < m; ++i){
        rn = 0;
        for(int j = 0; j < n; ++j){
            fscanf(fp,"%lf",&tmp);
            if(!tmp) continue;  // 读取到0，读取下一个
            id[rn]  = j;
            v[rn++] = tmp;
        }
        index[i] = (int*)malloc(sizeof(int)*rn);
        values[i] =(double*)malloc(sizeof(double)*rn);
        rownum[i] = rn;
        for(int j = 0;j < rn; ++j){
            index[i][j] = id[j];
            values[i][j] = v[j];
        }
    }
    fclose(fp);
}

// 初始化
matrix::matrix(int m,int n, int *rownum, int **index,double **values){
    this->m = m;
    this->n = n;
    this->rownum = rownum;
    this->index = index;
    this->values = values;
}

// 初始化:从文件读取创建一个稀疏矩阵
vec::vec(const char *FileName){
    FILE *fp;
    if(!(fp = fopen(FileName,"r"))) return;
    fscanf(fp,"n=%d",&n);
    values = (double *)malloc(sizeof(double)*n);

    for(int i = 0; i < n; ++i)
        fscanf(fp,"%lf",values+i);

    fclose(fp);
}

// 初始化
vec::vec(int n,double *values){
    this->n = n;
    this->values = values;
}

#endif
