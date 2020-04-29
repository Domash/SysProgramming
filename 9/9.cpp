// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey, LPCWSTR recursiveInfo)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode;

    TCHAR  achValue[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 

    // Enumerate the subkeys, until RegEnumKeyEx fails.

    TCHAR outputPath[1024];
    int numberOfTabs = 0;
    if (lstrlen(recursiveInfo) == 0)
        _tprintf(L"\nHKEY_CURRENT_CONFIG:\n");
    else {
        printf("\n");
        _stprintf(outputPath, L"HKEY_CURRENT_CONFIG\\%s:\n", recursiveInfo);
        for (int i = 0; outputPath[i] != '\0'; i++) {
            if (outputPath[i] == '\\')
                numberOfTabs++;
        }
        for (int _ = 0; _ < numberOfTabs; _++)
            _tprintf(L"\t");
        _tprintf(L"HKEY_CURRENT_CONFIG\\%s:\n", recursiveInfo);

    }
    if (cSubKeys)
    {
        printf("\n");
        for (int _ = 0; _ < numberOfTabs; _++)
            _tprintf(L"\t");
        printf("Number of subkeys: %d\n", cSubKeys);

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {
                for (int _ = 0; _ < numberOfTabs; _++)
                    _tprintf(L"\t");
                _tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
            }
        }
    }

    // Enumerate the key values. 

    if (cValues)
    {
        printf("\n");
        for (int _ = 0; _ < numberOfTabs; _++)
            _tprintf(L"\t");
        printf("Number of values: %d\n", cValues);

        for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
        {
            cchValue = MAX_VALUE_NAME;
            achValue[0] = '\0';
            retCode = RegEnumValue(hKey, i,
                achValue,
                &cchValue,
                NULL,
                NULL,
                NULL,
                NULL);

            if (retCode == ERROR_SUCCESS)
            {
                for (int _ = 0; _ < numberOfTabs; _++)
                    _tprintf(L"\t");
                _tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
            }
        }
    }

    if (cSubKeys) {
        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {
                HKEY hTestKey;

                TCHAR recursivePath[1024];
                int len = lstrlen(recursiveInfo);
                if (len == 0) {
                    _stprintf(recursivePath, L"%s", achKey);
                }
                else {
                    _stprintf(recursivePath, L"%s\\%s", recursiveInfo, achKey);
                }

                if (RegOpenKeyEx(HKEY_CURRENT_CONFIG,
                    recursivePath,
                    0,
                    KEY_READ,
                    &hTestKey) == ERROR_SUCCESS
                    )
                {
                    QueryKey(hTestKey, recursivePath);
                }

                RegCloseKey(hTestKey);
            }
        }
    }

}

void __cdecl _tmain(void)
{
    HKEY hTestKey;

    if (RegOpenKeyEx(HKEY_CURRENT_CONFIG,
        TEXT(""),
        0,
        KEY_READ,
        &hTestKey) == ERROR_SUCCESS
        )
    {
        QueryKey(hTestKey, L"");
    }

    RegCloseKey(hTestKey);
}