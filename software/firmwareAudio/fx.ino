void chFX(byte para, float val){
//   if(para==0)outMix8.gain(1,val);
//   if(para==1)verb.roomsize(val);
//   if(para==2)verb.damping(val);
   if(para==3){
    for (int i=0; i<nStrings; i++) {
  }
   }
}

void chCoilDel(int string, float freq) {

    if(freq <= 0.0f){
        coilDelay[string].delay(0, 0);
        return;
    }

    // constants
    const float FS = 44100.0f;        // sample rate
    const int BLOCK = 32;             // block size (quantization)
    const int SYSTEM_LAT = 64;        // fixed system latency in samples
    const int K_MAX = 6;             // how many multiples of T to consider

    float tau0 = SYSTEM_LAT / FS;     // fixed latency in seconds
    float T = 1.0f / freq;            // period duration

    int bestDelaySamples = 0;
    float bestError = 1e9;

    for(int k = 1; k <= K_MAX; k++)
    {
        float targetTotal = k * T;             // desired total latency (s)
        float tauAdd = targetTotal - tau0;     // required additional delay (s)

        if(tauAdd < 0.f) 
            continue; // total latency too short → skip

        float samplesAdd = tauAdd * FS;        // theoretical samples needed

        // quantize to nearest block
        int quantized = (int)roundf(samplesAdd / BLOCK) * BLOCK;
        if(quantized < 0) quantized = 0;

        float realizedAdd = (float)quantized / FS;
        float realizedTotal = tau0 + realizedAdd;

        float err = fabsf(realizedTotal - targetTotal);

        if(err < bestError){
            bestError = err;
            bestDelaySamples = quantized;
        }
    }

    float delayMs = 1000.f * bestDelaySamples / FS;

    coilDelay[string].delay(0, delayMs);
}

