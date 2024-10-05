#include "execute.hpp"

std::string istr_string_setup(int counter, int timespan){
    return std::to_string(counter) + ", " + std::to_string(timespan) + ", ";
}

std::string to_hex(int num){
    static const char* digits = "0123456789ABCDEF";
    std::string ret_str = "";
    int qt;
    while(num >= 1){
        qt = num % 16;
        num = num/16;
        ret_str = digits[qt] + ret_str;
    }
    while(ret_str.length() < 4){
        ret_str = "0" + ret_str;
    }
    ret_str = "0x" + ret_str;
    return ret_str;
}

int main(int argc, char **argv){
    std::vector<Instruction> instruction_table;
    std::vector<std::string> execute_table;
    std::vector<std::string> fileLines;
    std::string line;
    std::vector<std::string> vector_table;

    //exit the program if no trace file provided
    if(argc < 2){
        perror("error, missing trace filename");
        return 1;
    }

    //getting the file url based on the input argument
    const char *furl_start = ".\\";
    char *furl_full = NULL;
    int furl_size = 2+strlen(argv[1]);
    furl_full = (char*) malloc(furl_size);

    //lol this is so cursed already
    strcpy(furl_full, furl_start);
    strcpy(furl_full + 2, argv[1]);

    //get the trace filepath 
    std::ifstream trace_file(furl_full);
    printf("trying to locate trace file %s \n", furl_full);
    if(!trace_file.is_open()){
        perror("could not find trace file.");
        return 1;
    }
    printf("found file %s \n", furl_full);

    //load all the trace file lines into a vector
    while(getline(trace_file, line)){
        fileLines.push_back(line);
    }

    //parse line into length and command type
    std::string current_line;
    int delim_index;
    int time_length;
    int isr_number;
    std::string instruct_type;
    for(unsigned int i = 0; i<fileLines.size(); i++){
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
        if(isr_str.compare(instruct_type)){
            isr_number = stoi(isr_str);
        }else{
            isr_number = -1;
        }

        struct Instruction istr;
        istr.full_name = instruct_type;
        istr.isr_num = isr_number;
        istr.timespan = time_length;

        instruction_table.push_back(istr);
    }

    //load the vector table into a vector<int> array
    std::ifstream vector_file("vector_table.txt");
    printf("loading vector table...\n");
    if(!vector_file.is_open()){
        perror("could not find vector table.");
        return 1;
    }

    while(getline(vector_file, line)){
        vector_table.push_back(line);
    }
    printf("loaded vector table\n");

    //handle the instruction
    int pc_counter = 0;
    std::string istr_string;
    struct Instruction istr;
    for(unsigned int i = 0; i<instruction_table.size(); i++){
         istr = instruction_table[i];
         istr_string = "";
        if(!istr.full_name.compare("CPU")){
            istr_string =  istr_string_setup(pc_counter, istr.timespan) + "CPU execution";
            execute_table.push_back(istr_string);
            pc_counter += istr.timespan;
            continue;
        }

        if(istr.full_name.find("END_IO") != -1){
            istr_string = istr_string_setup(pc_counter, 1) + "check priority of interrupt";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "check if masked";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "switch to kernel mode";
            execute_table.push_back(istr_string);
            pc_counter ++;
            int context_save_time = rand() % 3 + 1;
            istr_string = istr_string_setup(pc_counter, context_save_time) + "context saved";
            execute_table.push_back(istr_string);
            pc_counter += context_save_time;
            istr_string = istr_string_setup(pc_counter, 1) + "find vector " + std::to_string(istr.isr_num) + " in memory position " + to_hex((istr.isr_num + 1) * 2);
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "load address " + vector_table[istr.isr_num] + " into the PC";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, istr.timespan) + "END_IO";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "IRET";
            execute_table.push_back(istr_string);
            pc_counter ++;
            continue;
        }

        if(istr.full_name.find("SYSCALL") != -1){
            istr_string = istr_string_setup(pc_counter, 1) + "switch to kernel mode";
            execute_table.push_back(istr_string);
            pc_counter ++;
            int context_save_time = rand() % 3 + 1;
            istr_string = istr_string_setup(pc_counter, context_save_time) + "context saved";
            execute_table.push_back(istr_string);
            pc_counter += context_save_time;
            istr_string = istr_string_setup(pc_counter, 1) + "find vector " + std::to_string(istr.isr_num) + " in memory position " + to_hex((istr.isr_num + 1) * 2);
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "load address " + vector_table[istr.isr_num] + " into the PC";
            execute_table.push_back(istr_string);
            pc_counter ++;
            int timespan_split = istr.timespan;
            int rand_time = rand() % (timespan_split/2);
            timespan_split -= rand_time;
            istr_string = istr_string_setup(pc_counter, rand_time) + "SYSCALL: run the ISR";
            execute_table.push_back(istr_string);
            pc_counter ++;
            rand_time = rand() % (timespan_split/2);
            timespan_split -= rand_time;
            istr_string = istr_string_setup(pc_counter, rand_time) + "transfer data";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, timespan_split) + "check for errors";
            execute_table.push_back(istr_string);
            pc_counter ++;
            istr_string = istr_string_setup(pc_counter, 1) + "IRET";
            execute_table.push_back(istr_string);
            pc_counter ++;
            continue;
        }

        printf("error on line %d with instruction %s", i, istr.full_name);
        perror("invalid instruction provided");
        return 1;
    }


    //write output
    printf("trying to write output file\n");

    std::string output_num = argv[1];
    int trace_index = output_num.find_first_of("1234567890");
    if(trace_index != std::string::npos){
        output_num = output_num.substr(trace_index);
    }
    std::string output_name = ".\\execution" + output_num;
    std::ofstream execute_file(output_name);
    std::cout << "outputting to " << output_name << std::endl;
    if(!execute_file.is_open()){
        perror("could not open output file.");
        return 1;
    }
    for(int i = 0; i<execute_table.size(); i++){
        execute_file << execute_table[i] << std::endl;
    }
    printf("output file written.\n");

    printf("closing files...\n");
    //close open files
    trace_file.close();
    execute_file.close();
    //free mallocs before return
    free(furl_full);
    
    return 0;
}