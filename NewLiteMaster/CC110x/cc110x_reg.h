#ifndef _CC110x_REG_H_
#define _CC110x_REG_H_
struct cc110x_reg_ss_t
{
	char regaddr;
	char value;
};

/**********************************************************************************************/
#define IOCFG2   0x00
#define IOCFG1  0x01
#define IOCFG0  0x02
#define FIFOTHR  0x03//rx��tx FIFO����
#define SYNC1  0x04//ͬ���ʻ㣬���ֽ�
#define SYNC0  0x05//ͬ���ʻ㣬���ֽ�
#define PKTLEN  0x06//���ݰ�����
#define PKTCTRL1 0x07//���ݰ��Զ�����
#define PKTCTRL0 0x08//���ݰ��Զ�����
#define ADDR  0x09//�豸��ַ
#define CHANNR  0x0a//�ŵ���
#define FSCTRL1  0x0b//Ƶ�ʺϳɿ�����
#define FSCTRL0  0x0c//Ƶ�ʿ��ƴʻ㣬���ֽ�
#define FREQ2  0x0d//Ƶ�ʿ��ƴʻ㣬�м��ֽ�
#define FREQ1  0x0e//Ƶ�ʿ��ƴʻ㣬���ֽ�
#define FREQ0  0x0f//����������
#define MDMCFG4  0x10//����������
#define MDMCFG3  0x11//����������
#define MDMCFG2  0x12//����������
#define MDMCFG1  0x13//����������
#define MDMCFG0  0x14//����������
#define DEVIATN  0x15//��ͨ�ſ���״̬������
#define MCSM2  0x16//��ͨ�ſ���״̬������
#define MCSM1  0x17//��ͨ�ſ���״̬������
#define MCSM0  0x18//Ƶ��ƫ�Ʋ�������
#define FOCCFG  0x19//λͬ������
#define BSCFG  0x1a//AGC����
#define AGCCTRL2  0x1b//AGC����
#define AGCCTRL1  0x1c//AGC����
#define AGCCTRL0  0x1d//���ֽ�ʱ��0��ͣ
#define WOREVT1  0x1e//���ֽ�ʱ��0��ͣ
#define WOREVT0  0x1f//��Ų��������
#define WORCTRL  0x20//ǰĩ��RX����
#define FREND1  0x21//ǰĩ��TX����
#define FREND0  0x22//Ƶ�ʺϳ���У׼
#define FSCAL3  0x23//Ƶ�ʺϳ���У׼
#define FSCAL2  0x24//Ƶ�ʺϳ���У׼
#define FSCAL1  0x25//Ƶ�ʺϳ���У׼
#define FSCAL0  0x26//RC��������
#define RCCTRL1  0x27//RC��������
#define RCCTRL0  0x28//Ƶ�ʺϳ���У׼����
#define FSTEST  0x29//��Ʒ����
#define PTEST  0x2a//AGC����
#define AGCTEST  0x2b//��ͬ�Ĳ�������
#define TEST2  0x2c//��ͬ�Ĳ�������
#define TEST1  0x2d//��ͬ�Ĳ�������
#define TEST0  0x2e//

#define SRES  0x30//����оƬ
#define SFSTXON  0x31//������У׼Ƶ�ʺϳ���
#define SXOFF  0x32//�رվ�������
#define SCAL  0x33//У׼Ƶ�ʺϳ������ж�
#define SRX   0x34//����SRX
#define STX   0x35//����״̬:����STX
#define SIDLE  0x36//�뿪RX/TX���ض�Ƶ�ʺϳ���
#define SAFC  0x37//Ƶ�ʺϳ���AFC����
#define SWOR  0x38//�Զ�RXѡ�����У���Ų����
#define SPWD  0x39//��CSNΪ��ʱ���빦�ʽ���ģʽ
#define SFRX  0x3a//��ϴRX FIFO����
#define SFTX  0x3b//��ϴTX FIFO����
#define SWORRST  0x3c//����������ʵʱ��ʱ��
#define SNOP  0x3d//�޲���

#define PARTNUM  0x30//CC2550��ɲ�����Ŀ
#define VERSION  0x31//��ǰ�汾��
#define FREQEST  0x32//Ƶ��ƫ�ƹ���
#define LQI   0x33//���������Ľ��������
#define RSSI  0x34//�����ź�ǿ��ָʾ
#define MARCSTATE 0x35//����״̬��״̬
#define WORTIME1 0x36//WOR��ʱ�����ֽ�
#define WORTIME0 0x37//WOR��ʱ�����ֽ�
#define PKTSTATUS 0x38//��ǰGDOx״̬�����ݰ�״̬
#define VCO_VC_DAC 0x39//PLLУ׼ģ��ĵ�ǰ�趨
#define TXBYTES  0x3a//TX FIFO�е�����ͱ�����
#define RXBYTES  0x3b//RX FIFO�е�����ͱ�����
#define PATABLE  0x3e
#define TX_FIFO  0x3f
#define RX_FIFO  0x3f
/**********************************************************************************************/

#define CC1101_IOCFG2  0x00
#define CC1101_IOCFG1  0x01
#define CC1101_IOCFG0  0x02
#define CC1101_FIFOTHR  0x03//rx��tx FIFO����
#define CC1101_SYNC1  0x04//ͬ���ʻ㣬���ֽ�
#define CC1101_SYNC0  0x05//ͬ���ʻ㣬���ֽ�
#define CC1101_PKTLEN  0x06//���ݰ�����
#define CC1101_PKTCTRL1 0x07//���ݰ��Զ�����
#define CC1101_PKTCTRL0 0x08//���ݰ��Զ�����
#define CC1101_ADDR  0x09//�豸��ַ
#define CC1101_CHANNR  0x0a//�ŵ���
#define CC1101_FSCTRL1  0x0b//Ƶ�ʺϳɿ�����
#define CC1101_FSCTRL0  0x0c//Ƶ�ʿ��ƴʻ㣬���ֽ�
#define CC1101_FREQ2  0x0d//Ƶ�ʿ��ƴʻ㣬�м��ֽ�
#define CC1101_FREQ1  0x0e//Ƶ�ʿ��ƴʻ㣬���ֽ�
#define CC1101_FREQ0  0x0f//����������
#define CC1101_MDMCFG4  0x10//����������
#define CC1101_MDMCFG3  0x11//����������
#define CC1101_MDMCFG2  0x12//����������
#define CC1101_MDMCFG1  0x13//����������
#define CC1101_MDMCFG0  0x14//����������
#define CC1101_DEVIATN  0x15//��ͨ�ſ���״̬������
#define CC1101_MCSM2  0x16//��ͨ�ſ���״̬������
#define CC1101_MCSM1  0x17//��ͨ�ſ���״̬������
#define CC1101_MCSM0  0x18//Ƶ��ƫ�Ʋ�������
#define CC1101_FOCCFG  0x19//λͬ������
#define CC1101_BSCFG  0x1a//AGC����
#define CC1101_AGCCTRL2  0x1b//AGC����
#define CC1101_AGCCTRL1  0x1c//AGC����
#define CC1101_AGCCTRL0  0x1d//���ֽ�ʱ��0��ͣ
#define CC1101_WOREVT1  0x1e//���ֽ�ʱ��0��ͣ
#define CC1101_WOREVT0  0x1f//��Ų��������
#define CC1101_WORCTRL  0x20//ǰĩ��RX����
#define CC1101_FREND1  0x21//ǰĩ��TX����
#define CC1101_FREND0  0x22//Ƶ�ʺϳ���У׼
#define CC1101_FSCAL3  0x23//Ƶ�ʺϳ���У׼
#define CC1101_FSCAL2  0x24//Ƶ�ʺϳ���У׼
#define CC1101_FSCAL1  0x25//Ƶ�ʺϳ���У׼
#define CC1101_FSCAL0  0x26//RC��������
#define CC1101_RCCTRL1  0x27//RC��������
#define CC1101_RCCTRL0  0x28//Ƶ�ʺϳ���У׼����
#define CC1101_FSTEST  0x29//��Ʒ����
#define CC1101_PTEST  0x2a//AGC����
#define CC1101_AGCTEST  0x2b//��ͬ�Ĳ�������
#define CC1101_TEST2  0x2c//��ͬ�Ĳ�������
#define CC1101_TEST1  0x2d//��ͬ�Ĳ�������
#define CC1101_TEST0  0x2e//
        
#define CC1101_SRES  0x30//����оƬ
#define CC1101_SFSTXON  0x31//������У׼Ƶ�ʺϳ���
#define CC1101_SXOFF  0x32//�رվ�������
#define CC1101_SCAL  0x33//У׼Ƶ�ʺϳ������ж�
#define CC1101_SRX   0x34//����SRX
#define CC1101_STX   0x35//����״̬:����STX
#define CC1101_SIDLE  0x36//�뿪RX/TX���ض�Ƶ�ʺϳ���
#define CC1101_SAFC  0x37//Ƶ�ʺϳ���AFC����
#define CC1101_SWOR  0x38//�Զ�RXѡ�����У���Ų����
#define CC1101_SPWD  0x39//��CSNΪ��ʱ���빦�ʽ���ģʽ
#define CC1101_SFRX  0x3a//��ϴRX FIFO����
#define CC1101_SFTX  0x3b//��ϴTX FIFO����
#define CC1101_SWORRST  0x3c//����������ʵʱ��ʱ��
#define CC1101_SNOP  0x3d//�޲���
        
#define CC1101_PARTNUM  0x30//CC2550��ɲ�����Ŀ
#define CC1101_VERSION  0x31//��ǰ�汾��
#define CC1101_FREQEST  0x32//Ƶ��ƫ�ƹ���
#define CC1101_LQI   0x33//���������Ľ��������
#define CC1101_RSSI  0x34//�����ź�ǿ��ָʾ
#define CC1101_MARCSTATE 0x35//����״̬��״̬
#define CC1101_WORTIME1 0x36//WOR��ʱ�����ֽ�
#define CC1101_WORTIME0 0x37//WOR��ʱ�����ֽ�
#define CC1101_PKTSTATUS 0x38//��ǰGDOx״̬�����ݰ�״̬
#define CC1101_VCO_VC_DAC 0x39//PLLУ׼ģ��ĵ�ǰ�趨
#define CC1101_TXBYTES  0x3a//TX FIFO�е�����ͱ�����
#define CC1101_RXBYTES  0x3b//RX FIFO�е�����ͱ�����
#define CC1101_PATABLE  0x3e
#define CC1101_TX_FIFO  0x3f
#define CC1101_RX_FIFO  0x3f
static void print_b(char *buf,int len)
{
	int i;
	if(len < 0)
		return ;
	
	sz_printf("Data:");
	len = len -1;
	for(i=0;i<len;i++)
	{
		sz_printf("%#x-",buf[i]);
	}
	sz_printf("%#x\r\n",buf[i]);
}

/**********************************************************************************************/

//#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
//#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
//#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
//#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
//#define CCxxx0_SYNC1        0x04        // Sync word, high INT8U
//#define CCxxx0_SYNC0        0x05        // Sync word, low INT8U
//#define CCxxx0_PKTLEN       0x06        // Packet length
//#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
//#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
//#define CCxxx0_ADDR         0x09        // Device address
//#define CCxxx0_CHANNR       0x0A        // Channel number
//#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
//#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
//#define CCxxx0_FREQ2        0x0D        // Frequency control word, high INT8U
//#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle INT8U
//#define CCxxx0_FREQ0        0x0F        // Frequency control word, low INT8U
//#define CCxxx0_MDMCFG4      0x10        // Modem configuration
//#define CCxxx0_MDMCFG3      0x11        // Modem configuration
//#define CCxxx0_MDMCFG2      0x12        // Modem configuration
//#define CCxxx0_MDMCFG1      0x13        // Modem configuration
//#define CCxxx0_MDMCFG0      0x14        // Modem configuration
//#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
//#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
//#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
//#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
//#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
//#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
//#define CCxxx0_AGCCTRL2     0x1B        // AGC control
//#define CCxxx0_AGCCTRL1     0x1C        // AGC control
//#define CCxxx0_AGCCTRL0     0x1D        // AGC control
//#define CCxxx0_WOREVT1      0x1E        // High INT8U Event 0 timeout
//#define CCxxx0_WOREVT0      0x1F        // Low INT8U Event 0 timeout
//#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
//#define CCxxx0_FREND1       0x21        // Front end RX configuration
//#define CCxxx0_FREND0       0x22        // Front end TX configuration
//#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
//#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
//#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
//#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
//#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
//#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
//#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
//#define CCxxx0_PTEST        0x2A        // Production test
//#define CCxxx0_AGCTEST      0x2B        // AGC test
//#define CCxxx0_TEST2        0x2C        // Various test settings
//#define CCxxx0_TEST1        0x2D        // Various test settings
//#define CCxxx0_TEST0        0x2E        // Various test settings
//
// Strobe commands
//#define CCxxx0_SRES         0x30        // Reset chip.
//#define CCxxx0_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
//                                        // If in RX/TX: Go to a wait state where only the synthesizer is
//                                        // running (for quick RX / TX turnaround).
//#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
//#define CCxxx0_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
//                                        // (enables quick start).
//#define CCxxx0_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
//                                        // MCSM0.FS_AUTOCAL=1.
//#define CCxxx0_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
//                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
//                                        // Only go to TX if channel is clear.
//#define CCxxx0_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
//                                        // Wake-On-Radio mode if applicable.
//#define CCxxx0_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
//#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
//#define CCxxx0_SPWD         0x39        // Enter power down mode when CSn goes high.
//#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
//#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
//#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
//#define CCxxx0_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
//                                        // INT8Us for simpler software.
//
//#define CCxxx0_PARTNUM      0x30
//#define CCxxx0_VERSION      0x31
//#define CCxxx0_FREQEST      0x32
//#define CCxxx0_LQI          0x33
//#define CCxxx0_RSSI         0x34
//#define CCxxx0_MARCSTATE    0x35
//#define CCxxx0_WORTIME1     0x36
//#define CCxxx0_WORTIME0     0x37
//#define CCxxx0_PKTSTATUS    0x38
//#define CCxxx0_VCO_VC_DAC   0x39
//#define CCxxx0_TXBYTES      0x3A
//#define CCxxx0_RXBYTES      0x3B
//
//#define CCxxx0_PATABLE      0x3E
//#define CCxxx0_TXFIFO       0x3F
//#define CCxxx0_RXFIFO       0x3F
/**********************************************************************************************/
#define 	BYTES_IN_RXFIFO     0x7F  						//���ջ���������Ч�ֽ���
#define 	CRC_OK              0x80 						//CRCУ��ͨ��λ��־
#endif
