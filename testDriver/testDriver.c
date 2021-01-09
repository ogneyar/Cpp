
#include <ntddk.h>
 
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject);
 
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, UnloadRoutine)
 
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
  DriverObject->DriverUnload = UnloadRoutine;
 
  DbgPrint("Hello world!\n"); 
 
  return STATUS_SUCCESS;
}
 
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject)
{
  DbgPrint("Goodbye!\n");
}