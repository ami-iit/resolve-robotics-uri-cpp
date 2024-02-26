// SPDX-FileCopyrightText: Fondazione Istituto Italiano di Tecnologia (IIT)
// SPDX-License-Identifier: BSD-3-Clause

#ifndef RESOLVE_ROBOTICS_URI_CPP_H
#define RESOLVE_ROBOTICS_URI_CPP_H

#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace ResolveRoboticsURICpp
{

std::string cleanPathSeparator(const std::string& filename, const bool isWindows)
{
    std::string output = filename;
    char pathSeparator = isWindows ? '\\' : '/';
    char wrongPathSeparator = isWindows ? '/' : '\\';
    for (size_t i = 0; i < output.size(); ++i)
    {
        if (output[i] == wrongPathSeparator)
        {
            output[i] = pathSeparator;
        }
    }
    return output;
}

bool isFileExisting(const std::string& filename)
{
    if (FILE* file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
        return true;
    } else
    {
        return false;
    }
}

bool getFilePath(const std::string& filename,
                 const std::string& prefixToRemove,
                 const std::unordered_set<std::string>& paths,
                 const bool isWindows,
                 std::string& outputFileName)
{

    if (filename.substr(0, prefixToRemove.size()) == prefixToRemove)
    {
        std::string filename_noprefix = filename;
        filename_noprefix.erase(0, prefixToRemove.size());
        for (const std::string& path : paths)
        {
            const std::string testPath = cleanPathSeparator(path + filename_noprefix, isWindows);
            if (isFileExisting(testPath))
            {
                outputFileName = testPath;
                return true;
            } else
            {
                return false;
            }
        }
    }
    outputFileName = filename; // By default return the input;
    return false;
}

std::optional<std::string>
resolveRoboticsURI(const std::string& uriFilename, std::string& errorMessage)
{
    bool isWindows = false;
#ifdef _WIN32
    isWindows = true;
#endif

    // If file starts with file:/, remove file:/ and return if it exists
    std::string fileUriPrefix = "file:/";
    if (uriFilename.substr(0, fileUriPrefix.size()) == fileUriPrefix)
    {
        std::string uriFilename_noprefix = uriFilename;
        uriFilename_noprefix.erase(0, fileUriPrefix.size());

        if (isFileExisting(uriFilename_noprefix))
        {
            return uriFilename_noprefix;
        }
    }

    // If the file exists with removing any prefix, just return it
    if (isFileExisting(uriFilename))
    {
        return uriFilename;
    }

    std::string packageUriPrefix = "package:/";
    std::string modelUriPrefix = "model:/";

    if (uriFilename.substr(0, packageUriPrefix.size()) != packageUriPrefix
        && modelUriPrefix.substr(0, modelUriPrefix.size()) != modelUriPrefix)
    {
        // The uri does not start with file:/ (earlier check, is not a file path
        // or start with model:/ or package:/, we can't resolve it
        errorMessage = "ResolveRoboticsURICpp: URI " + uriFilename
                       + " is not a existing file path and does not start with file:/, package:/ "
                         "or model:/, it is not possible to resolve it.";
        return {};
    }

    // Actually solve it

    // At this point, let's process the actual case of package:/ or model:/ URIs
    std::unordered_set<std::string> pathList;

    // List of variables that contain <prefix>/share paths
    std::vector<std::string> envListShare = {"ROS_PACKAGE_PATH",
                                             "GAZEBO_MODEL_PATH",
                                             "SDF_PATH",
                                             "IGN_GAZEBO_RESOURCE_PATH",
                                             "GZ_SIM_RESOURCE_PATH"};

    // List of variables that contains <prefix> paths (to which /share needs
    // to be added)
    std::vector<std::string> envListPrefix = {"AMENT_PREFIX_PATH"};

    // Extract list of directories where to search for the file
    for (size_t i = 0; i < envListShare.size(); ++i)
    {
        const char* env_var_value = std::getenv(envListShare[i].c_str());
        if (env_var_value)
        {
            std::stringstream env_var_string(env_var_value);
            std::string individualPath;
            while (std::getline(env_var_string, individualPath, isWindows ? ';' : ':'))
            {
                pathList.insert(individualPath);
            }
        }
    }

    for (size_t i = 0; i < envListPrefix.size(); ++i)
    {
        const char* env_var_value = std::getenv(envListPrefix[i].c_str());

        if (env_var_value)
        {
            std::stringstream env_var_string(env_var_value);

            std::string individualPath;

            while (std::getline(env_var_string, individualPath, isWindows ? ';' : ':'))
            {
                pathList.insert(individualPath + "/share");
            }
        }
    }

    std::string realAbsoluteFileName;
    bool ok = getFilePath(uriFilename, "package:/", pathList, isWindows, realAbsoluteFileName);

    if (ok)
    {
        return realAbsoluteFileName;
    } else
    {
        errorMessage = "Impossible to resolve uri " + uriFilename;
        return {};
    }
}

std::optional<std::string> resolveRoboticsURI(const std::string& uriFilename)
{
    std::string dummyErrorMessage;
    return resolveRoboticsURI(uriFilename, dummyErrorMessage);
}

} // namespace ResolveRoboticsURICpp

#endif