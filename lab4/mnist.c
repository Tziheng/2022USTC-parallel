#include <stdio.h>


typedef struct image{
    unsigned char label;
    unsigned char pixel[28*28];
}image;


int main(){
    image tmp;
    FILE *fplabel, *fpimage, *fpb;
    unsigned int magic_number, number_of_items, number_of_images, number_of_rows, number_of_columns;
    char bname[50];

//train  60000张图片生成
    if(!(fplabel = fopen("train-labels.idx1-ubyte","rb")) || !(fpimage = fopen("train-images.idx3-ubyte","rb")) )
        return printf("can't open file!\n");
    fread(&magic_number,4,1,fplabel);
    fread(&number_of_items,4,1,fplabel);
    printf("label:\nmagic_number:%x\nnumber_of_items:%x\n",magic_number,number_of_items);
    fread(&magic_number,4,1,fpimage);
    fread(&number_of_images,4,1,fpimage);
    fread(&number_of_rows,4,1,fpimage);
    fread(&number_of_columns,4,1,fpimage);
    printf("image:\nmagic_number:%x\number_of_images:%x\nnumber_of_rows:%x\nnumber_of_columns:%x\n",magic_number,number_of_images,number_of_rows,number_of_columns);
    /// 若发现以上输出恰好与实际倒序，大小端问题，

    for(int i = 0; i < 60000; ++i){
        fread(&tmp.label,1,1,fplabel);  
        fread(tmp.pixel,1,28*28,fpimage);
        sprintf(bname,"./train/%d.in",i);       // bname 是输出文件名字
        if( !(fpb = fopen(bname, "wb")))    
            return printf("can't create file %s\n",bname);

        fwrite(&tmp,sizeof(tmp),1,fpb); // 写二进制文件
        fclose(fpb);
    }
    fclose(fplabel);
    fclose(fpimage);

//t10k 10000张图片生成
    if(!(fplabel = fopen("t10k-labels.idx1-ubyte","rb")) || !(fpimage = fopen("t10k-images.idx3-ubyte","rb")) )
        return printf("can't open file!\n");
    fread(&magic_number,4,1,fplabel);
    fread(&number_of_items,4,1,fplabel);
    printf("label:\nmagic_number:%x\nnumber_of_items:%x\n",magic_number,number_of_items);
    fread(&magic_number,4,1,fpimage);
    fread(&number_of_images,4,1,fpimage);
    fread(&number_of_rows,4,1,fpimage);
    fread(&number_of_columns,4,1,fpimage);
    printf("image:\nmagic_number:%x\number_of_images:%x\nnumber_of_rows:%x\nnumber_of_columns:%x\n",magic_number,number_of_images,number_of_rows,number_of_columns);
    /// 若发现以上输出恰好与实际倒序，大小端问题，

    for(int i = 0; i < 10000; ++i){
        fread(&tmp.label,1,1,fplabel);  
        fread(tmp.pixel,1,28*28,fpimage);
        sprintf(bname,"./t10k/%d.in",i);       // bname 是输出文件名字
        if( !(fpb = fopen(bname, "wb")))    
            return printf("can't create file %s\n",bname);

        fwrite(&tmp,sizeof(tmp),1,fpb); // 写二进制文件
        fclose(fpb);
    }
    fclose(fplabel);
    fclose(fpimage);

    return 0;
}