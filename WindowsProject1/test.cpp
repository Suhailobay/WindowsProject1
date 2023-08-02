#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <devicetopology.h>
#include <audioclient.h> 
int main() {
    CoInitialize(nullptr);

    // Create a device enumerator and enumerate audio endpoints
    IMMDeviceEnumerator* pEnumerator = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        CoUninitialize();
        return ;
    }

    IMMDeviceCollection* pDeviceCollection = nullptr;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDeviceCollection);
    if (FAILED(hr)) {
        pEnumerator->Release();
        CoUninitialize();
        return ;
    }

    UINT deviceCount = 0;
    hr = pDeviceCollection->GetCount(&deviceCount);
    if (FAILED(hr)) {
        pDeviceCollection->Release();
        pEnumerator->Release();
        CoUninitialize();
        return ;
    }



   // Iterate through the device collection and query device properties
    for (UINT i = 0; i < deviceCount; i++) {
        IMMDevice* pDevice = nullptr;
        hr = pDeviceCollection->Item(i, &pDevice);
        if (FAILED(hr)) {
            continue;
        }

        IMMEndpoint* pEndpoint = nullptr;
        hr = pDevice->QueryInterface(__uuidof(IMMEndpoint), (void**)&pEndpoint);
        if (FAILED(hr)) {
            pDevice->Release();
            continue;
        }      
    }


    // Activate the chosen device specific interfaces 
    IMMDevice* pChosenDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;
    hr = pChosenDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);
    if (FAILED(hr)) {
        CoUninitialize();
        return ;
    }
    CoUninitialize();
    return 0;
}
