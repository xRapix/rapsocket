/******************************************************************************
 *  ██████╗  █████╗ ██████╗ ███████╗ ██████╗  ██████╗██╗  ██╗███████╗████████╗
 *  ██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔═══██╗██╔════╝██║ ██╔╝██╔════╝╚══██╔══╝
 *  ██████╔╝███████║██████╔╝███████╗██║   ██║██║     █████╔╝ █████╗     ██║   
 *  ██╔══██╗██╔══██║██╔═══╝ ╚════██║██║   ██║██║     ██╔═██╗ ██╔══╝     ██║   
 *  ██║  ██║██║  ██║██║     ███████║╚██████╔╝╚██████╗██║  ██╗███████╗   ██║   
 *  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝╚══════╝   ╚═╝   
 * 
 * Made by xRapix                                                                           
 *****************************************************************************/





#pragma once
#include<windows.h>
#include<winhttp.h>
#include"rapsocket.hpp" 

//DWORD LastErr;
#pragma comment(lib, "winhttp.lib")

char* MakeOneHeader(char Part1[],const char Text[], char Part2[], char exitString[]){
    int GlobalCounter = 0;


    for (int counter1 = 0;Part1[counter1];counter1++){
        exitString[GlobalCounter] = Part1[counter1];
        GlobalCounter++;
    }
    for (int counter2 = 0;Text[counter2];counter2++){
        exitString[GlobalCounter] = Text[counter2];
        GlobalCounter++;
    }
    for (int counter3 = 0;Part2[counter3];counter3++){
        //std::cout << "INSIDE LOOP: " << exitString << "\n";
        exitString[GlobalCounter] = Part2[counter3];
        GlobalCounter++;

    }
    exitString[GlobalCounter] = '\0';
    
    return exitString;
}

int strLen(const char* str){
    int len = 0;
    while (str[len] != '\0'){
        len++;
    }
    return len;
}

int wstrLen(const wchar_t* wstr){
    int len = 0;
    while (wstr[len] != '\0'){
        len++;
    }
    return len;
}




inline void PostTextAndForget(const wchar_t* URL,const char text[],const wchar_t* user_agent,int port,int &CodeVariable){
    //Stripping url into 2 parts (winhttp needs host and path)
    wchar_t StrippedHOST[2048] = {0};
    wchar_t StrippedPATH[2048] = {0};

    int hostIndex = 0;
    int pathIndex = 0;

    bool IsHostStripped = false;
    for (int i = 0; URL[i]; i++){
        if (hostIndex >= 2047 || pathIndex >= 2047) {
            break;
        }

        if (!IsHostStripped){
            if (URL[i] == '/'){
                IsHostStripped = true; 
                StrippedPATH[pathIndex++] = URL[i];
            }
            else{
                StrippedHOST[hostIndex++] = URL[i];
            }
        }
        else{
            IsHostStripped = true;
            StrippedPATH[pathIndex++] = URL[i];
        }
    }
    StrippedHOST[hostIndex] = '\0';
    StrippedPATH[pathIndex] = '\0';

    //CreatingRequest
    char postDataPart1[] = "{\"content\": \"";
    char postDataPart2[] =  "\"}";
    int len = strLen(text) + 20;
    char finalPostData[len] = {0};


    MakeOneHeader(postDataPart1, text, postDataPart2, finalPostData);

    const wchar_t* headers = L"Content-Type: application/json\r\n";

    DWORD requestflag = 0;

    if (port == 443){
        requestflag = WINHTTP_FLAG_SECURE;
    }

    HINTERNET hSession = WinHttpOpen(user_agent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession){
        return;
    }
    

    if (hSession){
        HINTERNET hconnect = WinHttpConnect(hSession,StrippedHOST,port,0);
        if (!hconnect){
            return;
        }
        if (hconnect){
            HINTERNET hRequest = WinHttpOpenRequest(hconnect,L"POST",StrippedPATH,NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, requestflag);
            if (!hRequest){
                return;
            }
            if (hRequest){
                WinHttpSendRequest(hRequest,headers,-1L,(LPVOID)finalPostData,strLen(finalPostData),strLen(finalPostData),0);
                WinHttpReceiveResponse(hRequest,NULL);

                DWORD statusCode = 0;
                DWORD statusCodeSize = sizeof(statusCode);
                WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX);
                CodeVariable = statusCode;

                WinHttpCloseHandle(hRequest);
                
            }
            WinHttpCloseHandle(hconnect);
        }
        WinHttpCloseHandle(hSession);
    }
}


inline const char* PostTextAndListen(const wchar_t* URL,const char text[],const wchar_t* user_agent,int port,int &CodeVariable){
    //Stripping url into 2 parts (winhttp needs host and path)
    wchar_t StrippedHOST[2048] = {0};
    wchar_t StrippedPATH[2048] = {0};

    int hostIndex = 0;
    int pathIndex = 0;

    bool bResult = false;
    DWORD dwsize;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    const char* response = "";

    bool IsHostStripped = false;
    for (int i = 0; URL[i]; i++){
        if (hostIndex >= 2047 || pathIndex >= 2047) {
            break;
        }

        if (!IsHostStripped){
            if (URL[i] == '/'){
                IsHostStripped = true; 
                StrippedPATH[pathIndex++] = URL[i];
            }
            else{
                StrippedHOST[hostIndex++] = URL[i];
            }
        }
        else{
            IsHostStripped = true;
            StrippedPATH[pathIndex++] = URL[i];
        }
    }
    StrippedHOST[hostIndex] = '\0';
    StrippedPATH[pathIndex] = '\0';
    char postDataPart1[] = "{\"content\": \"";
    char postDataPart2[] =  "\"}";
    int len = strLen(text) + 20;
    char finalPostData[len] = {0};

    MakeOneHeader(postDataPart1,text,postDataPart2,finalPostData);

    wchar_t headers[] = L"Content-Type: application/json\r\n";

    DWORD requestflag = 0;
    if (port == 443) {
        requestflag = WINHTTP_FLAG_SECURE;
    }

    HINTERNET hSession = WinHttpOpen(user_agent,WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession){
        HINTERNET hConnect = WinHttpConnect(hSession,StrippedHOST,port,0);
        if (hConnect){
            HINTERNET hRequest = WinHttpOpenRequest(hConnect,L"POST",StrippedPATH,NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, requestflag);
            if (hRequest){
                bResult = WinHttpSendRequest(hRequest, headers, wstrLen(headers), (LPVOID)finalPostData, strLen(finalPostData), strLen(finalPostData), 0);
                if (bResult){
                    bResult = WinHttpReceiveResponse(hRequest,NULL);
                }
                DWORD statusCode = 0;
                DWORD statusCodeSize = sizeof(statusCode);
                WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX);
                CodeVariable = statusCode;
                char* fullResponse = nullptr;
                size_t totalBytesRead = 0;
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
                                
                                size_t newSize = totalBytesRead + dwsize;
                                char* newBuffer = new char[newSize + 1];

                                if (fullResponse != nullptr) {
                                    for (size_t i = 0; i < totalBytesRead; i++) {
                                    newBuffer[i] = fullResponse[i];
                                    }
                                
                                delete[] fullResponse; 
                                }

                                for (size_t i = 0; i < dwsize; i++) {
                                    newBuffer[totalBytesRead + i] = pszOutBuffer[i];
                                }

                                fullResponse = newBuffer;
                                totalBytesRead = newSize;
                                fullResponse[totalBytesRead] = '\0';

                                delete[] pszOutBuffer;
                            }
                        }
                    }while(dwsize > 0);
                    char* FinalResponseStr = new char[totalBytesRead + 1];
                    if (fullResponse != nullptr){
                        for (int i = 0;i < totalBytesRead;i++){
                            FinalResponseStr[i] = fullResponse[i];
                        }
                        FinalResponseStr[totalBytesRead] = '\0';

                    }
                    if (fullResponse != nullptr) {
                        delete[] fullResponse;
                        fullResponse = nullptr;
                    }
                    if (hRequest) WinHttpCloseHandle(hRequest);
                    if (hConnect) WinHttpCloseHandle(hConnect);
                    if (hSession) WinHttpCloseHandle(hSession);
                
                    return FinalResponseStr;
                }
            }
        }

    }
    return "";
}
inline const char* GETTextAndListen(const wchar_t* URL,const wchar_t* user_agent,int port,int &CodeVariable){
    //Stripping url into 2 parts (winhttp needs host and path)
    wchar_t StrippedHOST[2048] = {0};
    wchar_t StrippedPATH[2048] = {0};

    int hostIndex = 0;
    int pathIndex = 0;

    bool bResult = false;
    DWORD dwsize;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    const char* response = "";

    bool IsHostStripped = false;
    for (int i = 0; URL[i]; i++){
        if (hostIndex >= 2047 || pathIndex >= 2047) {
            break;
        }

        if (!IsHostStripped){
            if (URL[i] == '/'){
                IsHostStripped = true; 
                StrippedPATH[pathIndex++] = URL[i];
            }
            else{
                StrippedHOST[hostIndex++] = URL[i];
            }
        }
        else{
            IsHostStripped = true;
            StrippedPATH[pathIndex++] = URL[i];
        }
    }
    StrippedHOST[hostIndex] = '\0';
    StrippedPATH[pathIndex] = '\0';

    wchar_t headers[] = L"Content-Type: application/json\r\n";

    DWORD requestflag = 0;
    if (port == 443) {
        requestflag = WINHTTP_FLAG_SECURE;
    }

    HINTERNET hSession = WinHttpOpen(user_agent,WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession){
        HINTERNET hConnect = WinHttpConnect(hSession,StrippedHOST,port,0);
        if (hConnect){
            HINTERNET hRequest = WinHttpOpenRequest(hConnect,L"GET",StrippedPATH,NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, requestflag);
            if (hRequest){
                bResult = WinHttpSendRequest(hRequest, headers, wstrLen(headers), NULL,0,0,0);
                if (bResult){
                    bResult = WinHttpReceiveResponse(hRequest,NULL);
                }
                DWORD statusCode = 0;
                DWORD statusCodeSize = sizeof(statusCode);
                WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX);
                CodeVariable = statusCode;
                char* fullResponse = nullptr;
                size_t totalBytesRead = 0;
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
                                
                                size_t newSize = totalBytesRead + dwsize;
                                char* newBuffer = new char[newSize + 1];

                                if (fullResponse != nullptr) {
                                    for (size_t i = 0; i < totalBytesRead; i++) {
                                    newBuffer[i] = fullResponse[i];
                                    }
                                
                                delete[] fullResponse; 
                                }

                                for (size_t i = 0; i < dwsize; i++) {
                                    newBuffer[totalBytesRead + i] = pszOutBuffer[i];
                                }

                                fullResponse = newBuffer;
                                totalBytesRead = newSize;
                                fullResponse[totalBytesRead] = '\0';

                                delete[] pszOutBuffer;
                            }
                        }
                    }while(dwsize > 0);
                    char* FinalResponseStr = new char[totalBytesRead + 1];
                    if (fullResponse != nullptr){
                        for (int i = 0;i < totalBytesRead;i++){
                            FinalResponseStr[i] = fullResponse[i];
                        }
                        FinalResponseStr[totalBytesRead] = '\0';

                    }
                    if (fullResponse != nullptr) {
                        delete[] fullResponse;
                        fullResponse = nullptr;
                    }
                    if (hRequest) WinHttpCloseHandle(hRequest);
                    if (hConnect) WinHttpCloseHandle(hConnect);
                    if (hSession) WinHttpCloseHandle(hSession);
                
                    return FinalResponseStr;
                }
            }
        }

    }
    return "";
}
