#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" // Include this library for PWM management


// define 3 LEDs (RGB) connected to GPIO pins 16,17,18 which are PWM slices 0,1,2
// the LEDs are common anode, so to turn on the LED, the GPIO pin should be set to 0
#define LED_R 16
#define LED_G 17
#define LED_B 18

// Define the maximum PWM value for 12-bit resolution
#define MAX_PWM 4095

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

// Define a function to generate rainbow colors
void generate_rainbow_colors(uint16_t *r, uint16_t *g, uint16_t *b, float hue)
{
    // Calculate the RGB values based on the hue
    *r = (uint16_t)((sin(hue) + 1) * (MAX_PWM / 2));
    *g = (uint16_t)((sin(hue + 2 * M_PI / 3) + 1) * (MAX_PWM / 2));
    *b = (uint16_t)((sin(hue + 4 * M_PI / 3) + 1) * (MAX_PWM / 2));
}

// Define a function to set the RGB values based on a color index
void set_color_by_index(uint8_t color_index)
{
    // Define a list of colors (R, G, B)
    uint8_t colors[][3] = {
        {255, 0, 0},    // Red
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {0, 255, 255},  // Cyan
        {255, 0, 255},  // Magenta
        {255, 255, 0},  // Yellow
        {255, 255, 255},// White
        {0, 0, 0},      // Black (off)
        {255, 165, 0},  // Orange
        {255, 192, 203},// Pink
        {128, 0, 128},  // Purple
        {173, 216, 230},// Light Blue
        {0, 128, 128}   // Teal
    };

    // Get the number of colors in the list
    uint8_t num_colors = sizeof(colors) / sizeof(colors[0]);

    // Ensure the color index is within the valid range
    if (color_index >= num_colors)
    {
        color_index = 0; // Default to the first color if the index is out of range
    }

    // Set the RGB values based on the color index
    LED_set(colors[color_index][0], colors[color_index][1], colors[color_index][2]);
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
    uint32_t freq = 1000; // 1 kHz
    uint32_t clock = 125000000; // 125 MHz system clock
    float divider = (float)clock / (freq * 4096); // Calculate the clock divider for 12-bit resolution
    pwm_set_clkdiv(slice_num, divider);

    // Set the PWM wrap value to 4095 (12-bit resolution)
    pwm_set_wrap(slice_num, 4095);

    // Set the PWM channel to a 50% duty cycle
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_PWM), 2048);

    // Enable the PWM slice
    pwm_set_enabled(slice_num, true);
}

// Define the LED_PWM_set function to set the duty cycle of the LED. The duty cycle is a value between 0 and 4095.
// 0 means the LED is off and 4095 means the LED is fully on.
void LED_PWM_set(uint16_t duty_cycle)
{
    // Set the duty cycle of the LED
    pwm_set_gpio_level(LED_PWM, duty_cycle);
}



int main()
{
    stdio_init_all();

    // Initialize the RGB LEDs
    LED_init();

    //infitite loop to test the different colors
    while (true)
    {
        // Set the RGB values based on the color index
        for (uint8_t i = 0; i < 13; i++)
        {
            set_color_by_index(i);
            sleep_ms(200); // Wait  before changing to the next color
        }
    }

}
