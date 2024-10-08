import sys
import glob

def convert(path, triangulate):
    try:
        file = open(path, 'r')
    except OSError:
        return "Couldn't open: " + path
    start = []
    v = []
    vt = []
    vn = []
    f = []    
    last_passed = start 
    for line in file.readlines():
        line_t = line.split()
        if len(line_t) == 0:
            continue
        elif line_t[0] == "v":
            v.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + ' ' + line_t[3] + '\n')
            last_passed = v 
        elif line_t[0] == "vn":
            vn.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + ' ' + line_t[3] + '\n')
            last_passed = vn 
        elif line_t[0] == "vt":
            vt.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + '\n')
            last_passed = vt 
        elif line_t[0] == "f":
            if len(line_t) < 5:
                f.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + ' ' + line_t[3] + '\n')
            else:
                if not triangulate:
                    f.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + ' ' + line_t[3] + ' ' + line_t[4] + '\n')
                else:
                    f.append(line_t[0] + ' ' + line_t[1] + ' ' + line_t[2] + ' ' + line_t[3] + '\n')
                    f.append(line_t[0] + ' ' + line_t[3] + ' ' + line_t[4] + ' ' + line_t[1] + '\n')
            last_passed = f
        elif line_t[0] == 'g' or line_t[0] == 's' or line_t[0] == 'usemtl' or line_t[0] == 'o':
            f.append(line_t[0] + ' ' + '_'.join(line_t[1:]) +'\n')
        else:
            last_passed.append(line)
    file.close()
    
    out_path = path
    if path.find(".obj") > -1:
        out_path = path.replace(".obj", "_conv.obj")
    else:
        out_path += "_conv"

    file = open(out_path, "w")
    file.writelines(start)
    file.writelines(v)
    file.writelines(vt)
    file.writelines(vn)
    file.writelines(f)
    file.close();
    return path + " -> " + out_path

def expand_argument_wild_card(path, recursive=False):
    return glob.glob(path, recursive=recursive)

recursive = False
triangulate = False
for arg in sys.argv[1:]:
    if arg.strip() == "-r":
        recursive = True 
        continue
    if arg.strip() == "-t":
        triangulate = True
        continue
    for a in expand_argument_wild_card(arg):
        print(convert(a, triangulate))