
@rem "C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.8 Tools\x64\ildasm.exe" D:\code\PDFConverter\setup\bin_win32\AsposeLib.dll  /metadata[=MDHEADER] /text /noil
if not exist "C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.8 Tools\x64\ildasm.exe" (
	echo "Œ¥’“µΩildasm.exe"
	goto end
)

"C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.8 Tools\x64\ildasm.exe" %1%  /metadata[=MDHEADER] /text /noil

:end