import json
with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
    data = json.load(HFOC_JSON)

with open("flat.txt", "w") as HFOC_JSON:
    for line in data:
        if not line[0].startswith("hfoc"):
            continue
        for run, ranges in line[1].items():
            for ls1, ls2 in ranges:
                HFOC_JSON.write("{}; {}; {};\n".format(run, ls1, ls2))
                print(int(run), int(ls1), int(ls2))
