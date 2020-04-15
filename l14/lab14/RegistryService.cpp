#include "stdafx.h"
#include "RegistryService.h"
#include <iostream>
#include <algorithm>



void RegistryService::searchInKey(HKEY hRootKey, std::wstring path) {
	TCHAR keyName[4096];
	DWORD subkeysCount = 0;
	HKEY hKey;
	FILETIME ftLastWriteTime;
	int retCode;

	if (RegOpenKeyEx(hRootKey, path.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return;
	};

	retCode = RegQueryInfoKey(hKey, NULL, NULL,
		NULL, &subkeysCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	for (int i = 0; i < subkeysCount; i++) {
		DWORD keySize = sizeof(keyName);
		retCode = RegEnumKeyEx(hKey, i, keyName, &keySize, NULL, NULL, NULL, &ftLastWriteTime);

		if (retCode == ERROR_SUCCESS) {
			std::wstring relativePath = path + keyName + L"\\";
			std::wstring keyFullPath = this->getKeyPath(hRootKey) + relativePath;

			if (isMatch(keyFullPath)) {
				this->addToList(this->resultList, keyFullPath);
			}

			deep++;
			this->searchInKey(hRootKey, relativePath);
			deep--;
		}
	}
}

std::wstring RegistryService::getKeyPath(HKEY hRootKey) {
	if (hRootKey == HKEY_CLASSES_ROOT) {
		return L"HKCR\\";
	} else if (hRootKey == HKEY_CURRENT_USER) {
		return L"HKCU\\";
	} else if (hRootKey == HKEY_LOCAL_MACHINE) {
		return L"HKLM\\";
	} else if (hRootKey == HKEY_USERS) {
		return L"HKU\\";
	} else if (hRootKey == HKEY_CURRENT_CONFIG) {
		return L"HKCC\\";
	} else {
		return L"";
	}
}

bool RegistryService::isMatch(std::wstring str) {
	return this->toLower(str).find(this->toLower(query)) != -1;
}

std::wstring RegistryService::toLower(std::wstring str) {
	std::wstring result;
	result.reserve(str.length());
	for (size_t i = 0; i < str.length(); i++) {
		result.push_back(tolower(str[i]));
	}

	return result;
}

void RegistryService::search(std::wstring query, std::vector<HKEY> searchKeys) {
	this->query = query;

	for (auto it = searchKeys.begin(); it != searchKeys.end(); it++) {
		searchInKey((*it), L"");
	}
}

void RegistryService::addToList(HWND hList, std::wstring str) {
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)(str.c_str()));
}

RegistryService::RegistryService(HWND hList) {
	this->resultList = hList;
}