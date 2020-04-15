#pragma once
#include <string>
#include <vector>

#define MAX_KEY_LENGTH 2048
#define MAX_VALUE_NAME 16383

class RegistryService {
private:
	std::vector<std::wstring> searchResults;
	std::wstring query;
	HWND resultList;

	int deep = 0;

	void searchInKey(HKEY hKey, std::wstring path);

	std::wstring getKeyPath(HKEY key);
	bool isMatch(std::wstring str);
	std::wstring toLower(std::wstring str);
	void addToList(HWND hList, std::wstring str);
public:
	void search(std::wstring query, std::vector<HKEY> searchKeys);
	RegistryService(HWND hList);
};

