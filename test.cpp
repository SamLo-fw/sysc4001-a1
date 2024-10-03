#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

struct Instruction{
    char* full_name;
    int timespan;
    int isr_num;
};

int main(int argc, char **argv){

    std::vector<Instruction> execute_table;
    std::vector<std::string> fileLines;
    std::string line;

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
    std::ifstream file(furl_full);
    printf("trying to locate trace file %s \n", furl_full);
    if(!file.is_open()){
        perror("could not find trace file.");
        return 1;
    }
    printf("found file %s \n", furl_full);

    //load all the trace file lines into a vector
    while(getline(file, line)){
        fileLines.push_back(line);
    }

    //parse line into length and command type
    std::string current_line;
    int delim_index;
    int time_length;
    int isr_number;
    std::string instruct_type;
    for(int i = 0; i<fileLines.size(); i++){
        current_line = fileLines[i];
        delim_index = current_line.find(',');
        if(delim_index == -1){
            printf("on line %d of file %s:", i+1, furl_full);
            perror("comma not found in line");
            return 1;
        }
        instruct_type = current_line.substr(0, delim_index);
        time_length = stoi(current_line.substr(delim_index + 2, current_line.length()));
        std::string isr_str = instruct_type.substr(instruct_type.find(' ') + 1, instruct_type.length());
        if(!isr_str.compare(instruct_type)){
            isr_number = stoi(isr_str);
        }else{
            isr_number = -1;
        }
        std::cout<<instruct_type<<std::endl;
        std::cout<<time_length<<std::endl;
        std::cout<<isr_number<<std::endl;
    }



    //write output
    FILE *optr;
    printf("trying to write output file");
    if((optr = fopen(".\\execution1.txt", "w")) == NULL){
        perror("could not open output file.");
        return 1;
    }



    //close open files
    fclose(optr);
    file.close();
    //free mallocs before return
    free(furl_full);

    return 0;
}