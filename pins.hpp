#pragma once

// ПИНЫ
namespace pins
{
    // МОТОРЫ
    enum MOTOR
    {
        MM_IN_1000_TICKS = 250, // В 1000 ТИКАХ
        DEFAULT_SPEED = 8,      // скорость движения по умолчанию
        TRACK = 178,            // КОЛЕЯ

        // ЛЕВЫЙ
        ML_INVERT = false,
        ML_SPEED = 5,
        ML_DIR = 4,
        ML_ENC_A = 2, // ЖЕЛТЫЙ
        ML_ENC_B = 8, // ЗЕЛЁНЫЙ

        // ПРАВЫЙ
        MR_INVERT = true,
        MR_SPEED = 6,
        MR_DIR = 7,
        MR_ENC_A = 3, // ЖЕЛТЫЙ
        MR_ENC_B = 9, // ЗЕЛЁНЫЙ
    };
}