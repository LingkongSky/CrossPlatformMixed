# CrossPlatformMixed
## English

## Project Overview
`CrossPlatformMixed` is a cross-platform executable file mixing tool. It allows users to merge one Windows executable file and one Linux executable file into a single file, and automatically selects the appropriate program to execute at runtime based on the operating system.

## Function Description
- **Input Content**: The program accepts two parameters, which are the paths of the Windows and Linux executable files, in any order.
- **Output Result**: Generates a mixed file, defaulting to the name of the Windows executable file.

## Usage Method

### Compilation
```sh
mkdir build
cd build
cmake ..
make
```

### Execution
When running the program, you need to provide two parameters, which are the paths of the Windows executable file and the Linux executable file. For example:

```sh
./CrossPlatformMixed [path/to/windows.exe] [path/to/linux]
```

## License
CrossPlatformMixed  is released under the [MIT](license).

## Thanks
- <https://github.com/ulwanski/md5>
- <https://github.com/jart/cosmopolitan>


## 简体中文

## 项目概述
`CrossPlatformMixed` 是一个跨平台的可执行文件混合工具。它允许用户将一个Windows可执行文件和一个Linux可执行文件合并成一个单一的文件，并在运行时根据操作系统自动选择合适的程序执行。

## 功能描述
- **输入内容**：程序接受两个参数，分别为 Windows 和 Linux 可执行文件的路径，不分顺序。
- **输出结果**：生成一个混合文件，默认使用 Windows 可执行文件的名称。

## 使用方法

### 编译
```sh
mkdir build
cd build
cmake ..
make
```

### 运行
运行程序时，需要提供两个参数，分别是 Windows 可执行文件和 Linux 可执行文件的路径。例如：
```sh
./CrossPlatformMixed [path/to/windows.exe] [path/to/linux]
```


## 协议
CrossPlatformMixed遵守[MIT](LICENSE)协议.


## 致谢
- <https://github.com/ulwanski/md5>
- <https://github.com/jart/cosmopolitan>