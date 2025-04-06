#pragma once
#include <unordered_map>

inline int debugFlag =  0;
inline int optFlag = 0;
inline int warnFlag = 0;
inline char *outFilename = nullptr;

enum cmplFlag
{
    obj,
    ll
};
typedef std::unordered_map<char*, cmplFlag> OpMap;

inline OpMap opMap;

void cmd(int argc, char** argv);
void usage();
void cmpl();