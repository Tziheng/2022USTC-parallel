#ifndef __NN_H__
#define __NN_H__

#define num_layers 5
#define num_layer_in 28*28
#define num_layer_hidden 100
#define num_layer_out 10

typedef struct neuron{  // 神经元
    double actv;        // 该神经元输出
    double bias;        // 偏差项
    double z;           // 该神经元加权输入总和

    // 反向传递参数
    double delta;       // 误差 δ         
    double dbias;
}neuron;

typedef struct layer{  // 层 
    int num;            // 该层神经元个数
    double *weights;    // 第l层到第l+1层的权重
    double *dweights;   // 反向传递参数
    neuron *neu;        // 神经元
}layer;

typedef struct NN{  // 神经网络
    int num;        // 层数
    layer *l;       // 层
}NN;

NN* createNN(int num_layer,int num_neu_in, int num_neu_hide, int num_neu_out);

void forward_prop(NN*net);

void back_prop(NN*net,double *desired);

void update_weights(NN*net,double alpha);

void NNinput(NN *net,unsigned char px[28*28]);

int NNoutput(NN* net);



#endif 

