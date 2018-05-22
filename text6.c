#include "LPC11xx.h"

unsigned char state = 0;
unsigned char cnt = 0;

enum _LABLE_
{
	L_TO_R = 0,
	R_TO_L = 1,
	BLINK = 2,
};

int main()
{
	LPC_GPIO2 ->DIR = 0xFF;				//??P2_0~P2_7???
	LPC_GPIO2 ->DATA |= 0xFF;

	LPC_SYSCON ->SYSAHBCLKCTRL |= (1 << 9);
	LPC_TMR32B0 ->IR = 0x01;
	LPC_TMR32B0 ->PR = 0;
	LPC_TMR32B0 ->MCR = 0x03;
	LPC_TMR32B0 ->MR0 = 25000000;
	LPC_TMR32B0 ->TCR = 0x01;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	
	
	
	while(1);
}

void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR = 0x01;

	switch(state)
	{
		case L_TO_R:
			LPC_GPIO2 ->DATA |= 0xFF;
			LPC_GPIO2 ->DATA &= ~(1 << (8 - cnt));
			if (cnt < 8)
			{
				cnt++;
			}
			else
			{
				cnt = 0;
				state = R_TO_L;
			}
			break;

		case R_TO_L:
			LPC_GPIO2 ->DATA |= 0xFF;
			LPC_GPIO2 ->DATA &= ~(1 << cnt);
			if (cnt < 8)
			{
				cnt++;
			}
			else
			{
				cnt = 0;
				state = BLINK;
			}
			break;

		case BLINK:
			if (cnt % 2)
			{
				LPC_GPIO2 ->DATA |= 0xFF;
			}
			else
			{
				LPC_GPIO2 ->DATA &= 0x00;
			}
			if (cnt < 5)
			{
				cnt++;
			}
			else
			{
				cnt = 0;
				state = L_TO_R;
			}
	}	
}



