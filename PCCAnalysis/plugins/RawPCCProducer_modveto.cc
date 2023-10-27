/**_________________________________________________________________
This module produces the PCC per module per LS. For each LS it writes a line with the PCC for each of the 1865 modules. 
________________________________________________________________**/
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <cmath>
#include "DataFormats/Luminosity/interface/PixelClusterCounts.h"
#include "DataFormats/Luminosity/interface/LumiInfo.h"
#include "DataFormats/Luminosity/interface/LumiConstants.h"
#include "CondFormats/Luminosity/interface/LumiCorrections.h"
#include "CondFormats/DataRecord/interface/LumiCorrectionsRcd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

class RawPCCProducer_modveto : public edm::global::EDProducer<edm::EndLuminosityBlockProducer> {
public:
  explicit RawPCCProducer_modveto(const edm::ParameterSet&);
  ~RawPCCProducer_modveto() override;

private:
  void globalEndLuminosityBlockProduce(edm::LuminosityBlock& lumiSeg, const edm::EventSetup& iSetup) const final;
  void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const final;

  //input object labels
  edm::EDGetTokenT<reco::PixelClusterCounts> pccToken_;


  //output object labels
  const edm::EDPutTokenT<LumiInfo> putToken_;



  //produce csv lumi file
  const bool saveCSVFile_;
  const std::string csvOutLabel_;
  mutable std::mutex fileLock_;
  std::vector<int> allmodules;
  const std::vector<int> modlist;
};

//--------------------------------------------------------------------------------------------------
RawPCCProducer_modveto::RawPCCProducer_modveto(const edm::ParameterSet& iConfig)
    : pccToken_(consumes<reco::PixelClusterCounts, edm::InLumi>(edm::InputTag(
          iConfig.getParameter<edm::ParameterSet>("RawPCCProducerParameters").getParameter<std::string>("inputPccLabel"),
          iConfig.getParameter<edm::ParameterSet>("RawPCCProducerParameters").getParameter<std::string>("ProdInst")))),
      putToken_(produces<LumiInfo, edm::Transition::EndLuminosityBlock>(
          iConfig.getParameter<edm::ParameterSet>("RawPCCProducerParameters")
	  .getUntrackedParameter<std::string>("outputProductName", "alcaLumi"))),
      saveCSVFile_(iConfig.getParameter<edm::ParameterSet>("RawPCCProducerParameters")
		   .getUntrackedParameter<bool>("saveCSVFile", false)),
      csvOutLabel_(iConfig.getParameter<edm::ParameterSet>("RawPCCProducerParameters")
		   .getUntrackedParameter<std::string>("label", std::string("rawPCC.csv"))),
  modlist({303042564, 303042568, 303042572, 303042576, 303042580, 303042584, 303042588, 303042592, 303046660,
	303046664, 303046668, 303046672, 303046676, 303046680, 303046684, 303046688, 303050756, 303050760,
	303050764, 303050768, 303050772, 303050776, 303050780, 303050784, 303054852, 303054856, 303054860,
	303054864, 303054868, 303054872, 303054876, 303054880, 303058948, 303058952, 303058956, 303058960,
	303058964, 303058968, 303058972, 303058976, 303063044, 303063048, 303063052, 303063056, 303063060,
	303063064, 303063068, 303063072, 303067140, 303067144, 303067148, 303067152, 303067156, 303067160,
	303067164, 303067168, 303071236, 303071240, 303071244, 303071248, 303071252, 303071256, 303071260,
	303071264, 303075332, 303075336, 303075340, 303075344, 303075348, 303075352, 303075356, 303075360,
	303079428, 303079432, 303079436, 303079440, 303079444, 303079448, 303079452, 303079456, 303083524,
	303083528, 303083532, 303083536, 303083540, 303083544, 303083548, 303083552, 303087620, 303087624,
	303087628, 303087632, 303087636, 303087640, 303087644, 303087648, 304091140, 304091144, 304091148,
	304091152, 304091156, 304091160, 304091164, 304091168, 304095236, 304095240, 304095244, 304095248,
	304095252, 304095256, 304095260, 304095264, 304099332, 304099336, 304099340, 304099344, 304099348,
	304099352, 304099356, 304099360, 304103428, 304103432, 304103436, 304103440, 304103444, 304103448,
	304103452, 304103456, 304107524, 304107528, 304107532, 304107536, 304107540, 304107544, 304107548,
	304107552, 304111620, 304111624, 304111628, 304111632, 304111636, 304111640, 304111644, 304111648,
	304115716, 304115720, 304115724, 304115728, 304115732, 304115736, 304115740, 304115744, 304119812,
	304119816, 304119820, 304119824, 304119828, 304119832, 304119836, 304119840, 304123908, 304123912,
	304123916, 304123920, 304123924, 304123928, 304123932, 304123936, 304128004, 304128008, 304128012,
	304128016, 304128020, 304128024, 304128028, 304128032, 304132100, 304132104, 304132108, 304132112,
	304132116, 304132120, 304132124, 304132128, 304136196, 304136200, 304136204, 304136208, 304136212,
	304136216, 304136220, 304136224, 304140292, 304140296, 304140300, 304140304, 304140308, 304140312,
	304140316, 304140320, 304144388, 304144392, 304144396, 304144400, 304144404, 304144408, 304144412,
	304144416, 304148484, 304148488, 304148492, 304148496, 304148500, 304148504, 304148508, 304148512,
	304152580, 304152584, 304152588, 304152592, 304152596, 304152600, 304152604, 304152608, 304156676,
	304156680, 304156684, 304156688, 304156692, 304156696, 304156700, 304156704, 304160772, 304160776,
	304160780, 304160784, 304160788, 304160792, 304160796, 304160800, 304164868, 304164872, 304164876,
	304164880, 304164884, 304164888, 304164892, 304164896, 304168964, 304168968, 304168972, 304168976,
	304168980, 304168984, 304168988, 304168992, 304173060, 304173064, 304173068, 304173072, 304173076,
	304173080, 304173084, 304173088, 304177156, 304177160, 304177164, 304177168, 304177172, 304177176,
	304177180, 304177184, 304181252, 304181256, 304181260, 304181264, 304181268, 304181272, 304181276,
	304181280, 304185348, 304185352, 304185356, 304185360, 304185364, 304185368, 304185372, 304185376,
	304189444, 304189448, 304189452, 304189456, 304189460, 304189464, 304189468, 304189472, 304193540,
	304193544, 304193548, 304193552, 304193556, 304193560, 304193564, 304193568, 304197636, 304197640,
	304197644, 304197648, 304197652, 304197656, 304197660, 304197664, 304201732, 304201736, 304201740,
	304201744, 304201748, 304201752, 304201756, 304201760, 305139716, 305139720, 305139724, 305139728,
	305139732, 305139736, 305139740, 305139744, 305143812, 305143816, 305143820, 305143824, 305143828,
	305143832, 305143836, 305143840, 305147908, 305147912, 305147916, 305147920, 305147924, 305147928,
	305147932, 305147936, 305152004, 305152008, 305152012, 305152016, 305152020, 305152024, 305152028,
	305152032, 305156100, 305156104, 305156108, 305156112, 305156116, 305156120, 305156124, 305156128,
	305160196, 305160200, 305160204, 305160208, 305160212, 305160216, 305160220, 305160224, 305164292,
	305164296, 305164300, 305164304, 305164308, 305164312, 305164316, 305164320, 305168388, 305168392,
	305168396, 305168400, 305168404, 305168408, 305168412, 305168416, 305172484, 305172488, 305172492,
	305172496, 305172500, 305172504, 305172508, 305172512, 305176580, 305176584, 305176588, 305176592,
	305176596, 305176600, 305176604, 305176608, 305180676, 305180680, 305180684, 305180688, 305180692,
	305180696, 305180700, 305180704, 305184772, 305184776, 305184780, 305184784, 305184788, 305184792,
	305184796, 305184800, 305188868, 305188872, 305188876, 305188880, 305188884, 305188888, 305188892,
	305188896, 305192964, 305192968, 305192972, 305192976, 305192980, 305192984, 305192988, 305192992,
	305197060, 305197064, 305197068, 305197072, 305197076, 305197080, 305197084, 305197088, 305201156,
	305201160, 305201164, 305201168, 305201172, 305201176, 305201180, 305201184, 305205252, 305205256,
	305205260, 305205264, 305205268, 305205272, 305205276, 305205280, 305209348, 305209352, 305209356,
	305209360, 305209364, 305209368, 305209372, 305209376, 305213444, 305213448, 305213452, 305213456,
	305213460, 305213464, 305213468, 305213472, 305217540, 305217544, 305217548, 305217552, 305217556,
	305217560, 305217564, 305217568, 305221636, 305221640, 305221644, 305221648, 305221652, 305221656,
	305221660, 305221664, 305225732, 305225736, 305225740, 305225744, 305225748, 305225752, 305225756,
	305225760, 305229828, 305229832, 305229836, 305229840, 305229844, 305229848, 305229852, 305229856,
	305233924, 305233928, 305233932, 305233936, 305233940, 305233944, 305233948, 305233952, 305238020,
	305238024, 305238028, 305238032, 305238036, 305238040, 305238044, 305238048, 305242116, 305242120,
	305242124, 305242128, 305242132, 305242136, 305242140, 305242144, 305246212, 305246216, 305246220,
	305246224, 305246228, 305246232, 305246236, 305246240, 305250308, 305250312, 305250316, 305250320,
	305250324, 305250328, 305250332, 305250336, 305254404, 305254408, 305254412, 305254416, 305254420,
	305254424, 305254428, 305254432, 305258500, 305258504, 305258508, 305258512, 305258516, 305258520,
	305258524, 305258528, 305262596, 305262600, 305262604, 305262608, 305262612, 305262616, 305262620,
	305262624, 305266692, 305266696, 305266700, 305266704, 305266708, 305266712, 305266716, 305266720,
	305270788, 305270792, 305270796, 305270800, 305270804, 305270808, 305270812, 305270816, 305274884,
	305274888, 305274892, 305274896, 305274900, 305274904, 305274908, 305274912, 305278980, 305278984,
	305278988, 305278992, 305278996, 305279000, 305279004, 305279008, 305283076, 305283080, 305283084,
	305283088, 305283092, 305283096, 305283100, 305283104, 305287172, 305287176, 305287180, 305287184,
	305287188, 305287192, 305287196, 305287200, 305291268, 305291272, 305291276, 305291280, 305291284,
	305291288, 305291292, 305291296, 305295364, 305295368, 305295372, 305295376, 305295380, 305295384,
	305295388, 305295392, 305299460, 305299464, 305299468, 305299472, 305299476, 305299480, 305299484,
	305299488, 305303556, 305303560, 305303564, 305303568, 305303572, 305303576, 305303580, 305303584,
	305307652, 305307656, 305307660, 305307664, 305307668, 305307672, 305307676, 305307680, 305311748,
	305311752, 305311756, 305311760, 305311764, 305311768, 305311772, 305311776, 305315844, 305315848,
	305315852, 305315856, 305315860, 305315864, 305315868, 305315872, 306188292, 306188296, 306188300,
	306188304, 306188308, 306188312, 306188316, 306188320, 306192388, 306192392, 306192396, 306192400,
	306192404, 306192408, 306192412, 306192416, 306196484, 306196488, 306196492, 306196496, 306196500,
	306196504, 306196508, 306196512, 306200580, 306200584, 306200588, 306200592, 306200596, 306200600,
	306200604, 306200608, 306204676, 306204680, 306204684, 306204688, 306204692, 306204696, 306204700,
	306204704, 306208772, 306208776, 306208780, 306208784, 306208788, 306208792, 306208796, 306208800,
	306212868, 306212872, 306212876, 306212880, 306212884, 306212888, 306212892, 306212896, 306216964,
	306216968, 306216972, 306216976, 306216980, 306216984, 306216988, 306216992, 306221060, 306221064,
	306221068, 306221072, 306221076, 306221080, 306221084, 306221088, 306225156, 306225160, 306225164,
	306225168, 306225172, 306225176, 306225180, 306225184, 306229252, 306229256, 306229260, 306229264,
	306229268, 306229272, 306229276, 306229280, 306233348, 306233352, 306233356, 306233360, 306233364,
	306233368, 306233372, 306233376, 306237444, 306237448, 306237452, 306237456, 306237460, 306237464,
	306237468, 306237472, 306241540, 306241544, 306241548, 306241552, 306241556, 306241560, 306241564,
	306241568, 306245636, 306245640, 306245644, 306245648, 306245652, 306245656, 306245660, 306245664,
	306249732, 306249736, 306249740, 306249744, 306249748, 306249752, 306249756, 306249760, 306253828,
	306253832, 306253836, 306253840, 306253844, 306253848, 306253852, 306253856, 306257924, 306257928,
	306257932, 306257936, 306257940, 306257944, 306257948, 306257952, 306262020, 306262024, 306262028,
	306262032, 306262036, 306262040, 306262044, 306262048, 306266116, 306266120, 306266124, 306266128,
	306266132, 306266136, 306266140, 306266144, 306270212, 306270216, 306270220, 306270224, 306270228,
	306270232, 306270236, 306270240, 306274308, 306274312, 306274316, 306274320, 306274324, 306274328,
	306274332, 306274336, 306278404, 306278408, 306278412, 306278416, 306278420, 306278424, 306278428,
	306278432, 306282500, 306282504, 306282508, 306282512, 306282516, 306282520, 306282524, 306282528,
	306286596, 306286600, 306286604, 306286608, 306286612, 306286616, 306286620, 306286624, 306290692,
	306290696, 306290700, 306290704, 306290708, 306290712, 306290716, 306290720, 306294788, 306294792,
	306294796, 306294800, 306294804, 306294808, 306294812, 306294816, 306298884, 306298888, 306298892,
	306298896, 306298900, 306298904, 306298908, 306298912, 306302980, 306302984, 306302988, 306302992,
	306302996, 306303000, 306303004, 306303008, 306307076, 306307080, 306307084, 306307088, 306307092,
	306307096, 306307100, 306307104, 306311172, 306311176, 306311180, 306311184, 306311188, 306311192,
	306311196, 306311200, 306315268, 306315272, 306315276, 306315280, 306315284, 306315288, 306315292,
	306315296, 306319364, 306319368, 306319372, 306319376, 306319380, 306319384, 306319388, 306319392,
	306323460, 306323464, 306323468, 306323472, 306323476, 306323480, 306323484, 306323488, 306327556,
	306327560, 306327564, 306327568, 306327572, 306327576, 306327580, 306327584, 306331652, 306331656,
	306331660, 306331664, 306331668, 306331672, 306331676, 306331680, 306335748, 306335752, 306335756,
	306335760, 306335764, 306335768, 306335772, 306335776, 306339844, 306339848, 306339852, 306339856,
	306339860, 306339864, 306339868, 306339872, 306343940, 306343944, 306343948, 306343952, 306343956,
	306343960, 306343964, 306343968, 306348036, 306348040, 306348044, 306348048, 306348052, 306348056,
	306348060, 306348064, 306352132, 306352136, 306352140, 306352144, 306352148, 306352152, 306352156,
	306352160, 306356228, 306356232, 306356236, 306356240, 306356244, 306356248, 306356252, 306356256,
	306360324, 306360328, 306360332, 306360336, 306360340, 306360344, 306360348, 306360352, 306364420,
	306364424, 306364428, 306364432, 306364436, 306364440, 306364444, 306364448, 306368516, 306368520,
	306368524, 306368528, 306368532, 306368536, 306368540, 306368544, 306372612, 306372616, 306372620,
	306372624, 306372628, 306372632, 306372636, 306372640, 306376708, 306376712, 306376716, 306376720,
	306376724, 306376728, 306376732, 306376736, 306380804, 306380808, 306380812, 306380816, 306380820,
	306380824, 306380828, 306380832, 306384900, 306384904, 306384908, 306384912, 306384916, 306384920,
	306384924, 306384928, 306388996, 306389000, 306389004, 306389008, 306389012, 306389016, 306389020,
	306389024, 306393092, 306393096, 306393100, 306393104, 306393108, 306393112, 306393116, 306393120,
	306397188, 306397192, 306397196, 306397200, 306397204, 306397208, 306397212, 306397216, 306401284,
	306401288, 306401292, 306401296, 306401300, 306401304, 306401308, 306401312, 306405380, 306405384,
	306405388, 306405392, 306405396, 306405400, 306405404, 306405408, 306409476, 306409480, 306409484,
	306409488, 306409492, 306409496, 306409500, 306409504, 306413572, 306413576, 306413580, 306413584,
	306413588, 306413592, 306413596, 306413600, 306417668, 306417672, 306417676, 306417680, 306417684,
	306417688, 306417692, 306417696, 306421764, 306421768, 306421772, 306421776, 306421780, 306421784,
	306421788, 306421792, 306425860, 306425864, 306425868, 306425872, 306425876, 306425880, 306425884,
	306425888, 306429956, 306429960, 306429964, 306429968, 306429972, 306429976, 306429980, 306429984,
	306434052, 306434056, 306434060, 306434064, 306434068, 306434072, 306434076, 306434080, 306438148,
	306438152, 306438156, 306438160, 306438164, 306438168, 306438172, 306438176, 306442244, 306442248,
	306442252, 306442256, 306442260, 306442264, 306442268, 306442272, 306446340, 306446344, 306446348,
	306446352, 306446356, 306446360, 306446364, 306446368, 352589828, 352593924, 352598020, 352602116,
	352606212, 352610308, 352614404, 352618500, 352622596, 352626692, 352630788, 352634884, 352638980,
	352643076, 352647172, 352651268, 352655364, 352659460, 352663556, 352667652, 352671748, 352675844,
	352679940, 352684036, 352688132, 352692228, 352696324, 352700420, 352704516, 352708612, 352712708,
	352716804, 352720900, 352724996, 352729092, 352733188, 352737284, 352741380, 352745476, 352749572,
	352753668, 352757764, 352761860, 352765956, 352770052, 352774148, 352778244, 352782340, 352786436,
	352790532, 352794628, 352798724, 352802820, 352806916, 352811012, 352815108, 352588804, 352592900,
	352596996, 352601092, 352605188, 352609284, 352613380, 352617476, 352621572, 352625668, 352629764,
	352633860, 352637956, 352642052, 352646148, 352650244, 352654340, 352658436, 352662532, 352666628,
	352670724, 352674820, 352678916, 352683012, 352687108, 352691204, 352695300, 352699396, 352703492,
	352707588, 352711684, 352715780, 352719876, 352723972, 352728068, 352732164, 352736260, 352740356,
	352744452, 352748548, 352752644, 352756740, 352760836, 352764932, 352769028, 352773124, 352777220,
	352781316, 352785412, 352789508, 352793604, 352797700, 352801796, 352805892, 352809988, 352814084,
	352851972, 352856068, 352860164, 352864260, 352868356, 352872452, 352876548, 352880644, 352884740,
	352888836, 352892932, 352897028, 352901124, 352905220, 352909316, 352913412, 352917508, 352921604,
	352925700, 352929796, 352933892, 352937988, 352942084, 352946180, 352950276, 352954372, 352958468,
	352962564, 352966660, 352970756, 352974852, 352978948, 352983044, 352987140, 352991236, 352995332,
	352999428, 353003524, 353007620, 353011716, 353015812, 353019908, 353024004, 353028100, 353032196,
	353036292, 353040388, 353044484, 353048580, 353052676, 353056772, 353060868, 353064964, 353069060,
	353073156, 353077252, 352850948, 352855044, 352859140, 352863236, 352867332, 352871428, 352875524,
	352879620, 352883716, 352887812, 352891908, 352896004, 352900100, 352904196, 352908292, 352912388,
	352916484, 352920580, 352924676, 352928772, 352932868, 352936964, 352941060, 352945156, 352949252,
	352953348, 352957444, 352961540, 352965636, 352969732, 352973828, 352977924, 352982020, 352986116,
	352990212, 352994308, 352998404, 353002500, 353006596, 353010692, 353014788, 353018884, 353022980,
	353027076, 353031172, 353035268, 353039364, 353043460, 353047556, 353051652, 353055748, 353059844,
	353063940, 353068036, 353072132, 353076228, 353114116, 353118212, 353122308, 353126404, 353130500,
	353134596, 353138692, 353142788, 353146884, 353150980, 353155076, 353159172, 353163268, 353167364,
	353171460, 353175556, 353179652, 353183748, 353187844, 353191940, 353196036, 353200132, 353204228,
	353208324, 353212420, 353216516, 353220612, 353224708, 353228804, 353232900, 353236996, 353241092,
	353245188, 353249284, 353253380, 353257476, 353261572, 353265668, 353269764, 353273860, 353277956,
	353282052, 353286148, 353290244, 353294340, 353298436, 353302532, 353306628, 353310724, 353314820,
	353318916, 353323012, 353327108, 353331204, 353335300, 353339396, 353113092, 353117188, 353121284,
	353125380, 353129476, 353133572, 353137668, 353141764, 353145860, 353149956, 353154052, 353158148,
	353162244, 353166340, 353170436, 353174532, 353178628, 353182724, 353186820, 353190916, 353195012,
	353199108, 353203204, 353207300, 353211396, 353215492, 353219588, 353223684, 353227780, 353231876,
	353235972, 353240068, 353244164, 353248260, 353252356, 353256452, 353260548, 353264644, 353268740,
	353272836, 353276932, 353281028, 353285124, 353289220, 353293316, 353297412, 353301508, 353305604,
	353309700, 353313796, 353317892, 353321988, 353326084, 353330180, 353334276, 353338372, 344201220,
	344205316, 344209412, 344213508, 344217604, 344221700, 344225796, 344229892, 344233988, 344238084,
	344242180, 344246276, 344250372, 344254468, 344258564, 344262660, 344266756, 344270852, 344274948,
	344279044, 344283140, 344287236, 344291332, 344295428, 344299524, 344303620, 344307716, 344311812,
	344315908, 344320004, 344324100, 344328196, 344332292, 344336388, 344340484, 344344580, 344348676,
	344352772, 344356868, 344360964, 344365060, 344369156, 344373252, 344377348, 344381444, 344385540,
	344389636, 344393732, 344397828, 344401924, 344406020, 344410116, 344414212, 344418308, 344422404,
	344426500, 344200196, 344204292, 344208388, 344212484, 344216580, 344220676, 344224772, 344228868,
	344232964, 344237060, 344241156, 344245252, 344249348, 344253444, 344257540, 344261636, 344265732,
	344269828, 344273924, 344278020, 344282116, 344286212, 344290308, 344294404, 344298500, 344302596,
	344306692, 344310788, 344314884, 344318980, 344323076, 344327172, 344331268, 344335364, 344339460,
	344343556, 344347652, 344351748, 344355844, 344359940, 344364036, 344368132, 344372228, 344376324,
	344380420, 344384516, 344388612, 344392708, 344396804, 344400900, 344404996, 344409092, 344413188,
	344417284, 344421380, 344425476, 344463364, 344467460, 344471556, 344475652, 344479748, 344483844,
	344487940, 344492036, 344496132, 344500228, 344504324, 344508420, 344512516, 344516612, 344520708,
	344524804, 344528900, 344532996, 344537092, 344541188, 344545284, 344549380, 344553476, 344557572,
	344561668, 344565764, 344569860, 344573956, 344578052, 344582148, 344586244, 344590340, 344594436,
	344598532, 344602628, 344606724, 344610820, 344614916, 344619012, 344623108, 344627204, 344631300,
	344635396, 344639492, 344643588, 344647684, 344651780, 344655876, 344659972, 344664068, 344668164,
	344672260, 344676356, 344680452, 344684548, 344688644, 344462340, 344466436, 344470532, 344474628,
	344478724, 344482820, 344486916, 344491012, 344495108, 344499204, 344503300, 344507396, 344511492,
	344515588, 344519684, 344523780, 344527876, 344531972, 344536068, 344540164, 344544260, 344548356,
	344552452, 344556548, 344560644, 344564740, 344568836, 344572932, 344577028, 344581124, 344585220,
	344589316, 344593412, 344597508, 344601604, 344605700, 344609796, 344613892, 344617988, 344622084,
	344626180, 344630276, 344634372, 344638468, 344642564, 344646660, 344650756, 344654852, 344658948,
	344663044, 344667140, 344671236, 344675332, 344679428, 344683524, 344687620, 344725508, 344729604,
	344733700, 344737796, 344741892, 344745988, 344750084, 344754180, 344758276, 344762372, 344766468,
	344770564, 344774660, 344778756, 344782852, 344786948, 344791044, 344795140, 344799236, 344803332,
	344807428, 344811524, 344815620, 344819716, 344823812, 344827908, 344832004, 344836100, 344840196,
	344844292, 344848388, 344852484, 344856580, 344860676, 344864772, 344868868, 344872964, 344877060,
	344881156, 344885252, 344889348, 344893444, 344897540, 344901636, 344905732, 344909828, 344913924,
	344918020, 344922116, 344926212, 344930308, 344934404, 344938500, 344942596, 344946692, 344950788,
	344724484, 344728580, 344732676, 344736772, 344740868, 344744964, 344749060, 344753156, 344757252,
	344761348, 344765444, 344769540, 344773636, 344777732, 344781828, 344785924, 344790020, 344794116,
	344798212, 344802308, 344806404, 344810500, 344814596, 344818692, 344822788, 344826884, 344830980,
	344835076, 344839172, 344843268, 344847364, 344851460, 344855556, 344859652, 344863748, 344867844,
	344871940, 344876036, 344880132, 344884228, 344888324, 344892420, 344896516, 344900612, 344904708,
	344908804, 344912900, 344916996, 344921092, 344925188, 344929284, 344933380, 344937476, 344941572,
	344945668, 344949764})
{
  //std::cout<< "pixel detector module list size is "<< modlist.size() <<std::endl;
}

//--------------------------------------------------------------------------------------------------
RawPCCProducer_modveto::~RawPCCProducer_modveto() {}

//--------------------------------------------------------------------------------------------------
void RawPCCProducer_modveto::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const {}

//--------------------------------------------------------------------------------------------------
void RawPCCProducer_modveto::globalEndLuminosityBlockProduce(edm::LuminosityBlock& lumiSeg,
                                                     const edm::EventSetup& iSetup) const {


  //////////////////////////////////
  /// read input , clusters per module per bx
  /////////////////////////////////
  const edm::Handle<reco::PixelClusterCounts> pccHandle = lumiSeg.getHandle(pccToken_);
  const reco::PixelClusterCounts& inputPcc = *(pccHandle.product());
  //vector with Module IDs 1-1 map to bunch x-ing in clusers
  auto modID = inputPcc.readModID();
  //vector with total events at each bxid.
  auto events = inputPcc.readEvents();
  //cluster counts per module per bx
  auto clustersPerBXInput = inputPcc.readCounts();

 
//  std::cout<< std::to_string(lumiSeg.run()) << ",";
//  std::cout << std::to_string(lumiSeg.luminosityBlock()) << ",";
//  std::cout<<modID.size()<<std::endl;
//  for (unsigned int j=0;j<modID.size();j++)
//    if (std::count(modlist.begin(), modlist.end(), modID.at(j))==0) 
//      std::cout << "Module id not found in modlist: "<<modID.at(j)<<std::endl;
//  for (unsigned int j=0;j<modID.size();j++)
//    std::cout<<modID.at(j)<<std::endl;


  ///////////////////////////////////////////////////////
  ///Lumi saved in the LuminosityBlocks
  LumiInfo outputLumiInfo;
  lumiSeg.emplace(putToken_, std::move(outputLumiInfo));


  //Lumi saved in the csv file
  if (saveCSVFile_) {

    std::map<int, int> modmap;
    for (unsigned int i=0;i<modlist.size();i++){
      modmap.insert(std::pair<int, int>(modlist.at(i), 0));
    }

    int nevt=0;
    for (int bx = 0; bx < int(LumiConstants::numBX); bx++)
      nevt+=events[bx];

    //calculate total of clusters per module
    for (unsigned int i=0;i<modlist.size();i++){
      int k=-1;
      for (unsigned int j=0;j<modID.size();j++)
	if(modID.at(j)==modlist.at(i))
	  k=j;

      int m_c=0;
      if(k>=0){
	for (int bx=0;bx<int(LumiConstants::numBX);bx++)
	  m_c += clustersPerBXInput.at(bx+k*int(LumiConstants::numBX));

      }

      if(nevt>0) m_c /= nevt;
      else m_c=0;

      modmap.find(modlist.at(i))->second = m_c;
    }


    std::lock_guard<std::mutex> lock(fileLock_);
    std::ofstream csfile(csvOutLabel_, std::ios_base::app);
    csfile << std::to_string(lumiSeg.run()) << ",";
    csfile << std::to_string(lumiSeg.luminosityBlock()) << ",";
    std::map<int, int>::iterator itr;
    for (itr = modmap.begin(); itr != modmap.end(); ++itr) {
      //csfile << itr->first<<",";
      csfile << itr->second<<",";
    }
    
    csfile<<'\n';
    csfile.close();
  }
}

DEFINE_FWK_MODULE(RawPCCProducer_modveto);
