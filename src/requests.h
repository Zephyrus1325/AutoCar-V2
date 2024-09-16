#include <Arduino.h>

String HTMLMotorMode(int mode){
    return (String) "[" + mode + "]";
}

String HTMLMotorReadings(float reading[4]){
    return (String) "[" + reading[0] + "," + reading[1] + "," + reading[2] + "," + reading[3] + "]";
}

String HTMLUltrassoundReadings(float reading[8]){
    return (String) "[" + reading[0] + "," + reading[1] + "," + reading[2] + "," + reading[3] + "," + reading[4] + "," + reading[5] + "," + reading[6] + "," + reading[7] + "]";
}