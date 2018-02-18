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
#include <inttypes.h>
#include <objbase.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
// #define TO_SEARCH_DEVICE_UUID "{0000180D-0000-1000-8000-00805F9B34FB}" //we use UUID for an HR BLE device 
#define TO_SEARCH_DEVICE_UUID "{EF680400-9B35-4933-9B10-52FFA9740042}" //


ULONGLONG TickCount64 = 0;
HANDLE hLEDevice=NULL;
BLUETOOTH_GATT_EVENT_HANDLE EventHandle=NULL;
// 
float Qw=0, Qx=0, Qy=0, Qz=0;

void OpenBluetooth();
void CloseBluetooth();



//this is the notification function
//the way ValueChangedEventParameters is utilized is shown in
//HealthHeartRateService::HeartRateMeasurementEvent()
//a function in Windows Driver Kit (WDK) 8.0 Samples.zip\C++\WDK 8.0 Samples\Bluetooth Low Energy (LE) Generic Attribute (GATT) Profile Drivers\Solution\WpdHealthHeartRate\HealthHeartRateService.cpp
void CALLBACK SomethingHappened(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
    float SCALE_FACTOR = 1073741824.0F;
    // float SCALE_FACTOR = 65536.0F;


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
        printf("HR ");
        //for(int i=0; i<ValueChangedEventParameters->CharacteristicValue->DataSize;i++) {
        //	printf("%0x",ValueChangedEventParameters->CharacteristicValue->Data[i]);
        //}
        // if the first bit is set, then the value is the next 2 bytes.  If it is clear, the value is in the next byte
        //The Heart Rate Value Format bit (bit 0 of the Flags field) indicates if the data format of 
        //the Heart Rate Measurement Value field is in a format of UINT8 or UINT16. 
        //When the Heart Rate Value format is sent in a UINT8 format, the Heart Rate Value 
        //Format bit shall be set to 0. When the Heart Rate Value format is sent in a UINT16 
        //format, the Heart Rate Value Format bit shall be set to 1
        //from this PDF https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=239866
        /*unsigned heart_rate;
        if (0x01 == (ValueChangedEventParameters->CharacteristicValue->Data[0] & 0x01)) {
        heart_rate = ValueChangedEventParameters->CharacteristicValue->Data[1] * 256 + ValueChangedEventParameters->CharacteristicValue->Data[2];
        }
        else {
        heart_rate = ValueChangedEventParameters->CharacteristicValue->Data[1];
        }
        printf("%d\n", heart_rate);*/

        QwB[0] = ValueChangedEventParameters->CharacteristicValue->Data[0];
        QwB[1] = ValueChangedEventParameters->CharacteristicValue->Data[1];
        QwB[2] = ValueChangedEventParameters->CharacteristicValue->Data[2];
        QwB[3] = ValueChangedEventParameters->CharacteristicValue->Data[3];

        // qs = (float)(BitConverter.ToInt32(QsB, 0)) / SCALE_FACTOR;
        int iQwB = ((QwB[3] << 24) & 0xff000000) |
            ((QwB[2] << 16) & 0x00ff0000) |
            ((QwB[1] << 8) & 0x0000ff00) |
            ((QwB[0] << 0) & 0x000000ff);


        QxB[0] = ValueChangedEventParameters->CharacteristicValue->Data[4];
        QxB[1] = ValueChangedEventParameters->CharacteristicValue->Data[5];
        QxB[2] = ValueChangedEventParameters->CharacteristicValue->Data[6];
        QxB[3] = ValueChangedEventParameters->CharacteristicValue->Data[7];

        // qs = (float)(BitConverter.ToInt32(QsB, 0)) / SCALE_FACTOR;
        int iQxB = ((QxB[3] << 24) & 0xff000000) |
            ((QxB[2] << 16) & 0x00ff0000) |
            ((QxB[1] << 8) & 0x0000ff00) |
            ((QxB[0] << 0) & 0x000000ff);

        QyB[0] = ValueChangedEventParameters->CharacteristicValue->Data[8];
        QyB[1] = ValueChangedEventParameters->CharacteristicValue->Data[9];
        QyB[2] = ValueChangedEventParameters->CharacteristicValue->Data[10];
        QyB[3] = ValueChangedEventParameters->CharacteristicValue->Data[11];
        //
        int iQyB = ((QyB[3] << 24) & 0xff000000) |
            ((QyB[2] << 16) & 0x00ff0000) |
            ((QyB[1] << 8) & 0x0000ff00) |
            ((QyB[0] << 0) & 0x000000ff);


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
        Qw = (float)(iQwB) / SCALE_FACTOR;
        Qx = (float)(iQxB) / SCALE_FACTOR;
        Qy = (float)(iQyB) / SCALE_FACTOR;
        Qz = (float)(iQzB) / SCALE_FACTOR;
        //
        printf("%X,%X,%X,%X\n", iQwB, iQxB, iQyB, iQzB);
        printf("%f,%f,%f,%f\n", Qw, Qx, Qy, Qz);
        //
        ULONGLONG NewTickCount64 = GetTickCount64();
        //
        printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64, NewTickCount64);
        //
        TickCount64 = NewTickCount64;
    }
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

extern "C" float __declspec(dllexport) _stdcall GetFloatW()
{
    return Qw;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatX()
{
	return Qx;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatY()
{
    return Qy;
}

extern "C" float __declspec(dllexport) _stdcall GetFloatZ()
{
    return Qz;
}

extern "C" void __declspec(dllexport) _stdcall OpenBle()
{
    // Ouverture de la connexion
    OpenBluetooth();
}

extern "C" void __declspec(dllexport) _stdcall CloseBle()
{
    // Fermeture de la connexion
    CloseBluetooth();
}


void CloseBluetooth()
{
    BluetoothGATTUnregisterEvent(EventHandle, BLUETOOTH_GATT_FLAG_NONE);
    CloseHandle(hLEDevice);
}

void OpenBluetooth()
{
    //Step 1: find the BLE device handle from its GUID
    GUID AGuid;
    //GUID can be constructed from "{xxx....}" string using CLSID
    CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID), &AGuid);

    //now get the handle 
    hLEDevice = GetBLEHandle(AGuid);

    //
    TickCount64 = GetTickCount64();

    //Step 2: Get a list of services that the device advertises
    // first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
    // services in serviceBufferCount
    USHORT serviceBufferCount;
    ////////////////////////////////////////////////////////////////////////////
    // Determine Services Buffer Size
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = BluetoothGATTGetServices(
        hLEDevice,
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
        hLEDevice,
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
        hLEDevice,
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
            hLEDevice,
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
            hLEDevice,
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
                hLEDevice,
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
                    hLEDevice,
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
                    hLEDevice,
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
                        hLEDevice,
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
                hLEDevice,
                EventType,
                &EventParameterIn,
                SomethingHappened,
                NULL,
                &EventHandle,
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
                hLEDevice,
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
                hLEDevice,
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
