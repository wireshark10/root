// @(#)root/tmva $Id$
// Author:

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodDAE                                                             *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      NeuralNetwork                                                             *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *                                                                                *
 *                                                                                *
 *                                                                                *
 * Copyright (c) 2005-2015:                                                       *
 *                                                                                *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_MethodDAE
#define ROOT_TMVA_MethodDAE

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MethodDAE                                                            //
//                                                                      //
// Deep AutoEnoder implementation                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//#include "TMVA/DNN/DAE/CorruptionLayer.h"
//include "TMVA/DNN/DAE/CompressionLayer.h"
//#include "TMVA/DNN/DAE/ReconstructionLayer.h"
//#include "TMVA/DNN/DAE/LogisticRegressionLayer.h"
#include "TMVA/DNN/DeepNet.h"
#include "TMVA/MethodBase.h"
#include "TString.h"

#include "TMVA/DNN/Architectures/Reference.h"

#ifdef DNNCPU
#include "TMVA/DNN/Architectures/Cpu.h"
#endif

#ifdef DNNCUDA
#include "TMVA/DNN/Architectures/Cuda.h"
#endif

#include <tuple>
#include <vector>

using namespace TMVA::DNN::DAE;
using namespace TMVA::DNN;

namespace TMVA {

  struct TTrainingSettings {
    size_t batchSize;
    size_t testInterval;
    size_t convergenceSteps;
    //DNN::ERegularization regularization;
    Double_t learningRate;
    //Double_t momentum;
    Double_t weightDecay;
    Double_t corruption;
    bool multithreading;
  };

class MethodDAE : public MethodBase {
public:
  using Architecture_t = TReference<Double_t>;
  using Matrix_t = typename Architecture_t::Matrix_t;

private:
  using LayoutVector_t = std::vector<Int_t>;
  using KeyValueVector_t = std::vector<std::map<TString, TString>>;


  TDeepAutoEncoder<Architecture_t>* net;
  EInitialization fWeightInitialization; ///< The initialization method
  EOutputFunction fOutputFunction; ///< The output function for making the predictions

  TString fLayoutString; ///< The string defining the layout of the DAE
  TString fErrorStrategy; ///< The string defining the error strategy for training
  TString fTrainingStrategyString; ///< The string defining the training strategy
  TString fWeightInitializationString; ///< The string defining the weight
  TString fArchitectureString; ///< The string defining the architecure: CPU or GPU
  bool fResume;

  LayoutVector_t fLayout; ///< Dimensions and activation functions of each layer
  std::vector<TTrainingSettings> fTrainingSettings; ///< The vector defining each training strategy

  void Init();

  // the option handling methods
  void DeclareOptions();
  void ProcessOptions();

  // Write and read weights from an XML file
  static inline void WriteMatrixXML(void *parent, const char *name,
                                    const TMatrixT<Double_t> &X);
  static inline void ReadMatrixXML(void *xml, const char *name,
                                   TMatrixT<Double_t> &X);

  ClassDef(MethodDAE, 0);

  /*template <typename T>
  TString fetchValue (const std::map<TString, TString>& keyValueMap, TString key);
  T MethodDAE::fetchValue(const std::map<TString,TString>& keyValueMap, TString key, T defaultValue);
  std::vector<double> fetchValue(const std::map<TString, TString> & keyValueMap, TString key, std::vector<double> defaultValue);
  int fetchValue(const std::map<TString,TString>& keyValueMap, TString key, int defaultValue);
  double fetchValue (const std::map<TString,TString>& keyValueMap, TString key, double defaultValue);
  TString fetchValue (const std::map<TString,TString>& keyValueMap, TString key, TString defaultValue);
  bool fetchValue (const std::map<TString,TString>& keyValueMap, TString key, bool defaultValue); */


protected:
  void GetHelpMessage() const;

public:
  // Standard Constructors
  MethodDAE(const TString &jobName, const TString &methodTitle,
            DataSetInfo &theData, const TString &theOption);

  MethodDAE(DataSetInfo &theData, const TString &theWeightFile);

  virtual ~MethodDAE();

  virtual Bool_t HasAnalysisType(Types::EAnalysisType type,
                                 UInt_t numberClasses, UInt_t numberTargets);

  LayoutVector_t ParseLayoutString(TString layerSpec);

  void Train();
  void TrainGpu();
  void TrainCpu();

  virtual Double_t GetMvaValue(Double_t *err = 0, Double_t *errUpper = 0);
  // virtual const std::vector<Float_t>& GetRegressionValues();
  // virtual const std::vector<Float_t>& GetMulticlassValues();

  using MethodBase::ReadWeightsFromStream;

  // write weights to stream
  void AddWeightsXMLTo(void *parent) const;

  // read weights from stream
  void ReadWeightsFromStream(std::istream &i);
  void ReadWeightsFromXML(void *wghtnode);

  // ranking of input variables
  const Ranking *CreateRanking();

  KeyValueVector_t ParseKeyValueString(TString parseString,
                                      TString blockDelim,
                                      TString tokenDelim);
};
} // namespace TMVA

#endif
