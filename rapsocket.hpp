
#pragma once
#include<windows.h>
#include<winhttp.h>
#include<string>
#include <format>
#include<iostream>
#include"rapsocket.hpp"


inline void PostTextAndForget(std::wstring URL,std::string text,std::wstring user_agent,int port){
    //Stripping url into 2 parts (winhttp needs host and path)
    std::wstring StrippedHOST = L"";
    std::wstring StrippedPATH = L"";


    bool IsHostStripped = false;
    for (int i = 0; URL[i]; i++){
        if (!IsHostStripped){
            if (URL[i] == '/'){
                IsHostStripped = true; 
            }
            else{
                StrippedHOST += URL[i];
            }
        }
        if (IsHostStripped){
            IsHostStripped = true;
            StrippedPATH += URL[i];  
        }
    }

    //CreatingRequest
    std::string postData = std::format("{{\"content\": \"{}\"}}", text);
    std::wstring headers = L"Content-Type: application/json\r\n";

    HINTERNET hSession = WinHttpOpen(user_agent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)return;

    if (hSession){
        HINTERNET hconnect = WinHttpConnect(hSession,StrippedHOST.c_str(),port,0);
        if (hconnect){
            HINTERNET hRequest = WinHttpOpenRequest(hconnect,L"POST",StrippedPATH.c_str(),NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (hRequest){
                WinHttpSendRequest(hRequest,headers.c_str(),headers.length(),(LPVOID)postData.c_str(),postData.length(),postData.length(),0);
                WinHttpReceiveResponse(hRequest,NULL);

                WinHttpCloseHandle(hRequest);
            }
            WinHttpCloseHandle(hconnect);
        }
        WinHttpCloseHandle(hSession);
    }
}

inline std::string PostTextAndListen(std::wstring URL,std::string text,std::wstring user_agent,int port){
    //Stripping url into 2 parts (winhttp needs host and path)
    std::wstring StrippedHOST = L"";
    std::wstring StrippedPATH = L"";


    bool IsHostStripped = false;
    bool bResult = false;
    DWORD dwsize;
    LPSTR pszOutBuffer;
    DWORD dwDownloaded = 0;
    std::string response;

    for (int i = 0; URL[i]; i++){
        if (!IsHostStripped){
            if (URL[i] == '/'){
                IsHostStripped = true; 
            }
            else{
                StrippedHOST += URL[i];
            }
        }
        if (IsHostStripped){
            IsHostStripped = true;
            StrippedPATH += URL[i];  
        }
    }
    std::string postData = std::format("{{\"content\": \"{}\"}}", text);
    std::wstring headers = L"Content-Type: application/json\r\n";

    HINTERNET hSession = WinHttpOpen(user_agent.c_str(),WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession){
        HINTERNET hConnect = WinHttpConnect(hSession,StrippedHOST.c_str(),port,0);
        if (hConnect){
            HINTERNET hRequest = WinHttpOpenRequest(hConnect,L"POST",StrippedPATH.c_str(),NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (hRequest){
                bResult = WinHttpSendRequest(hRequest,headers.c_str(),headers.length(),(LPVOID)postData.c_str(),postData.length(),postData.length(),0);
                if (bResult){
                    bResult = WinHttpReceiveResponse(hRequest,NULL);
                }
                if (bResult){
                    do{
                        dwsize = 0;
                        if (!WinHttpQueryDataAvailable(hRequest, &dwsize)) {
                            break; 
                        }

                        if (dwsize == 0) {
                            break; 
                        }
                        
                        pszOutBuffer = new char[dwsize + 1];
                        if (!pszOutBuffer){
                            dwsize = 0;

                        }
                        else{
                            ZeroMemory(pszOutBuffer,dwsize + 1);
                            if (!WinHttpReadData(hRequest,(LPVOID)pszOutBuffer,dwsize,&dwDownloaded)){

                            }
                            else{
                                response = response + std::string(pszOutBuffer);

                                delete[] pszOutBuffer;
                            }
                        }
                    }while(dwsize > 0);
                }
                if (hRequest) WinHttpCloseHandle(hRequest);
            }
            if (hConnect) WinHttpCloseHandle(hConnect);
        }
        if (hSession) WinHttpCloseHandle(hSession);

    }
    return response;
}
