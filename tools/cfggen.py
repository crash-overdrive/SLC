import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("one argument missing")
        sys.exit(1)

    try:
        with open(sys.argv[1]) as f:
            upper = set()
            lower = set()
            grammar = []
            for line in f:
                line = line.strip().strip('\n')
                if not line: continue
                if line[0] == '#': continue
                grammar.append(line)
                words = [x.strip() for x in line.split(' ')]
                for word in words:
                    if word[0].isupper():
                        upper.add(word)
                    else:
                        lower.add(word)
            upper = sorted(upper)
            lower = sorted(lower)
            print(len(upper))
            for x in upper:
                print(x)
            print(len(lower))
            for x in lower:
                print(x)
            print("start")
            print(len(grammar))
            for x in grammar:
                print(x)
    except IOError:
        print("Can't find file")
