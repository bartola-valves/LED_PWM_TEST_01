#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" // Include this library for PWM management


// define 3 LEDs (RGB) connected to GPIO pins 15, 14, 13
// the LEDs are common anode, so to turn on the LED, the GPIO pin should be set to 0
#define LED_R 15
#define LED_G 14
#define LED_B 13

// define single LED for PWM test connected to GPIO pin 1
#define LED_PWM 1

// define a LED_init function to set the GPIO pins to output and also configure the 3 LEDs in PWM mode
// LEDs are common anode, so the duty cycle should be inverted. 0 means the LED is on and 255 means the LED is fully off.
void LED_init()
{
    // Initialize the GPIO pins for the LEDs
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Set the GPIO functions to PWM
    gpio_set_function(LED_R, GPIO_FUNC_PWM);
    gpio_set_function(LED_G, GPIO_FUNC_PWM);
    gpio_set_function(LED_B, GPIO_FUNC_PWM);

    // Get the PWM slice numbers for each LED
    uint slice_num_r = pwm_gpio_to_slice_num(LED_R);
    uint slice_num_g = pwm_gpio_to_slice_num(LED_G);
    uint slice_num_b = pwm_gpio_to_slice_num(LED_B);

    // Set the PWM frequency and enable PWM for each slice
    pwm_set_wrap(slice_num_r, 255);
    pwm_set_wrap(slice_num_g, 255);
    pwm_set_wrap(slice_num_b, 255);

    // Set the PWM channels to a known state (0)
    pwm_set_chan_level(slice_num_r, pwm_gpio_to_channel(LED_R), 0);
    pwm_set_chan_level(slice_num_g, pwm_gpio_to_channel(LED_G), 0);
    pwm_set_chan_level(slice_num_b, pwm_gpio_to_channel(LED_B), 0);


   // Enable PWM for each slice
    pwm_set_enabled(slice_num_r, true);
    pwm_set_enabled(slice_num_g, true);
    pwm_set_enabled(slice_num_b, true);
}

// define a LED_set function to set the duty cycle of the 3 LEDs. The duty cycle is a value between 0 and 255. 
//0 means the LED is on and 255 means the LED is fully off. 
// The function takes 3 parameters: the duty cycle for the red LED, the duty cycle for the green LED, and the duty cycle for the blue LED.
void LED_set(uint8_t r, uint8_t g, uint8_t b)
{
    // Invert the duty cycle values for common anode LEDs
    uint16_t duty_r = 255 - r;
    uint16_t duty_g = 255 - g;
    uint16_t duty_b = 255 - b;
    
    // Print the duty cycle values for debugging
    printf("Setting duty cycles - R: %d, G: %d, B: %d\n", duty_r, duty_g, duty_b);

    pwm_set_gpio_level(LED_R, duty_r);
    pwm_set_gpio_level(LED_G, duty_g);
    pwm_set_gpio_level(LED_B, duty_b);
}

// define a single LED PWM test using LED_PWM. Firstly we will initialise GPIO pin 1 as a PWM output and set the PWM frequency to 1 kHz.
// Then we will set the duty cycle to 50% and wait for 5 seconds before turning off the LED.
void LED_PWM_test()
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PWM);
    gpio_set_dir(LED_PWM, GPIO_OUT);

    // Set the GPIO function to PWM
    gpio_set_function(LED_PWM, GPIO_FUNC_PWM);

    // Get the PWM slice number for the LED
    uint slice_num = pwm_gpio_to_slice_num(LED_PWM);

    // Set the PWM frequency to 1 kHz
    pwm_set_clkdiv(slice_num, 125.0f);

    // Set the PWM channel to a 50% duty cycle
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PWM), 127);

    // Enable the PWM slice
    pwm_set_enabled(slice_num, true);

    // Print the duty cycle value for debugging
    printf("Setting duty cycle to 50%% on GPIO %d\n", LED_PWM);

    // Wait for 5 seconds
    sleep_ms(5000);

    // Turn off the LED
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PWM), 0);

    // Print the duty cycle value for debugging
    printf("Turning off the LED on GPIO %d\n", LED_PWM);
}

// define a simple test routine to turn on an off a single LED connected to GPIO pin 1
void LED_test()
{
    // Turn on the LED
    gpio_put(LED_PWM, 0);
    sleep_ms(1000);

    // Turn off the LED
    gpio_put(LED_PWM, 1);
    sleep_ms(1000);
}

// define the LED_PWM_init function to set the GPIO pin to output and configure the LED in PWM mode
void LED_PWM_init()
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PWM);
    gpio_set_dir(LED_PWM, GPIO_OUT);

    // Set the GPIO function to PWM
    gpio_set_function(LED_PWM, GPIO_FUNC_PWM);

    // Get the PWM slice number for the LED
    uint slice_num = pwm_gpio_to_slice_num(LED_PWM);

    // Set the PWM frequency to 10 kHz
    uint32_t freq = 10000; // 10 kHz
    uint32_t clock = 125000000; // 125 MHz system clock
    float divider = (float)clock / (freq * 256); // Calculate the clock divider
    pwm_set_clkdiv(slice_num, divider);

    // Set the PWM wrap value to 255 (8-bit resolution)
    pwm_set_wrap(slice_num, 255);

    // Set the PWM channel to a 50% duty cycle
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PWM), 127);

    // Enable the PWM slice
    pwm_set_enabled(slice_num, true);
}

//define the LED_PWM_set function to set the duty cycle of the LED. The duty cycle is a value between 0 and 255.
// 0 means the LED is on and 255 means the LED is fully off.
void LED_PWM_set(uint8_t duty_cycle)
{
    // Set the duty cycle of the LED
    pwm_set_gpio_level(LED_PWM, duty_cycle);
}



int main()
{
    stdio_init_all();

    //configure LED_PWM on GPIO 1 as output
    //gpio_init(LED_PWM);
    //set the direction of the GPIO pin to output
    //gpio_set_dir(LED_PWM, GPIO_OUT);

    // initialise the LED_PWM
    LED_PWM_init();

    while (true)
    {    
        
        sleep_ms(1000);
        // increment the LED_PWM duty cycle gradually over 5 seconds
        for (int i = 0; i <= 255; i++)
        {
            //set the duty cycle of the LED_PWM
            LED_PWM_set(i);
            //wait for 20 ms
            sleep_ms(20);
        }
    }
}
