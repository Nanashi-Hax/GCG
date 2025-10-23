#pragma once

#include <functional>
#include <string>

namespace Terminal
{
    class Process
    {
    private:
        class Closer
        {
        private:
            int& exitCode;

        public:
            Closer(int& exitCode) : exitCode(exitCode) {}

            void operator()(FILE* p) const;
        };

    private:
        std::string command;
        int exitCode;

        using Callback = std::function<void(std::string)>;
        Callback cb;

    public:
        Process(std::string command, Callback cb) : command(command), exitCode(0), cb(cb) {}

        int execute();
    };
}