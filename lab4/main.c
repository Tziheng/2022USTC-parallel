#include "NN.h"
#include <stdio.h>
#include <stdlib.h>
#define times 100



typedef struct image{
    unsigned char label;
    unsigned char pixel[28*28];
}image;


int main(int argc,char *argv[]){
    NN * net = createNN(num_layers,num_layer_in,num_layer_hidden,num_layer_out);

    char bname[50];
    image tmp;
    double desired[10],alpha = 1.0;
    
    for(int i = 0,hitn = 0; i < 60000 ; i++){
        sprintf(bname,"./train/%d.in",i);
        FILE *fp = fopen(bname,"rb");
        fread(&tmp,sizeof(image),1,fp);
        fclose(fp);
        
        NNinput(net,tmp.pixel);
        forward_prop(net);
        if(tmp.label == NNoutput(net)) hitn++;
        for(unsigned char d = 0; d < 10; ++d)   desired[d] = tmp.label == d ? 1.0 : 0.0;
        back_prop(net,desired);
        update_weights(net,alpha);

        if(alpha > 0.001) alpha = alpha*0.999;
        if(i && !(i%times)){    // 每times统计一次命中率
            printf("%5dth  hit:%2d %% \n",i/times,hitn*100/times);
            hitn = 0;
        }
    }

       

    return 0;
}