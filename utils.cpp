#include "crossPlatform.h"

// ����ļ��Ƿ����
bool fileExists(const string& filename) {
	ifstream file(filename);
	return file.good();
}

// ��ȡ�ļ�·���е��ļ�������
string extractFileNameWithoutExtension(const string& filePath) {
	string fileName = filesystem::path(filePath).filename().string();

	// Ѱ���ļ��������һ�� '.' ��λ��
	size_t pos = fileName.find_last_of(".");

	// ����ҵ���'.'���򷵻ز�����׺���ļ���
	if (pos != string::npos) {
		return fileName.substr(0, pos);
	}

	// ���û���ҵ�'.'��ֱ�ӷ����ļ���
	return fileName;
}

// �ļ�����
bool copyFile(const string& sourceFilename, const string& destFilename) {
	ifstream sourceFile(sourceFilename, ios::binary); // ��Դ�ļ�
	if (!sourceFile) {
		cerr << "Can not open the source file:" + sourceFilename << endl;
		return false;
	}

	ofstream destFile(destFilename, ios::binary | ios::trunc); // ����Ŀ���ļ�
	if (!destFile) {
		cerr << "Can not Create the destination file." << endl;
		return false;
	}

	destFile << sourceFile.rdbuf(); // ��Դ�ļ����ݸ��Ƶ�Ŀ���ļ�

	sourceFile.close();
	destFile.close();

	return true;
}


// �滻�ļ�����
bool replaceStringInFile(const string& filename, const string& targetString, const string& replacement) {
	ifstream file(filename, ios::in | ios::binary); // �Զ����Ʒ�ʽ���ļ�
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

	ofstream outFile(filename, ios::out | ios::binary); // �Զ����Ʒ�ʽ������ļ�
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

	// ���ƫ���Ƿ����ļ���Χ��
	file.seekp(0, ios::end);
	if (offset > file.tellp()) {
		cout << "Error: Offset is beyond the end of the file!" << endl;
		file.close();
		return false;
	}

	// ��λ��ָ����ƫ��λ��
	file.seekp(offset);

	// д�� 16 ��������
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
