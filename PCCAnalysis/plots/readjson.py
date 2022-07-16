import json
with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as f:
    data = json.load(f)


with open("flat.txt", "w") as f:
    for line in data:
        if not line[0].startswith("hfoc"):
            continue
        for run, ranges in line[1].items():
            for ls1, ls2 in ranges:
                f.write("{}; {}; {};\n".format(run, ls1, ls2))
                print(int(run), int(ls1), int(ls2))
