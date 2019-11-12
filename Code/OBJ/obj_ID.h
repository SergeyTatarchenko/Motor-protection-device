#ifndef OBJ_ID_H_
#define	OBJ_ID_H_


#define	IND_obj_NULL			0x00
#define	IND_obj_END				0xFF


/*-----------------------------------------------*/
#define obj_STATUS				(IND_obj_NULL + 1)	
/*---------------------HWOBJ---------------------*/
/*adc*/
#define IND_obj_aADC0			(IND_obj_NULL + 2)		// !���������� ��� 1
#define IND_obj_aADC1			(IND_obj_NULL + 3)		// !���������� ��� 2
#define IND_obj_aADC2			(IND_obj_NULL + 4)		// !���������� ��� 3
#define IND_obj_aADC3			(IND_obj_NULL + 5)		// !���������� ��� 4
#define IND_obj_aADC4			(IND_obj_NULL + 6)		// !���������� ��� 5
#define IND_obj_aADC5			(IND_obj_NULL + 7)		// !���������� ��� 6
#define IND_obj_aADC6			(IND_obj_NULL + 8)		// !���������� ��� 7
#define IND_obj_aADC7			(IND_obj_NULL + 9)		// !���������� ��� 8
/*inputs*/
#define IND_obj_iInput0			(IND_obj_NULL + 10)		// !���������� ���� 0 
#define IND_obj_iInput1			(IND_obj_NULL + 11)		// !���������� ���� 1 
#define IND_obj_iInput2			(IND_obj_NULL + 12)		// !���������� ���� 2 
#define IND_obj_iInput3			(IND_obj_NULL + 13)		// !���������� ���� 3 
#define IND_obj_iInput4			(IND_obj_NULL + 14)		// !���������� ���� 4 
#define IND_obj_iInput5			(IND_obj_NULL + 15)		// !���������� ���� 5 
#define IND_obj_iInput6			(IND_obj_NULL + 16)		// !���������� ���� 6 
#define IND_obj_iInput7			(IND_obj_NULL + 17)		// !���������� ���� 7 
#define IND_obj_iInput8			(IND_obj_NULL + 18)		// !���������� ���� 8 
/*-----------------------------------------------*/
/*outputs*/
#define IND_obj_oOUT0			(IND_obj_NULL + 19)		// !���������� ����� 0 
#define IND_obj_oOUT1			(IND_obj_NULL + 20)		// !���������� ����� 1 
#define IND_obj_oOUT2			(IND_obj_NULL + 21)		// !���������� ����� 2
#define IND_obj_oOUT3			(IND_obj_NULL + 22)		// !���������� ����� 3
#define IND_obj_oOUT4			(IND_obj_NULL + 23)		// !���������� ����� 4
#define IND_obj_oOUT5			(IND_obj_NULL + 24)		// !���������� ����� 5
#define IND_obj_PredZar			(IND_obj_NULL + 25)		// !���������� ����� 6 [���. ����� ����������]
#define IND_obj_KM1 			(IND_obj_NULL + 26)		// !���������� ����� 7 [���. ��������� ��1] 
/*-----------------SOFTOBJ-----------------------*/
#define IND_obj_USART_TX		(IND_obj_NULL + 30)		// [!������ ���������� ��������� �������� ����� �� USART]
/*-----------------------------------------------*/
#define IND_obj_aINV			(IND_obj_NULL + 31)		// [!������� ���������� ]
#define IND_obj_aINC			(IND_obj_NULL + 32)		// [!������� ���        ]
#define IND_obj_aOUTV			(IND_obj_NULL + 33)		// [!�������� ����������]
#define IND_obj_aOUTC			(IND_obj_NULL + 34)		// [!�������� ���       ]
#define IND_obj_aDRV			(IND_obj_NULL + 35)		// [!���������� ��������]
#define IND_obj_aDRC			(IND_obj_NULL + 36)		// [!��� ��������       ] 
/*-----------------------------------------------*/
#define IND_obj_PID1_KP			(IND_obj_NULL + 37)		// [����������� ��� ��]
#define IND_obj_PID1_KI			(IND_obj_NULL + 38)		// [����������� ��� ��]
#define IND_obj_PID1_KD			(IND_obj_NULL + 39)		// [����������� ��� ��]
#define IND_obj_PID_ON			(IND_obj_NULL + 40)		// [!��������� ����������]
/*-----------------------------------------------*/
#define IND_obj_cPMinV			(IND_obj_NULL + 41)		// [������������ ������� ����������]
#define IND_obj_cPMaxV			(IND_obj_NULL + 42)		// [����������� ������� ����������]
#define IND_obj_cSVError		(IND_obj_NULL + 43)		// [���������� ������ ������� ����������]
#define IND_obj_cSCError		(IND_obj_NULL + 44)		// [���������� ������ ������� ����]
#define IND_obj_cMaxDutyC		(IND_obj_NULL + 45)		// [������������ ����������� ����������]
#define IND_obj_cMinDutyC		(IND_obj_NULL + 46)		// [����������� ����������� ����������]
#define IND_obj_cMaxTemp		(IND_obj_NULL + 47)		// [������������ ����������� ���������]
#define IND_obj_cFreq			(IND_obj_NULL + 48)		// [������� ���������������]
#define IND_obj_cGysteresis		(IND_obj_NULL + 49)		// [���������� ����������]
#define IND_obj_cSync			(IND_obj_NULL + 50)		// [!�������� ������������ � ���]
/*-----------------------------------------------*/
#define IND_obj_ACUR_C			(IND_obj_NULL + 51)		// [�������������� ��������� ���� ��������]
#define IND_obj_ACUR_G			(IND_obj_NULL + 52)		// [����������  ��������� ���� ��������]
#define IND_obj_CUR_C			(IND_obj_NULL + 53)		// [�������������� ������������ ��� � ��������]
#define IND_obj_CUR_G			(IND_obj_NULL + 54)		// [���������� ������������ ��� � ��������]
#define IND_obj_ECE	  			(IND_obj_NULL + 55)		// [������� ��� �������]
#define IND_obj_Q	  			(IND_obj_NULL + 56)		// [����������� ����������� ��������� LC �������]
#define IND_obj_ERR_ARRAY	  	(IND_obj_NULL + 57)		// [!������ ������ ��]
/*-----------------------------------------------*/
#define IND_obj_PWM_FREQ		(IND_obj_NULL + 60)		// [��������� ������� ���] 
#define IND_obj_PWM_ON			(IND_obj_NULL + 61)		// [���./����. ��� � ���������� �����������] 
#define IND_obj_M_BUCK_MODE		(IND_obj_NULL + 62)		// [���������� ����� ���������������]
#define IND_obj_M_BOOST_MODE	(IND_obj_NULL + 63)		// [���������� ����� ��������������]
#define IND_obj_tKM_Off			(IND_obj_NULL + 64)		// [������ ���������� ���������� ��1]
#define IND_obj_tChDevOff		(IND_obj_NULL + 65)		// [���������� ��������������� ]
#define IND_obj_BuckPWMValue	(IND_obj_NULL + 66)		// [�������� D �������� �����]
#define IND_obj_BoostPWMValue	(IND_obj_NULL + 67)		// [�������� D ������� �����]
/*-----------------------------------------------*/
#endif
