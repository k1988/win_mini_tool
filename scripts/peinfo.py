import os
import sys
import string
import hashlib
import pefile
import datetime


def gethash(file):
    m = hashlib.md5()
    s = hashlib.sha1()
    s256 = hashlib.sha256()
    with open(file, 'rb') as f:
        for line in f:
            m.update(line)
            s.update(line)
            s256.update(line)
    md5code = m.hexdigest()
    sha1code = s.hexdigest()
    sha256code = s256.hexdigest()
    return (md5code, sha1code, sha256code)


def print_split_line(header_type:str=''):
    print('---------------------',header_type,'-----------------------------------')


def print_optional_header(pe: pefile.PE):
    # 可选头信息 - IMAGE_OPTIONAL_HEADER  https://learn.microsoft.com/zh-cn/windows/win32/api/winnt/ns-winnt-image_optional_header32
    print_split_line("可选头")
    if not pe.OPTIONAL_HEADER.all_zeroes():
        # 可选头关注信息为：最小支持的操作系统、程序入口、子系统等
        print("链接器: ", pe.OPTIONAL_HEADER.MajorLinkerVersion,
              pe.OPTIONAL_HEADER.MinorLinkerVersion)
        print("支持系统: ", pe.OPTIONAL_HEADER.MajorOperatingSystemVersion,
              pe.OPTIONAL_HEADER.MinorOperatingSystemVersion)
        print("子系统版本: ", pe.OPTIONAL_HEADER.MajorSubsystemVersion,
              pe.OPTIONAL_HEADER.MinorSubsystemVersion)
        print('入口点: ', hex(pe.OPTIONAL_HEADER.AddressOfEntryPoint))
        subsystem = pefile.SUBSYSTEM_TYPE[pe.OPTIONAL_HEADER.Subsystem]
        if subsystem == "IMAGE_SUBSYSTEM_WINDOWS_GUI":
            print('子系统：GUI图形')
        else:
            print('子系统：', subsystem)

    pass


def print_pe_header(pe: pefile.PE):
    # PE头信息 - 对应IMAGE_FILE_HEADER  https://learn.microsoft.com/zh-cn/windows/win32/api/winnt/ns-winnt-image_file_header
    print_split_line("PE头")

    # 单独打印一些关注的定义
    # https://learn.microsoft.com/zh-cn/windows/win32/api/winnt/ns-winnt-image_file_header
    care_attrs: dict = {
        "IMAGE_FILE_DLL": "DLL",
        "IMAGE_FILE_SYSTEM": "系统文件",
        "IMAGE_FILE_DEBUG_STRIPPED": "无调试信息",
        "IMAGE_FILE_EXECUTABLE_IMAGE": "可执行映像",
        "IMAGE_FILE_LARGE_ADDRESS_AWARE": "2GB大地址",
        "IMAGE_FILE_32BIT_MACHINE": "支持32位",
    }

    # 从属性字典定义中找到指定前缀的属性数组
    image_flags = pefile.retrieve_flags(
        pefile.IMAGE_CHARACTERISTICS, "IMAGE_FILE_")
    flags = []

    # 先计算位数
    if pe.FILE_HEADER.Machine == pefile.MACHINE_TYPE["IMAGE_FILE_MACHINE_I386"]:
        flags.append("32位")
    elif pe.FILE_HEADER.Machine == pefile.MACHINE_TYPE["IMAGE_FILE_MACHINE_AMD64"]:
        flags.append("64位")
    else:
        flags.append(pefile.MACHINE_TYPE[pe.FILE_HEADER.Machine])

    for flag in sorted(image_flags):
        # 如果pe的文件头中有IMAGE_FILE_开头的属性，则将期名称添加到数组
        if getattr(pe.FILE_HEADER, flag[0]) and flag[0] in care_attrs.keys():
            flags.append(care_attrs[flag[0]])

    print("文件特征:", ",".join(flags))
    print('编译时间:', datetime.datetime.fromtimestamp(
        pe.FILE_HEADER.TimeDateStamp))
    print('段数: ', pe.FILE_HEADER.NumberOfSections)
    pass


def print_hash(path: str):
    print_split_line("Hash")
    file_md5, file_sha1, file_sha256 = gethash(path)
    print('MD5: ', file_md5)
    print('SHA-1: ', file_sha1)
    print('SHA-256: ', file_sha256)
    pass


def print_pe(path: str):
    try:
        pe = pefile.PE(path)
        # print(pe.dump_info())

        # 基本信息及md5
        print_split_line('基本信息')
        print('File Name: ', os.path.basename(path))
        print('File Size: ', os.path.getsize(path), 'byte')

        print_hash(path)
        print_pe_header(pe)
        print_optional_header(pe)

        # 导入dll信息
        print_split_line('导入dll')
        dlls = []
        for importdll in pe.DIRECTORY_ENTRY_IMPORT:
            dlls.append(importdll.dll.decode())
        print(", ".join(dlls))

        warning = pe.get_warnings()
        if warning:
            print_split_line('warning')
            for w in warning:
                print(w)
    except Exception as e:
        print(e)
        pass


def usage():
    print("usage: \tpeinfo [file path]")
    print("\tpeinfo -h|/?|--help")


if __name__ == '__main__':
    args = len(sys.argv)
    if args == 1:
        usage()
    elif args > 2:
        usage()
    else:
        params = sys.argv[1]
        if (params in ['-h', '/?', '--help']):
            usage()
            exit

        if not os.path.exists(params):
            print("error: the file is not exist!")

        print_pe(params)
