#include "il2c_private.h"
#include <System/Console.h>

/////////////////////////////////////////////////////////////
// System.Console

extern void ReadLine(wchar_t* pBuffer, uint16_t length);

void System_Console_Write_9(System_String* value)
{
    // TODO: NullReferenceException
    il2c_assert(value != NULL);

    il2c_assert(value->string_body__ != NULL);
    il2c_wwrite(value->string_body__);
}

void System_Console_WriteLine(void)
{
    il2c_wwriteline(L"");
}

void System_Console_WriteLine_6(int32_t value)
{
    wchar_t buf[20];
    il2c_itow(value, buf, 19);
    il2c_wwrite(buf);
}

void System_Console_WriteLine_10(System_String* value)
{
    // TODO: NullReferenceException
    il2c_assert(value != NULL);

    il2c_assert(value->string_body__ != NULL);
    il2c_wwriteline(value->string_body__);
}

// TODO: limitation
#define MAX_READLINE 128

System_String* System_Console_ReadLine(void)
{
    wchar_t buffer[MAX_READLINE];

    il2c_fgetws(buffer, MAX_READLINE, stdin);
    return il2c_new_string(buffer);
}

/////////////////////////////////////////////////
// VTable and runtime type info declarations

IL2C_RUNTIME_TYPE_STATIC(System_Console, "System.Console", System_Object);