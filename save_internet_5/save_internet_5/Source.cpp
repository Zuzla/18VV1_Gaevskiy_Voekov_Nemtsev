#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <tchar.h>

#ifdef _DEBUG
#include <conio.h>
#endif

void error(const char *error) {
	printf("%s\n", error);

#ifdef _DEBUG
	_getch();
#endif
	exit(1);
}

int main(int argc, char* argv[]) {

	EXPLICIT_ACCESS ea[2];
	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));

	//получение идентификатора группы Everyone

	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	PSID pEveryoneSID = NULL;

	if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID)) {
		error("AllocateAndInitializeSid Error");
	}

	ea[0].grfAccessPermissions = KEY_READ;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

	//получение идентификатора группы Admin

	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
	PSID pAdminSID = NULL;

	if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSID)) {
		FreeSid(pEveryoneSID);
		error("AllocateAndInitializeSid Error");
	}

	ea[1].grfAccessPermissions = KEY_ALL_ACCESS;
	ea[1].grfAccessMode = SET_ACCESS;
	ea[1].grfInheritance = NO_INHERITANCE;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

	PACL pACL = NULL;
	DWORD dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);

	if (dwRes != ERROR_SUCCESS) {
		FreeSid(pEveryoneSID);
		FreeSid(pAdminSID);
		error("SetEntriesInAcl Error");
	}

	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

	if (pSD == NULL) {
		FreeSid(pEveryoneSID);
		FreeSid(pAdminSID);
		LocalFree(pACL);
		error("LocalAlloc Error");
	}

	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {
		FreeSid(pEveryoneSID);
		FreeSid(pAdminSID);
		LocalFree(pACL);
		LocalFree(pSD);
		error("InitializeSecurityDescriptor Error");
	}

	if (!SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE)) {
		FreeSid(pEveryoneSID);
		FreeSid(pAdminSID);
		LocalFree(pACL);
		LocalFree(pSD);
		error("SetSecurityDescriptorDacl Error");
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	HKEY hkSub;
	DWORD dwDisposition;
	LONG lRes = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\pi"), 0, NULL, 0, KEY_READ | KEY_WRITE, &sa, &hkSub, NULL);

	printf("RegCreateKeyEx result: ");

	switch (lRes) {

	case ERROR_SUCCESS:
		printf("OK!\n");
		break;
	default:
		printf("ERROR!\n");
	}
	printf("\nFinished!");

#ifdef _DEBUG
	_getch();
#endif
	return 0;
}


//#define _CRT_SECURE_NO_WARNINGS
//
//#include <tchar.h>
//#include <Windows.h>
//#include <AclAPI.h>
//#include <iostream>
//
//int _tmain(int argc, TCHAR * argv[], TCHAR * envp[])
//{
//	PSID pEveryoneSID = NULL;
//	SID_IDENTIFIER_AUTHORITY SIDAuthWorld =	SECURITY_WORLD_SID_AUTHORITY;
//	AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID);
//	EXPLICIT_ACCESS ea[2];
//	ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
//	
//	ea[0].grfAccessPermissions = GENERIC_EXECUTE | GENERIC_READ;
//	ea[0].grfAccessMode = SET_ACCESS;
//	ea[0].grfInheritance = NO_INHERITANCE;
//	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
//	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
//	ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;
//	
//	HANDLE hToken = NULL;
//	DWORD dwBufferSize = 0;
//	PTOKEN_USER pTokenUser = NULL;
//	
//	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY,
//		&hToken)) {
//		_tprintf(_T("OpenProcessToken failed.\n"),
//			GetLastError());
//		return -1;
//	}
//	
//	(void)GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize);
//	pTokenUser = (PTOKEN_USER)malloc(dwBufferSize);
//	memset(pTokenUser, 0, dwBufferSize);
//	
//	if (!GetTokenInformation(hToken, TokenUser, pTokenUser,	dwBufferSize,	&dwBufferSize)) {
//		_tprintf(_T("GetTokenInformation failed.\n"),
//			GetLastError());
//		return -2;
//	}
//
//	CloseHandle(hToken);
//
//	ea[1].grfAccessPermissions = GENERIC_EXECUTE | GENERIC_READ;
//	ea[1].grfAccessMode = DENY_ACCESS;
//	ea[1].grfInheritance = NO_INHERITANCE;
//	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
//	ea[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
//	ea[1].Trustee.ptstrName = (LPTSTR)pTokenUser->User.Sid;
//
//	DWORD dwRes;
//	PACL pACL = NULL;
//	dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
//
//	if (ERROR_SUCCESS != dwRes)
//	{
//		_tprintf(_T("SetEntriesInAcl Error %u\n"),
//			GetLastError());
//		FreeSid(pEveryoneSID);
//		return 1;
//	}
//	PSECURITY_DESCRIPTOR pSD = NULL;
//	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
//
//	if (NULL == pSD)
//	{
//		_tprintf(_T("pSD 1 Error %u\n"), GetLastError());
//		FreeSid(pEveryoneSID);
//		LocalFree(pACL);
//		return 2;
//	}
//
//	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
//	{
//		_tprintf(_T("pSD 2 Error %u\n"), GetLastError());
//		FreeSid(pEveryoneSID);
//		LocalFree(pACL);
//		LocalFree(pSD);
//		return 3;
//	}
//
//	if (!SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE))
//	{
//		_tprintf(_T("Dacl Error %u\n"), GetLastError());
//			FreeSid(pEveryoneSID);
//		LocalFree(pACL);
//		LocalFree(pSD);
//		return 4;
//	}
//
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.lpSecurityDescriptor = pSD;
//	sa.bInheritHandle = FALSE;
//	HANDLE hFile;
//	
//	if (INVALID_HANDLE_VALUE == (hFile =
//		CreateFile(_T("D://_0/_fi1l10Vzvv.txt"),
//			GENERIC_WRITE, FILE_SHARE_WRITE,
//			&sa, CREATE_ALWAYS,
//			FILE_ATTRIBUTE_NORMAL, NULL)))
//	{
//		CloseHandle(hFile);
//		FreeSid(pEveryoneSID);
//		LocalFree(pACL);
//		LocalFree(pSD);
//		return 5;
//	}
//	
//	TCHAR * tcArray = TEXT("fil10.txt");
//	DWORD dwBytesWritten;
//	WriteFile(hFile, tcArray, _tcslen(tcArray) * sizeof(TCHAR), &dwBytesWritten, NULL);
//	CloseHandle(hFile);
//	FreeSid(pEveryoneSID);
//	LocalFree(pACL);
//	LocalFree(pSD);
//	free(pTokenUser);
//	return 0;
//}