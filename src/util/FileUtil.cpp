#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// 递归创建文件
bool createFile(const std::string& relativePath)
{
    std::ofstream ofs(relativePath, std::ios::binary);
    if (ofs)
    {
        ofs.close();
        return true;
    }

    try
    {
        fs::path filePath(relativePath);
        // 获取规范化的相对路径
        filePath = weakly_canonical(filePath);

        // 确保路径是相对于当前目录的
        if (filePath.is_absolute())
        {
            std::cerr << "Path must be relative: " << relativePath << std::endl;
            return false;
        }

        // 创建必要的目录
        fs::path parentDir = filePath.parent_path();
        if (!parentDir.empty() && !exists(parentDir))
        {
            create_directories(parentDir);
        }
        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}
