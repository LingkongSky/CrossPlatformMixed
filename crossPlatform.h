#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <filesystem>
#include "md5.h"

using namespace std;


bool fileExists(const string& filename);
string extractFileNameWithoutExtension(const string& filePath);
bool copyFile(const string& sourceFilename, const string& destFilename);
bool replaceStringInFile(const string& filename, const string& targetString, const string& replacement);
bool appendFileToFile(const string& sourceFileName, const string& targetFileName);
bool writeHexToFile(const string& filename, streampos offset, const unsigned char* data, size_t size);
bool createFolder(const std::string& folderPath);
