#include <mega32a.h>
#include <delay.h>

#define del_coef 0.5

long int one_m = 1000000 ;
long int freq_hz ;
long int icr_pre_val ;
int icr_val ;
char i = 0 ;

unsigned int notes[] = {264,264,297,264,352,330,264,264,297,264,396,352,264,264,264,440,352,352,330,297,466,466,440,352,396,352};
unsigned int lengths[] = {250,250,1000,1000,1000,2000,250,250,1000,1000,1000,2000,250,250,1000,1000,500,250,1000,2000,250,250,1000,1000,1000,2000};
unsigned int delays[] = {500,250,250,250,250,500,500,250,250,250,250,500,500,250,250,250,250,250,250,500,500,250,250,250,250,250};

void freq_cal();

void main(void)
{
// Port D initialization
// Function: Bit7=In Bit6=In Bit5=Out Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (1<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=P Bit6=T Bit5=0 Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 1000.000 kHz
// Mode: Fast PWM top=ICR1
// OC1A output: Non-Inverted PWM
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 1 us
// Output Pulse(s):
// OC1A Period: 1 us
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

while (1)
    { 

    freq_hz = notes[i] ;
    freq_cal();
    delay_ms(lengths[i]*del_coef);
    freq_hz = 50000 ;  //voice with no sound
    freq_cal();
    delay_ms(delays[i]*del_coef);
            
    i++;
    if(i==26)
        {   
        freq_hz = 50000 ;
        freq_cal();
        i=0;
        delay_ms(3000);
        }      

    } 

}


void freq_cal()
    {//ctc
        icr_pre_val = one_m / freq_hz ; //top
        icr_val = icr_pre_val ;
        ICR1H= ( icr_val >> 8 );  //shift high value and save
        ICR1L= ( icr_val & 0xFF );
        icr_val = icr_val / 2 ;//pwm
        OCR1AH = ( icr_val >> 8 );
        OCR1AL = ( icr_val & 0xFF );
    } +
    *