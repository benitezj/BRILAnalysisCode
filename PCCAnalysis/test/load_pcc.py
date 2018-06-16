import argparse
import time
import lumidb
import load_detector_csv as loader


def main():
    args = parse_main_args()

    authpath = args.authpath
    servicemap = lumidb.parseservicemap(authpath)
    lumiengine = lumidb.create_engine(servicemap, 'cmsr')
    lumicon = lumiengine.connect()
    dblink = 'cmsonr' if args.database.upper() == 'PROD' else 'int2r'

    start_time = time.time()
    print('Starting transaction. On failure, intermediate changes '
          'should be rolled back.')
    lumicon.transaction(
        loader.load_detector_data, fname=args.csvfile, detector='PXL',
        raw=args.raw, update=False, lumitype='BOTH', shardid=args.shard,
        datatag='online', dblink=dblink)
    lumicon.close()
    print('Finished succesfully')
    print('Took {0} seconds'.format(time.time() - start_time))


def parse_main_args():
    parser = argparse.ArgumentParser(
        description='Load PCC data csv to BRIL DB'
        'CSV format: "run,ls,avg,bxlumi1,bxlumi2,...,bxlumi3564"')
    parser.add_argument(
        'csvfile', type=str, help='csv file with data to load')
    parser.add_argument(
        '--raw', dest='raw', action='store_true',
        help='insert data to PXL_RAW_<shard> table '
        '(otherwise PXL_RESULT_<shard>)')
    parser.add_argument(
        '--shard', type=int,
        help='table shard id depends on year: 2017->5, 2018->6, ...')
    parser.add_argument(
        '-d',  '--database', required=False, default='prod',
        help='database to use - prod/dev')
    parser.add_argument(
        '-p',  '--authpath', required=False,
        default='/afs/cern.ch/user/l/lumipro/db/cmsonr.ini',
        help='database authentication ini file. '
        'Default: /afs/cern.ch/user/l/lumipro/db/cmsonr.ini')
    return parser.parse_args()


if __name__ == '__main__':
    main()
