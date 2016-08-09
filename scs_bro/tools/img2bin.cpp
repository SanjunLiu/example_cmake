/* @Brief this is the tool that translate the input image to binary data
 * @Param argv[1]: the input image file
 * @Param argv[2]: the output binary file
 */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <CImg.h>


using namespace cimg_library;
int main(int argc, char *argv[]){

    if(argc != 6){
        fprintf(stderr, "usage: %s inputfile  outputfile W H C\n", argv[0]);
        exit(0);
    }
    const char *input_name = argv[1];
    const char *output_name = argv[2];
    const size_t W = (size_t)atoi(argv[3]);
    const size_t H = (size_t)atoi(argv[4]);
    const size_t C = (size_t)atoi(argv[5]);
    CImg<unsigned char> img(input_name);
    img.resize(W, H, C);

    printf("Channel C is %zu.\n", C);

    float *buffer = (float*) malloc(sizeof(float)*W*H*C);
    for(int h=0; h<H; h++)
    {
        for(int w=0; w<W; w++)
        {
            for(int c=0; c<C; c++){
                buffer[(h*W+w)*C+c] =img(w, h, 0, c);
            }
        }
    }

    size_t needSize = W*H*C;

    //print
#if defined (DEBUG)
    for(size_t i = 0; i < needSize; i++)
        std::cout<<"buffer["<<i<<"] = "<<buffer[i]<<std::endl;
#endif

    FILE* fp = fopen(output_name, "wb");
    fwrite(buffer, sizeof(float), needSize, fp);
    fclose(fp);
    free(buffer);
    return 0;
}
