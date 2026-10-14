// ============================================================
//  System Time Changer - Animated Console Edition
//  Compatible with Windows XP SP3 through Windows 11
// ============================================================
//  Author  : BenyVK
//  Website : https://Gharri.ir
//  GitHub  : https://github.com/BenyVK
//  Project : https://github.com/BenyVK/change-time-windows
// ============================================================

#define _WIN32_WINNT 0x0501
#define WINVER       0x0501

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <windows.h>
#include <shellapi.h>

namespace AppInfo {
    const char* AUTHOR   = "BenyVK";
    const char* WEBSITE  = "Gharri.ir";
    const char* GITHUB   = "github.com/BenyVK";
    const char* PROJECT  = "github.com/BenyVK/change-time-windows";
    const char* APP_NAME = "System Time Changer";
}

class ConsoleAnim {
public:
    HANDLE hOut;
    bool   ansiEnabled;

    ConsoleAnim() : hOut(GetStdHandle(STD_OUTPUT_HANDLE)), ansiEnabled(false) {
        enableAnsi();
    }

    void enableAnsi() {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            // ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004
            if (SetConsoleMode(hOut, mode | 0x0004)) {
                ansiEnabled = true;
            }
        }
    }

    void clear() {
        if (ansiEnabled) {
            std::cout << "\x1b[2J\x1b[H";
        } else {
            system("cls");
        }
        std::cout.flush();
    }

    void hideCursor(bool hide) {
        CONSOLE_CURSOR_INFO ci;
        if (GetConsoleCursorInfo(hOut, &ci)) {
            ci.bVisible = hide ? FALSE : TRUE;
            SetConsoleCursorInfo(hOut, &ci);
        }
    }

    void setColor(WORD attr) {
        SetConsoleTextAttribute(hOut, attr);
    }

    void resetColor() {
        SetConsoleTextAttribute(hOut, FOREGROUND_RED |
                                      FOREGROUND_GREEN |
                                      FOREGROUND_BLUE);
    }

    void gotoxy(int x, int y) {
        COORD c;
        c.X = static_cast<SHORT>(x);
        c.Y = static_cast<SHORT>(y);
        SetConsoleCursorPosition(hOut, c);
    }

    int width() const {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hOut, &csbi))
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        return 80;
    }

    void typeText(const std::string& text, int delayMs = 12, WORD color = 0x07) {
        setColor(color);
        for (size_t i = 0; i < text.size(); ++i) {
            std::cout << text[i] << std::flush;
            Sleep(delayMs);
        }
        resetColor();
    }

    void printCentered(const std::string& text, WORD color = 0x07) {
        int w = width();
        int pad = (w - static_cast<int>(text.size())) / 2;
        if (pad < 0) pad = 0;
        setColor(color);
        std::cout << std::string(pad, ' ') << text << "\n";
        resetColor();
    }

    void spinner(const std::string& label, int durationMs = 1500) {
        const char frames[] = { '|', '/', '-', '\\' };
        int steps = durationMs / 100;
        for (int i = 0; i < steps; ++i) {
            std::cout << "\r" << label << " " << frames[i % 4] << std::flush;
            Sleep(100);
        }
        std::cout << "\r" << label << " [OK]   \n";
    }

    void progressBar(const std::string& label, int durationMs = 1200, int barWidth = 30) {
        int steps = durationMs / 40;
        if (steps < 1) steps = 1;
        for (int i = 0; i <= steps; ++i) {
            int filled = (i * barWidth) / steps;
            std::cout << "\r" << label << " [";
            for (int j = 0; j < barWidth; ++j)
                std::cout << (j < filled ? '#' : '-');
            std::cout << "] " << std::setw(3) << ((i * 100) / steps) << "%"
                      << std::flush;
            Sleep(40);
        }
        std::cout << "\n";
    }

    void showBanner() {
        clear();
        std::cout << "\n";
        printCentered("  ____            _                  ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(" / ___| _   _ ___| |_ ___ _ __ ___   ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(" \\___ \\| | | / __| __/ _ \\ '_ ` _ \\  ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered("  ___) | |_| \\__ \\ ||  __/ | | | | | ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(" |____/ \\__, |___/\\__\\___|_| |_| |_| ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered("        |___/                        ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "\n";
        printCentered("T I M E   C H A N G E R", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "\n";

        printCentered("-----------------------------------------------",
                      FOREGROUND_BLUE | FOREGROUND_GREEN);
        printCentered(std::string("Author  : ") + AppInfo::AUTHOR,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(std::string("Website : ") + AppInfo::WEBSITE,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(std::string("GitHub  : ") + AppInfo::GITHUB,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(std::string("Project : ") + AppInfo::PROJECT,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered("-----------------------------------------------",
                      FOREGROUND_BLUE | FOREGROUND_GREEN);
        std::cout << "\n";
        resetColor();
    }

    void showFooter() {
        std::cout << "\n";
        printCentered("===============================================",
                      FOREGROUND_BLUE | FOREGROUND_GREEN);
        printCentered(std::string("  ") + AppInfo::APP_NAME +
                      "  |  by " + AppInfo::AUTHOR + "  |  " + AppInfo::WEBSITE,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered(std::string("  ") + AppInfo::PROJECT,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printCentered("===============================================",
                      FOREGROUND_BLUE | FOREGROUND_GREEN);
        resetColor();
    }
};

struct OsVersion {
    DWORD major, minor, build;
    bool  valid;
};

typedef LONG (WINAPI *RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

OsVersion getRealOsVersion() {
    OsVersion result;
    result.major = 0;
    result.minor = 0;
    result.build = 0;
    result.valid = false;

    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll) {
        RtlGetVersionPtr pFn = reinterpret_cast<RtlGetVersionPtr>(
            GetProcAddress(hNtdll, "RtlGetVersion"));
        if (pFn) {
            RTL_OSVERSIONINFOW osvi;
            ZeroMemory(&osvi, sizeof(osvi));
            osvi.dwOSVersionInfoSize = sizeof(osvi);
            if (pFn(&osvi) == 0) {
                result.major = osvi.dwMajorVersion;
                result.minor = osvi.dwMinorVersion;
                result.build = osvi.dwBuildNumber;
                result.valid = true;
                return result;
            }
        }
    }

    OSVERSIONINFOW osvi;
    ZeroMemory(&osvi, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    #pragma warning(push)
    #pragma warning(disable: 4996)
    if (GetVersionExW(&osvi)) {
        result.major = osvi.dwMajorVersion;
        result.minor = osvi.dwMinorVersion;
        result.build = osvi.dwBuildNumber;
        result.valid = true;
    }
    #pragma warning(pop)
    return result;
}

bool isWindowsXpOrOlder(const OsVersion& v) { return v.valid && v.major <= 5; }
bool hasUac(const OsVersion& v)              { return v.valid && v.major >= 6; }

std::string describeOs(const OsVersion& v) {
    if (!v.valid) return "Unknown";
    if (v.major == 5 && v.minor == 1) return "Windows XP";
    if (v.major == 5 && v.minor == 2) return "Windows Server 2003 / XP x64";
    if (v.major == 6 && v.minor == 0) return "Windows Vista";
    if (v.major == 6 && v.minor == 1) return "Windows 7";
    if (v.major == 6 && v.minor == 2) return "Windows 8";
    if (v.major == 6 && v.minor == 3) return "Windows 8.1";
    if (v.major == 10 && v.build < 22000) return "Windows 10";
    if (v.major == 10 && v.build >= 22000) return "Windows 11";
    return "Unknown";
}

bool isRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin != FALSE;
}

bool relaunchAsAdmin() {
    wchar_t exePath[MAX_PATH];
    if (!GetModuleFileNameW(NULL, exePath, MAX_PATH)) return false;
    std::wstring args = GetCommandLineW();

    SHELLEXECUTEINFOW sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize       = sizeof(sei);
    sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb       = L"runas";
    sei.lpFile       = exePath;
    sei.lpParameters = args.c_str();
    sei.nShow        = SW_SHOWNORMAL;

    if (!ShellExecuteExW(&sei)) return false;
    if (sei.hProcess) CloseHandle(sei.hProcess);
    return true;
}

class SystemTimeManager {
public:
    bool enableSetTimePrivilege() {
        HANDLE hToken = NULL;
        TOKEN_PRIVILEGES tp;
        LUID luid;

        if (!OpenProcessToken(GetCurrentProcess(),
                              TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                              &hToken)) return false;
        if (!LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid)) {
            CloseHandle(hToken);
            return false;
        }

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)) {
            CloseHandle(hToken);
            return false;
        }
        if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
            CloseHandle(hToken);
            return false;
        }
        CloseHandle(hToken);
        return true;
    }

    bool setLocalTime(int y, int mo, int d, int h, int mi, int s) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        st.wYear   = static_cast<WORD>(y);
        st.wMonth  = static_cast<WORD>(mo);
        st.wDay    = static_cast<WORD>(d);
        st.wHour   = static_cast<WORD>(h);
        st.wMinute = static_cast<WORD>(mi);
        st.wSecond = static_cast<WORD>(s);
        st.wMilliseconds = 0;
        return SetLocalTime(&st) != 0;
    }

    std::string currentTimeString() {
        SYSTEMTIME st;
        GetLocalTime(&st);
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << st.wYear   << "-"
            << std::setw(2) << st.wMonth  << "-"
            << std::setw(2) << st.wDay    << " "
            << std::setw(2) << st.wHour   << ":"
            << std::setw(2) << st.wMinute << ":"
            << std::setw(2) << st.wSecond;
        return oss.str();
    }
};

int askInt(ConsoleAnim& anim, const std::string& prompt,
           int minVal, int maxVal)
{
    while (true) {
        anim.setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "  " << prompt;
        anim.resetColor();

        std::string line;
        std::getline(std::cin, line);

        while (!line.empty() && isspace((unsigned char)line[0]))
            line.erase(line.begin());
        while (!line.empty() && isspace((unsigned char)line[line.size()-1]))
            line.erase(line.size()-1);

        if (line.empty()) continue;

        bool numeric = true;
        for (size_t i = 0; i < line.size(); ++i) {
            if (!isdigit((unsigned char)line[i])) { numeric = false; break; }
        }
        if (!numeric) {
            anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "  [!] Enter a number.\n";
            anim.resetColor();
            continue;
        }

        int val = atoi(line.c_str());
        if (val < minVal || val > maxVal) {
            anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "  [!] Range: " << minVal << " .. " << maxVal << "\n";
            anim.resetColor();
            continue;
        }
        return val;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    ConsoleAnim anim;
    anim.hideCursor(true);

    OsVersion os = getRealOsVersion();

    anim.showBanner();

    anim.spinner("Detecting OS", 700);
    {
        std::ostringstream oss;
        oss << "  Detected: " << describeOs(os);
        if (os.valid) oss << " (" << os.major << "." << os.minor
                          << " build " << os.build << ")";
        anim.typeText(oss.str() + "\n", 5,
                      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    anim.spinner("Checking privileges", 600);

    // ---------------- elevation ----------------
    if (!isRunningAsAdmin()) {
        if (isWindowsXpOrOlder(os)) {
            std::cout << "\n";
            anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            anim.printCentered("[!] Not running as Administrator.",
                               FOREGROUND_RED | FOREGROUND_INTENSITY);
            anim.printCentered("Windows XP has no UAC.",
                               FOREGROUND_RED | FOREGROUND_INTENSITY);
            anim.printCentered("Please log in as an Administrator and retry.",
                               FOREGROUND_RED | FOREGROUND_INTENSITY);
            anim.resetColor();
            anim.showFooter();
            std::cout << "\n  Press Enter to exit...";
            anim.hideCursor(false);
            std::cin.get();
            return 1;
        }

        if (!hasUac(os)) {
            std::cout << "\n";
            anim.printCentered("[!] Administrator privileges required.",
                               FOREGROUND_RED | FOREGROUND_INTENSITY);
            anim.showFooter();
            std::cout << "\n  Press Enter to exit...";
            anim.hideCursor(false);
            std::cin.get();
            return 1;
        }

        std::cout << "\n";
        anim.setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        anim.typeText("  Requesting administrator privileges...\n", 10,
                      FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        std::cout << "\n  [UAC] Waiting for user response ";
        for (int i = 0; i < 6; ++i) {
            std::cout << "." << std::flush;
            Sleep(250);
        }
        std::cout << "\n";

        if (relaunchAsAdmin()) {
            anim.setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::cout << "\n  [+] Elevation accepted. Launching elevated instance...\n";
            anim.resetColor();
            anim.showFooter();
            Sleep(800);
            anim.hideCursor(false);
            return 0;
        } else {
            anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            std::cout << "\n  [-] Elevation cancelled or failed.\n";
            anim.resetColor();
            anim.showFooter();
            std::cout << "\n  Press Enter to exit...";
            anim.hideCursor(false);
            std::cin.get();
            return 1;
        }
    }

    anim.setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "\n  [+] Running with Administrator privileges.\n";
    anim.resetColor();

    SystemTimeManager mgr;
    anim.progressBar("  Enabling time privilege", 900, 28);

    if (!mgr.enableSetTimePrivilege()) {
        anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "\n  [-] Failed to enable SE_SYSTEMTIME_NAME.\n";
        anim.resetColor();
        anim.showFooter();
        std::cout << "\n  Press Enter to exit...";
        anim.hideCursor(false);
        std::cin.get();
        return 1;
    }

    std::cout << "\n";
    anim.setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "  Current time:  ";
    anim.resetColor();
    std::cout << mgr.currentTimeString() << "\n\n";

    anim.setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    anim.printCentered("--- Enter new date and time ---",
                       FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "\n";

    anim.hideCursor(false);

    int year   = askInt(anim, "Year   [1970-2099]: ", 1970, 2099);
    int month  = askInt(anim, "Month  [1-12]:      ", 1, 12);
    int day    = askInt(anim, "Day    [1-31]:      ", 1, 31);
    int hour   = askInt(anim, "Hour   [0-23]:      ", 0, 23);
    int minute = askInt(anim, "Minute [0-59]:      ", 0, 59);
    int second = askInt(anim, "Second [0-59]:      ", 0, 59);

    anim.hideCursor(true);

    std::cout << "\n";
    anim.progressBar("  Applying new time", 1100, 28);

    if (mgr.setLocalTime(year, month, day, hour, minute, second)) {
        std::cout << "\n";
        anim.setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        anim.printCentered("[+] System time updated successfully!",
                           FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        anim.resetColor();
    } else {
        std::cout << "\n";
        anim.setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::ostringstream oss;
        oss << "[-] Failed to set time. Error: " << GetLastError();
        anim.printCentered(oss.str(), FOREGROUND_RED | FOREGROUND_INTENSITY);
        anim.resetColor();
    }

    std::cout << "\n";
    for (int i = 0; i < 3; ++i) {
        anim.setColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << "\r  New time:  " << mgr.currentTimeString() << std::flush;
        Sleep(250);
        anim.resetColor();
        std::cout << "\r  New time:  " << mgr.currentTimeString() << std::flush;
        Sleep(250);
    }
    std::cout << "\n";

    anim.setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    anim.printCentered("--- Done ---", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    anim.resetColor();

    anim.showFooter();

    anim.hideCursor(false);
    std::cout << "\n  Press Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}