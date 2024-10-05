import sys
import json
import csv

#arg1 = filename
#arg2 = enable/disable array output

def get_value(arr, index):
    try:
        return arr[index]
    except IndexError:
        return 0

if __name__ == "__main__":
    filename = sys.argv[1]
    file = open(filename)

    cpu_arr = []
    io_arr = []
    sys_arr = []
    overhead_arr = []
    overhead_signs = ["switch", "context", "vector", "address", "IRET", "priority", "masked"]

    out_json = {}

    print("sorting data...")
    for line in file:
        is_overhead = False
        if(line.find("CPU") != -1): cpu_arr.append(int(line.split(",")[1]))
        if(line.find("SYSCALL") != -1): sys_arr.append(int(line.split(",")[1]))
        if(line.find("transfer") != -1): sys_arr.append(int(line.split(",")[1]))
        if(line.find("check for") != -1): sys_arr.append(int(line.split(",")[1]))
        if(line.find("END_IO") != -1): io_arr.append(int(line.split(",")[1]))
        for item in overhead_signs :
            if line.find(item) != -1: overhead_arr.append(int(line.split(",")[1]))
    
    print("calcing stats...")
    cpu_time = sum(cpu_arr)
    io_time = sum(io_arr)
    sys_time = sum(sys_arr)
    isr_time = io_time + sys_time
    overhead_time = sum(overhead_arr)
    total_time  = cpu_time + isr_time + overhead_time

    isr_arr = io_arr + sys_arr

    cpu_over_isr = round(float(cpu_time/isr_time),3)
    cpu_over_total = round(float(cpu_time/total_time),3)

    file_str = sys.argv[1].split(".")[0]
    json_filename =  file_str + "_output" + ".json" 
    csv_filename =  file_str + "_output" + ".csv" 
    print("outputting to files " + json_filename + " and " + csv_filename + "...")
    out_json["CPU_arr"] = cpu_arr
    out_json["IO_arr"] = io_arr
    out_json["SYSCALL_arr"] = sys_arr
    out_json["ISR_arr"] = isr_arr
    out_json["overhead_arr"] = overhead_arr

    transpose_json = []
    for i in range(len(overhead_arr)):
        new_row = {
            "CPU_arr": get_value(cpu_arr, i),
            "IO_arr": get_value(io_arr, i),
            "SYSCALL_arr": get_value(sys_arr, i),
            "ISR_arr": get_value(isr_arr, i),
            "overhead_arr": get_value(overhead_arr, i),
        }
        transpose_json.append(new_row)

    json_file = open(json_filename, "w")
    json_file.write(json.dumps(transpose_json))
    json_file.close()

    fields = ["CPU_arr", "IO_arr", "SYSCALL_arr", "ISR_arr", "overhead_arr"]
    csv_file = open(csv_filename, "w", newline="")
    csv_writer = csv.DictWriter(csv_file, fields)
    csv_writer.writeheader()
    for row in transpose_json:
        csv_writer.writerow(dict(zip(fields, list(row.values()))))
    csv_file.close()


    print("printing output...")
    if str.lower(sys.argv[2]) == "true":
        print("======ARRAY OUTPUT======")
        print("cpu array:")
        print(cpu_arr)
        print("io array:")
        print(io_arr)
        print("sys array:")
        print(sys_arr)
        print("isr array:")
        print(isr_arr)
        print("overhead array:")
        print(overhead_arr)
    
    print("======TIME INFORMATION======")
    print("total CPU time was " + str(cpu_time))
    print("total IO time was " + str(io_time))
    print("total SYSCALL time was " + str(sys_time))
    print("total ISR time was " + str(isr_time))
    print("total overhead time was " + str(overhead_time))
    print("total time was " + str(total_time))

    print("======RATIOS======")
    print("the ratio of time spent doing cpu to isr is " + str(cpu_over_isr))
    print("in total, the CPU was running for " + str(cpu_over_total) + " of the overall program runtime.")