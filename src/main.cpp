#define MEASURES 20
//#define USB_OTG_FS
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_ll_usb.h"
#include "AES.h"

TIM_HandleTypeDef tim4;
TIM_HandleTypeDef tim1;
uint_fast64_t millis2;
uint32_t millis;
uint32_t timMeasures[MEASURES] = { 0 };

enum measures {
	AES128_ENCRYPT,
	AES128_DECRYPT,
	AES192_ENCRYPT,
	AES192_DECRYPT,
	AES256_ENCRYPT,
	AES256_DECRYPT
};

void clocksInit();

int main(void) {

	clocksInit();

	// индикация и замер частоты мультиметром
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = GPIO_PIN_13;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &gpio);

	// таймер для delay и подсчета времени
	tim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.Period = 1000U - 1U;
	tim4.Init.Prescaler = 36U;
	tim4.Instance = TIM4;
	HAL_TIM_Base_Init(&tim4);

	// выключаем прерывания
	__disable_irq();

	HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	__HAL_TIM_ENABLE_IT(&tim4, TIM_IT_UPDATE);

	// включаем таймеры

	__HAL_TIM_ENABLE(&tim4);

	// включаем прерывания
	__enable_irq();

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	unsigned char openTextInput[16] = "abracadabra1234";
	unsigned char openTextOutput[16] = { 0 };
	unsigned char encryptedText[16] = { 0 };
	unsigned char key[32] = "The quick brown fox jumps over";

	// AES 128 *******************************************

	for(uint32_t i = 0; i < sizeof(encryptedText); encryptedText[i++] = 0);

	uint32_t start = millis;
	Encrypt(openTextInput, key, encryptedText, 128);
	timMeasures[measures::AES128_ENCRYPT] = millis - start;

	for(uint32_t i = 0; i < sizeof(openTextOutput); openTextOutput[i++] = 0);

	start = millis;
	Decrypt(encryptedText, key, openTextOutput, 128);
	timMeasures[measures::AES128_DECRYPT] = millis - start;

	// AES 192 *******************************************

	for(uint32_t i = 0; i < sizeof(encryptedText); encryptedText[i++] = 0);

	start = millis;
	Encrypt(openTextInput, key, encryptedText, 192);
	timMeasures[measures::AES192_ENCRYPT] = millis - start;

	for(uint32_t i = 0; i < sizeof(openTextOutput); openTextOutput[i++] = 0);

	start = millis;
	Decrypt(encryptedText, key, openTextOutput, 192);
	timMeasures[measures::AES192_DECRYPT] = millis - start;

	// AES 256 *******************************************

	for(uint32_t i = 0; i < sizeof(encryptedText); encryptedText[i++] = 0);

	start = millis;
	Encrypt(openTextInput, key, encryptedText, 256);
	timMeasures[measures::AES256_ENCRYPT] = millis - start;

	for(uint32_t i = 0; i < sizeof(openTextOutput); openTextOutput[i++] = 0);

	start = millis;
	Decrypt(encryptedText, key, openTextOutput, 256);
	timMeasures[measures::AES256_DECRYPT] = millis - start;

	while (1)
		;

}

void clocksInit() {
	RCC->CR |= RCC_CR_HSEON;

	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	FLASH->ACR |= FLASH_ACR_PRFTBE;

	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	/* HCLK = SYSCLK */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/* PCLK2 = HCLK */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	/* PCLK1 = HCLK */
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE | RCC_CFGR_PLLMULL9;

	RCC->CR |= RCC_CR_PLLON;

	/* Ожидаем, пока PLL выставит бит готовности */
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;

	/* Выбираем PLL как источник системной частоты */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/* Ожидаем, пока PLL выберется как источник системной частоты */
	while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != (uint32_t) 0x08)
		;

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
//	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
//	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}

extern "C" void TIM4_IRQHandler() {

	if (__HAL_TIM_GET_FLAG(&tim4, TIM_FLAG_UPDATE)) {
		__HAL_TIM_CLEAR_IT(&tim4, TIM_FLAG_UPDATE);
		millis++; // подсчет миллисекунд
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}

}

