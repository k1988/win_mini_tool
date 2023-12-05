#!python3

import sys,os
import codecs
import uuid

def get_uuid():
    gen_uuid = uuid.uuid4() # 根据 随机数生成 uuid
    return str(gen_uuid)


def build_tree(root):
    out_path = os.path.join(root, "generate.vcxproj.filters")
    of = codecs.open(out_path, 'wb', 'utf-8')

    vcxproj_part_txt = codecs.open(os.path.join(root, "generate.vcxproj_part.txt"), 'wb', 'utf-8')

    path_map = {}

    for cur_root, dirs, files in os.walk(root):
        if root == cur_root:
            continue
        # 每次进入这个循环，root就是一个目录的全路径
        # dirs是root目录下所有的文件夹名称
        # files是root目录下所有的文件
        relativeFolder = cur_root[len(root) + 1:]
        paths = []
        for f in files:
            if f.endswith('.h') or f.endswith('.cpp'):
                path = relativeFolder + '\\'  + f
                paths.append(path)

        path_map[relativeFolder] = paths

    uuid_map = {}
    of.write('''<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <Filter Include="源文件">
      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>
      <Extensions>cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx</Extensions>
    </Filter>
    <Filter Include="头文件">
      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>
      <Extensions>h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd</Extensions>
    </Filter>
    <Filter Include="资源文件">
      <UniqueIdentifier>{67DA6AB6-F800-4c08-8B7A-83BB121AAD01}</UniqueIdentifier>
      <Extensions>rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms</Extensions>
    </Filter>''')
    for filter,files in path_map.items():
        uuid = get_uuid()
        uuid_map[filter] = uuid
        of.write('''
    <Filter Include="''' + filter + '''">
      <UniqueIdentifier>{''' + uuid + '''}</UniqueIdentifier>
    </Filter>''')

    of.write('''
  </ItemGroup>''')

    group_begin='''
    <ItemGroup>'''
    group_end='''
    </ItemGroup>'''

    for filter,files in path_map.items():
        f = ''
        of.write(group_begin)
        vcxproj_part_txt.write(group_begin)
        for f in files:
            if (f.endswith(".h")):
                of.write('''
        <ClInclude Include="''' + f + '''">
            <Filter>''' + filter + '''</Filter>
        </ClInclude>''')
                vcxproj_part_txt.write('''
        <ClInclude Include="''' + f + '''" />''')
            else:
                of.write('''
        <ClCompile Include="''' + f + '''">
            <Filter>''' + filter + '''</Filter>
        </ClCompile>''')
                vcxproj_part_txt.write('''
        <ClCompile Include="''' + f + '''" />''')
        of.write(group_end)

        vcxproj_part_txt.write(group_end)

    of.write('''
</Project>''')
    pass

if (len(sys.argv) < 2):
    print("usage generate_vcxproj.filters.py source_root")
    script_dir = os.path.split(os.path.realpath(__file__))[0]
    build_tree(script_dir)
else:
    source_root = sys.argv[1]
    if not os.path.exists(source_root):
        print("path is not exist")
    else:
        build_tree(source_root)