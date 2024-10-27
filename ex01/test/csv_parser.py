#! /usr/bin/python3

import sys
import csv

def parse_csv_and_print(file):
    try:
        with open(file) as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                print(row)
            return 0
    except Exception as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

def main(argc, argv):
    if argc == 1:
        print(f"usuage: {argv[0]} <csv_file>", file=sys.stderr)
        return 1
    return parse_csv_and_print(argv[1])

if __name__ == "__main__":
    argc = len(sys.argv)
    argv = sys.argv
    sys.exit(main(argc, argv))
