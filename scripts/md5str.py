from hashlib import md5
import sys


if len(sys.argv) == 0 or "-h" in sys.argv:
	print("Usage: md5str <string>")
else:
	md5 = md5()
	for arg in sys.argv:
		md5.update(arg)
	print md5.hexdigest()
