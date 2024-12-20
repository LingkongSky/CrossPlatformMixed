#include "crossPlatform.h"

// 检查文件是否存在
bool fileExists(const string& filename) {
	ifstream file(filename);
	return file.good();
}

// 获取文件路径中的文件名部分
string extractFileNameWithoutExtension(const string& filePath) {
	string fileName = filesystem::path(filePath).filename().string();

	// 寻找文件名中最后一个 '.' 的位置
	size_t pos = fileName.find_last_of(".");

	// 如果找到了'.'，则返回不带后缀的文件名
	if (pos != string::npos) {
		return fileName.substr(0, pos);
	}

	// 如果没有找到'.'，直接返回文件名
	return fileName;
}

// 文件复制
bool copyFile(const string& sourceFilename, const string& destFilename) {
	ifstream sourceFile(sourceFilename, ios::binary); // 打开源文件
	if (!sourceFile) {
		cerr << "Can not open the source file:" + sourceFilename << endl;
		return false;
	}

	ofstream destFile(destFilename, ios::binary | ios::trunc); // 创建目标文件
	if (!destFile) {
		cerr << "Can not Create the destination file." << endl;
		return false;
	}

	destFile << sourceFile.rdbuf(); // 将源文件内容复制到目标文件

	sourceFile.close();
	destFile.close();

	return true;
}


// 替换文件内容
bool replaceStringInFile(const string& filename, const string& targetString, const string& replacement) {
	ifstream file(filename, ios::in | ios::binary); // 以二进制方式打开文件
	if (!file) {
		cerr << "Can not open the file:" + filename << endl;
		return false;
	}

	string content;
	string line;
	while (getline(file, line)) {
		size_t pos = 0;
		while ((pos = line.find(targetString, pos)) != string::npos) {
			line.replace(pos, targetString.length(), replacement);
			pos += replacement.length();
		}
		content += line + "\n";
	}
	file.close();

	ofstream outFile(filename, ios::out | ios::binary); // 以二进制方式打开输出文件
	if (!outFile) {
		cerr << "Can not write to the destination file." << endl;
		return false;
	}
	outFile << content;
	outFile.close();

	return true;
}


bool writeHexToFile(const string& filename, streampos offset, const unsigned char* data, size_t size) {
	ofstream file(filename, ios::binary | ios::out | ios::in);

	if (!file.is_open()) {
		cout << "Error opening file!" << endl;
		return false;
	}

	// 检查偏移是否在文件范围内
	file.seekp(0, ios::end);
	if (offset > file.tellp()) {
		cout << "Error: Offset is beyond the end of the file!" << endl;
		file.close();
		return false;
	}

	// 定位到指定的偏移位置
	file.seekp(offset);

	// 写入 16 进制内容
	file.write(reinterpret_cast<const char*>(data), size);
	file.close();

	cout << "Data written successfully!" << endl;
	return true;

}

bool appendFileToFile(const string& sourceFileName, const string& targetFileName) {
	ifstream sourceFile(sourceFileName, ios::binary);
	ofstream targetFile(targetFileName, ios::app | ios::binary);

	if (sourceFile.is_open() && targetFile.is_open()) {
		char buffer[1024];
		streamsize bytesRead = 0;

		while (!sourceFile.eof()) {
			sourceFile.read(buffer, sizeof(buffer));
			bytesRead = sourceFile.gcount();
			targetFile.write(buffer, bytesRead);
		}

		sourceFile.close();
		targetFile.close();
		return true;
	}
	else {
		cerr << "Can not open the file:" + sourceFileName << endl;
		return false;
	}
}


bool createFolder(const string& folderPath) {
	if (filesystem::exists(folderPath) && filesystem::is_directory(folderPath)) {
		return true;
	}

	try {
		if (filesystem::create_directory(folderPath)) {
			return true;
		}
		else {
			cerr << "Can not creat the folder:" + folderPath << endl;
			return false;
		}
	}
	catch (const exception& e) {
		cerr << "Can not creat the folder" << e.what() << endl;
		return false;
	}
}
