#include <math.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <limits.h>
// #define F_CPU 16000000UL  // 16 MHz
// #ifndef ALIBC_OLD
// #include <util/delay.h>
// #else
// #include <avr/delay.h>
// #endif

#define clr_bit(a,b) ((a) &=~(1<<(b)))
#define set_bit(a,b) ((a) |= (1<<(b)))
#define tst_bit(a,b) ((a) & (1<<(b)))
#define change_bit(a,b) ((a) ^= (1<<(b)))

void __attribute__ ((noinline)) fastest_as_can_change_timer_period2(register uint16_t ICR1_cache,
                                  register uint16_t OCR1A_cache  );
float fmap(float x, float in_min, float in_max, float out_min, float out_max);

void setup()
{
	cli();
	set_bit(DDRB,1);
	TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(0<<WGM12);

	TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);
	TCCR1B=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12);
	switch (1)//prescaler
		{	case 1:
				TCCR1B|=(0<<CS12)|(0<<CS11)|(1<<CS10);
				break;
			case 1024:
				TCCR1B|=(1<<CS12)|(0<<CS11)|(1<<CS10);
				break;
		}
	TCNT1=0;
  ICR1=1;
  OCR1A=0;
	sei();

}

void loop()
{
	change_bit(PORTB,5);

	const uint32_t MAX=1000;
	uint32_t sum=0;
	for(uint32_t i=0;i<MAX;i++)	
		sum+=analogRead(A0);
	float period=fmap(log10(sum),0,log10(1023.*MAX), 65536,2);
  float new_ICR1=period-1-0.5;
  if(new_ICR1<1)new_ICR1=1;
  float new_OCR1=new_ICR1/2;
  // fmap(val,0,1023,max_frequency
	// period=2;
	// while(TCNT1>200);
	fastest_as_can_change_timer_period2( new_ICR1,new_OCR1);
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  float result=(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//  if(result<out_min)return out_min;
//  if(result>out_max)return out_max;
  return result;
}

// void alert(void)
// {
// 	while(1)
// 		{	set_bit(DDRB,5);
// 			set_bit(PORTB,5);
// 			_delay_ms(200);
// 			clr_bit(PORTB,5);
// 			_delay_ms(200);
// 			set_bit(PORTB,5);
// 			_delay_ms(200);
// 			clr_bit(PORTB,5);
// 			_delay_ms(800);
// 		}
// }

void __attribute__ ((noinline)) fastest_as_can_change_timer_period2(register uint16_t ICR1_cache,
																	register uint16_t OCR1A_cache	 )
{
	ICR1=ICR1_cache;
	OCR1A=OCR1A_cache;
	TCNT1=0;
}
