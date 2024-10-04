import random
import sys

#arg1 is file output name
#arg2 is how many interrupts to output
#arg3 is the max val for CPU
#arg4 is the max val for SYSCALL and END_IO

if __name__ == "__main__":
    random.seed()
    name = sys.argv[1]
    file = open(name, "w")
    length = int(sys.argv[2])
    cpu_max = int(sys.argv[3])
    max = int(sys.argv[4])
    for i in range(length):
        file.write("CPU, " + str(random.randint(6,cpu_max)) + "\n")
        file.write("SYSCALL " + str(random.randint(0,13)) + ", " +  str(random.randint(6,max))+ "\n")
        file.write("CPU, " + str(random.randint(6,cpu_max))+ "\n")
        file.write("END_IO " + str(random.randint(14,25)) + ", " +  str(random.randint(6,max))+ "\n")