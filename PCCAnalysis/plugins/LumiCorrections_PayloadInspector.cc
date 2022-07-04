/*!
  \file PCCAfterglowCorr_PayloadInspector
  \Payload Inspector Plugin for PCC luminosity corrections 
  \author Jose Benitez
  \version $Revision: 1.0 $
  \date $Date: 2022/06/22
*/

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondCore/CondDB/interface/Time.h"

// the data format of the condition to be inspected
//#include "CondFormats/SiPixelObjects/interface/SiPixelQuality.h"
//#include "DataFormats/DetId/interface/DetId.h"
#include "CondFormats/Luminosity/interface/LumiCorrections.h"
#include "CondFormats/DataRecord/interface/LumiCorrectionsRcd.h"

#include <memory>
#include <sstream>
#include <iostream>

namespace {

  /************************************************
    test class
  *************************************************/

  class LumiCorrectionsTest : public cond::payloadInspector::Histogram1D<LumiCorrections> {
    
  public:
    LumiCorrectionsTest() : cond::payloadInspector::Histogram1D<LumiCorrections>("LumiCorrections test",
										  "LumiCorrections test", 10,0.0,10.0){
      Base::setSingleIov( true );
    }
    
    bool fill( const std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs ) override {
      for ( auto const & iov: iovs) {
	std::shared_ptr<LumiCorrections> payload = Base::fetchPayload( std::get<1>(iov) );
	
	if( payload.get() ){
	  fillWithValue(1.);
	}// payload

      }// iovs
      return true;
    }// fill
  };
  


  /************************************************
    summary class
  *************************************************/

  class LumiCorrectionsSummary : public cond::payloadInspector::PlotImage<LumiCorrections> {

  public:
    LumiCorrectionsSummary() : cond::payloadInspector::PlotImage<LumiCorrections>("LumiCorrections Summary"){
      setSingleIov( false );
    }

    bool fill( const std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs ) override{

      std::vector<std::tuple<cond::Time_t,cond::Hash> > sorted_iovs = iovs;

      for(const auto &iov: iovs){

	std::shared_ptr<LumiCorrections> payload = fetchPayload( std::get<1>(iov) );

        auto unpacked = unpack(std::get<0>(iov));
	std::cout<< unpacked.first <<","<< unpacked.second<<std::endl;

	std::ofstream csvfile;
	char csvname[100];
	sprintf(csvname,"LumiCorrections_%d_%d.payload",unpacked.first,unpacked.second);
	csvfile.open(csvname, std::ios_base::out);
	csvfile<<unpacked.first<<","<<unpacked.second;

	std::vector<float> correctionScaleFactors_ = payload->getCorrectionsBX();
	for(int i=0;i<3564;i++){
	  //std::cout<<i<<","<<correctionScaleFactors_[i]<<std::endl;
	  
	  csvfile<<","<<correctionScaleFactors_[i];
	}

	csvfile<<std::endl;
	csvfile.close();
      }

      return true;
    }

    std::pair<unsigned int,unsigned int> unpack(cond::Time_t since){
      auto kLowMask = 0XFFFFFFFF;
      auto run  = (since >> 32);
      auto lumi = (since & kLowMask);
      return std::make_pair(run,lumi);
    }

  };


  /************************************************
    time history class
  *************************************************/

  class LumiCorrectionsTimeHistory : public cond::payloadInspector::TimeHistoryPlot<LumiCorrections,std::pair<double,double> > {

  public:
    LumiCorrectionsTimeHistory() : cond::payloadInspector::TimeHistoryPlot<LumiCorrections,std::pair<double,double> >(".. vs time",".."){}

    std::pair<double,double> getFromPayload(LumiCorrections& payload ) override{
      return std::make_pair(1.,0.);
    }

  };


} // namespace

// Register the classes as boost python plugin
PAYLOAD_INSPECTOR_MODULE(LumiCorrections){
  PAYLOAD_INSPECTOR_CLASS(LumiCorrectionsTest);
  PAYLOAD_INSPECTOR_CLASS(LumiCorrectionsSummary);
  PAYLOAD_INSPECTOR_CLASS(LumiCorrectionsTimeHistory);
}
