#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 10000 //How data you want to get and copy

void help(); //help user function
void copy_mmap(char* fd_from, char* fd_to); //1rst way of copy function
void copy_read_write(char* fd_from, char* fd_to); //2nd way of copy function

bool is_file_existing(const char* filename); //looking for existing file function

int main(int argc, char** argv) //Starting the main program
{
    if (argc == 1 || argc > 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-H") == 0) //looking the arguments
    {
        help(); //Calling help function in this case
        return 0;
    }
    else
    {
        if (strcmp(argv[1], "-m") == 0 && argc == 4) //if -m
        {
            if (is_file_existing(argv[2])) //looking if the file exist then if yes
            {
                copy_mmap(argv[2], argv[3]);//copy the file
            }
            else//if file don't exist
            {
                printf("No such file named %s\n", argv[2]);//telling the user the file doesnt exist
                help(); // And print him the help funvtion
                return 0;
            }
        }
        else //Part where the number of arguments is not good
        {
            if (strcmp(argv[1], "-m") == 0)//not enough arguments
            {
                printf("\nInvalid number of arguments\n"); //telling the user there is not enough arguments
                help(); // And print him the help funvtion
                return 0;
            }
            if (*argv[1] == *"-")
            {
                printf("\ncopy: invalid option -- '%s'\n", argv[1]); //telling the user the forget the argument
                help(); // And print him the help funvtion
                return 0;
            }
            if (argc != 3) //not enough arguments
            {
                printf("\nInvalid number of arguments\n"); //telling the user there is not enough or too much arguments
                help(); // And print him the help funvtion
                return 0;
            }
            if(is_file_existing(argv[1])) // copy without -m but this file specified exist
            {
                copy_read_write(argv[1], argv[2]); //copy the file
            }
            else // copy without -m but this file doesnt exist
            {
                printf("No such file named %s\n", argv[1]); //telling the user the file doesnt exist
                help(); // And print him the help funvtion
                return 0;
            }
        }
    }
}

void help() //Help if the user don't know what args to use
{
    printf("\ncopy [-m] <file_name> <new_file_name>\n");
    printf("copy [-h]\n\n\n");
    printf("  -m use memory map\n");
    printf("  -h, --help display this help and exit\n\n");
}

void copy_mmap(char* fd_from, char* fd_to) //copy function with data in memory
{
    int source, target;
    char *sourcePtr, *targetPtr;
    size_t sourceFileSize = 0;
    source = open(fd_from, O_RDONLY); // Open source file
    sourceFileSize = lseek(source, 0, SEEK_END); // Compute the size of the source file
    sourcePtr = mmap(NULL, sourceFileSize, PROT_READ, MAP_PRIVATE, source, 0); // Copy source file in memory
    target = open(fd_to, O_CREAT | O_RDWR, 0666); // Create a file named 'argv[3]' with the rights 0666
    ftruncate(target, sourceFileSize);// Set the byte length
    targetPtr = mmap(NULL, sourceFileSize, PROT_READ | PROT_WRITE, MAP_SHARED, target, 0); // Copy target file in memory
    memcpy(targetPtr, sourcePtr, sourceFileSize); // Replace targetPtr content by the sourcePtr content
    munmap(sourcePtr, sourceFileSize); // Delete memory allocated to sourcePtr and targetPtr
    munmap(targetPtr, sourceFileSize);
    printf("%s successfully copied in %s ... \n", fd_from, fd_to);
    close(source);
    close(target);
}

void copy_read_write(char* fd_from, char* fd_to) //copy function from file to file
{
    char buffer[BUFF_SIZE];
    int readfile, source, target;
    source = open(fd_from, O_RDONLY); // Open source file
    target = open(fd_to, O_CREAT | O_WRONLY, 0666); // Create a file named 'argv[2]' with the rights 0666
    readfile = read(source, buffer, sizeof(buffer));
    write(target, buffer, readfile);
    printf("%s successfully copied in %s ... \n", fd_from, fd_to);
    close(source);
    close(target);
}

bool is_file_existing(const char* filename) //function that check if a specified file is existing of not
{
  FILE *file;
  if (file = fopen(filename, "r"))
  {
      fclose(file);
      return true;
  }
  return false;
}
