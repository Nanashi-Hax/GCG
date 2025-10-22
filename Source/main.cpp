#include <array>  // array
#include <cstdio>    // _popen
#include <iostream>  // cout
#include <memory>    // shared_ptr
#include <filesystem>

bool ExecuteCommand(char const *const command, int& exitCode)
{
    std::shared_ptr<FILE> pipe(
        _popen(command, "r"),
        [&](FILE* p)
        {
            exitCode = _pclose(p);
        }
    );

    if (!pipe)
    {
        return false;
    }

    std::array<char, 256> buffer;
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            std::cout << buffer.data() << std::endl;
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    std::filesystem::path exe(argv[0]);
    std::filesystem::path tools(exe.parent_path() / "Tools");
    std::filesystem::path as(tools / "powerpc-eabi-as.exe");
    std::filesystem::path nm(tools / "powerpc-eabi-nm.exe");
    std::filesystem::path objcopy(tools / "powerpc-eabi-objcopy.exe");

    std::filesystem::path source(exe.parent_path() / "Source" / "Test.s");
    std::filesystem::path object(exe.parent_path() / "Object" / "Test.o");
    std::filesystem::path binary(exe.parent_path() / "Binary" / "Test.bin");

    std::string command;
    int exitCode;

    command = as.string() + " -mregnames -mgekko " + source.string() + " -o " + object.string();
    ExecuteCommand(command.c_str(), exitCode);

    command = nm.string() + " " + object.string();
    ExecuteCommand(command.c_str(), exitCode);

    command = objcopy.string() + " -O binary " + object.string() + " " + binary.string();
    ExecuteCommand(command.c_str(), exitCode);

    return exitCode;
}