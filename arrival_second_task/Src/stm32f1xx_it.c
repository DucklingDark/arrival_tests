uint16_t ticks_count = 0;       // Variable that is used to count the number of timer 'ticks'

extern TIM_HandleTypeDef htim6; // htim6 is a basic timer structure that stores TIMER configuration and other parameters. Code generated by STM32CubeIDE

/*
    This function handles TIM6 global interrupt. Also generated by STM32CubeIDE
*/
void TIM6_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
  ticks_count++;                // Increase count of timer 'ticks'
}
