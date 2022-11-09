import json
with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_hfoc.json', "r") as HFOC_JSON:
    data = json.load(HFOC_JSON)
with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json', "r") as PCC_JSON:
    data1 = json.load(PCC_JSON)
    for line in data:
        for i in line[1]:
            for line1 in data1:
                for j in line1[1]:
                    ##print (i)
                    ##print(line[1][i])
                    for ls1, ls2 in line[1][i]:
                        for ls3, ls4 in line1[1][j]:
                            if ls1>ls3:
                                if ls2>ls4:
                                    print 'ls1', ls1, 'ls2', ls2, 'ls3', ls3, 'ls4', ls4

##with open('/cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_pcc.json', "r") as PCC_JSON:
  ##  data1 = json.load(PCC_JSON)
    ##for line1 in data1:
      ##  for j in line1[1]:
            ##print (i)                                                                                                                      
            ##print(line[1][i])                                                                                                              
        ##    for ls3, ls4 in line1[1][j]:
          ##      print 'ls3', ls3, 'ls4', ls4




            ##print(line[1])    
##            if not line[0].startswith("hfoc"):
  ##              continue
    ##            for run, ranges in line[1].items():
    ##for ls1, ls2 in ranges:
        ##                HFOC_JSON.write("{}; {}; {};\n".format(run, ls1, ls2))
                        ##print(int(run), int(ls1), int(ls2))
