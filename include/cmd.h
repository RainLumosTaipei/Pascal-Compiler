#pragma once
#include <string>
#include <unordered_map>

// 是否输出详细信息
inline int debugFlag =  0;
// 优化等级
inline int optFlag = 0;
// 警告等级
inline int warnFlag = 0;
// 合并文件名
inline std::string outFilename = "output/output.exe" ;
// 编译格式标记
enum cmplFlag
{
    obj,
    assembly,
    ll
};
typedef std::unordered_map<char*, cmplFlag> OpMap;

inline OpMap opMap;

void cmd(int argc, char** argv);
void usage();
void cmpl();