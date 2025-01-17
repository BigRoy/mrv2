// SPDX-License-Identifier: BSD-3-Clause
// mrv2
// Copyright Contributors to the mrv2 Project. All rights reserved.

#include <filesystem>
namespace fs = std::filesystem;

#include "mrvCore/mrvHome.h"
#include "mrvEdit/mrvEditUtil.h"

namespace mrv
{
    using namespace tl;

    bool isTemporaryEDL(const file::Path& path)
    {
        const std::string tmpdir = tmppath() + '/';

        auto dir = path.getDirectory();
        auto base = path.getBaseName();
        auto extension = path.getExtension();
        if (dir != tmpdir || base.substr(0, 5) != "EDL0x" ||
            extension != ".otio")
        {
            return false;
        }
        return true;
    }

    bool isTemporaryEDL(const std::string& filename)
    {
        file::Path path(filename);
        return isTemporaryEDL(path);
    }

    void removeTemporaryEDLs(ViewerUI* ui)
    {
        const std::string directory = tmppath();
        const char* pointer = "";
#ifdef _WIN32
        pointer = "0x";
#endif
        char prefix[256];
        snprintf(prefix, 256, "EDL%s%p", pointer, ui);

        try
        {
            for (const auto& entry : fs::directory_iterator(directory))
            {
                if (entry.is_regular_file() &&
                    entry.path().filename().string().find(prefix) == 0)
                {
                    // Remove the matching file
                    fs::remove(entry.path());
                }
            }
        }
        catch (const fs::filesystem_error& ex)
        {
        }
    }
} // namespace mrv
