import os
path =r'/Users/daniyalwarraich/Documents/CeneOS/'
list_of_files = []
src_lines = 0

for root, dirs, files in os.walk(path):
	for file in files:
		list_of_files.append(os.path.join(root,file))

# get core os lines
for filename in list_of_files:
	if filename.endswith(".cpp") or filename.endswith(".h") or filename.endswith(".asm"):
		file = open(filename, 'r')
		content = file.read()
		file.close()
		for line in content.split('\n'):
			if line.strip() != "":
				src_lines += 1


print("CeneOS lines of code:", src_lines)
