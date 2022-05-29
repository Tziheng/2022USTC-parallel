#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "NN.h"

double sigmoid(double x){
    return 1.0/(1.0+exp(-x));
}
double dsigmoiddx(double x){
    double s = sigmoid(x);
    return s*(1.0-s);
}

NN* createNN(int num_layer,int num_neu_in, int num_neu_hide, int num_neu_out){
    NN* net = (NN*)malloc(sizeof(NN));
    net->num = num_layer;
    net->l = (layer*)malloc(sizeof(layer) * num_layer);


    // 输入层（i=0）;输出层（i=num_layers-1）;隐藏层(其他i)
    for(int i = 0; i < num_layer; ++i){
        int num = i == 0 ? num_neu_in : (i == num_layer - 1 ? num_neu_out : num_neu_hide);
        net->l[i].num = num;
        net->l[i].neu = (neuron *)malloc(sizeof(neuron)*num);
        for(int j = 0; j < num; ++j)
           net->l[i].neu[j].bias = 0.0;
    }

    // 初始化weights，赋 0~1的随机值，输入层没有weight
    for(int i = 1; i < num_layer; ++i){
        int size = net->l[i].num * net->l[i-1].num;
        net->l[i].weights = (double *)malloc(sizeof(double) * size);
        net->l[i].dweights = (double *)malloc(sizeof(double) * size);
        for(int j = 0; j < size;++j)
            net->l[i].weights[j] = 1.0*rand()/RAND_MAX - 0.5;
    }
    return net;
}

// 向前传递
void forward_prop(NN*net){
    for(int i = 1; i < net->num; ++i){
        for(int m = 0; m < net->l[i].num; ++m){
            net->l[i].neu[m].z = net->l[i].neu[m].bias;
            for(int n = 0; n < net->l[i-1].num; ++n)
                net->l[i].neu[m].z += net->l[i-1].neu[n].actv * net->l[i].weights[m*net->l[i-1].num + n];   // Z_i = bias + sum(a_{i-1}*weight)
            net->l[i].neu[m].actv = sigmoid(net->l[i].neu[m].z); // a = g(Z)
        }
    }
}

// 反向传递
void back_prop(NN*net,double *desired){ // 输入期望输出
    // 误差值，采用方差 J = 0.5 * Σ (a-y)^2

    // 那么输出单个结点的delta为 dJ/dz_i =  (a_i-y_i) * a_i' = (a_i-y_i) * dg(z_i)/dz_i
    // 已知l+1层，第l层隐藏层的delta为 dJ/dz_i = Σ dJ/dz^(l+1) * dz^(l+1)/dz_i = Σ δ^(l+1)_j * w_ji * dg(z_i)/dz_i
    // 第l层的 w 与 bias 的偏导 dJ/dw_ij = δ_i * a^(l-1)_j ，dJ/db = δ

    // 输出层
    
    for(int i = 0; i < net->l[net->num - 1].num; ++i){
        net->l[net->num - 1].neu[i].delta = (net->l[net->num - 1].neu[i].actv - desired[i]) * dsigmoiddx(net->l[net->num - 1].neu[i].z);
        for(int j = 0; j < net->l[net->num - 2].num; ++j)
            net->l[net->num - 1].dweights[i*net->l[net->num - 2].num + j] = net->l[net->num - 1].neu[i].delta * net->l[net->num - 2].neu[j].actv;
        net->l[net->num - 1].neu[i].dbias = net->l[net->num - 1].neu[i].delta;
    }

    // 隐藏层
    for(int l = net->num - 2; l > 0; --l){
        for(int i = 0; i < net->l[l].num; ++i){
            net->l[l].neu[i].delta = 0;
            for(int j = 0; j < net->l[l + 1].num; ++j)
                net->l[l].neu[i].delta += (net->l[l + 1].neu[j].delta * net->l[l + 1].weights[j * net->l[l].num + i]) * dsigmoiddx(net->l[l].neu[i].z);

            for(int j = 0; j < net->l[l - 1].num; ++j)
                net->l[l].dweights[i*net->l[l - 1].num + j] = net->l[l].neu[i].delta * net->l[l - 1].neu[j].actv;
            
            net->l[l].neu[i].dbias = net->l[l].neu[i].delta;
        }
    }

}

// 更新权重
void update_weights(NN*net,double alpha){   // alpha 是学习率
    for(int i = 1; i < net->num; ++i)
        for(int j = 0; j < net->l[i].num; ++j){
            for(int k = 0; k < net->l[i - 1].num;++k)
                net->l[i].weights[j * net->l[i - 1].num + k] -= alpha * net->l[i].dweights[j * net->l[i - 1].num + k]; 
            net->l[i].neu[j].bias -= alpha * net->l[i].neu[j].dbias;
        }
}

void NNinput(NN *net,unsigned char px[28*28]){
    for(int p = 0; p < 28*28; ++p)
        net->l[0].neu[p].actv = 1.0*px[p]/256;
}

int NNoutput(NN* net){
    int maxactv = 0,i = 1;
    for(double s = net->l[4].neu[0].actv; i < 10; ++i )
        if(s < net->l[4].neu[i].actv){
            s = net->l[4].neu[i].actv;
            maxactv = i;
        }
    return maxactv;
}

void NNprintf(NN*net){
    printf("NN output:%d\n",NNoutput(net));
    for(int i = 0; i < 10; ++i)
        printf("%2d:%lf\n",i,net->l[4].neu[i].actv);
}