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
    clear_table(lumicon, raw=args.raw, shard=args.shard, dblink='int2r')
    lumicon.close()


def parse_main_args():
    parser = argparse.ArgumentParser(
        description='Clear PXL tables in BRIL dev database by shard number')
    parser.add_argument(
        '--raw', dest='raw', action='store_true',
        help='clear PXL_RAW_<shard> table (otherwise PXL_RESULT_<shard>)')
    parser.add_argument(
        '--shard', type=int,
        help='table shard id depends on year: 2017->5, 2018->6, ...')
    parser.add_argument(
        '-p',  '--authpath', required=False,
        default='/afs/cern.ch/user/l/lumipro/db/cmsonr.ini',
        help='database authentication ini file. '
        'Default: /afs/cern.ch/user/l/lumipro/db/cmsonr.ini')
    return parser.parse_args()


def clear_table(con, raw, shard, dblink):
    table_name = 'PXL_' + ('RAW' if raw else 'RESULT') + '_' + str(shard)
    if dblink:
        table_name += '@' + dblink
    statement = 'delete from ' + table_name
    print(statement)
    con.execute(statement)


if __name__ == '__main__':
    main()
