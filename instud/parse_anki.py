import sys

in_file = "README.md"
out_file = "parsed_readme.md"


def read_lines():
    output = open(out_file, "w")
    in_code = False
    in_ans = False

    for line in sys.stdin:
        t_line = line.strip()

        if t_line[:3] == "```":
            in_code = not in_code

        if len(t_line) > 0 and not in_code:
            if t_line[0].isdigit():
                output.write("===\n\n")
                in_ans = False

            if t_line[0] == ">":
                if in_ans == False:
                    output.write("---\n\n")
                in_ans = True
                line = t_line[1:].strip() + "\n"

            output.write(line)
        else:
            output.write(line)

    output.close()
    return


def main():
    read_lines()


if __name__ == "__main__":
    main()
