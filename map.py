c = 0

def contains(string, char):
    v = 0
    for ch in string:
        if ch == char:
            return v
        v += 1
    return -1

with open("base/usr/share/kernel.map", "r") as file:
    lines = file.readlines()

    n = 0
    for l in lines:
        if l.split(" ")[1] != 'T':
            lines.pop(n)
            continue
        n += 1

    for line in lines:
        _line = ""
        for b in line:
            if b == '(':
                _line += '\n'
                break
            _line += b
        lines[c] = _line
        c += 1

with open("base/usr/share/kernel.map", "w") as file:
    file.write("".join(lines))
