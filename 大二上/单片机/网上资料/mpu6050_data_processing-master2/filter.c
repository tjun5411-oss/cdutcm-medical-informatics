#include "filter.h"

float one_filter_angle = 0;

float one_filter(float angle_m, float gyro_m)
{
    float K1 = 0.1;
    float dt = 0.005;
    
    one_filter_angle = K1 * angle_m + (1-K1) * (one_filter_angle + gyro_m * dt);
    return one_filter_angle;
}

// ???????(???)
float simple_kalman_filter(float angle_m)
{
    static float angle = 0;
    static float P = 1.0;
    float K;
    
    // ??
    angle = angle;
    P = P + 0.01;  // Q
    
    // ??
    K = P / (P + 0.1);  // R=0.1
    angle = angle + K * (angle_m - angle);
    P = (1 - K) * P;
    
    return angle;
}