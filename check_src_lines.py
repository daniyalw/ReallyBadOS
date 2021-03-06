import os

path ='.'
list_of_files = []
asm = 0
cpp = 0
total = 0

for root, dirs, files in os.walk(path):
	for file in files:
		list_of_files.append(os.path.join(root,file))

for file in list_of_files:
	if file.endswith("cpp") or file.endswith("h"):
		try:
			f = open(file, 'r')
		except UnicodeDecodeError:
			continue
		try:
			for line in f.read().split('\n'):
				if line.strip() != "":
					cpp += 1
		except UnicodeDecodeError:
			continue
		f.close()
	elif file.lower().endswith("asm") or file.lower().endswith("s"):
		try:
			f = open(file, 'r')
		except UnicodeDecodeError:
			continue
		try:
			for line in f.read().split('\n'):
				if line.strip() != "":
					asm += 1
		except UnicodeDecodeError:
			continue
		f.close()

total += cpp
total += asm

print("Total number of C++ lines:     ", cpp)
print("Total number of Assembly lines:", asm)
print("Total lines of code:           ", total)
