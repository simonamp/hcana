#ifndef ROOT_THcDC
#define ROOT_THcDC

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THcDC                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaTrackingDetector.h"
#include "THcHitList.h"
#include "THcRawDCHit.h"
#include "THcSpacePoint.h"
#include "THcDriftChamberPlane.h"
#include "THcDriftChamber.h"
#include "TMath.h"

//class THaScCalib;
class TClonesArray;

class THcDC : public THaTrackingDetector, public THcHitList {

public:
  THcDC( const char* name, const char* description = "",
		   THaApparatus* a = NULL );
  virtual ~THcDC();

  virtual Int_t      Decode( const THaEvData& );
  virtual EStatus    Init( const TDatime& run_time );
  virtual Int_t      CoarseTrack( TClonesArray& tracks );
  virtual Int_t      FineTrack( TClonesArray& tracks );
  
  virtual Int_t      ApplyCorrections( void );

  //  Int_t GetNHits() const { return fNhit; }
  
  Int_t GetNTracks() const { return fTrackProj->GetLast()+1; }
  const TClonesArray* GetTrackHits() const { return fTrackProj; }

  Int_t GetNWires(Int_t plane) const { return fNWires[plane-1];}
  Int_t GetNChamber(Int_t plane) const { return fNChamber[plane-1];}
  Int_t GetWireOrder(Int_t plane) const { return fWireOrder[plane-1];}
  Double_t GetPitch(Int_t plane) const { return fPitch[plane-1];}
  Double_t GetCentralWire(Int_t plane) const { return fCentralWire[plane-1];}
  Int_t GetTdcWinMin(Int_t plane) const { return fTdcWinMin[plane-1];}
  Int_t GetTdcWinMax(Int_t plane) const { return fTdcWinMax[plane-1];}

  Double_t GetZPos(Int_t plane) const { return fZPos[plane-1];}
  Double_t GetAlphaAngle(Int_t plane) const { return fAlphaAngle[plane-1];}
  Double_t GetBetaAngle(Int_t plane) const { return fBetaAngle[plane-1];}
  Double_t GetGammaAngle(Int_t plane) const { return fGammaAngle[plane-1];}

  Int_t GetMinHits(Int_t chamber) const { return fMinHits[chamber-1];}
  Int_t GetMaxHits(Int_t chamber) const { return fMaxHits[chamber-1];}
  Int_t GetMinCombos(Int_t chamber) const { return fMinCombos[chamber-1];}
  Double_t GetSpacePointCriterion(Int_t chamber) const { return fSpace_Point_Criterion2[chamber-1];}
  Double_t GetCentralTime(Int_t plane) const { return fCentralTime[plane-1];}
  Int_t GetDriftTimeSign(Int_t plane) const { return fDriftTimeSign[plane-1];}

  Double_t GetPlaneTimeZero(Int_t plane) const { return fPlaneTimeZero[plane-1];}
  Double_t GetSigma(Int_t plane) const { return fSigma[plane-1];}

  Double_t GetNSperChan() const { return fNSperChan;}

  Double_t GetCenter(Int_t plane) const {
    Int_t chamber = GetNChamber(plane)-1;
    return
      fXCenter[chamber]*sin(fAlphaAngle[plane-1]) +
      fYCenter[chamber]*cos(fAlphaAngle[plane-1]);
  }
  //  friend class THaScCalib;

  THcDC();  // for ROOT I/O
protected:
  Int_t fDebugDC;
  // Calibration

  // Per-event data
  Int_t fNhits;

  // Potential Hall C parameters.  Mostly here for demonstration
  Int_t fNPlanes;
  char** fPlaneNames;
  Int_t fNChambers;

  Double_t fNSperChan;		/* TDC bin size */
  Double_t fWireVelocity;
  Int_t fSingleStub;		/* If 1, single stubs make tracks */
  Int_t fNTracksMaxFP;
  Double_t fXtTrCriterion;
  Double_t fYtTrCriterion;
  Double_t fXptTrCriterion;
  Double_t fYptTrCriterion;

  // Each of these will be dimensioned with the number of chambers
  Double_t* fXCenter;
  Double_t* fYCenter;
  Int_t* fMinHits;
  Int_t* fMaxHits;
  Int_t* fMinCombos;
  Double_t* fSpace_Point_Criterion2;

  // Each of these will be dimensioned with the number of planes
  // A THcDCPlane class object will need to access the value for
  // its plane number.  Should we have a Get method for each or 
  Int_t* fTdcWinMin;
  Int_t* fTdcWinMax;
  Int_t* fCentralTime;
  Int_t* fNWires;		// Number of wires per plane
  Int_t* fNChamber;
  Int_t* fWireOrder;
  Int_t* fDriftTimeSign;

  Double_t* fZPos;
  Double_t* fAlphaAngle;
  Double_t* fBetaAngle;
  Double_t* fGammaAngle;
  Double_t* fPitch;
  Double_t* fCentralWire;
  Double_t* fPlaneTimeZero;
  Double_t* fSigma;

  // Useful derived quantities
  // double tan_angle, sin_angle, cos_angle;
  
  // Intermediate structure for building 
  static const char MAXTRACKS = 50;
  struct TrackSP {
    Int_t nSP; /* Number of space points in this track */
    Int_t spID[10];		/* List of space points in this track */
  } fTrackSP[MAXTRACKS];

  std::vector<THcDriftChamberPlane*> fPlanes; // List of plane objects
  std::vector<THcDriftChamber*> fChambers; // List of chamber objects

  TClonesArray*  fTrackProj;  // projection of track onto scintillator plane
                              // and estimated match to TOF paddle
  void           ClearEvent();
  void           DeleteArrays();
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode = kDefine );
  void           LinkStubs();

  void Setup(const char* name, const char* description);

  ClassDef(THcDC,0)   // Drift Chamber class
};

////////////////////////////////////////////////////////////////////////////////

#endif
