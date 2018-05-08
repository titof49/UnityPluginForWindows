#include <stdafx.h>
#include <guiddef.h>
#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <bthledef.h>
#include <guiddef.h>
#include <Bluetoothleapis.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <objbase.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
// #define TO_SEARCH_DEVICE_UUID "{0000180D-0000-1000-8000-00805F9B34FB}" //we use UUID for an HR BLE device 
#define TO_SEARCH_DEVICE_UUID               "{EF680400-9B35-4933-9B10-52FFA9740042}" //
//
#define TO_SEARCH_WESU_DEVICE_UUID			"{00000000-0001-11E1-9AB4-0002A5D5C51B}"
//
#define TO_SEARCH_WESU_CHARACTERISTIC_WESU	"{00000080-0001-11E1-AC36-0002A5D5C51B}"

//
#define TO_SEARCH_MOTENV_DEVICE_UUID			"{00000000-0002-11E1-9AB4-0002A5D5C51B}"
//
#define TO_SEARCH_MOTENV_CHARACTERISTIC_MOTENV	"{00000100-0001-11E1-AC36-0002A5D5C51B}"

const int MAX_DEVICE_THINGY     =50;
const int MAX_DEVICE_WESU       =50;
const int MAX_DEVICE_MOTENV     =50;




ULONGLONG TickCount64Thingy[MAX_DEVICE_THINGY];
ULONGLONG TickCount64Wesu[MAX_DEVICE_WESU];
ULONGLONG TickCount64MotEnv[MAX_DEVICE_MOTENV];

//Step 1: find the BLE device handle from its GUID
GUID AGuidThingy;
GUID AGuidWesu;
GUID AGuidMotEnv;

HANDLE                      hLEDeviceThingy[MAX_DEVICE_THINGY];
BLUETOOTH_GATT_EVENT_HANDLE EventHandleThingy[MAX_DEVICE_THINGY];
int                         CallBackValueThingy[MAX_DEVICE_THINGY];


HANDLE                      hLEDeviceWesu[MAX_DEVICE_WESU];
BLUETOOTH_GATT_EVENT_HANDLE EventHandleWesu[MAX_DEVICE_WESU];
int                         CallBackValueWesu[MAX_DEVICE_WESU];


HANDLE                      hLEDeviceMotEnv[MAX_DEVICE_MOTENV];
BLUETOOTH_GATT_EVENT_HANDLE EventHandleMotEnv[MAX_DEVICE_MOTENV];
int                         CallBackValueEnv[MAX_DEVICE_MOTENV];


// Thingy  
float                       QwThingy[MAX_DEVICE_THINGY],
                            QxThingy[MAX_DEVICE_THINGY],
                            QyThingy[MAX_DEVICE_THINGY],
                            QzThingy[MAX_DEVICE_THINGY];
// Wesu
float                       QwWesu[MAX_DEVICE_WESU],
                            QxWesu[MAX_DEVICE_WESU],
                            QyWesu[MAX_DEVICE_WESU],
                            QzWesu[MAX_DEVICE_WESU];
// MOTENV
float                       QwMotEnv[MAX_DEVICE_MOTENV],
                            QxMotEnv[MAX_DEVICE_MOTENV],
                            QyMotEnv[MAX_DEVICE_MOTENV],
                            QzMotEnv[MAX_DEVICE_MOTENV];


//
void OpenBluetoothThingy(int Index);
void CloseBluetoothThingy(int Index);
//
void OpenBluetoothWesu(int Index);
void CloseBluetoothWesu(int Index);
//
void OpenBluetoothMotEnv(int Index);
void CloseBluetoothMotEnv(int Index);
//
float getQs(float qi, float qj, float qk);

//this is the notification function
void CALLBACK SomethingHappenedThingy(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
    float SCALE_FACTOR = 1073741824.0F;
    //
    unsigned char QwB[4], QxB[4], QyB[4], QzB[4];

    printf("notification obtained ");
    PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

    HRESULT hr;
    if (0x10 != ValueChangedEventParameters->CharacteristicValue->DataSize) {
        hr = E_FAIL;
        printf("datasize 0\n");
    }
    else
    {
        printf("Thingy");
        //
        QwB[0] = ValueChangedEventParameters->CharacteristicValue->Data[0];
        QwB[1] = ValueChangedEventParameters->CharacteristicValue->Data[1];
        QwB[2] = ValueChangedEventParameters->CharacteristicValue->Data[2];
        QwB[3] = ValueChangedEventParameters->CharacteristicValue->Data[3];
        //
        int iQwB = ((QwB[3] << 24) & 0xff000000) |
            ((QwB[2] << 16) & 0x00ff0000) |
            ((QwB[1] << 8) & 0x0000ff00) |
            ((QwB[0] << 0) & 0x000000ff);
        //
        QxB[0] = ValueChangedEventParameters->CharacteristicValue->Data[4];
        QxB[1] = ValueChangedEventParameters->CharacteristicValue->Data[5];
        QxB[2] = ValueChangedEventParameters->CharacteristicValue->Data[6];
        QxB[3] = ValueChangedEventParameters->CharacteristicValue->Data[7];
        //
        int iQxB = ((QxB[3] << 24) & 0xff000000) |
            ((QxB[2] << 16) & 0x00ff0000) |
            ((QxB[1] << 8) & 0x0000ff00) |
            ((QxB[0] << 0) & 0x000000ff);
        //
        QyB[0] = ValueChangedEventParameters->CharacteristicValue->Data[8];
        QyB[1] = ValueChangedEventParameters->CharacteristicValue->Data[9];
        QyB[2] = ValueChangedEventParameters->CharacteristicValue->Data[10];
        QyB[3] = ValueChangedEventParameters->CharacteristicValue->Data[11];
        //
        int iQyB = ((QyB[3] << 24) & 0xff000000) |
            ((QyB[2] << 16) & 0x00ff0000) |
            ((QyB[1] << 8) & 0x0000ff00) |
            ((QyB[0] << 0) & 0x000000ff);
        //
        QzB[0] = ValueChangedEventParameters->CharacteristicValue->Data[12];
        QzB[1] = ValueChangedEventParameters->CharacteristicValue->Data[13];
        QzB[2] = ValueChangedEventParameters->CharacteristicValue->Data[14];
        QzB[3] = ValueChangedEventParameters->CharacteristicValue->Data[15];
        //
        int iQzB = ((QzB[3] << 24) & 0xff000000) |
            ((QzB[2] << 16) & 0x00ff0000) |
            ((QzB[1] << 8) & 0x0000ff00) |
            ((QzB[0] << 0) & 0x000000ff);

        // reduction en flottant
        QwThingy[*((PINT)Context)] = (float)(iQwB) / SCALE_FACTOR;
        QxThingy[*((PINT)Context)] = (float)(iQxB) / SCALE_FACTOR;
        QyThingy[*((PINT)Context)] = (float)(iQyB) / SCALE_FACTOR;
        QzThingy[*((PINT)Context)] = (float)(iQzB) / SCALE_FACTOR;
        //
        printf("%X,%X,%X,%X\n", iQwB, iQxB, iQyB, iQzB);
        printf("%f,%f,%f,%f\n", QwThingy, QxThingy, QyThingy, QzThingy);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Thingy[*((PINT)Context)], NewTickCount64);
        //
        TickCount64Thingy[*((PINT)Context)] = NewTickCount64;
    }
}

//this is the notification function for the Wesu
void CALLBACK SomethingHappenedWesu(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
    unsigned char QiB[4], QjB[4], QkB[4], QsB[4];

    printf("notification obtained ");
    PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

    HRESULT hr;
    if (0x14 != ValueChangedEventParameters->CharacteristicValue->DataSize) {
        hr = E_FAIL;
        printf("datasize 0\n");
    }
    else
    {
        printf("Wesu ");
        //
        QiB[0] = ValueChangedEventParameters->CharacteristicValue->Data[2];
        QiB[1] = ValueChangedEventParameters->CharacteristicValue->Data[3];
        QiB[2] = ValueChangedEventParameters->CharacteristicValue->Data[4];
        QiB[3] = ValueChangedEventParameters->CharacteristicValue->Data[5];

        memcpy(&QxWesu[*((PINT)Context)], &QiB, sizeof QxWesu);

        QjB[0] = ValueChangedEventParameters->CharacteristicValue->Data[6];
        QjB[1] = ValueChangedEventParameters->CharacteristicValue->Data[7];
        QjB[2] = ValueChangedEventParameters->CharacteristicValue->Data[8];
        QjB[3] = ValueChangedEventParameters->CharacteristicValue->Data[9];

        memcpy(&QyWesu[*((PINT)Context)], &QjB, sizeof QyWesu);

        QkB[0] = ValueChangedEventParameters->CharacteristicValue->Data[10];
        QkB[1] = ValueChangedEventParameters->CharacteristicValue->Data[11];
        QkB[2] = ValueChangedEventParameters->CharacteristicValue->Data[12];
        QkB[3] = ValueChangedEventParameters->CharacteristicValue->Data[13];
 
        memcpy(&QzWesu[*((PINT)Context)], &QkB, sizeof QzWesu);
        
        QsB[0] = ValueChangedEventParameters->CharacteristicValue->Data[14];
        QsB[1] = ValueChangedEventParameters->CharacteristicValue->Data[15];
        QsB[2] = ValueChangedEventParameters->CharacteristicValue->Data[16];
        QsB[3] = ValueChangedEventParameters->CharacteristicValue->Data[17];

        memcpy(&QwWesu[*((PINT)Context)], &QsB, sizeof QwWesu);

        //
        printf("%f,%f,%f,%f\n", QxWesu[*((PINT)Context)], QyWesu[*((PINT)Context)], QzWesu[*((PINT)Context)], QwWesu[*((PINT)Context)]);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Wesu[*((PINT)Context)], NewTickCount64);
        //
        TickCount64Wesu[*((PINT)Context)] = NewTickCount64;
    }
}

//this is the notification function for the Wesu
void CALLBACK SomethingHappenedMotEnv(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
    //
    unsigned char   QiB[4], QjB[4], QkB[4];
    short  iQxB  , iQyB,   iQzB;

    //
    float SCALE_FACTOR = 10000.0f;
    //
    printf("notification obtained ");
    PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

    HRESULT hr;
    if (0x14 != ValueChangedEventParameters->CharacteristicValue->DataSize) {
        hr = E_FAIL;
        printf("datasize 0\n");
    }
    else
    {
        printf("MOT ");

        //
        QiB[0] = ValueChangedEventParameters->CharacteristicValue->Data[2];
        QiB[1] = ValueChangedEventParameters->CharacteristicValue->Data[3];
        memcpy(&iQxB, &QiB, sizeof iQxB);
        //
        QjB[0] = ValueChangedEventParameters->CharacteristicValue->Data[4];
        QjB[1] = ValueChangedEventParameters->CharacteristicValue->Data[5];
        memcpy(&iQyB, &QjB, sizeof iQyB);
        //
        QkB[0] = ValueChangedEventParameters->CharacteristicValue->Data[6];
        QkB[1] = ValueChangedEventParameters->CharacteristicValue->Data[7];
        memcpy(&iQzB, &QkB, sizeof iQzB);

        QxMotEnv[*((PINT)Context)] = (float)(iQxB) / SCALE_FACTOR;
        QyMotEnv[*((PINT)Context)] = (float)(iQyB) / SCALE_FACTOR;
        QzMotEnv[*((PINT)Context)] = (float)(iQzB) / SCALE_FACTOR;
        QwMotEnv[*((PINT)Context)] = getQs(QxMotEnv[*((PINT)Context)], QyMotEnv[*((PINT)Context)], QzMotEnv[*((PINT)Context)]);


        //
        printf("%f,%f,%f,%f\n", QxMotEnv, QyMotEnv, QzMotEnv, QwMotEnv);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Wesu[*((PINT)Context)], NewTickCount64);
        //
        TickCount64Wesu[*((PINT)Context)] = NewTickCount64;
     }
}


/*
public String ParseMOTBLM(byte[] data)
{
float SCALE_FACTOR = 10000.0f;


//
String Retour = "";
//
float qi, qj, qk, qs;

byte[] QiB = new byte[4];
byte[] QjB = new byte[4];
byte[] QkB = new byte[4];
byte[] QsB = new byte[4];


//
QiB[0] = data[2];
QiB[1] = data[3];
qi = (float)(BitConverter.ToInt16(QiB, 0)) / SCALE_FACTOR;

//
QjB[0] = data[4];
QjB[1] = data[5];
qj = (float)(BitConverter.ToInt16(QjB, 0)) / SCALE_FACTOR;

//
QkB[0] = data[6];
QkB[1] = data[7];
qk = (float)(BitConverter.ToInt16(QkB, 0)) / SCALE_FACTOR;

// on calcule qs
qs = getQs(qi, qj, qk);
rootPage.Q1 = new SharpDX.Quaternion(qi, qj, qk, qs);

//
Retour = String.Format("Qi:{0},Qj:{1},Qk:{2},Qs:{3}, Norm {4}", qi, qj, qk, qs, rootPage.Q1.IsNormalized);
// Retour = String.Format("Angle: x:{0},y:{1},z:{2}", rootPage.Q1.Axis.X, rootPage.Q1.Axis.Y, rootPage.Q1.Axis.Z);
return Retour;
}

float getQs(float qi, float qj, float qk)
{
float t = 1 - (qi * qi + qj * qj + qk * qk);
return t > 0 ? (float)(Math.Sqrt(t)) : 0;

}


*/

float getQs(float qi, float qj, float qk)
{
    float t = 1 - (qi * qi + qj * qj + qk * qk);
    return t > 0 ? (float)(sqrtf(t)) : 0;

}


HANDLE GetBLEHandleOffset(__in GUID AGuid, __in int Index)
{
    HDEVINFO hDI;
    SP_DEVICE_INTERFACE_DATA did;
    SP_DEVINFO_DATA dd;
    GUID BluetoothInterfaceGUID = AGuid;
    HANDLE hComm = NULL;

    hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if (hDI == INVALID_HANDLE_VALUE) return NULL;

    did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    dd.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, i, &did); i++)
    {
        SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

        DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        DWORD size = 0;

        if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
        {
            int err = GetLastError();

            if (err == ERROR_NO_MORE_ITEMS) break;

            PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

            pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
                break;

            //
            // On recupere le handle du device qui nous intéresse.
            // Si il n'existe pas ou si il n'est pas dans le range, la valeur sera NULL
            if (i == Index)
            {
                hComm = CreateFile(
                    pInterfaceDetailData->DevicePath,
                    GENERIC_WRITE | GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    NULL);
            }

            GlobalFree(pInterfaceDetailData);
        }
    }

    SetupDiDestroyDeviceInfoList(hDI);
    return hComm;
}



/*
//this function works to get a handle for a BLE device based on its GUID
//copied from http://social.msdn.microsoft.com/Forums/windowshardware/en-US/e5e1058d-5a64-4e60-b8e2-0ce327c13058/erroraccessdenied-error-when-trying-to-receive-data-from-bluetooth-low-energy-devices?forum=wdk
//credits to Andrey_sh
HANDLE GetBLEHandle(__in GUID AGuid)
{
    HDEVINFO hDI;
    SP_DEVICE_INTERFACE_DATA did;
    SP_DEVINFO_DATA dd;
    GUID BluetoothInterfaceGUID = AGuid;
    HANDLE hComm = NULL;

    hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if (hDI == INVALID_HANDLE_VALUE) return NULL;

    did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    dd.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, i, &did); i++)
    {
        SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

        DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        DWORD size = 0;

        if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
        {
            int err = GetLastError();

            if (err == ERROR_NO_MORE_ITEMS) break;

            PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

            pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
                break;

            hComm = CreateFile(
                pInterfaceDetailData->DevicePath,
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                NULL);

            GlobalFree(pInterfaceDetailData);
        }
    }

    SetupDiDestroyDeviceInfoList(hDI);
    return hComm;
}
*/

extern "C" float __declspec(dllexport) _stdcall GetFloatWThingy(int Index)
{
    return QwThingy[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXThingy(int Index)
{
	return QxThingy[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYThingy(int Index)
{
    return QyThingy[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZThingy(int Index)
{
    return QzThingy[Index];
}




extern "C" float __declspec(dllexport) _stdcall GetFloatWWesu(int Index)
{
    return QwWesu[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXWesu(int Index)
{
    return QxWesu[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYWesu(int Index)
{
    return QyWesu[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZWesu(int Index)
{
    return QzWesu[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXMotEnv(int Index)
{
    return QxMotEnv[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYMotEnv(int Index)
{
    return QyMotEnv[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZMotEnv(int Index)
{
    return QzMotEnv[Index];
}

extern "C" float __declspec(dllexport) _stdcall GetFloatWMotEnv(int Index)
{
    return QwMotEnv[Index];
}


extern "C" void __declspec(dllexport) _stdcall OpenBleThingy(int Index)
{
    // Ouverture de la connexion
    OpenBluetoothThingy(Index);
}

extern "C" void __declspec(dllexport) _stdcall CloseBleThingy(int Index)
{
    // Fermeture de la connexion
    CloseBluetoothThingy(Index);
}

extern "C" void __declspec(dllexport) _stdcall OpenBleWesu(int Index)
{
    // Ouverture de la connexion
    OpenBluetoothWesu(Index);
}

extern "C" void __declspec(dllexport) _stdcall CloseBleWesu(int Index)
{
    // Fermeture de la connexion
    CloseBluetoothWesu(Index);
}

extern "C" void __declspec(dllexport) _stdcall OpenBleMotEnv(int Index)
{
    // Ouverture de la connexion
    OpenBluetoothMotEnv(Index);
}

extern "C" void __declspec(dllexport) _stdcall CloseBleMotEnv(int Index)
{
    // Fermeture de la connexion
    CloseBluetoothMotEnv(Index);
}

//
void CloseBluetoothThingy(int Index)
{
    BluetoothGATTUnregisterEvent(EventHandleThingy[Index], BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceThingy[Index]);
}

void CloseBluetoothWesu(int Index)
{
    BluetoothGATTUnregisterEvent(EventHandleWesu[Index], BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceWesu[Index]);
}

void CloseBluetoothMotEnv(int Index)
{
    BluetoothGATTUnregisterEvent(EventHandleMotEnv[Index], BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceMotEnv[Index]);
}

void OpenBluetoothThingy(int Index)
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID), &AGuidThingy);

    //now get the handle 
    hLEDeviceThingy[Index] = GetBLEHandleOffset(AGuidThingy, Index);

    //
    TickCount64Thingy[Index] = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceThingy[Index],
        0,
        NULL,
        &serviceBufferCount,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
        malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);

    if (NULL == pServiceBuffer) {
        printf("pServiceBuffer out of memory\r\n");
    }
    else {
        RtlZeroMemory(pServiceBuffer,
            sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Retrieve Services
    ////////////////////////////////////////////////////////////////////////////

    USHORT numServices;
    hr = BluetoothGATTGetServices(
        hLEDeviceThingy[Index],
        serviceBufferCount,
        pServiceBuffer,
        &numServices,
        BLUETOOTH_GATT_FLAG_NONE);

    if (S_OK != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }


    //Step 3: now get the list of charactersitics. note how the pServiceBuffer is required from step 2
    ////////////////////////////////////////////////////////////////////////////
    // Determine Characteristic Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    USHORT charBufferSize;
    hr = BluetoothGATTGetCharacteristics(
        hLEDeviceThingy[Index],
        pServiceBuffer,
        0,
        NULL,
        &charBufferSize,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = NULL;
    if (charBufferSize > 0) {
        pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
            malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));

        if (NULL == pCharBuffer) {
            printf("pCharBuffer out of memory\r\n");
        }
        else {
            RtlZeroMemory(pCharBuffer,
                charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
        }

        ////////////////////////////////////////////////////////////////////////////
        // Retrieve Characteristics
        ////////////////////////////////////////////////////////////////////////////
        USHORT numChars;
        hr = BluetoothGATTGetCharacteristics(
            hLEDeviceThingy[Index],
            pServiceBuffer,
            charBufferSize,
            pCharBuffer,
            &numChars,
            BLUETOOTH_GATT_FLAG_NONE);

        if (S_OK != hr) {
            printf("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
        }

        if (numChars != charBufferSize) {
            printf("buffer size and buffer size actual size mismatch\r\n");
        }
    }


    //Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
    //descriptors are required as we descriptors that are notification based will have to be written
    //once IsSubcribeToNotification set to true, we set the appropriate callback function
    //need for setting descriptors for notification according to
    //http://social.msdn.microsoft.com/Forums/en-US/11d3a7ce-182b-4190-bf9d-64fefc3328d9/windows-bluetooth-le-apis-event-callbacks?forum=wdk
    PBTH_LE_GATT_CHARACTERISTIC currGattChar;
    for (int ii = 0; ii <charBufferSize; ii++) {
        currGattChar = &pCharBuffer[ii];
        USHORT charValueDataSize;
        PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;


        ///////////////////////////////////////////////////////////////////////////
        // Determine Descriptor Buffer Size
        ////////////////////////////////////////////////////////////////////////////
        USHORT descriptorBufferSize;
        hr = BluetoothGATTGetDescriptors(
            hLEDeviceThingy[Index],
            currGattChar,
            0,
            NULL,
            &descriptorBufferSize,
            BLUETOOTH_GATT_FLAG_NONE);

        if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
            printf("BluetoothGATTGetDescriptors - Buffer Size %d", hr);
        }

        PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
        if (descriptorBufferSize > 0) {
            pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
                malloc(descriptorBufferSize
                    * sizeof(BTH_LE_GATT_DESCRIPTOR));

            if (NULL == pDescriptorBuffer) {
                printf("pDescriptorBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve Descriptors
            ////////////////////////////////////////////////////////////////////////////

            USHORT numDescriptors;
            hr = BluetoothGATTGetDescriptors(
                hLEDeviceThingy[Index],
                currGattChar,
                descriptorBufferSize,
                pDescriptorBuffer,
                &numDescriptors,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetDescriptors - Actual Data %d", hr);
            }

            if (numDescriptors != descriptorBufferSize) {
                printf("buffer size and buffer size actual size mismatch\r\n");
            }

            for (int kk = 0; kk<numDescriptors; kk++) {
                PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[kk];
                ////////////////////////////////////////////////////////////////////////////
                // Determine Descriptor Value Buffer Size
                ////////////////////////////////////////////////////////////////////////////
                USHORT descValueDataSize;
                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceThingy[Index],
                    currGattDescriptor,
                    0,
                    NULL,
                    &descValueDataSize,
                    BLUETOOTH_GATT_FLAG_NONE);

                if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
                }

                PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

                if (NULL == pDescValueBuffer) {
                    printf("pDescValueBuffer out of memory\r\n");
                }
                else {
                    RtlZeroMemory(pDescValueBuffer, descValueDataSize);
                }

                ////////////////////////////////////////////////////////////////////////////
                // Retrieve the Descriptor Value
                ////////////////////////////////////////////////////////////////////////////

                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceThingy[Index],
                    currGattDescriptor,
                    (ULONG)descValueDataSize,
                    pDescValueBuffer,
                    NULL,
                    BLUETOOTH_GATT_FLAG_NONE);
                if (S_OK != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
                }
                //you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
                // we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
                if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
                {
                    BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

                    RtlZeroMemory(&newValue, sizeof(newValue));

                    newValue.DescriptorType = ClientCharacteristicConfiguration;
                    newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

                    hr = BluetoothGATTSetDescriptorValue(
                        hLEDeviceThingy[Index],
                        currGattDescriptor,
                        &newValue,
                        BLUETOOTH_GATT_FLAG_NONE);
                    if (S_OK != hr) {
                        printf("BluetoothGATTGetDescriptorValue - Actual Data %d\n", hr);
                    }
                    else {
                        printf("setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
                    }

                }

            }
        }


        if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
        {
            printf("Setting Notification for ServiceHandle %d --- %X \n", currGattChar->ServiceHandle, currGattChar->CharacteristicUuid.Value.ShortUuid);
            BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;

            BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
            EventParameterIn.Characteristics[0] = *currGattChar;
            EventParameterIn.NumCharacteristics = 1;
            //
            CallBackValueThingy[Index] = Index;
            //
            hr = BluetoothGATTRegisterEvent(
                hLEDeviceThingy[Index],
                EventType,
                &EventParameterIn,
                SomethingHappenedThingy,
                &CallBackValueThingy[Index],
                &EventHandleThingy[Index],
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTRegisterEvent - Actual Data %d", hr);
            }
        }


        if (currGattChar->IsReadable) {//currGattChar->IsReadable
                                       ////////////////////////////////////////////////////////////////////////////
                                       // Determine Characteristic Value Buffer Size
                                       ////////////////////////////////////////////////////////////////////////////
            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceThingy[Index],
                currGattChar,
                0,
                NULL,
                &charValueDataSize,
                BLUETOOTH_GATT_FLAG_NONE);

            if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
            }

            pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

            if (NULL == pCharValueBuffer) {
                printf("pCharValueBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pCharValueBuffer, charValueDataSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve the Characteristic Value
            ////////////////////////////////////////////////////////////////////////////

            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceThingy[Index],
                currGattChar,
                (ULONG)charValueDataSize,
                pCharValueBuffer,
                NULL,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
            }

            //print the characeteristic Value
            //for an HR monitor this might be the body sensor location
            printf("\n Printing a read (not notifiable) characterstic (maybe) body sensor value");
            for (int iii = 0; iii< pCharValueBuffer->DataSize; iii++) {// ideally check ->DataSize before printing
                printf("%d", pCharValueBuffer->Data[iii]);
            }
            printf("\n");

            // Free before going to next iteration, or memory leak.
            free(pCharValueBuffer);
            pCharValueBuffer = NULL;
        }

    }

}
void OpenBluetoothWesu(int Index)
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_WESU_DEVICE_UUID), &AGuidWesu);

    //now get the handle 
    hLEDeviceWesu[Index] = GetBLEHandleOffset(AGuidWesu,Index);

    //
    TickCount64Wesu[Index] = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceWesu[Index],
        0,
        NULL,
        &serviceBufferCount,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
        malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);

    if (NULL == pServiceBuffer) {
        printf("pServiceBuffer out of memory\r\n");
    }
    else {
        RtlZeroMemory(pServiceBuffer,
            sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Retrieve Services
    ////////////////////////////////////////////////////////////////////////////

    USHORT numServices;
    hr = BluetoothGATTGetServices(
        hLEDeviceWesu[Index],
        serviceBufferCount,
        pServiceBuffer,
        &numServices,
        BLUETOOTH_GATT_FLAG_NONE);

    if (S_OK != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }


    //Step 3: now get the list of charactersitics. note how the pServiceBuffer is required from step 2
    ////////////////////////////////////////////////////////////////////////////
    // Determine Characteristic Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    USHORT charBufferSize;
    hr = BluetoothGATTGetCharacteristics(
        hLEDeviceWesu[Index],
        pServiceBuffer,
        0,
        NULL,
        &charBufferSize,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = NULL;
    if (charBufferSize > 0) {
        pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
            malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));

        if (NULL == pCharBuffer) {
            printf("pCharBuffer out of memory\r\n");
        }
        else {
            RtlZeroMemory(pCharBuffer,
                charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
        }

        ////////////////////////////////////////////////////////////////////////////
        // Retrieve Characteristics
        ////////////////////////////////////////////////////////////////////////////
        USHORT numChars;
        hr = BluetoothGATTGetCharacteristics(
            hLEDeviceWesu[Index],
            pServiceBuffer,
            charBufferSize,
            pCharBuffer,
            &numChars,
            BLUETOOTH_GATT_FLAG_NONE);

        if (S_OK != hr) {
            printf("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
        }

        if (numChars != charBufferSize) {
            printf("buffer size and buffer size actual size mismatch\r\n");
        }
    }


    //Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
    //descriptors are required as we descriptors that are notification based will have to be written
    //once IsSubcribeToNotification set to true, we set the appropriate callback function
    //need for setting descriptors for notification according to
    //http://social.msdn.microsoft.com/Forums/en-US/11d3a7ce-182b-4190-bf9d-64fefc3328d9/windows-bluetooth-le-apis-event-callbacks?forum=wdk
    PBTH_LE_GATT_CHARACTERISTIC currGattChar;
    for (int ii = 0; ii <charBufferSize; ii++) {
        currGattChar = &pCharBuffer[ii];
        USHORT charValueDataSize;
        PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;


        ///////////////////////////////////////////////////////////////////////////
        // Determine Descriptor Buffer Size
        ////////////////////////////////////////////////////////////////////////////
        USHORT descriptorBufferSize;
        hr = BluetoothGATTGetDescriptors(
            hLEDeviceWesu[Index],
            currGattChar,
            0,
            NULL,
            &descriptorBufferSize,
            BLUETOOTH_GATT_FLAG_NONE);

        if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
            printf("BluetoothGATTGetDescriptors - Buffer Size %d", hr);
        }

        PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
        if (descriptorBufferSize > 0) {
            pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
                malloc(descriptorBufferSize
                    * sizeof(BTH_LE_GATT_DESCRIPTOR));

            if (NULL == pDescriptorBuffer) {
                printf("pDescriptorBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve Descriptors
            ////////////////////////////////////////////////////////////////////////////

            USHORT numDescriptors;
            hr = BluetoothGATTGetDescriptors(
                hLEDeviceWesu[Index],
                currGattChar,
                descriptorBufferSize,
                pDescriptorBuffer,
                &numDescriptors,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetDescriptors - Actual Data %d", hr);
            }

            if (numDescriptors != descriptorBufferSize) {
                printf("buffer size and buffer size actual size mismatch\r\n");
            }

            for (int kk = 0; kk<numDescriptors; kk++) {
                PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[kk];
                ////////////////////////////////////////////////////////////////////////////
                // Determine Descriptor Value Buffer Size
                ////////////////////////////////////////////////////////////////////////////
                USHORT descValueDataSize;
                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceWesu[Index],
                    currGattDescriptor,
                    0,
                    NULL,
                    &descValueDataSize,
                    BLUETOOTH_GATT_FLAG_NONE);

                if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
                }

                PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

                if (NULL == pDescValueBuffer) {
                    printf("pDescValueBuffer out of memory\r\n");
                }
                else {
                    RtlZeroMemory(pDescValueBuffer, descValueDataSize);
                }

                ////////////////////////////////////////////////////////////////////////////
                // Retrieve the Descriptor Value
                ////////////////////////////////////////////////////////////////////////////

                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceWesu[Index],
                    currGattDescriptor,
                    (ULONG)descValueDataSize,
                    pDescValueBuffer,
                    NULL,
                    BLUETOOTH_GATT_FLAG_NONE);
                if (S_OK != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
                }
                //you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
                // we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
                if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x0080))
                {
                    BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

                    RtlZeroMemory(&newValue, sizeof(newValue));

                    newValue.DescriptorType = ClientCharacteristicConfiguration;
                    newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

                    hr = BluetoothGATTSetDescriptorValue(
                        hLEDeviceWesu[Index],
                        currGattDescriptor,
                        &newValue,
                        BLUETOOTH_GATT_FLAG_NONE);
                    if (S_OK != hr) {
                        printf("BluetoothGATTGetDescriptorValue - Actual Data %d\n", hr);
                    }
                    else {
                        printf("setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
                    }

                }

            }
        }


        if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x0080))
        {
            printf("Setting Notification for ServiceHandle %d --- %X \n", currGattChar->ServiceHandle, currGattChar->CharacteristicUuid.Value.ShortUuid);
            BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;

            BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
            EventParameterIn.Characteristics[0] = *currGattChar;
            EventParameterIn.NumCharacteristics = 1;
            CallBackValueWesu[Index] = Index;

            hr = BluetoothGATTRegisterEvent(
                hLEDeviceWesu[Index],
                EventType,
                &EventParameterIn,
                SomethingHappenedWesu,
                &CallBackValueWesu[Index],
                &EventHandleWesu[Index],
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTRegisterEvent - Actual Data %d", hr);
            }
        }


        if (currGattChar->IsReadable) {//currGattChar->IsReadable
                                       ////////////////////////////////////////////////////////////////////////////
                                       // Determine Characteristic Value Buffer Size
                                       ////////////////////////////////////////////////////////////////////////////
            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceWesu[Index],
                currGattChar,
                0,
                NULL,
                &charValueDataSize,
                BLUETOOTH_GATT_FLAG_NONE);

            if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
            }

            pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

            if (NULL == pCharValueBuffer) {
                printf("pCharValueBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pCharValueBuffer, charValueDataSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve the Characteristic Value
            ////////////////////////////////////////////////////////////////////////////

            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceWesu[Index],
                currGattChar,
                (ULONG)charValueDataSize,
                pCharValueBuffer,
                NULL,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
            }

            //print the characeteristic Value
            //for an HR monitor this might be the body sensor location
            printf("\n Printing a read (not notifiable) characterstic (maybe) body sensor value");
            for (int iii = 0; iii< pCharValueBuffer->DataSize; iii++) {// ideally check ->DataSize before printing
                printf("%d", pCharValueBuffer->Data[iii]);
            }
            printf("\n");

            // Free before going to next iteration, or memory leak.
            free(pCharValueBuffer);
            pCharValueBuffer = NULL;
        }

    }

}

void OpenBluetoothMotEnv(int Index)
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_MOTENV_DEVICE_UUID), &AGuidMotEnv);

    //now get the handle 
    hLEDeviceMotEnv[Index] = GetBLEHandleOffset(AGuidMotEnv,Index);

    //
    TickCount64MotEnv[Index] = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceMotEnv[Index],
        0,
        NULL,
        &serviceBufferCount,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
        malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);

    if (NULL == pServiceBuffer) {
        printf("pServiceBuffer out of memory\r\n");
    }
    else {
        RtlZeroMemory(pServiceBuffer,
            sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Retrieve Services
    ////////////////////////////////////////////////////////////////////////////

    USHORT numServices;
    hr = BluetoothGATTGetServices(
        hLEDeviceMotEnv[Index],
        serviceBufferCount,
        pServiceBuffer,
        &numServices,
        BLUETOOTH_GATT_FLAG_NONE);

    if (S_OK != hr) {
        printf("BluetoothGATTGetServices - Buffer Size %d", hr);
    }


    //Step 3: now get the list of charactersitics. note how the pServiceBuffer is required from step 2
    ////////////////////////////////////////////////////////////////////////////
    // Determine Characteristic Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    USHORT charBufferSize;
    hr = BluetoothGATTGetCharacteristics(
        hLEDeviceMotEnv[Index],
        pServiceBuffer,
        0,
        NULL,
        &charBufferSize,
        BLUETOOTH_GATT_FLAG_NONE);

    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
        printf("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
    }

    PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = NULL;
    if (charBufferSize > 0) {
        pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
            malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));

        if (NULL == pCharBuffer) {
            printf("pCharBuffer out of memory\r\n");
        }
        else {
            RtlZeroMemory(pCharBuffer,
                charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
        }

        ////////////////////////////////////////////////////////////////////////////
        // Retrieve Characteristics
        ////////////////////////////////////////////////////////////////////////////
        USHORT numChars;
        hr = BluetoothGATTGetCharacteristics(
            hLEDeviceMotEnv[Index],
            pServiceBuffer,
            charBufferSize,
            pCharBuffer,
            &numChars,
            BLUETOOTH_GATT_FLAG_NONE);

        if (S_OK != hr) {
            printf("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
        }

        if (numChars != charBufferSize) {
            printf("buffer size and buffer size actual size mismatch\r\n");
        }
    }


    //Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
    //descriptors are required as we descriptors that are notification based will have to be written
    //once IsSubcribeToNotification set to true, we set the appropriate callback function
    //need for setting descriptors for notification according to
    //http://social.msdn.microsoft.com/Forums/en-US/11d3a7ce-182b-4190-bf9d-64fefc3328d9/windows-bluetooth-le-apis-event-callbacks?forum=wdk
    PBTH_LE_GATT_CHARACTERISTIC currGattChar;
    for (int ii = 0; ii <charBufferSize; ii++) {
        currGattChar = &pCharBuffer[ii];
        USHORT charValueDataSize;
        PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;


        ///////////////////////////////////////////////////////////////////////////
        // Determine Descriptor Buffer Size
        ////////////////////////////////////////////////////////////////////////////
        USHORT descriptorBufferSize;
        hr = BluetoothGATTGetDescriptors(
            hLEDeviceMotEnv[Index],
            currGattChar,
            0,
            NULL,
            &descriptorBufferSize,
            BLUETOOTH_GATT_FLAG_NONE);

        if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
            printf("BluetoothGATTGetDescriptors - Buffer Size %d", hr);
        }

        PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
        if (descriptorBufferSize > 0) {
            pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
                malloc(descriptorBufferSize
                    * sizeof(BTH_LE_GATT_DESCRIPTOR));

            if (NULL == pDescriptorBuffer) {
                printf("pDescriptorBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve Descriptors
            ////////////////////////////////////////////////////////////////////////////

            USHORT numDescriptors;
            hr = BluetoothGATTGetDescriptors(
                hLEDeviceMotEnv[Index],
                currGattChar,
                descriptorBufferSize,
                pDescriptorBuffer,
                &numDescriptors,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetDescriptors - Actual Data %d", hr);
            }

            if (numDescriptors != descriptorBufferSize) {
                printf("buffer size and buffer size actual size mismatch\r\n");
            }

            for (int kk = 0; kk<numDescriptors; kk++) {
                PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[kk];
                ////////////////////////////////////////////////////////////////////////////
                // Determine Descriptor Value Buffer Size
                ////////////////////////////////////////////////////////////////////////////
                USHORT descValueDataSize;
                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceMotEnv[Index],
                    currGattDescriptor,
                    0,
                    NULL,
                    &descValueDataSize,
                    BLUETOOTH_GATT_FLAG_NONE);

                if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
                }

                PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

                if (NULL == pDescValueBuffer) {
                    printf("pDescValueBuffer out of memory\r\n");
                }
                else {
                    RtlZeroMemory(pDescValueBuffer, descValueDataSize);
                }

                ////////////////////////////////////////////////////////////////////////////
                // Retrieve the Descriptor Value
                ////////////////////////////////////////////////////////////////////////////

                hr = BluetoothGATTGetDescriptorValue(
                    hLEDeviceMotEnv[Index],
                    currGattDescriptor,
                    (ULONG)descValueDataSize,
                    pDescValueBuffer,
                    NULL,
                    BLUETOOTH_GATT_FLAG_NONE);
                if (S_OK != hr) {
                    printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
                }
                //you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
                // we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
                if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x0100))
                {
                    BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

                    RtlZeroMemory(&newValue, sizeof(newValue));

                    newValue.DescriptorType = ClientCharacteristicConfiguration;
                    newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

                    hr = BluetoothGATTSetDescriptorValue(
                        hLEDeviceMotEnv[Index],
                        currGattDescriptor,
                        &newValue,
                        BLUETOOTH_GATT_FLAG_NONE);
                    if (S_OK != hr) {
                        printf("BluetoothGATTGetDescriptorValue - Actual Data %d\n", hr);
                    }
                    else {
                        printf("setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
                    }

                }

            }
        }


        if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x0100))
        {
            printf("Setting Notification for ServiceHandle %d --- %X \n", currGattChar->ServiceHandle, currGattChar->CharacteristicUuid.Value.ShortUuid);
            BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;

            BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
            EventParameterIn.Characteristics[0] = *currGattChar;
            EventParameterIn.NumCharacteristics = 1;
            CallBackValueEnv[Index] = Index;
            hr = BluetoothGATTRegisterEvent(
                hLEDeviceMotEnv[Index],
                EventType,
                &EventParameterIn,
                SomethingHappenedMotEnv,
                &CallBackValueEnv[Index],
                &EventHandleMotEnv[Index],
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTRegisterEvent - Actual Data %d", hr);
            }
        }


        if (currGattChar->IsReadable) {//currGattChar->IsReadable
                                       ////////////////////////////////////////////////////////////////////////////
                                       // Determine Characteristic Value Buffer Size
                                       ////////////////////////////////////////////////////////////////////////////
            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceMotEnv[Index],
                currGattChar,
                0,
                NULL,
                &charValueDataSize,
                BLUETOOTH_GATT_FLAG_NONE);

            if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
            }

            pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

            if (NULL == pCharValueBuffer) {
                printf("pCharValueBuffer out of memory\r\n");
            }
            else {
                RtlZeroMemory(pCharValueBuffer, charValueDataSize);
            }

            ////////////////////////////////////////////////////////////////////////////
            // Retrieve the Characteristic Value
            ////////////////////////////////////////////////////////////////////////////

            hr = BluetoothGATTGetCharacteristicValue(
                hLEDeviceMotEnv[Index],
                currGattChar,
                (ULONG)charValueDataSize,
                pCharValueBuffer,
                NULL,
                BLUETOOTH_GATT_FLAG_NONE);

            if (S_OK != hr) {
                printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
            }

            //print the characeteristic Value
            //for an HR monitor this might be the body sensor location
            printf("\n Printing a read (not notifiable) characterstic (maybe) body sensor value");
            for (int iii = 0; iii< pCharValueBuffer->DataSize; iii++) {// ideally check ->DataSize before printing
                printf("%d", pCharValueBuffer->Data[iii]);
            }
            printf("\n");

            // Free before going to next iteration, or memory leak.
            free(pCharValueBuffer);
            pCharValueBuffer = NULL;
        }

    }

}
