class PwmOutput {
  public:

    PwmOutput() {
      this->name = "";
      this->pin = -1;
      this->freq = 0;
      this->resolution = 8;
    }

    PwmOutput(char* name, int pin, int freq, int resolution) {
      this->name = name;
      this->pin = pin;
      this->freq = freq;
      this->resolution = resolution;
    }

    
    char* name;
    int pin;
    int freq;
    int resolution;
    int dutyCycle = 0;
    int correctedDutyCycle = 0;
    int maxDutyCicle = (int)(pow(2, resolution) - 1); 
};