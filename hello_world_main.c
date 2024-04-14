#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <esp_adc_cal_types_legacy.h>
// #include <adc_types_legacy.h>

#define READ_DATA_GPIO 34
#define DEFAULT_VREF    1100 // Giá trị điện áp tham chiếu mặc định (mV)
#define NO_OF_SAMPLES   64   // Số lượng mẫu để làm trung bình 

void app_main() {
    // Khởi tạo ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);

    // Khởi tạo cấu hình ADC cal để đọc giá trị hiệu chỉnh
    esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    while (1) {
        // Đọc giá trị ADC
        int adc_reading = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw(ADC1_CHANNEL_6);
        }
        adc_reading /= NO_OF_SAMPLES;

        // Chuyển đổi giá trị ADC sang giá trị voltage
        int voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

        printf("%dmV\n",voltage);

        // Chờ 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // Giải phóng bộ nhớ cấu hình ADC cal
    free(adc_chars);
}
