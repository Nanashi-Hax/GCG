#include <filesystem>
#include <string>
#include <iostream>

#include "Terminal/Process.hpp"

void callback(std::string str)
{
    std::cout << str << std::endl;
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

    std::string str;
    int exitCode;

    str = as.string() + " -mregnames -mgekko --no-warn " + source.string() + " -o " + object.string();
    Terminal::Process assemble(str, callback);
    exitCode = assemble.execute();
    if(exitCode != 0) return exitCode;

    str = nm.string() + " " + object.string();
    Terminal::Process showSymbol(str, callback);
    exitCode = showSymbol.execute();
    if(exitCode != 0) return exitCode;

    str = objcopy.string() + " -O binary " + object.string() + " " + binary.string();
    Terminal::Process toBin(str, callback);
    exitCode = toBin.execute();
    if(exitCode != 0) return exitCode;

    return 0;
}