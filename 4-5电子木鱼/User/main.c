#include "stm32f10x.h"                  // Device header#incluy
#include "Delay.h" 
#include "main.h"

// 定义蜂鸣器引脚（请根据你的硬件连接修改）
#define BEEP_PIN GPIO_PIN_8
#define BEEP_PORT GPIOA

// 定义定时器句柄，这里以TIM2为例
TIM_HandleTypeDef htim2;

// 函数声明
void Buzzer_Init(void);
void Buzzer_Play_Tone(uint16_t freq, uint16_t duration);
void Buzzer_Play_WoodenFish(void);
void Buzzer_Volume_Envelope(uint16_t duration);

// 蜂鸣器初始化函数
void Buzzer_Init(void)
{
    // 1. 使能GPIO时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. 配置GPIO为复用推挽输出 (假设蜂鸣器接在PA8)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BEEP_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        // 复用推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;      // 复用功能为TIM2
    HAL_GPIO_Init(BEEP_PORT, &GPIO_InitStruct);

    // 3. 使能定时器时钟并配置PWM
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 72 - 1;                  // 72MHz/72 = 1MHz，即1us计一个数
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000 - 1;                   // 初始周期1ms (1kHz)
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim2);

    // 4. 配置PWM通道 (CH1)
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;                            // 初始占空比为0，蜂鸣器不发声
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
}

// 播放指定频率和时长的音调（阻塞方式）
void Buzzer_Play_Tone(uint16_t freq, uint16_t duration)
{
    if (freq == 0) { // 休止符
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        HAL_Delay(duration);
        return;
    }

    // 计算定时器重载值
    // 定时器输入频率 = 72MHz / (Prescaler+1) = 1MHz = 1,000,000 Hz
    uint32_t timer_clock = 1000000;
    uint32_t period = timer_clock / freq - 1;
    
    // 更新PWM周期和占空比
    __HAL_TIM_SET_AUTORELOAD(&htim2, period);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, period / 2); // 占空比50%
    
    // 启动PWM输出
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    
    // 延时指定时长
    HAL_Delay(duration);
    
    // 停止PWM输出
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

// 模拟木鱼敲击的音量包络（快速衰减）
void Buzzer_Volume_Envelope(uint16_t duration)
{
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(&htim2);
    uint16_t steps = 10;
    uint16_t step_duration = duration / steps;
    
    for (int i = 0; i < steps; i++) {
        // 占空比从80%快速降至10%，模拟敲击后声音的衰减
        uint16_t duty = (period * (80 - i * 7)) / 100;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
        HAL_Delay(step_duration);
    }
}

// 电子木鱼敲击音效（带颤动效果）
void Buzzer_Play_WoodenFish(void)
{
    // 定义音效参数
    uint16_t base_freq = 700;   // 基准频率 (Hz)
    uint16_t duration = 80;      // 总时长 (ms)
    uint16_t tremolo_rate = 20;  // 颤动速率 (Hz)
    uint16_t steps = duration / (1000 / tremolo_rate);
    
    if (steps == 0) steps = 4;
    
    uint16_t step_duration = duration / steps;
    
    // 先计算基准周期，避免在循环中重复计算
    uint32_t timer_clock = 1000000;
    uint32_t base_period = timer_clock / base_freq - 1;
    
    // 更新PWM周期
    __HAL_TIM_SET_AUTORELOAD(&htim2, base_period);
    
    // 启动PWM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    
    // 通过微调频率模拟颤动，并通过占空比模拟衰减
    for (int i = 0; i < steps; i++) {
        // 微小频率变化 (±5%)，模拟颤动
        int8_t freq_offset = (i % 2 == 0) ? 5 : -5;
        uint16_t current_freq = base_freq + base_freq * freq_offset / 100;
        uint32_t current_period = timer_clock / current_freq - 1;
        __HAL_TIM_SET_AUTORELOAD(&htim2, current_period);
        
        // 同时调整占空比，实现音量衰减
        uint16_t duty = current_period * (80 - i * 8) / 100;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
        
        HAL_Delay(step_duration);
    }
    
    // 停止PWM，确保声音干净结束
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

// 主函数示例
int main(void)
{
    HAL_Init();
    SystemClock_Config(); // 系统时钟配置，通常为72MHz
    Buzzer_Init();
    
    while (1)
    {
        // 示例：每500ms敲击一次木鱼
        Buzzer_Play_WoodenFish();
        HAL_Delay(500);
    }
}