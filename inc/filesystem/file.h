#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <string_view>
#include <cstring> //for memcpy

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

constexpr std::size_t DEFAULT_BUFFER_SIZE = 8192;

class File {
public:
    File(const char* path) : _path(path), _file(-1) {}

    void open(bool read)
    {
        // Close previously opened file
        assert(!isFileOpened() && "File is already opened!");
        if (isFileOpened())
            close();

        _file = ::open(_path.data(), O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (_file < 0)
            std::cout << "can not open file\n";

        // Initialize file read buffer
        _read = read;
        _read_index = 0;
        _read_size = 0;
        if (read)
            _read_buffer.resize(DEFAULT_BUFFER_SIZE);
    }

    void close() {
        int result = ::close(_file);
        if (result != 0)
            std::cout << "can not close the file descriptor\n";

        _file = -1;

        // Clear file read buffer
        _read = false;
        _read_index = 0;
        _read_size = 0;
        _read_buffer.clear();
    }

    bool isFileOpened()const {
        return (_file >= 0);
    }
    
    size_t read(void* buffer, size_t size)
    {
        if ((buffer == nullptr) || (size == 0))
            return 0;

        // Read file with zero buffer
        if (_read_buffer.empty())
        {
            ssize_t result = ::read(_file, buffer, size);
            if (result < 0)
                std::cout << "can not read from file\n";
            return (size_t)result;
        }

        uint8_t* bytes = (uint8_t*)buffer;
        size_t counter = 0;

        while (size > 0)
        {
            // Update the local read buffer from the file
            if (_read_index == _read_size)
            {
                _read_index = 0;
                ssize_t result = ::read(_file, _read_buffer.data(), _read_buffer.size());
                if (result < 0)
                    std::cout << "can not read from file\n";
                _read_size = (size_t)result;

                // Stop if the end of file was met
                if (_read_size == 0)
                    break;
            }

            // Read remaining data form the local read buffer
            size_t remain = _read_size - _read_index;
            size_t num = (size < remain) ? size : remain;
            std::memcpy(bytes, (_read_buffer.data() + _read_index), num);
            counter += num;
            _read_index += num;
            bytes += num;
            size -= num;
        }

        return counter;
    }


private:
    std::string_view _path;
    int _file;

    // File read buffer
    bool _read;
    size_t _read_index;
    size_t _read_size;
    std::vector<uint8_t> _read_buffer;
};