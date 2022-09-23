#!python3
import sys
import win32api # pip install pywin32
import os

def getFileVersion(file_name):
    info = win32api.GetFileVersionInfo(file_name, os.sep)
    ms = info['FileVersionMS']
    ls = info['FileVersionLS']
    version = '%d.%d.%d.%04d' % (win32api.HIWORD(ms), win32api.LOWORD(ms), win32api.HIWORD(ls), win32api.LOWORD(ls))
    return version

def getFileProps(file_path):
  """
  Read all properties of the given file return them as a dictionary.
  """
  propNames = ('Comments', 'InternalName', 'ProductName',
    'CompanyName', 'LegalCopyright', 'ProductVersion',
    'FileDescription', 'LegalTrademarks', 'PrivateBuild',
    'FileVersion', 'OriginalFilename', 'SpecialBuild')

  props = {'FixedFileInfo': None, 'StringFileInfo': None, 'FileVersion': None}

  try:
    # backslash as parm returns dictionary of numeric info corresponding to VS_FIXEDFILEINFO struc
    fixedInfo = win32api.GetFileVersionInfo(file_path, '\\')
    props['FixedFileInfo'] = fixedInfo
    props['FileVersion'] = "%d.%d.%d.%d" % (fixedInfo['FileVersionMS'] / 65536,
        fixedInfo['FileVersionMS'] % 65536, fixedInfo['FileVersionLS'] / 65536,
        fixedInfo['FileVersionLS'] % 65536)

    # \VarFileInfo\Translation returns list of available (language, codepage)
    # pairs that can be used to retreive string info. We are using only the first pair.
    lang, codepage = win32api.GetFileVersionInfo(file_path, '\\VarFileInfo\\Translation')[0]

    # any other must be of the form \StringfileInfo\%04X%04X\parm_name, middle
    # two are language/codepage pair returned from above

    strInfo = {}
    for propName in propNames:
      strInfoPath = u'\\StringFileInfo\\%04X%04X\\%s' % (lang, codepage, propName)
      ## print str_info
      strInfo[propName] = win32api.GetFileVersionInfo(file_path, strInfoPath)

    props['StringFileInfo'] = strInfo
    return props
  except:
    pass


def getCompanyNameAndProductName(file_path):
  props = getFileProps(file_path)
  if not props["StringFileInfo"]:
    return (None, None)
  else:
    return (props["StringFileInfo"]["CompanName"], props["StringFileInfo"]["ProductName"])

def getStringFileInfoProp(file_path, key):
  props = getFileProps(file_path)
  if not props["StringFileInfo"]:
    return None
  else:
    return props["StringFileInfo"][key]

propNames = ('Comments', 'InternalName', 'ProductName',
    'CompanyName', 'LegalCopyright', 'ProductVersion',
    'FileDescription', 'LegalTrademarks', 'PrivateBuild',
    'FileVersion', 'OriginalFilename', 'SpecialBuild')

if len(sys.argv) == 1:
    print("Usage: <Command> filepath propName.")
    print(propNames)
elif len(sys.argv) == 2:
    print(getFileProps(sys.argv[1]))
else:
    print(getStringFileInfoProp(sys.argv[1], sys.argv[2]))
