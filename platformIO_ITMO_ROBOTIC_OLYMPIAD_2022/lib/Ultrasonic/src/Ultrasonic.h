#ifndef _ULTRA_H_
#define _ULTRA_H_

class Ultrasonic {
    private:
        int trigPin, echoPin;
    public:
        long duration;
        int distance;
        void init(int trigPin, int echoPin);
        int getDistance();
        bool objectDetected(int range);
};

#endif