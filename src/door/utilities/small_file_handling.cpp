#include <string>
#include "small_file_handling.h"
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void write_string_to_file(const std::string &dest_filepath, const std::string &content)
{
    int dest_fd = open(dest_filepath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if(dest_fd < 0)
    {
        throw std::runtime_error("Error opening file for writing string");
    }

    ssize_t nbytes_written = write(dest_fd, content.c_str(), content.length());

    if(nbytes_written != (ssize_t) content.length())
    {


        throw std::runtime_error("Error writing string to file");
    }

    close(dest_fd);
}

void write_number_to_file(const std::string &dest_filepath,const int number)
{
    int dest_fd = open(dest_filepath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if(dest_fd < 0)
    {
        throw std::runtime_error("Error opening file for writing number");
    }

    ssize_t nbytes_written = write(dest_fd,(const void*) &number, sizeof(int));

    if(nbytes_written != (ssize_t) sizeof(int))
    {
        throw std::runtime_error("Error writing number to file");
    }

    close(dest_fd);
}

int read_number_from_file(const std::string &source_filepath)
{
    int source_fd = open(source_filepath.c_str(), O_RDONLY , S_IRUSR | S_IWUSR);
    if(source_fd < 0)
    {
        throw std::runtime_error("Error opening file for reading number");
    }


    int number_to_read = 0;

    ssize_t nbytes_read = read(source_fd, (void*) &number_to_read, sizeof(int));

    if(nbytes_read != (ssize_t) sizeof(int))
    {
        throw std::runtime_error("Error reading number from file");
    }

    close(source_fd);

    return number_to_read;
}
 
std::string read_string_from_file(const std::string &source_filepath)
{
    int source_fd = open(source_filepath.c_str(), O_RDONLY , S_IRUSR | S_IWUSR);
    if(source_fd < 0)
    {
        throw std::runtime_error("Error opening file for reading string");
    }

    char buffer[1024];
    size_t buffer_size = sizeof(buffer);


    ssize_t nbytes_read = read(source_fd, buffer, buffer_size);

    if(nbytes_read < 0)
    {
        throw std::runtime_error("Error reading string from file");
    }

    close(source_fd);

    std::string return_string(buffer, nbytes_read);

    return return_string;
}

bool dir_exists(const std::string &dirpath)
{
    if(access(dirpath.c_str(), W_OK) < 0)
      return false;
    else
      return true;
}

bool file_exists(const std::string &filepath)
{
    if(access(filepath.c_str(), W_OK) < 0)
      return false;
    else
      return true;
}