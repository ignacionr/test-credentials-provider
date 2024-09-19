#include "SimpleCredential.hpp"

#include <NTSecAPI.h>

enum FIELD_ID
{
    FI_USERNAME = 0,
    FI_PASSWORD = 1,
    FI_SUBMIT_BUTTON = 2,
    FI_NUM_FIELDS = 3, // Total number of fields
};

CREDENTIAL_PROVIDER_FIELD_DESCRIPTOR g_FieldDescriptors[] =
{
    { FI_USERNAME, CPFT_EDIT_TEXT, L"Username" },
    { FI_PASSWORD, CPFT_PASSWORD_TEXT, L"Password" },
    { FI_SUBMIT_BUTTON, CPFT_SUBMIT_BUTTON, L"Submit" },
};

HRESULT SimpleCredential::GetFieldState(
    DWORD dwFieldID,
    CREDENTIAL_PROVIDER_FIELD_STATE* pcpfs,
    CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE* pcpfis)
{
    if (dwFieldID >= FI_NUM_FIELDS)
        return E_INVALIDARG;

    *pcpfs = CPFS_DISPLAY_IN_SELECTED_TILE;
    *pcpfis = CPFIS_NONE;

    return S_OK;
}

HRESULT SimpleCredential::GetStringValue(DWORD dwFieldID, PWSTR* ppwsz)
{
    if (dwFieldID == FI_USERNAME)
    {
        *ppwsz = _pszUsername ? _wcsdup(_pszUsername) : _wcsdup(L"");
    }
    else if (dwFieldID == FI_PASSWORD)
    {
        *ppwsz = _pszPassword ? _wcsdup(_pszPassword) : _wcsdup(L"");
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT SimpleCredential::SetStringValue(DWORD dwFieldID, PCWSTR pwz)
{
    if (dwFieldID == FI_USERNAME)
    {
        if (_pszUsername)
        {
            CoTaskMemFree(_pszUsername);
        }
        _pszUsername = _wcsdup(pwz);
    }
    else if (dwFieldID == FI_PASSWORD)
    {
        if (_pszPassword)
        {
            CoTaskMemFree(_pszPassword);
        }
        _pszPassword = _wcsdup(pwz);
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

HRESULT SimpleCredential::GetSerialization(
    CREDENTIAL_PROVIDER_GET_SERIALIZATION_RESPONSE* pcpgsr,
    CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcs,
    PWSTR* ppwszOptionalStatusText,
    CREDENTIAL_PROVIDER_STATUS_ICON* pcpsiOptionalStatusIcon)
{
    if (!_pszUsername || !_pszPassword)
    {
        *pcpgsr = CPGSR_NO_CREDENTIAL_NOT_FINISHED;
        return E_UNEXPECTED;
    }

    KERB_INTERACTIVE_UNLOCK_LOGON kiul;
    ZeroMemory(&kiul, sizeof(kiul));

    kiul.Logon.MessageType = KerbInteractiveLogon;
    kiul.Logon.UserName.MaximumLength = sizeof(_)
    kiul.Logon.UserName.Buffer = _pszUsername;
    kiul.Logon.Password = _pszPassword;
    kiul.Logon.LogonDomainName = L"";

    // Serialize the structure
    HRESULT hr = KerbInteractiveUnlockLogonPack(
        &kiul,
        &pcpcs->rgbSerialization,
        &pcpcs->cbSerialization
    );

    if (SUCCEEDED(hr))
    {
        pcpcs->ulAuthenticationPackage = GetAuthPackage();
        pcpcs->clsidCredentialProvider = CLSID_SimpleCredentialProvider;
        *pcpgsr = CPGSR_RETURN_CREDENTIAL_FINISHED;
    }

    return hr;
}

