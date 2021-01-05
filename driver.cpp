#include "driver.h"

// Предварительные объявления функций:
NTSTATUS DeviceControlRoutine(IN PDEVICE_OBJECT fdo, IN PIRP Irp);

VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject);

NTSTATUS ReadWrite_IRPhandler(IN PDEVICE_OBJECT fdo, IN PIRP Irp);

NTSTATUS Create_File_IRPprocessing(IN PDEVICE_OBJECT fdo, IN PIRP Irp);

NTSTATUS Close_HandleIRPprocessing(IN PDEVICE_OBJECT fdo, IN PIRP Irp);

KSPIN_LOCK SpinLock;


#pragma code_seg("INIT") // положим начало секции INIT

//Итак, мы подключили заголовочный файл Driver.h, сделали несколько предварительных объявлений важных функций, объявили глобальную переменную и определили начало секции INIT. А теперь - реализация функции загрузки драйвера.


extern "C"
NTSTATUS DriverEntry (IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) 
{
    NTSTATUS status = STATUS_SUCCESS;
    PDEVICE_OBJECT fdo; // указатель на объект драйвера
    UNICODE_STRING devName; // указатель на раздел реестра
    // Так как функция - NTSTATUS, то и возвращает она - STATUS_XXX.

    // Все строки, обрамлённые if`ами DBG - будут исполняться только в отладочной версии драйвера.
    #if DBG
    DbgPrint("=Primer= DriverEntry.");
    DbgPrint("=Primer= RegistryPath = %ws.", RegistryPath->Buffer);
    #endif

    // Экспортируем точки входа в драйвер. Поскольку наш драйвер - legacy, процедуру AddDevice мы не экспортируем.
    DriverObject->DriverUnload = UnloadRoutine;
    DriverObject->MajorFunction[IRP_MJ_CREATE]= Create_File_IRPprocessing;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = Close_HandleIRPprocessing;
    DriverObject->MajorFunction[IRP_MJ_READ] = ReadWrite_IRPhandler;
    DriverObject->MajorFunction[IRP_MJ_WRITE] = ReadWrite_IRPhandler;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]=DeviceControlRoutine;

    // Начнём создавать символьную ссылку
    RtlInitUnicodeString(&devName, L"\\Device\\PRIMER"); // данная процедура тоже должна бы располагаться в AddDevice.

    // Создаём свой FDO и получаем указатель на него в fdo. Размер структуры PRIMER_DEVICE_EXTENSION передаётся для // того, чтобы при создании FDO выделить под неё память.
    status = IoCreateDevice(DriverObject,
    sizeof(PRIMER_DEVICE_EXTENSION),
    &devName, FILE_DEVICE_UNKNOWN,0,FALSE, &fdo);
    if(!NT_SUCCESS(status)) return status; // данная процедура также должна бы располагаться в AddDevice.

    // Получаем указатель на область, предназначенную для PRIMER_DEVICE_EXTENSION. 
    PPRIMER_DEVICE_EXTENSION dx = (PPRIMER_DEVICE_EXTENSION)fdo->DeviceExtension;

    dx->fdo = fdo; // сохраняем обратный указатель

    #if DBG
    DbgPrint("=PRIMER= FDO %X, DevExt=%X.",fdo,dx);
    #endif

    // Продолжаем работу по созданию символьной ссылки
    // (примечания такие же, что и в предыдущей части работы).

    UNICODE_STRING symLinkName; 

    #define SYM_LINK_NAME L"\\DosDevices\\Primer" // именно ТАКОЙ код предназначен для того, чтобы символьная ссылка // работала и в Windows 9x, и в NT.
    RtlInitUnicodeString(&symLinkName, SYM_LINK_NAME);
    dx->ustrSymLinkName = symLinkName;

    // И, наконец, создаем собственно символьную ссылку.
    status = IoCreateSymbolicLink(&symLinkName, &devName);
    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(fdo);
        return status;
    } 

    // Инициализируем объект спин - блокировки, который мы будем использовать для обрушения системы во время // выполнения кода обработчика IOCTL - запросов.
    KeInitializeSpinLock(&SpinLock);

    #if DBG
    DbgPrint("=PRIMER= DriverEntry successfully completed.");
    #endif
    return status;
}
#pragma code_seg() // конец секции INIT



// Процедура CompleteIrp, первый аргумент которой - указатель на объект нашего FDO, предназначена для завершения обработки IRP пакетов с кодом завершения status. Эту функцию не нужно нигде регистрировать, так как она предназначена для внутренних потребностей драйвера. 


NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status, ULONG info)
{
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = info; // если этот параметр не равен нулю, то он обычно содержит количество байт, // переданных клиенту.
    IoCompleteRequest(Irp,IO_NO_INCREMENT);
    return status;
}


// Функция ReadWrite_IRPhandler - это рабочая процедура обработки read/write запросов. Если подробнее - она выполняет обработку запросов Диспетчера ввода/вывода, сформированных им в виде IRP пакетов
// (с кодами IRP_MJ_READ/WRITE) в результате обращений к драйверу из пользовательских приложений с вызовами
// read/write (или же из кода режима ядра с вызовами
// ZwRead/WriteFile). В нашем случае функция ReadWrite_IRPhandler ничего особенного не делает, поэтому она реализована в виде заглушки. Определила же я эту процедуру для демонстрации её использования
// (что позднее ты сможешь применить в разработке своих, уже намного более продвинутых драйверов).


NTSTATUS ReadWrite_IRPhandler(IN PDEVICE_OBJECT fdo, // указатель на объект нашего FDO
IN PIRP Irp) // указатель на структуру принятого от Диспетчера ввода/вывода IRP
{
    ULONG BytesTxd = 0;
    NTSTATUS status = STATUS_SUCCESS;

    #if DBG
    DbgPrint("-Primer- in ReadWrite_IRPhandler.");
    #endif
    return CompleteIrp(Irp,status,BytesTxd); // CompleteIrp завершает обработку IRP. BytesTxd (число переданных или полученных байт) - равно нулю. 
}


// Теперь - две связанных функции: Create_File_IRPprocessing и Close_File_IRPprocessing, предназначенные для обработки запросов открытия/закрытия драйвера
// (CreateFile/CloseHandle, ZwCreateFile/ZwClose). Create_File_IRPprocessing обрабатывает IRP_MJ_CREATE, а Close_File_IRPprocessing - IRP_MJ_CLOSE.


NTSTATUS Create_File_IRPprocessing(IN PDEVICE_OBJECT fdo,IN PIRP Irp) // параметры такие же, что и у ReadWrite_IRPhandler
{
    PIO_STACK_LOCATION IrpStack = IoGetCurrentIrpStackLocation(Irp);

    #if DBG
    DbgPrint("-Primer- Create File is %ws",
    &(IrpStack->FileObject->FileName.Buffer));
    #endif

    return CompleteIrp(Irp,STATUS_SUCCESS,0);
}


NTSTATUS Close_HandleIRPprocessing(IN PDEVICE_OBJECT fdo,IN PIRP Irp) // параметры те же, что и у ReadWrite_IRPhandler и // Create_File_IRPprocessing 
{
    #if DBG
    DbgPrint("-Primer- In Close handler."); 
    #endif
    return CompleteIrp(Irp,STATUS_SUCCESS,0);
}


// Теперь - огромная рабочая процедура DeviceControlRoutine, предназначенная для обработки IOCTL - запросов
// (точнее - для обработки IRP_MJ_DEVICE_CONTROL - запросов, возникающих в результате обращения пользовательских приложений к драйверу с вызовом DeviceIoControl). В нашем драйвере эта функция реализует обработку нескольких IOCTL запросов. Все необходимые комментарии я буду давать в коде
// (определения возможно незнакомых тебе типов данных
// (UCHAR, PUCHAR etc) можно посмотреть в Windef.h). 

NTSTATUS DeviceControlRoutine(IN PDEVICE_OBJECT fdo, IN PIRP Irp) // параметры - смотри в предыдущих трёх функциях.
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG BytesTxd =0; // Число переданных/полученных байт
    PIO_STACK_LOCATION IrpStack=IoGetCurrentIrpStackLocation(Irp);

    // Получаем указатель на расширение устройства
    PPRIMER_DEVICE_EXTENSION dx = (PPRIMER_DEVICE_EXTENSION)fdo->DeviceExtension;

    ULONG ControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
    ULONG method = ControlCode & 0x03;

    // Получаем текущее значение уровня IRQL – приоритета,
    // на котором выполняется поток:
    KIRQL irql, 
    currentIrql = KeGetCurrentIrql();

    #if DBG
    DbgPrint("-Primer- In DeviceControlRoutine (fdo= %X)\n",fdo);
    DbgPrint("-Primer- DeviceIoControl: IOCTL %x.", ControlCode);
    if(currentIrql==PASSIVE_LEVEL)
    DbgPrint("-Primer- PASSIVE_LEVEL (val=%d)",currentIrql);
    #endif

    KeAcquireSpinLock(&SpinLock,&irql);

    // Диспетчеризация по IOCTL кодам:
    switch(ControlCode) {

    #ifndef SMALL_VERSION
    case IOCTL_PRINT_DEBUG_MESS: // выводит мессагу в дебаговую консоль
    { 
        #if DBG
        DbgPrint("-Primer- IOCTL_PRINT_DEBUG_MESS.");
        #endif
        break;
    }
    case IOCTL_CHANGE_IRQL: // играемся с уровнями IRQL 
    {
        #if DBG
        DbgPrint("-Primer- IOCTL_CHANGE_IRQL.");
        KIRQL dl = DISPATCH_LEVEL, 
        oldIrql,
        newIrql=25; 

        KeRaiseIrql(newIrql,&oldIrql);
        newIrql=KeGetCurrentIrql(); 

        DbgPrint("-Primer- DISPATCH_LEVEL value =%d",dl);
        DbgPrint("-Primer- IRQLs are old=%d new=%d", oldIrql,newIrql);
        KeLowerIrql(oldIrql); 
        #endif
        break;
    }
    #endif 

    case IOCTL_MAKE_SYSTEM_CRASH: // "роняем" систему (падает только NT)
    {
        int errDetected=0;
        char x = (char)0xFF;

        #if DBG :
        DbgPrint("-Primer- IOCTL_MAKE_SYSTEM_CRASH.");
        #endif
        __try { 
            x = *(char*)0x0L;
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        { // Перехват исключения не срабатывает
            errDetected=1;
        };
        #if DBG
        DbgPrint("-Primer- Value of x is %X.",x);
        if(errDetected)
        DbgPrint("-Primer- Except detected in Primer driver.");
        #endif
        break;
    }

    #ifndef SMALL_VERSION
    case IOCTL_TOUCH_PORT_378H: // пробуем поработать с аппаратными ресурсами системы
    { 
        unsigned short ECRegister = 0x378+0x402;
        #if DBG
        DbgPrint("-Primer- IOCTL_TOUCH_PORT_378H.");
        #endif
        _asm {
            mov dx,ECRegister ;
            xor al,al ;
            out dx,al ; Установить EPP mode 000
            mov al,095h ; Биты 7:5 = 100
            out dx,al ; Установить EPP mode 100
        }
        break;
    }

    case IOCTL_SEND_BYTE_TO_USER: // шлём байтик юзерскому приложению 
    { 
        // Размер данных, поступивших от пользователя:
        ULONG InputLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
        ULONG OutputLength = 
        IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
        #if DBG
        DbgPrint("-Primer- Buffer outlength %d",OutputLength);
        #endif

        if(OutputLength<1)
        {
            status = STATUS_INVALID_PARAMETER;
            break;
        }
        UCHAR *buff; // 
        if(method==METHOD_BUFFERED)
        {
            buff = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
            #if DBG
            DbgPrint("-Primer- Method : BUFFERED.");
            #endif
        }
        else
        if (method==METHOD_NEITHER)
        {
            buff=(unsigned char*)Irp->UserBuffer;
            #if DBG
            DbgPrint("-Primer- Method : NEITHER.");
            #endif
        }
        else 
        {
            #if DBG
            DbgPrint("-Primer- Method : unsupported.");
            #endif
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }
        #if DBG
        DbgPrint("-Primer- Buffer address is %08X",buff);
        #endif
        *buff=33; 
        BytesTxd = 1; 
        break;
    }
    #endif 

    default: status = STATUS_INVALID_DEVICE_REQUEST;
    }

    KeReleaseSpinLock(&SpinLock,irql);

    #if DBG
    DbgPrint("-Primer- DeviceIoControl: %d bytes written.", (int)BytesTxd);
    #endif

    return CompleteIrp(Irp,status,BytesTxd); 
}

// Небольшая Эрих Мария Ремарка: почему в обработке IOCTL_MAKE_SYSTEM_CRASH не происходит перехвата исключения? Да потому, что вызов KeAcquireSpinLock меняет уровень IRQL на 2, а ведь данный обработчик IOCTL был вызван драйвером с уровня IRQL == 0
// (PASSIVE_LEVEL)! Таким образом, конструкция try/exception, корректно работающая на нулевом IRQL уровне, на IRQL == 2 исключение не перехватывает, и, следовательно, не обрабатывает. Так что во всём виноваты спин - блокировки! Также стоит обратить пристальное внимание на процедуру обработки IOCTL_TOUCH_PORT_378H, так как в момент нашего обращения к порту может случиться так, что с ним уже будут работать другие драйвера или устройства. 

// Ну а теперь, наконец, процедура UnloadRoutine, выполняющая выгрузку драйвера, высвобождающая занятые драйвером объекты, и принимающая единственный параметр - указатель на объект драйвера. 

#pragma code_seg("PAGE") // начинает секцию PAGE

VOID UnloadRoutine(IN PDRIVER_OBJECT pDriverObject)
{
    PDEVICE_OBJECT pNextDevObj;
    int i;

    #if DBG
    DbgPrint("-Primer- In Unload Routine.");
    #endif
    // Нижеприведённые операции в полноценном WDM драйвере следут поместить в обработчик IRP_MJ_PNP - запросов (с  субкодом IRP_MN_REMOVE_DEVICE, естественно).

    pNextDevObj = pDriverObject->DeviceObject;

    for(i=0; pNextDevObj!=NULL; i++)
    {
        PPRIMER_DEVICE_EXTENSION dx = 
        (PPRIMER_DEVICE_EXTENSION)pNextDevObj->DeviceExtension;
        // Удаляем символьную ссылку и уничтожаем
        FDO:
        UNICODE_STRING *pLinkName = & (dx->ustrSymLinkName);
        // сохраняем указатель:
        pNextDevObj = pNextDevObj->NextDevice;

        #if DBG
        DbgPrint("-Primer- Deleted device (%d) : pointer to FDO = %X.",
        i,dx->fdo);
        DbgPrint("-Primer- Deleted symlink = %ws.", pLinkName->Buffer);
        #endif

        IoDeleteSymbolicLink(pLinkName);
        IoDeleteDevice(dx->fdo);
    }
}
#pragma code_seg() // завершаем секцию PAGE