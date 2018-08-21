#########################
#Author: Sam Higginbotham
#Purpose: To investigate the AlCaPCCProducer input and output. 
#########################
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("corrRECO")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'file:/eos/cms/store/data/Run2017C/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/300/027/00000/C8332BFE-B176-E711-9463-02163E019C80.root',
#    #'file:/eos/cms/store/data/Run2017C/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/300/043/00000/3AA17D8E-D676-E711-BED6-02163E01425A.root',
#    #'file:/eos/cms/store/data/Run2017E/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/303/832/00000/B0DF435F-99A4-E711-8E6D-02163E01A45C.root',
#    #'file:/eos/cms/store/data/Run2017E/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/303/832/00000/CE2A42FA-91A4-E711-9697-02163E019C72.root',
#    #'file:/eos/cms/store/data/Run2017E/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/303/832/00000/DA0ECCCB-96A4-E711-ADA1-02163E01A214.root',
#    #'file:/eos/cms/store/data/Run2017E/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/303/832/00000/E6B8ACA4-95A4-E711-9AA2-02163E014793.root',
#     'file:/eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v1/000/316/060/00000/1AAF584E-0956-E811-B8CF-FA163E73FFA8.root' 
#     #'file:/eos/cms/store/data/Run2018A/AlCaLumiPixels/ALCARECO/AlCaPCCRandom-PromptReco-v3/000/316/944/00000/72BB039F-3466-E811-9576-FA163E33F375.root'
#)    
#)




## read from file, environment variable must be set
inputfile=os.getenv('INPUTFILE')
if inputfile == '' : sys.exit('invalid INPUTFILE')
print 'reading from input file: ', inputfile
infile = open(inputfile,'r')
inputlist=cms.untracked.vstring(infile.readlines())
infile.close()

print inputlist
process.source = cms.Source("PoolSource",
    fileNames =     inputlist
)



#Make sure that variables match in producer.cc and .h
process.rawPCCProd = cms.EDProducer("RawPCCProducer",
    RawPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inputPccLabel = cms.string("alcaPCCProducerRandom"),
        ProdInst = cms.string("alcaPCCRandom"),
        resetEveryNLumi = cms.untracked.int32(1),
        outputProductName = cms.untracked.string("rawPCCRandom"), 
        modVeto=cms.vint32()
    )
    
)


process.rawPCCProd.RawPCCProducerParameters.modVeto.extend([302055684,302055688,302055692,302055696,302055700,302055704,302055708,302055712,302055940,302055944,302055948,302055952,302055956,302055960,302055964,302055968,302056196,302056200,302056204,302056208,302056212,302056216,302056220,302056224,302056452,302056456,302056460,302056464,302056468,302056472,302056476,302056480,302056708,302056712,302056716,302056720,302056724,302056728,302056732,302056736,302056964,302056968,302056972,302056976,302056980,302056984,302056988,302056992,302057220,302057224,302057228,302057232,302057236,302057240,302057244,302057248,302057476,302057480,302057484,302057488,302057492,302057496,302057500,302057504,302057732,302057736,302057740,302057744,302057748,302057752,302057756,302057760,302057988,302057992,302057996,302058000,302058004,302058008,302058012,302058016,302058244,302058248,302058252,302058256,302058260,302058264,302058268,302058272,302058500,302058504,302058508,302058512,302058516,302058520,302058524,302058528,302058756,302058760,302058764,302058768,302058772,302058776,302058780,302058784,302059012,302059016,302059020,302059024,302059028,302059032,302059036,302059040,302059268,302059272,302059276,302059280,302059284,302059288,302059292,302059296,302059524,302059528,302059532,302059536,302059540,302059544,302059548,302059552,302059780,302059784,302059788,302059792,302059796,302059800,302059804,302059808,302060036,302060040,302060044,302060048,302060052,302060056,302060060,302060064,302060292,302060296,302060300,302060304,302060308,302060312,302060316,302060320,302060548,302060552,302060556,302060560,302060564,302060568,302060572,302060576,303042564,303042568,303042572,303042576,303042580,303042584,303042588,303042592,303046660,303046664,303046668,303046672,303046676,303046680,303046684,303046688,303050756,303050760,303050764,303050768,303050772,303050776,303050780,303050784,303054852,303054856,303054860,303054864,303054868,303054872,303054876,303054880,303058948,303058952,303058956,303058960,303058964,303058968,303058972,303058976,303063044,303063048,303063052,303063056,303063060,303063064,303063068,303063072,303067140,303067144,303067148,303067152,303067156,303067160,303067164,303067168,303071236,303071240,303071244,303071248,303071252,303071256,303071260,303071264,303075332,303075336,303075340,303075344,303075348,303075352,303075356,303075360,303079428,303079432,303079436,303079440,303079444,303079448,303079452,303079456,303083524,303083528,303083532,303083536,303083540,303083544,303083548,303083552,303087620,303087624,303087628,303087632,303087636,303087640,303087644,303087648,304091140,304091144,304091148,304091156,304091160,304091164,304091168,304095236,304095240,304095244,304095248,304095252,304095256,304095260,304095264,304099332,304099344,304099348,304099352,304099356,304099360,304103428,304103432,304103436,304103440,304103444,304103448,304103452,304103456,304107524,304107528,304107532,304107536,304107540,304107544,304107548,304111620,304111624,304111628,304111632,304111636,304111640,304111644,304111648,304115716,304115720,304115724,304115728,304115732,304115736,304115740,304115744,304119812,304119816,304119820,304119828,304119832,304119836,304119840,304123908,304123912,304123916,304123924,304123928,304123932,304123936,304128004,304128008,304128012,304128016,304128020,304128024,304128028,304128032,304132100,304132104,304132108,304132112,304132116,304132120,304132124,304132128,304136196,304136200,304136204,304136212,304136216,304136220,304136224,304140292,304140296,304140300,304140304,304144388,304144392,304144396,304144400,304148484,304148488,304148492,304148496,304148500,304148504,304148508,304148512,304152580,304152584,304152588,304152592,304152596,304152600,304152604,304152608,304156676,304156680,304156684,304156688,304156692,304156696,304156700,304156704,304160772,304160776,304160780,304160788,304160792,304160796,304160800,304164868,304164872,304164876,304164880,304164884,304164888,304164892,304164896,304168964,304168968,304168972,304168976,304168980,304168984,304168988,304168992,304173060,304173064,304173068,304173072,304173076,304173080,304173084,304173088,304177156,304177160,304177164,304177168,304177172,304177176,304177180,304177184,304181252,304181256,304181260,304181264,304181268,304181272,304181276,304181280,304185348,304185352,304185356,304185360,304185364,304185368,304185372,304185376,304189444,304189448,304189456,304189460,304189464,304189468,304189472,304193540,304193544,304193548,304193552,304193556,304193560,304193564,304193568,304197636,304197640,304197644,304197648,304197652,304197656,304197660,304197664,304201732,304201736,304201740,304201744,304201748,304201752,304201756,304201760,305139720,305139732,305139736,305139740,305139744,305143812,305143816,305143824,305143840,305147908,305147928,305147932,305152008,305152020,305152024,305152028,305156108,305156112,305156116,305156120,305156124,305156128,305160200,305160204,305160216,305160220,305160224,305164312,305168392,305168412,305168416,305172488,305172492,305172496,305172500,305172512,305176580,305176584,305176588,305176592,305176596,305176604,305180676,305180680,305180696,305184784,305184792,305184800,305188868,305188872,305188876,305188880,305188888,305188896,305192968,305192972,305192976,305192980,305192984,305192988,305192992,305197060,305197064,305197076,305197080,305197084,305197088,305201172,305205252,305205268,305205272,305205276,305205280,305209352,305209356,305209364,305209368,305209372,305209376,305213452,305213464,305213468,305213472,305217540,305217544,305217548,305221644,305225732,305225744,305229828,305229832,305229836,305229840,305233924,305233928,305233932,305233936,305233948,305238020,305238024,305238028,305238032,305242128,305242140,305246216,305246220,305250320,305250324,305250328,305250336,305254404,305254408,305254412,305254416,305254420,305254424,305254428,305258500,305258504,305258508,305258512,305258516,305258524,305258528,305262596,305262612,305262620,305266692,305266704,305266708,305266712,305270788,305270796,305270808,305274884,305274912,305278980,305278984,305279000,305279004,305283092,305283096,305283104,305287176,305287180,305287200,305291272,305291284,305291288,305291292,305291296,305295372,305295376,305295380,305295384,305295388,305295392,305299460,305299472,305299480,305299484,305299488,305303556,305303564,305303568,305303572,305303580,305303584,305307652,305307660,305307664,305307672,305307676,305307680,305311748,305311756,305311768,305315844,305315860,305315864,305315868,306188296,306188300,306188312,306188316,306188320,306192392,306196484,306196504,306200580,306200600,306200608,306204676,306204680,306204692,306204696,306208796,306212872,306212876,306212880,306212892,306212896,306216964,306216984,306216992,306221068,306221072,306221080,306221084,306221088,306225156,306229252,306229260,306229268,306233348,306233352,306233368,306233376,306237456,306237468,306237472,306241540,306241552,306241560,306241564,306241568,306245636,306245644,306245656,306249740,306249756,306253832,306257948,306257952,306262024,306262040,306262044,306262048,306266116,306266132,306266144,306270212,306270228,306274316,306274320,306274332,306274336,306278404,306278412,306282516,306282520,306282528,306286596,306286600,306286620,306286624,306290708,306290712,306290716,306290720,306294796,306294808,306294812,306294816,306298892,306298900,306298908,306298912,306302980,306303000,306307076,306307080,306307088,306307096,306307104,306311176,306311180,306311188,306311200,306315268,306315272,306315276,306315288,306315292,306319372,306319376,306323472,306323480,306323488,306327568,306327580,306327584,306331664,306331668,306335748,306335768,306339860,306339864,306339868,306339872,306343940,306343956,306348044,306348056,306348060,306348064,306352156,306352160,306356232,306356244,306356248,306356256,306360336,306360340,306360348,306364420,306364432,306364440,306364444,306368524,306368532,306368536,306368540,306368544,306372616,306372636,306376708,306376720,306376732,306380808,306380820,306380828,306380832,306384908,306384912,306384916,306384920,306384924,306384928,306389000,306389008,306389012,306389016,306389020,306389024,306393096,306393108,306393112,306393116,306393120,306397188,306397200,306397204,306397208,306397212,306397216,306401288,306401292,306405392,306405400,306405404,306409480,306409496,306409500,306409504,306413576,306413580,306413596,306417668,306417680,306417684,306417688,306417692,306421764,306421768,306421780,306421784,306425864,306425888,306429960,306429972,306434060,306434064,306434068,306438148,306438152,306438156,306438168,306442248,306446368,344204292,344205316,344208388,344209412,344212484,344213508,344216580,344217604,344220676,344221700,344224772,344225796,344228868,344229892,344232964,344233988,344237060,344241156,344242180,344250372,344254468,344257540,344258564,344261636,344262660,344265732,344266756,344269828,344270852,344274948,344278020,344279044,344282116,344283140,344287236,344290308,344291332,344294404,344295428,344303620,344310788,344311812,344314884,344315908,344318980,344320004,344323076,344324100,344327172,344328196,344331268,344332292,344335364,344336388,344339460,344340484,344347652,344351748,344360964,344365060,344369156,344372228,344376324,344377348,344380420,344381444,344384516,344385540,344388612,344389636,344392708,344393732,344397828,344404996,344409092,344413188,344426500,344462340,344463364,344466436,344467460,344470532,344474628,344478724,344482820,344486916,344491012,344492036,344495108,344496132,344500228,344503300,344504324,344507396,344508420,344512516,344515588,344516612,344520708,344523780,344524804,344527876,344528900,344531972,344532996,344536068,344537092,344540164,344541188,344544260,344545284,344549380,344552452,344556548,344557572,344564740,344568836,344569860,344572932,344577028,344581124,344585220,344594436,344601604,344606724,344610820,344613892,344614916,344617988,344619012,344622084,344623108,344626180,344627204,344631300,344635396,344639492,344643588,344647684,344651780,344654852,344655876,344658948,344659972,344663044,344664068,344667140,344668164,344671236,344672260,344676356,344679428,344680452,344684548,344687620,344688644,344724484,344725508,344728580,344729604,344733700,344736772,344740868,344741892,344744964,344745988,344749060,344750084,344765444,344766468,344769540,344773636,344774660,344777732,344781828,344782852,344785924,344786948,344790020,344791044,344794116,344795140,344798212,344799236,344802308,344803332,344810500,344811524,344814596,344815620,344822788,344843268,344847364,344859652,344860676,344863748,344871940,344876036,344877060,344881156,344884228,344885252,344888324,344900612,344904708,344905732,344908804,344909828,344912900,344913924,344916996,344918020,344921092,344922116,344926212,344933380,344937476,344938500,344942596,344945668,344946692,344950788,352588804,352589828,352592900,352593924,352596996,352601092,352602116,352605188,352606212,352609284,352610308,352613380,352614404,352618500,352621572,352622596,352626692,352630788,352633860,352634884,352637956,352638980,352642052,352643076,352647172,352651268,352654340,352655364,352658436,352659460,352662532,352663556,352666628,352667652,352670724,352671748,352675844,352678916,352683012,352684036,352687108,352688132,352691204,352692228,352695300,352696324,352699396,352700420,352703492,352704516,352707588,352708612,352711684,352712708,352716804,352723972,352728068,352732164,352736260,352737284,352740356,352744452,352745476,352748548,352752644,352760836,352764932,352765956,352770052,352773124,352774148,352778244,352781316,352782340,352786436,352790532,352793604,352794628,352798724,352805892,352806916,352811012,352815108,352850948,352851972,352855044,352856068,352859140,352860164,352863236,352864260,352867332,352868356,352871428,352872452,352888836,352891908,352896004,352900100,352904196,352905220,352908292,352909316,352912388,352913412,352917508,352920580,352921604,352924676,352932868,352941060,352945156,352949252,352950276,352953348,352954372,352957444,352958468,352962564,352965636,352966660,352969732,352970756,352973828,352974852,352999428,353003524,353011716,353014788,353018884,353019908,353022980,353028100,353032196,353035268,353040388,353044484,353047556,353052676,353055748,353056772,353060868,353063940,353068036,353072132,353073156,353076228,353077252,353113092,353114116,353117188,353118212,353121284,353122308,353125380,353126404,353129476,353130500,353133572,353134596,353137668,353141764,353142788,353145860,353146884,353150980,353154052,353158148,353162244,353166340,353170436,353174532,353175556,353178628,353179652,353182724,353183748,353186820,353187844,353190916,353191940,353195012,353196036,353199108,353200132,353203204,353204228,353211396,353212420,353215492,353220612,353223684,353224708,353227780,353228804,353231876,353232900,353235972,353236996,353240068,353244164,353248260,353252356,353253380,353256452,353257476,353260548,353261572,353264644,353273860,353276932,353277956,353281028,353285124,353286148,353289220,353290244,353301508,353302532,353305604,353306628,353309700,353310724,353313796,353314820,353317892,353318916,353321988,353323012,353326084,353327108,353330180,353331204,353334276,353335300,353338372,353339396])
#Make sure that variables match in producer.cc and .h
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
DQMStore = cms.Service("DQMStore")

process.load("DQMServices.Core.DQM_cfg")
#process.load('HLTrigger.Timer.FastTimerService_cff')

process.corrPCCProd = DQMEDAnalyzer("CorrPCCProducer",
    CorrPCCProducerParameters = cms.PSet(
        #Mod factor to count lumi and the string to specify output 
        inLumiObLabel = cms.string("rawPCCProd"),
        ProdInst = cms.string("rawPCCRandom"),
        approxLumiBlockSize=cms.int32(50),
        trigstring = cms.untracked.string("corrPCCRand"), 
        type2_a= cms.double(0.00072),
        type2_b= cms.double(0.014),
        subSystemFolder=cms.untracked.string('AlCaReco')
    )
)

process.dqmEnvLumiPCC = DQMEDAnalyzer('DQMEventInfo',
                              subSystemFolder=cms.untracked.string('AlCaRecoEventInfo'))

#Output for the Database
process.load("CondCore.CondDB.CondDB_cfi")


process.CondDB.connect = "sqlite_file:PCC_Corr.db"

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    toPut = cms.VPSet(
    cms.PSet(record = cms.string('LumiCorrectionsRcd'),tag = cms.string('TestCorrections'))#,
     ),
    loadBlobStreamer = cms.untracked.bool(False),
    timetype   = cms.untracked.string('lumiid')
    #timetype   = cms.untracked.string('runnumber')
    )
#From the end path, this is where we specify format for our output.
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('PCC_RD.root'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_rawPCCProd_*_*',
        'keep *_corrPCCProd_*_*')
)


process.path1 = cms.Path(process.rawPCCProd+process.corrPCCProd+process.dqmEnvLumiPCC)

#
process.MessageLogger = cms.Service("MessageLogger",
    FrameworkJobReport = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    cerr = cms.untracked.PSet(
        FwkJob = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        FwkReport = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(100000)
        ),
        FwkSummary = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000),
            optionalPSet = cms.untracked.bool(True),
            reportEvery = cms.untracked.int32(10000)
        ),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            optionalPSet = cms.untracked.bool(True)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        noTimeStamps = cms.untracked.bool(False),
        optionalPSet = cms.untracked.bool(True),
        threshold = cms.untracked.string('INFO')
    ),
    cerr_stats = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        output = cms.untracked.string('cerr'),
        threshold = cms.untracked.string('WARNING')
    ),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport'),
    infos = cms.untracked.PSet(
        #Root_NoDictionary = cms.untracked.PSet(
        #    limit = cms.untracked.int32(0),
        #    optionalPSet = cms.untracked.bool(True)
        #),
        #optionalPSet = cms.untracked.bool(True),
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('INFO')
    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    suppressDebug = cms.untracked.vstring(),
    suppressInfo = cms.untracked.vstring(),
    suppressWarning = cms.untracked.vstring(),
    warnings = cms.untracked.PSet(
        #placeholder = cms.untracked.bool(True)
        threshold = cms.untracked.string('WARNING')
    )
)
#added line for additional output summary `
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.outpath = cms.EndPath(process.out)
