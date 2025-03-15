#include <fstream>

#include "syntax//Serialize.h"

using namespace std;
using namespace syntax::lr;

// 序列化 LrHashEntry
void serializeLrHashEntry(std::ofstream& ofs, const LrHashEntry& entry)
{
    ofs.write(reinterpret_cast<const char*>(&entry.op), sizeof(entry.op));
    ofs.write(reinterpret_cast<const char*>(&entry.id), sizeof(entry.id));
}

// 反序列化 LrHashEntry
void deserializeLrHashEntry(std::ifstream& ifs, LrHashEntry& entry)
{
    ifs.read(reinterpret_cast<char*>(&entry.op), sizeof(entry.op));
    ifs.read(reinterpret_cast<char*>(&entry.id), sizeof(entry.id));
}

// 序列化 LrTable
void syntax::lr::serializeLrTable(const LrTable& table)
{
    string filename = "lrTable.bin";

    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs)
    {
        std::cerr << "Failed to open file for writing: " << filename << '\n';
        return;
    }

    // 写入表的大小
    size_t tableSize = table.size();
    ofs.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize));

    // 写入每个状态及其对应的条目
    for (const auto& [stateId, entries] : table)
    {
        // 写入状态 ID
        ofs.write(reinterpret_cast<const char*>(&stateId), sizeof(stateId));

        // 写入条目的大小
        size_t entrySize = entries.size();
        ofs.write(reinterpret_cast<const char*>(&entrySize), sizeof(entrySize));

        // 写入每个条目
        for (const auto& [token, entry] : entries)
        {
            // 写入 token
            ofs.write(reinterpret_cast<const char*>(&token), sizeof(token));

            // 写入 entry
            serializeLrHashEntry(ofs, entry);
        }
    }
}

// 反序列化 LrTable
void syntax::lr::deserializeLrTable(LrTable& table)
{
    string filename = "lrTable.bin";
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs)
    {
        std::cerr << "Failed to open file for reading: " << filename << '\n';
        return;
    }

    // 读取表的大小
    size_t tableSize;
    ifs.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    // 读取每个状态及其对应的条目
    for (size_t i = 0; i < tableSize; ++i)
    {
        LrStateId stateId;
        ifs.read(reinterpret_cast<char*>(&stateId), sizeof(stateId));

        // 读取条目的大小
        size_t entrySize;
        ifs.read(reinterpret_cast<char*>(&entrySize), sizeof(entrySize));

        // 读取每个条目
        for (size_t j = 0; j < entrySize; ++j)
        {
            size_t token;
            ifs.read(reinterpret_cast<char*>(&token), sizeof(token));

            LrHashEntry entry;
            deserializeLrHashEntry(ifs, entry);

            table[stateId][token] = entry;
        }
    }
}
