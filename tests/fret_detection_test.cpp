#include <assert.h>
#include <stdint.h>

#include "../software/firmwareCtl/FretDetection.h"

static const FretDetectionTimings timings = {4, 12, 30, 12};

static FretObservation contacts(uint32_t mask) {
  return FretDetector::observe(mask, 21);
}

int main() {
  FretObservation multiple = contacts((1u << 1) | (1u << 4) | (1u << 5));
  assert(multiple.highest == 6);
  assert(multiple.candidate == 6);
  assert(multiple.hasPair);
  assert(multiple.contactCount == 3);

  FretDetector detector;
  assert(detector.update(contacts(1u << 3), 0, timings) == 0);
  assert(detector.update(contacts((1u << 3) | (1u << 4)), 1, timings) == 0);
  assert(detector.update(contacts((1u << 3) | (1u << 4)), 5, timings) == 5);

  // Pair confirmation requires uninterrupted pair observations.
  detector.reset();
  assert(detector.update(contacts((1u << 3) | (1u << 4)), 10, timings) == 0);
  assert(detector.update(contacts(1u << 4), 13, timings) == 0);
  assert(detector.update(contacts((1u << 3) | (1u << 4)), 14, timings) == 0);
  assert(detector.update(contacts((1u << 3) | (1u << 4)), 18, timings) == 5);

  // A dissolving pair does not become the lower fret during the release hold.
  assert(detector.update(contacts(1u << 3), 19, timings) == 5);
  assert(detector.update(contacts(0), 20, timings) == 5);
  assert(detector.update(contacts(0), 31, timings) == 5);
  assert(detector.update(contacts(0), 32, timings) == 0);

  // Fret one and an incomplete higher fret use their separate fallback times.
  assert(detector.update(contacts(1u), 40, timings) == 0);
  assert(detector.update(contacts(1u), 52, timings) == 1);
  detector.reset();
  assert(detector.update(contacts(1u << 6), 50, timings) == 0);
  assert(detector.update(contacts(1u << 6), 79, timings) == 0);
  assert(detector.update(contacts(1u << 6), 80, timings) == 7);

  // A confirmed new pair changes fret without an intermediate release.
  assert(detector.update(contacts((1u << 8) | (1u << 9)), 81, timings) == 7);
  assert(detector.update(contacts((1u << 8) | (1u << 9)), 85, timings) == 10);

  // Unsigned subtraction keeps timing valid over the millis() wraparound.
  detector.reset();
  assert(detector.update(contacts(1u), UINT32_MAX - 5, timings) == 0);
  assert(detector.update(contacts(1u), 6, timings) == 1);
}
