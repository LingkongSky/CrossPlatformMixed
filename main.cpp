#include "crossPlatform.h"

/*
待办：
有第三个参数则设置输出文件地址
无第三个参数则文件名默认为windows文件名
*/


// 通过linux和win对于指令解释的差异来执行linux
// 修改exe文件的pe部分实现注释 将拥有可执行功能的shell脚本添加到文件末尾 将linux可执行文件添加到文件末尾

int main(int argc, char** argv) {

	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " <file_path_1> <file_path_2>" << endl;
		return 1;
	}
	// 传入两个程序的路径
	string file1 = argv[1];
	string file2 = argv[2];

	// 获取win架构的程序路径
	string win_path = "";
	// 获取linux架构的程序路径
	string linux_path = "";


	// 检查传入参数与文件
	if (!fileExists(file1)) {
		cerr << "File 1 does not exist: " << file1 << endl;
		return 1;
	}

	if (!fileExists(file2)) {
		cerr << "File 2 does not exist: " << file2 << endl;
		return 1;
	}

	if (file1.substr(file1.size() - 4) == ".exe") {
		win_path = file1;
		linux_path = file2;
	}
	else if (file2.substr(file1.size() - 4) == ".exe") {
		win_path = file2;
		linux_path = file1;
	}
	else {
		return 1;
	}

	string execute_name = extractFileNameWithoutExtension(win_path);


	createFolder("./tmp");

	// 处理exe文件的pe头
	/* 防止pe的二进制数据干扰
	通过EOF标签将包裹住的内容转化为输入
	存在不需要包裹的部分 -> 脚本结束后直接exit
	*/
	string target_file = "./tmp/" + execute_name;
	if (!copyFile(win_path, target_file)) {
		return 1;
	}

	/*处理开头部分：
	MZ后面加 =' 0A
	隔开一段后加 0A ' << 'Cross0XPlatform' 0A
	结尾添加0A Cross0XPlatform 0A
		*/
	unsigned char data1[] = { 0x3D, 0x27, 0x0A }; // =' \n
	writeHexToFile(target_file, 0x2, data1, sizeof(data1));

	unsigned char data2[] = { 0x0A,0x27,0x20,0x3C,0x3C,0x20,0x27,0x43,0x72,0x6F,0x73,0x73,0x30,0x58,0x50,0x6C,0x61,0x74,0x66,0x6F,0x72,0x6D,0x27,0x0A}; // \n ' << 'Cross0XPlatform' \n
	writeHexToFile(target_file, 0x1D, data2, sizeof(data2));

	streampos fileSize;
	ifstream file(target_file, ios::ate);
	if (file.is_open()) {
		fileSize = file.tellg(); // 获取文件指针位置，即文件大小
		file.close(); // 关闭文件
	}else {
		return 1;
	}

	unsigned char data3[] = { 0x0A,0x43,0x72,0x6F,0x73,0x73,0x30,0x58,0x50,0x6C,0x61,0x74,0x66,0x6F,0x72,0x6D,0x0A }; // \n Cross0XPlatform \n
	writeHexToFile(target_file, fileSize, data3, sizeof(data3));


	// 处理脚本部分
	/*
	直接追加脚本和新数据到尾部
	先检查文件夹是否存在
	再检查版本删除旧数据
	依情况判断是否需要解压数据
	确认数据存在后chmod后执行
	数据是否需要解压
	*/

	// 文件解压的目标目录
	string extract_dir = "/tmp/" + execute_name;

	// 可执行文件的前缀 前缀固定
	string file_prefix = execute_name;

	// 把md5写进文件名里 通过md5作为版本识别
	string file_md5 = md5file(win_path.c_str()).substr(0,5) + md5file(linux_path.c_str()).substr(0,5);

	string file_name = file_prefix + "_" + file_md5;

	string target_shell = "./tmp/template.sh";

	// 复制模板shell到临时文件夹
	if (!copyFile("./template.sh", target_shell)) {
		return 1;
	}

	// 对模板进行处理
	if (!replaceStringInFile(target_shell, "EXTRACTDIR", extract_dir)
		|| !replaceStringInFile(target_shell, "TARGETPREFIX", file_prefix)
		|| !replaceStringInFile(target_shell , "TARGETNAME", file_name)
		|| !replaceStringInFile(target_shell , "TARGETMD5", file_md5)) {
		return 1;
	}



	// 将脚本放入程序中
	if (!appendFileToFile(target_shell, target_file)) {
		return 1;
	}
	

	//将linux部分 也追加到其中
	if (!appendFileToFile(linux_path, target_file)) {
		return 1;
	}


	cout << "生成成功." << endl;




}

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
		cerr << "无法打开源文件." << endl;
		return false;
	}

	ofstream destFile(destFilename, ios::binary | ios::trunc); // 创建目标文件
	if (!destFile) {
		cerr << "无法创建目标文件." << endl;
		return false;
	}

	destFile << sourceFile.rdbuf(); // 将源文件内容复制到目标文件

	sourceFile.close();
	destFile.close();

	cout << "文件复制成功." << endl;
	return true;
}


// 替换文件内容
bool replaceStringInFile(const string& filename, const string& targetString, const string& replacement) {
	ifstream file(filename, ios::in | ios::binary); // 以二进制方式打开文件
	if (!file) {
		cerr << "无法打开文件." << endl;
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
		cerr << "无法打开输出文件." << endl;
		return false;
	}
	outFile << content;
	outFile.close();

	cout << "字符串替换完成." << endl;
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
		cout << "成功将文件内容追加到另一个文件的尾部！" << endl;
		return true;
	}
	else {
		cerr << "无法打开文件！" << endl;
		return false;
	}
}


bool createFolder(const string& folderPath) {
	if (filesystem::exists(folderPath) && filesystem::is_directory(folderPath)) {
		cout << "文件夹已存在！" << endl;
		return true;
	}

	try {
		if (filesystem::create_directory(folderPath)) {
			cout << "文件夹创建成功！" << endl;
			return true;
		}
		else {
			cerr << "无法创建文件夹！" << endl;
			return false;
		}
	}
	catch (const exception& e) {
		cerr << "无法创建文件夹: " << e.what() << endl;
		return false;
	}
}


/*
see also
Thanks for https://github.com/ulwanski/md5/

*/