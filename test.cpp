#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

struct Instruction{
    char* name;
    int timespan;
};

int main(int argc, char **argv){

    std::vector<Instruction> execute_table; 

    //exit the program if no trace file provided
    if(argc < 2){
        perror("error, missing trace filename");
        return 1;
    }

    //getting the file url based on the input argument
    char *furl_start = ".\\";
    char *furl_full = NULL;
    int furl_size = 2+strlen(argv[1]);
    furl_full = (char*) malloc(furl_size);

    //lol this is so cursed already
    strcpy(furl_full, furl_start);
    strcpy(furl_full + 2, argv[1]);

    //get the trace filepath 
    FILE *fptr;
    printf("trying to locate trace file %s \n", furl_full);
    if((fptr = fopen(furl_full, "r")) == NULL){
        perror("could not find trace file.");
        return 1;
    }
    printf("found file %s \n", furl_full);

    //write output
    FILE *optr;
    printf("trying to write output file");
    if((optr = fopen(".\\execution1.txt", "w")) == NULL){
        perror("could not open output file.");
        return 1;
    }

    fclose(fptr);
    //free mallocs before return
    free(furl_full);

    return 0;
}