import os
import subprocess
import threading

# 定义要格式化的文件夹列表
root_dirs = ["./Core", "./Bsp"]

# 指定 clang-format 路径
clang_format_path = "clang-format"

# 定义文件类型列表
file_types = [".c", ".h"]


# 定义执行 clang-format 的函数
def run_clang_format(file_list):
    for file_path in file_list:
        print(f"正在格式化文件：{file_path}")
        subprocess.call([clang_format_path, "-i", file_path, "--style=file"])
        print(f"文件格式化完成：{file_path}")


# 遍历文件夹并收集满足要求的文件
def traverse_and_collect(root_dir):
    file_list = []
    for root, dirs, files in os.walk(root_dir):
        for file in files:
            file_path = os.path.join(root, file)
            if file_path.endswith(tuple(file_types)):
                file_list.append(file_path)
    if file_list:
        run_clang_format(file_list)


# 创建线程用于遍历根文件夹并格式化文件
threads = []
for root_dir in root_dirs:
    thread = threading.Thread(target=traverse_and_collect, args=(root_dir,))
    thread.start()
    threads.append(thread)

# 等待所有线程完成
for thread in threads:
    thread.join()
