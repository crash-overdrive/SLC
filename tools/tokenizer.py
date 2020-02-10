from collections import OrderedDict
import logging
import re
import sys

REJECTS = ['COMMENT', 'LINE_TERMINATOR', 'WHITESPACE']

def look_matching(d, raw):

    for token, regex in d.items():
        matching = re.match(regex, raw);
        if matching is not None:
            string = matching.group(0)
            if token not in REJECTS:
                print(token + ' ' + string)
            return string

    sys.stderr.write('Error could not find any token anymore: \n' + sys.argv[2])
    sys.exit(42)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("usage: tokenizer [regex] [java]")
        sys.exit(42)

    d = OrderedDict()
    try:
        with open(sys.argv[1]) as f:
            for line in f:
                token, regex = [x.strip() for x in line.split('-=-')]
                d[token] = regex;
    except IOError:
        print("Can't find regex file")

    try:
        with open(sys.argv[2]) as f:
            print("BOF BOF")
            raw = ''.join(f.readlines());
            while raw:
                string = look_matching(d, raw)
                raw = raw[len(string):]
            print("EOF EOF")

    except IOError:
        print("Can't find java file")
