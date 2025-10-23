#include <cstdio>
#include <memory>
#include <stdexcept>

#include "Terminal/Process.hpp"

namespace Terminal
{
    void Process::Closer::operator()(FILE* p) const
    {
        exitCode = _pclose(p);
    }

    int Process::execute()
    {
        {
            std::unique_ptr<FILE, Closer> pipe(_popen(command.c_str(), "r"),Closer(exitCode));
            if (!pipe)
            {
                throw std::runtime_error("Failed to open pipe with command: " + command);
            }
        
            if(cb)
            {
                std::string line;
                while (true)
                {
                    int ret = fgetc(pipe.get());
                    if(ret == EOF) break;
                
                    char c = static_cast<char>(ret);
                    if(c == '\n')
                    {
                        cb(line);
                        line.clear();
                    }
                    else
                    {
                        line.push_back(c);
                    }
                }
                if(!line.empty()) cb(line);
            }
        }
        return exitCode;
    }
}