#ifndef __MYRTC_H
#define __MYRTC_H

extern uint16_t MyRTC_Time[];
void My_RTCInit(void);
void  MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
