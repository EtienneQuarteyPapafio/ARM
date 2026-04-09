//delayRTOS

//A simple code for a delay effect on the daisy seed implementing i2c communication
//for tap tempo utilisation using FreeRTOS

//Header files for Daisyseed
#include <DaisyDSP.h>
#include <DaisyDuino.h>

DaisyHardware hw;

size_t num_channels;

//Set max delay time to half of sample rate

#define MAX_DELAY static_cast<size_t>(48000*0.50f)

// Declare a delay line of MAX_DELAY number of floats

static DelayLine<float, MAX_DELAY> del;

//creates callback function

void MyCallBack(float **in, float **out, size_t size){
 //declare variables feedback, delay output, and signal output

 float feedback, del_out, sig_out; 

 for(size_t i=0; i<size; i++){ //for loop incrimenting to write to the size of buffer

 //read from delay line
 del_out=del.Read();

 //Calculate output and feedback

 sig_out=del_out;
 feedback=(del_out*0.75f);

 //Write to the delay

 del.Write(feedback);

 //Write to all channels

  for (size_t chn = 0; chn < num_channels; chn++){
  out[chn][i]=sig_out;
  }
 }
}


void setup(){
  float sample_rate;

 //initialize daisy seed at 48khz
  hw=DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  num_channels=hw.num_channels;
  sample_rate=DAISY.get_samplerate();

  del.Init();

  //set delay time to 0.75 seconds
  del.SetDelay(sample_rate*0.75f);

  DAISY.begin(MyCallBack);


}

void loop() {}