#ifndef FRET_DETECTION_H
#define FRET_DETECTION_H

#include <stdint.h>

struct FretObservation {
  uint8_t highest;
  uint8_t candidate;
  uint8_t contactCount;
  bool hasPair;
};

enum FretDetectionState : uint8_t {
  FRET_RELEASED,
  FRET_CANDIDATE,
  FRET_PRESSED,
  FRET_RELEASING
};

struct FretDetectionTimings {
  uint16_t pairMs;
  uint16_t firstFretMs;
  uint16_t singleMs;
  uint16_t releaseMs;
};

class FretDetector {
 public:
  FretDetector()
      : state_(FRET_RELEASED), output_(0), candidate_(0), candidateKind_(0),
        candidateSince_(0) {}

  static FretObservation observe(uint32_t contacts, uint8_t fretCount) {
    FretObservation result = {0, 0, 0, false};
    uint8_t highestPair = 0;

    for (uint8_t fret = 1; fret <= fretCount; ++fret) {
      if ((contacts & (uint32_t(1) << (fret - 1))) == 0) continue;
      ++result.contactCount;
      result.highest = fret;
      if (fret > 1 && (contacts & (uint32_t(1) << (fret - 2))) != 0) {
        highestPair = fret;
      }
    }

    result.hasPair = highestPair != 0;
    result.candidate = result.hasPair ? highestPair : result.highest;
    return result;
  }

  uint8_t update(const FretObservation &observation, uint32_t now,
                 const FretDetectionTimings &timings) {
    const uint8_t observed = observation.candidate;

    if (output_ != 0 && observed == output_) {
      state_ = FRET_PRESSED;
      candidate_ = 0;
      return output_;
    }

    const uint8_t kind = candidateKind(observation);
    if (observed != candidate_ || kind != candidateKind_) {
      candidate_ = observed;
      candidateKind_ = kind;
      candidateSince_ = now;
    }

    if (output_ == 0) {
      if (observed == 0) {
        state_ = FRET_RELEASED;
        return output_;
      }
      state_ = FRET_CANDIDATE;
    } else {
      state_ = FRET_RELEASING;
    }

    const uint16_t requiredMs = thresholdFor(observation, timings);
    if (elapsed(now, candidateSince_) < requiredMs) return output_;

    output_ = observed;
    candidate_ = 0;
    candidateKind_ = 0;
    state_ = output_ == 0 ? FRET_RELEASED : FRET_PRESSED;
    return output_;
  }

  void reset(uint8_t output = 0) {
    output_ = output;
    candidate_ = 0;
    candidateKind_ = 0;
    candidateSince_ = 0;
    state_ = output == 0 ? FRET_RELEASED : FRET_PRESSED;
  }

  uint8_t output() const { return output_; }
  FretDetectionState state() const { return state_; }

 private:
  static uint32_t elapsed(uint32_t now, uint32_t since) { return now - since; }

  static uint16_t thresholdFor(const FretObservation &observation,
                               const FretDetectionTimings &timings) {
    if (observation.candidate == 0) return timings.releaseMs;
    if (observation.hasPair) return timings.pairMs;
    if (observation.candidate == 1) return timings.firstFretMs;
    return timings.singleMs;
  }

  static uint8_t candidateKind(const FretObservation &observation) {
    if (observation.candidate == 0) return 0;
    if (observation.hasPair) return 1;
    if (observation.candidate == 1) return 2;
    return 3;
  }

  FretDetectionState state_;
  uint8_t output_;
  uint8_t candidate_;
  uint8_t candidateKind_;
  uint32_t candidateSince_;
};

#endif
