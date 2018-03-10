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


ULONGLONG TickCount64Thingy = 0;
ULONGLONG TickCount64Wesu   = 0;
ULONGLONG TickCount64MotEnv = 0;

//Step 1: find the BLE device handle from its GUID
GUID AGuidThingy;
GUID AGuidWesu;
GUID AGuidMotEnv;






HANDLE hLEDeviceThingy=NULL;
BLUETOOTH_GATT_EVENT_HANDLE EventHandleThingy=NULL;

HANDLE hLEDeviceWesu = NULL;
BLUETOOTH_GATT_EVENT_HANDLE EventHandleWesu = NULL;

HANDLE hLEDeviceMotEnv = NULL;
BLUETOOTH_GATT_EVENT_HANDLE EventHandleMotEnv = NULL;

// Thingy  
float QwThingy=0, QxThingy=0, QyThingy=0, QzThingy=0;
// Wesu
float QwWesu= 0, QxWesu = 0, QyWesu = 0, QzWesu = 0;
// MOTENV
float QwMotEnv = 0, QxMotEnv = 0, QyMotEnv = 0, QzMotEnv = 0;


//
void OpenBluetoothThingy();
void CloseBluetoothThingy();
//
void OpenBluetoothWesu();
void CloseBluetoothWesu();
//
void OpenBluetoothMotEnv();
void CloseBluetoothMotEnv();
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
        QwThingy = (float)(iQwB) / SCALE_FACTOR;
        QxThingy = (float)(iQxB) / SCALE_FACTOR;
        QyThingy = (float)(iQyB) / SCALE_FACTOR;
        QzThingy = (float)(iQzB) / SCALE_FACTOR;
        //
        printf("%X,%X,%X,%X\n", iQwB, iQxB, iQyB, iQzB);
        printf("%f,%f,%f,%f\n", QwThingy, QxThingy, QyThingy, QzThingy);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Thingy, NewTickCount64);
        //
        TickCount64Thingy = NewTickCount64;
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

        memcpy(&QxWesu, &QiB, sizeof QxWesu);

        QjB[0] = ValueChangedEventParameters->CharacteristicValue->Data[6];
        QjB[1] = ValueChangedEventParameters->CharacteristicValue->Data[7];
        QjB[2] = ValueChangedEventParameters->CharacteristicValue->Data[8];
        QjB[3] = ValueChangedEventParameters->CharacteristicValue->Data[9];

        memcpy(&QyWesu, &QjB, sizeof QyWesu);

        QkB[0] = ValueChangedEventParameters->CharacteristicValue->Data[10];
        QkB[1] = ValueChangedEventParameters->CharacteristicValue->Data[11];
        QkB[2] = ValueChangedEventParameters->CharacteristicValue->Data[12];
        QkB[3] = ValueChangedEventParameters->CharacteristicValue->Data[13];
 
        memcpy(&QzWesu, &QkB, sizeof QzWesu);
        
        QsB[0] = ValueChangedEventParameters->CharacteristicValue->Data[14];
        QsB[1] = ValueChangedEventParameters->CharacteristicValue->Data[15];
        QsB[2] = ValueChangedEventParameters->CharacteristicValue->Data[16];
        QsB[3] = ValueChangedEventParameters->CharacteristicValue->Data[17];

        memcpy(&QwWesu, &QsB, sizeof QwWesu);

        //
        printf("%f,%f,%f,%f\n", QxWesu, QyWesu, QzWesu, QwWesu);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Wesu, NewTickCount64);
        //
        TickCount64Wesu = NewTickCount64;
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

        QxMotEnv = (float)(iQxB) / SCALE_FACTOR;
        QyMotEnv = (float)(iQyB) / SCALE_FACTOR;
        QzMotEnv = (float)(iQzB) / SCALE_FACTOR;
        QwMotEnv = getQs(QxMotEnv, QyMotEnv, QzMotEnv);


        //
        printf("%f,%f,%f,%f\n", QxMotEnv, QyMotEnv, QzMotEnv, QwMotEnv);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Wesu, NewTickCount64);
        //
        TickCount64Wesu = NewTickCount64;
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

extern "C" float __declspec(dllexport) _stdcall GetFloatWThingy()
{
    return QwThingy;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXThingy()
{
	return QxThingy;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYThingy()
{
    return QyThingy;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZThingy()
{
    return QzThingy;
}




extern "C" float __declspec(dllexport) _stdcall GetFloatWWesu()
{
    return QwWesu;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXWesu()
{
    return QxWesu;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYWesu()
{
    return QyWesu;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZWesu()
{
    return QzWesu;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatXMotEnv()
{
    return QxMotEnv;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatYMotEnv()
{
    return QyMotEnv;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZMotEnv()
{
    return QzMotEnv;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatWMotEnv()
{
    return QwMotEnv;
}


extern "C" void __declspec(dllexport) _stdcall OpenBleThingy()
{
    // Ouverture de la connexion
    OpenBluetoothThingy();
}

extern "C" void __declspec(dllexport) _stdcall CloseBleThingy()
{
    // Fermeture de la connexion
    CloseBluetoothThingy();
}

extern "C" void __declspec(dllexport) _stdcall OpenBleWesu()
{
    // Ouverture de la connexion
    OpenBluetoothWesu();
}

extern "C" void __declspec(dllexport) _stdcall CloseBleWesu()
{
    // Fermeture de la connexion
    CloseBluetoothWesu();
}

extern "C" void __declspec(dllexport) _stdcall OpenBleMotEnv()
{
    // Ouverture de la connexion
    OpenBluetoothMotEnv();
}

extern "C" void __declspec(dllexport) _stdcall CloseBleMotEnv()
{
    // Fermeture de la connexion
    CloseBluetoothMotEnv();
}


//
void CloseBluetoothThingy()
{
    BluetoothGATTUnregisterEvent(EventHandleThingy, BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceThingy);
}

void CloseBluetoothWesu()
{
    BluetoothGATTUnregisterEvent(EventHandleWesu, BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceWesu);
}

void CloseBluetoothMotEnv()
{
    BluetoothGATTUnregisterEvent(EventHandleMotEnv, BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDeviceMotEnv);
}

void OpenBluetoothThingy()
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID), &AGuidThingy);

    //now get the handle 
    hLEDeviceThingy = GetBLEHandle(AGuidThingy);

    //
    TickCount64Thingy = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceThingy,
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
        hLEDeviceThingy,
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
        hLEDeviceThingy,
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
            hLEDeviceThingy,
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
            hLEDeviceThingy,
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
                hLEDeviceThingy,
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
                    hLEDeviceThingy,
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
                    hLEDeviceThingy,
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
                        hLEDeviceThingy,
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
            hr = BluetoothGATTRegisterEvent(
                hLEDeviceThingy,
                EventType,
                &EventParameterIn,
                SomethingHappenedThingy,
                NULL,
                &EventHandleThingy,
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
                hLEDeviceThingy,
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
                hLEDeviceThingy,
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
void OpenBluetoothWesu()
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_WESU_DEVICE_UUID), &AGuidWesu);

    //now get the handle 
    hLEDeviceWesu = GetBLEHandle(AGuidWesu);

    //
    TickCount64Wesu = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceWesu,
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
        hLEDeviceWesu,
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
        hLEDeviceWesu,
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
            hLEDeviceWesu,
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
            hLEDeviceWesu,
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
                hLEDeviceWesu,
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
                    hLEDeviceWesu,
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
                    hLEDeviceWesu,
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
                        hLEDeviceWesu,
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
            hr = BluetoothGATTRegisterEvent(
                hLEDeviceWesu,
                EventType,
                &EventParameterIn,
                SomethingHappenedWesu,
                NULL,
                &EventHandleWesu,
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
                hLEDeviceWesu,
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
                hLEDeviceWesu,
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

void OpenBluetoothMotEnv()
{
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_MOTENV_DEVICE_UUID), &AGuidMotEnv);

    //now get the handle 
    hLEDeviceMotEnv = GetBLEHandle(AGuidMotEnv);

    //
    TickCount64MotEnv = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDeviceMotEnv,
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
        hLEDeviceMotEnv,
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
        hLEDeviceMotEnv,
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
            hLEDeviceMotEnv,
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
            hLEDeviceMotEnv,
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
                hLEDeviceMotEnv,
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
                    hLEDeviceMotEnv,
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
                    hLEDeviceMotEnv,
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
                        hLEDeviceMotEnv,
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
            hr = BluetoothGATTRegisterEvent(
                hLEDeviceMotEnv,
                EventType,
                &EventParameterIn,
                SomethingHappenedMotEnv,
                NULL,
                &EventHandleMotEnv,
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
                hLEDeviceMotEnv,
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
                hLEDeviceMotEnv,
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
