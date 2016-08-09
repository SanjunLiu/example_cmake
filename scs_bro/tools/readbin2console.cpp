// read binary data
#include "tools.hpp"

int main(int argc, char *argv[]){
   char *input_file;
   size_t print_num = 10;
   if(argc ==2){
        input_file = argv[1];
        printf("Usage: %s binary_file print_num (default 10)\n", argv[0]);
   }else if(argc ==3){
        input_file = argv[1];
        print_num = (size_t)atoi(argv[2]);
   }else{
        fprintf(stderr, "Usage: %s binary_file print_num (default 10)\n", argv[0]);
        return -1;
   }
   float *data = (float*)malloc(print_num*sizeof(float));
   readDataFromBinary(input_file, print_num, data);
   printNNumsData(print_num, 1, data);

   free(data);

   return 0;
}
