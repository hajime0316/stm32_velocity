/*
 * Module : stm32_velocity
 * File   : stm32_velocity.cpp
 * Version: 01.00
 * Author : hajime
 * 
 * Created on 2019/06/19, 09:38
 */

#include <math.h>
#include "stm32_velocity.hpp"

#ifndef MAX_ENCODER_COUNT
    #error MAX_ENCODER_COUNTが定義されていません．\
           CubeMXの[User Constants]でこのマクロ定\
           数を定義してください．
#endif

Stm32Velocity::Stm32Velocity(const TIM_HandleTypeDef *htim) {
    this->htim = htim;
    this->velocity = 0;
    this->past_encoder_count = htim->Instance->CNT;

    velocity_sign = 1;

    // ハードウェアスタート
    HAL_TIM_Encoder_Start((TIM_HandleTypeDef *)htim, TIM_CHANNEL_ALL);
}

Stm32Velocity::Stm32Velocity(const TIM_HandleTypeDef *htim, int is_reverse_rotation) {
    this->htim = htim;
    this->velocity = 0;
    this->past_encoder_count = htim->Instance->CNT;

    if(is_reverse_rotation) {
        this->velocity_sign = -1;
    }
    else {
        this->velocity_sign = 1;
    }

    // ハードウェアスタート
    HAL_TIM_Encoder_Start((TIM_HandleTypeDef *)htim, TIM_CHANNEL_ALL);
}

Stm32Velocity::~Stm32Velocity() {

}

int Stm32Velocity::periodic_calculate_velocity(void) {
    int present_encoder_count = htim->Instance->CNT;
    
    velocity = present_encoder_count - past_encoder_count;

    if(velocity > MAX_ENCODER_COUNT / 2) {
        velocity -= (MAX_ENCODER_COUNT + 1);
    }
    else if(velocity < -(MAX_ENCODER_COUNT / 2)) {
        velocity += (MAX_ENCODER_COUNT + 1);
    }

    past_encoder_count = present_encoder_count;

    return velocity_sign * velocity;
}

int Stm32Velocity::get_velocity(void) {
    return velocity_sign * velocity;
}

void Stm32Velocity::reverse_rotation() {
    if(velocity_sign == 1) {
        velocity_sign = -1;
    }
    else {
        velocity_sign = 1;
    }
}