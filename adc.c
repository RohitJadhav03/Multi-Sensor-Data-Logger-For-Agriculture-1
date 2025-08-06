
#include <lpc214x.h>
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "spi.h"
#include "dht11.h"
#include <stdio.h> // For sprintf()

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}

int main() {
    float temp, humidity, soil_moisture, light;
    char buffer[32];

    lcd_init();
    uart0_init();
    spi_init();
    adc_init();

    lcd_clear();
    lcd_print("Agri Data Logger");

    while (1) {
        dht11_read(&temp, &humidity);
        soil_moisture = adc_read(1) * 3.3 / 1024.0 * 100;
        light = adc_read(2) * 3.3 / 1024.0 * 100;

        lcd_set_cursor(0, 0);
        sprintf(buffer, "T:%.1f H:%.1f", temp, humidity);
        lcd_print(buffer);

        lcd_set_cursor(1, 0);
        sprintf(buffer, "M:%.1f L:%.1f", soil_moisture, light);
        lcd_print(buffer);

        sprintf(buffer, "T:%.1f H:%.1f M:%.1f L:%.1f\n", temp, humidity, soil_moisture, light);
        uart0_tx_string(buffer);
        spi_write_string(buffer);

        delay_ms(2000);
    }
}
