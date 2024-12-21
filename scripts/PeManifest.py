import sys
import pefile
import xml.dom.minidom

'''
打印出第一个参数对应的PE文件的manifest内容(以排版过的xml格式展示)
'''

def get_manifest_from_pe(pe_file_path):
    try:
        # 加载PE文件
        pe = pefile.PE(pe_file_path)

        # 尝试从资源表中提取Manifest
        for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries:
            #if entry.name and entry.name.string.lower() == b'manifest':
            if entry.id == pefile.RESOURCE_TYPE['RT_MANIFEST']:
                manifest_data = None

                for subentry in entry.directory.entries:
                    for lang_entry in subentry.directory.entries:
                        data_rva = lang_entry.data.struct.OffsetToData
                        size = lang_entry.data.struct.Size
                        manifest_data = pe.get_memory_mapped_image()[data_rva:data_rva + size]

                        if manifest_data:
                            # 前面有bom头 \xef\xbb\xbf
                            return manifest_data[3:].decode('utf-8')

        print("[!] Manifest resource not found in the PE file.")
        return None
    except Exception as e:
        print(f"[!] Error: {e}")
        return None

def format_xml(xml_content):
    try:
        dom = xml.dom.minidom.parseString(xml_content)
        return dom.toprettyxml(indent="  ")
    except Exception as e:
        print(f"[!] Failed to format XML: {e}")
        return xml_content

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <path_to_pe_file>")
        sys.exit(1)

    pe_file_path = sys.argv[1]

    print("[*] Reading PE file and extracting manifest...")
    manifest_content = get_manifest_from_pe(pe_file_path)

    if manifest_content:
        print("[*] Formatting and displaying manifest:")
        formatted_manifest = format_xml(manifest_content)
        print(formatted_manifest)
    else:
        print("[!] No manifest content found.")

if __name__ == "__main__":
    main()
