#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>

struct Instruction{
    std::string full_name;
    int timespan;
    int isr_num;
};

std::string istr_string_setup(int counter, int timespan);
std::string to_hex(int num);
