/**
@file  convertiblebond.hpp
@brief The payoff of a Convertible Bond
*/

#ifndef ORF_CONVERTIBLEBOND_HPP
#define ORF_CONVERTIBLEBOND_HPP

#include <orflib/products/product.hpp>
#include <algorithm>
#include <vector>  
BEGIN_NAMESPACE(orf)

class ConvertibleBond : public Product
{
public:
  ConvertibleBond(double faceValue,
                  double timeToMat,
                  double conversionRatio,
                  double convStartTime,
                  double convEndTime,
                  double callStrike,
                  double callStartTime,
                  double callEndTime);

  virtual size_t nAssets() const override { return 1; }

  /**  Not used here but required by abstract base */
  virtual void eval(Matrix const& pricePath) override { ORF_ASSERT(0, "MC not supported for CB"); }

  virtual void eval(size_t idx, Vector const& spots, double contValue) override;

private:
  double faceValue_;
  double conversionRatio_;
  double convStartTime_;
  double convEndTime_;
  double callStrike_;
  double callStartTime_;
  double callEndTime_;
};

inline
ConvertibleBond::ConvertibleBond(double faceValue,
                                 double timeToMat,
                                 double conversionRatio,
                                 double convStartTime,
                                 double convEndTime,
                                 double callStrike,
                                 double callStartTime,
                                 double callEndTime)
  : faceValue_(faceValue),
    conversionRatio_(conversionRatio),
    convStartTime_(convStartTime),
    convEndTime_(convEndTime),
    callStrike_(callStrike),
    callStartTime_(callStartTime),
    callEndTime_(callEndTime)
{
  ORF_ASSERT(timeToMat > 0.0, "ConvertibleBond: time to maturity must be positive!");
  
  // Use std::vector for dynamic construction
  size_t ndays = static_cast<size_t>(timeToMat * DAYS_PER_YEAR);
  std::vector<double> sched;
  sched.reserve(ndays + 2);

  for (size_t i = 0; i <= ndays; ++i) {
    sched.push_back(i / DAYS_PER_YEAR);
  }

  // Ensure exact maturity is the last point
  if (sched.back() < timeToMat - 1.0e-8) {
      sched.push_back(timeToMat);
  } else {
      sched.back() = timeToMat;
  }

  // Copy to the Armadillo fixTimes_ vector
  fixTimes_.resize(sched.size());
  for(size_t i = 0; i < sched.size(); ++i) {
      fixTimes_[i] = sched[i];
  }

  payTimes_ = fixTimes_;
  payAmounts_.resize(payTimes_.size());
}

inline void ConvertibleBond::eval(size_t idx, Vector const& spots, double contValue)
{
  double t = fixTimes_[idx];
  double S = spots[0]; 

  // Calculate Conversion Value
  double conversionValue = 0.0;
  if (t >= convStartTime_ && t <= convEndTime_) {
      conversionValue = conversionRatio_ * S;
  }

  if (idx == fixTimes_.size() - 1) {
      // At maturity, we can convert or take the face value
      double val = std::max(faceValue_, conversionValue);
      payAmounts_[idx] = val;
      return; 
  }

  // Induction Logic (for t < T)
    double bondValue = contValue;

  if (t >= callStartTime_ && t <= callEndTime_) {
      bondValue = std::min(bondValue, callStrike_);
  }

  // Apply Convertibility 
  if (t >= convStartTime_ && t <= convEndTime_) {
      bondValue = std::max(bondValue, conversionValue);
  }

  payAmounts_[idx] = bondValue;
}

END_NAMESPACE(orf)

#endif 