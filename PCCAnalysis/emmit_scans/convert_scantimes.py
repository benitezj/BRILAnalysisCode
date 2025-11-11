import argparse
from datetime import datetime, timezone


parser = argparse.ArgumentParser(description='just converts the hd5 file names into scan time pairs')
parser.add_argument('--inputfile', type=str, default="", help='inputfile with emmitance scan files')
parser.add_argument('--outfile', type=str, default="emmit_scantimes.txt", help='output file with scan time pairs')
args = parser.parse_args()

def checkvalues(Y,M,D,h,m,s):
    if Y<=2000 or Y>2030:
        return False
    return True


with open( args.outfile , 'w') as ofile:

    with open( args.inputfile , 'r') as file:
        print("\nFile content line by line:")
        for line in file:
            print(line.strip())
            
            tokens = line.split('/')
            #print(tokens[9],tokens[10])
            tokens_hd5 = tokens[10].split('.')
            #print(tokens_hd5[0])
            tokens_time = tokens_hd5[0].split('_')
            #print(tokens_time[0],tokens_time[1],tokens_time[2])
            
            #251105181214
            (Y,M,D,h,m,s)=(int(int(tokens_time[1])/10000000000) % 100 + 2000, int(int(tokens_time[1])/100000000) % 100, int(int(tokens_time[1])/1000000) % 100, int(int(tokens_time[1])/10000) % 100, int(int(tokens_time[1])/100) % 100, int(tokens_time[1]) % 100)
            #print(Y,M,D,h,m,s)
            
            if not checkvalues(Y,M,D,h,m,s):
                continue
            
            starttime = datetime(Y,M,D,h,m,s,tzinfo=timezone.utc)
            
            
            (Y,M,D,h,m,s)=(int(int(tokens_time[2])/10000000000) % 100 + 2000, int(int(tokens_time[2])/100000000) % 100, int(int(tokens_time[2])/1000000) % 100, int(int(tokens_time[2])/10000) % 100, int(int(tokens_time[2])/100) % 100, int(tokens_time[2]) % 100)
            #print(Y,M,D,h,m,s)
            
            if not checkvalues(Y,M,D,h,m,s):
                continue
            
            endtime = datetime(Y,M,D,h,m,s,tzinfo=timezone.utc)

            print(starttime.timestamp(),endtime.timestamp())
        
            ofile.write(tokens[9] + ' ' + str(int(starttime.timestamp())) + ' ' + str(int(endtime.timestamp())) +'\n')
            
ofile.close()
