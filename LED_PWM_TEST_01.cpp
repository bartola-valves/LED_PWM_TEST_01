#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" // Include this library for PWM management


// define 3 LEDs (RGB) connected to GPIO pins 15, 14, 13
// the LEDs are common anode, so to turn on the LED, the GPIO pin should be set to 0
#define LED_R 15
#define LED_G 14
#define LED_B 13

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



int main()
{
   stdio_init_all();
    // Initialize the LED
    LED_init();
    // set the LED to red
    LED_set(50, 50, 50);

    // Wait for 5 sec
    sleep_ms(5000);
    while (true) {
        // Turn the red LED on and the green and blue LEDs off
        LED_set(0, 255, 255);
        sleep_ms(1000);
        // Turn the green LED on and the red and blue LEDs off
        LED_set(255, 0, 255);
        sleep_ms(1000);
        // Turn the blue LED on and the red and green LEDs off
        LED_set(255, 255, 0);
        sleep_ms(1000);
    }
}
