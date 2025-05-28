class PwmInput {
  public:

    PwmInput() {
      this->name = "";
      this->pin = -1;
      this->pulseCount = 0;
    }

    PwmInput(char* name, int pin) {
      this->name = name;
      this->pin = pin;
      this->pulseCount = 0;
    }

    
    char* name;
    int pin;
    int pulseCount;
    unsigned long lastPulseTime;
};