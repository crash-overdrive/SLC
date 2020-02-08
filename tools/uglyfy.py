import sys

def qualifier(line):
	if line.isspace():
		return False
	if(line[0]) == '#':
		return False
	return True

def uglyfy(source, dest):
	lines = open(source).readlines();
	res = filter(qualifier, lines)
	output = open(dest, "w+")
	for re in res:
		output.write(re)

uglyfy(sys.argv[1], sys.argv[2])
