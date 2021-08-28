#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define NO_OF_SAMPLES   64
#define DEFAULT_VREF    1100

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

void app_main()
{
	int val = 0;
	int volt = 0;

	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(ADC1_CHANNEL_6,atten);

	adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
	esp_adc_cal_characterize(unit, atten,ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

	while (true)
	{
		for (int i = 0; i < NO_OF_SAMPLES; i++)
		{
			val += adc1_get_raw(ADC1_CHANNEL_6);
		}

		val /= NO_OF_SAMPLES;
		volt = esp_adc_cal_raw_to_voltage(val,adc_chars);
		printf("Raw: %d Volatge: %f Volt\n", val,(volt * 7.266));
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}
