#include "mcp9808.h"

static const char *TAG = "ESP_WebServer_Project : ";
   uint8_t read_temp[2];
   volatile float temp;
esp_err_t  mcp9808_init(uint8_t addr, uint8_t add_reg, uint8_t sda_pin, uint8_t scl_pin){
   
    i2c_config_t conf = {
	.mode = I2C_MODE_MASTER,
	.sda_io_num = sda_pin,
	.scl_io_num = scl_pin,
	.sda_pullup_en = GPIO_PULLUP_ENABLE,
	.scl_pullup_en = GPIO_PULLUP_ENABLE,
	.master.clk_speed = 400000,
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0); // activated the I2C port 

    i2c_cmd_handle_t cmd=i2c_cmd_link_create(); //create the link to send data
    i2c_master_start(cmd); //to generate the start signal
    i2c_master_write_byte(cmd, addr<<1| I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, &add_reg, 10, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd); //release and return the data
    return ESP_OK ;
}

esp_err_t mcp9808_read(float *temperature){
    
    i2c_master_read_from_device(I2C_NUM_0, MCP_ADD, temperature, 2, TIMEOUT_MS/portTICK_PERIOD_MS);
    return ESP_OK ;

}

void mcp9808_test_main_c(){

   esp_err_t ret_init, ret_read;
   ret_init= mcp9808_init(MCP_ADD, MCP_REG, SDA_PIN, SCL_PIN);
   if(ret_init==ESP_OK){
    ESP_LOGI(TAG, "Initialization of I2C is OK");
   }
   else{
        ESP_LOGI(TAG, "ERROR Initialization of I2C");
   }
   
    vTaskDelay(TIMEOUT_MS/portTICK_PERIOD_MS);
while (1)
{
    ret_read= mcp9808_read(&read_temp);
    if(ret_read==ESP_OK){
        ESP_LOGI(TAG, "Data read start");
    }
    else{
        ESP_LOGI(TAG, "ERROR Data read");
    }
    

     if((read_temp[0] & (0x01 << 0x04)) >> 0x04)
    {
        temp = 256 - (((read_temp[0] & 0x0F) << 0x04) + (read_temp[1] >> 0x04));
    }
    // TA > 0°C
    else
    {
        temp = (((read_temp[0] & 0x0F) << 0x04) + (read_temp[1] >> 0x04));
    }
    ESP_LOGI(TAG, "temperature is : %.2f", temp);
    vTaskDelay(TIMEOUT_MS/portTICK_PERIOD_MS);

}

}