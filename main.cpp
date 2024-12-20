#include "crossPlatform.h"

/*
待办：
有第三个参数则设置输出文件地址
无第三个参数则文件名默认为windows文件名
*/


// 通过linux和win对于指令解释的差异来执行linux
// 修改exe文件的pe部分实现注释 将拥有可执行功能的shell脚本添加到文件末尾 将linux可执行文件添加到文件末尾


// 打包时连带template.sh
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
	else if (file2.substr(file2.size() - 4) == ".exe") {
		win_path = file2;
		linux_path = file1;
	}
	else {
		cerr << "Exe file does not exist: " << file2 << endl;
		return 1;
	}

	string execute_name = extractFileNameWithoutExtension(win_path);


	if (!createFolder("./tmp") || !createFolder("./output")) {
		cerr << "Can not create the target folder" << endl;
		return 1;
	}

	// 处理exe文件的pe头
	/* 防止pe的二进制数据干扰
	通过EOF标签将包裹住的内容转化为输入
	存在不需要包裹的部分 -> 脚本结束后直接exit
	*/
	string target_file = "./output/" + execute_name;
	if (!copyFile(win_path, target_file)) {
		cerr << "Can not create the target file" << endl;
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

	// 保存shell变量为文件
	ofstream outputFile(target_shell, ios::binary);
	if (outputFile.is_open()) {
		outputFile << template_shell_content << '\n';
		outputFile.close();
	}
	else {
		cerr << "Can not create the target file" << endl;
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

	//filesystem::remove_all("./tmp");

	cout << "Process finished!" << endl;

}

