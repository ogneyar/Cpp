#ifndef _DRIVER_H_04802_BASHBD_1UIWQ1_8239_1NJKDH832_901_ // эти строки запрещают повторный проход по тексту заголовочных 
#define _DRIVER_H_04802_BASHBD_1UIWQ1_8239_1NJKDH832_901_ // (*.h) файлов, что особенно актуально для больших проектов 
// и повышает скорость компиляции
#ifdef __cplusplus
extern "C"
{
#endif

#include "ntddk.h" // если заменить эту строку на #include "wdm.h", то компиляция драйвера
//(с использованием Build из DDK)
// пройдёт успешно, но WDM он всё равно не станет 

#ifdef __cplusplus
}
#endif

// Далее - структура расширения устройства 

typedef struct _PRIMER_DEVICE_EXTENSION
{
    PDEVICE_OBJECT fdo; // указатель на FDO.
    UNICODE_STRING ustrSymLinkName; // L"\\DosDevices\\Primer" - имя символьной ссылки
} PRIMER_DEVICE_EXTENSION, *PPRIMER_DEVICE_EXTENSION;

// С помощью этих, определённых нами, кодов IOCTL, в будущем можно будет обращаться к драйверу через DeviceIoControl

#define IOCTL_PRINT_DEBUG_MESS CTL_CODE(\
FILE_DEVICE_UNKNOWN, 0x701, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CHANGE_IRQL CTL_CODE(\
FILE_DEVICE_UNKNOWN, 0x702, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MAKE_SYSTEM_CRASH CTL_CODE(\
FILE_DEVICE_UNKNOWN, 0x703, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_TOUCH_PORT_378H CTL_CODE(\
FILE_DEVICE_UNKNOWN, 0x704, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SEND_BYTE_TO_USER CTL_CODE(\
FILE_DEVICE_UNKNOWN, 0x705, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif