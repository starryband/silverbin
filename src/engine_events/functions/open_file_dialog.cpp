#include "engine_events/functions/open_file_dialog.h"

#include <windows.h>
#include <shobjidl.h>

namespace FileDialogs
{
    std::wstring OpenFileDialog()
    {
        std::wstring result;

        HRESULT hr = CoInitializeEx(NULL,
                                    COINIT_APARTMENTTHREADED |
                                    COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* dialog;

            hr = CoCreateInstance(
                CLSID_FileOpenDialog,
                NULL,
                CLSCTX_ALL,
                IID_IFileOpenDialog,
                reinterpret_cast<void**>(&dialog));

            if (SUCCEEDED(hr))
            {
                COMDLG_FILTERSPEC filters[] = {
                    {L"3D Models", L"*.obj;*.fbx;*.glTF"},
                    {L"OBJ Files", L"*.obj"},
                    {L"FBX Files", L"*.fbx"},
                    {L"glTF Files", L"*.glTF"}
                };

                dialog->SetFileTypes(4, filters);
                dialog->SetFileTypeIndex(1);

                hr = dialog->Show(NULL);

                if (SUCCEEDED(hr))
                {
                    IShellItem* item;

                    hr = dialog->GetResult(&item);

                    if (SUCCEEDED(hr))
                    {
                        PWSTR path = NULL;

                        hr = item->GetDisplayName(
                            SIGDN_FILESYSPATH,
                            &path);

                        if (SUCCEEDED(hr))
                        {
                            result = path;
                            CoTaskMemFree(path);
                        }

                        item->Release();
                    }
                }

                dialog->Release();
            }

            CoUninitialize();
        }

        return result;
    }
}