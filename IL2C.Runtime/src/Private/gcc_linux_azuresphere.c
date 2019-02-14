#include <il2c_private.h>

//////////////////////////////////////////////////
// GCC

// Azure Sphere
#if defined(__GNUC__) && defined(__linux__) && defined(__AZURE_SPHERE__)

#include <applibs/log.h>

void il2c_debug_write(const char* message)
{
    il2c_assert(message != NULL);

    Log_Debug(message);
}

void il2c_debug_write2(const char* message1, const char* message2)
{
    il2c_assert(message1 != NULL);
    il2c_assert(message2 != NULL);

    Log_Debug(message1);
    Log_Debug(message2);
}

// TODO: UTF16 --> UTF8

void il2c_write(const wchar_t* p)
{
    size_t l = il2c_wcslen(p);
    char* d = il2c_mcalloc(l + 1);
    size_t i;
    for (i = 0; i < l; i++) d[i] = (char)(p[i]);
    d[i] = '\0';
    Log_Debug(d);
    il2c_mcfree(d);
}

void il2c_writeline(const wchar_t* p)
{
    size_t l = il2c_wcslen(p);
    char* d = il2c_mcalloc(l + 2);
    size_t i;
    for (i = 0; i < l; i++) d[i] = (char)(p[i]);
    d[i++] = '\n';
    d[i] = '\0';
    Log_Debug(d);
    il2c_mcfree(d);
}

bool il2c_readline(wchar_t* buffer, int32_t length)
{
    // Can't read from the default console on the Azure Sphere MCU.
    return false;
}

#endif