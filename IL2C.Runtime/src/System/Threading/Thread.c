#include "il2c_private.h"

/////////////////////////////////////////////////////////////
// System.Threading.Thread

void System_Threading_Thread__ctor(System_Threading_Thread* this__, System_Threading_ThreadStart* start)
{
    il2c_assert(this__ != NULL);

    // TODO: ArgumentNullException
    il2c_assert(start != NULL);

    this__->start__ = (System_Delegate*)start;
    this__->rawHandle__ = -1;
}

void System_Threading_Thread_Finalize(System_Threading_Thread* this__)
{
    il2c_assert(this__ != NULL);

    if (this__->rawHandle__ != -1)
    {
        il2c_close_thread_handle__(this__->rawHandle__);
#if defined(_DEBUG)
        this__->rawHandle__ = -1;
        this__->id__ = 0;
#endif
    }
}

extern IL2C_TLS_INDEX g_TlsIndex__;

static IL2C_THREAD_ENTRY_POINT_RESULT_TYPE System_Threading_Thread_InternalEntryPoint(
    IL2C_THREAD_ENTRY_POINT_PARAMETER_TYPE parameter)
{
    il2c_assert(parameter != NULL);

    System_Threading_Thread* pThread = (System_Threading_Thread*)parameter;
    il2c_assert(pThread->vptr0__ == &System_Threading_Thread_VTABLE__);
    il2c_assert(il2c_isinst(pThread->start__, System_Threading_ThreadStart) != NULL);

    // Set real thread id.
    pThread->id__ = il2c_get_current_thread_id__();

    // Save IL2C_THREAD_CONTEXT into tls.
    il2c_set_tls_value(g_TlsIndex__, (void*)&pThread->pFrame__);

    // It's naive for passing handle if startup with suspending not implemented. (pthread/FreeRTOS)
    while (pThread->rawHandle__ == -1);

    // Invoke delegate.
    // TODO: catch exception.
    System_Threading_ThreadStart_Invoke((System_Threading_ThreadStart*)(pThread->start__));

    // Unregister.
    il2c_unregister_fixed_instance__(pThread);

#if defined(_DEBUG)
    il2c_set_tls_value(g_TlsIndex__, NULL);
#endif

    IL2C_THREAD_ENTRY_POINT_RETURN(0);
}

void System_Threading_Thread_Start(System_Threading_Thread* this__)
{
    il2c_assert(this__ != NULL);

    // TODO: InvalidOperationException? (Auto attached managed thread)
    il2c_assert(this__->start__ != NULL);

    // TODO: ThreadStateException
    il2c_assert(this__->rawHandle__ >= 0);

    // Register into statically resource.
    il2c_register_fixed_instance__(this__);

    // Create (suspended if available) thread.
    intptr_t rawHandle = il2c_create_thread__(
        System_Threading_Thread_InternalEntryPoint, this__);

    // TODO: OutOfMemoryException
    il2c_assert(rawHandle >= 0);

    // It's naive for passing handle if startup with suspending not implemented. (pthread/FreeRTOS)
    this__->rawHandle__ = rawHandle;
    il2c_resume_thread__(rawHandle);
}

void System_Threading_Thread_Join(System_Threading_Thread* this__)
{
    il2c_assert(this__ != NULL);
    il2c_assert(this__->rawHandle__ != 0);
    il2c_assert(this__->start__ != NULL);

    il2c_join_thread__(this__->rawHandle__);
}

int32_t System_Threading_Thread_get_ManagedThreadId(System_Threading_Thread* this__)
{
    il2c_assert(this__ != NULL);
    il2c_assert(this__->rawHandle__ >= 0);

    return this__->id__;
}

System_Threading_Thread* System_Threading_Thread_get_CurrentThread(void)
{
    // NOTE: Will assertion failed if doesn't construct any execution frames.
    //   But maybe construct it because we'll save the Thread instance into execution frame strcuture...
    IL2C_THREAD_CONTEXT* pThreadContext = il2c_get_tls_value(g_TlsIndex__);

    // TODO: Auto attach now?
    il2c_assert(pThreadContext != NULL);

    // Come from unoffsetted:
    return (System_Threading_Thread*)((*(uint8_t*)pThreadContext) - offsetof(System_Threading_Thread, pFrame__));
}

void System_Threading_Thread_Sleep(int millisecondsTimeout)
{
    il2c_sleep((uint32_t)millisecondsTimeout);
}

/////////////////////////////////////////////////
// VTable and runtime type info declarations

static void System_Threading_Thread_MarkHandler(System_Threading_Thread* thread)
{
    il2c_assert(thread != NULL);
    il2c_assert(thread->vptr0__ == &System_Threading_Thread_VTABLE__);

    // Check start field.
    il2c_default_mark_handler__(thread->start__);

    ///////////////////////////////////////////////////////////////
    // Check IL2C_EXECUTION_FRAME.
    // It's important step for GC collecting sequence.
    // All method execution frame traversal begins this location.

    il2c_step2_mark_gcmark__(thread->pFrame__);
}

System_Threading_Thread_VTABLE_DECL__ System_Threading_Thread_VTABLE__ = {
    0, // Adjustor offset
    (bool(*)(void*, System_Object*))System_Object_Equals,
    (void(*)(void*))System_Threading_Thread_Finalize,
    (int32_t(*)(void*))System_Object_GetHashCode,
    (System_String* (*)(void*))System_Object_ToString
};

IL2C_RUNTIME_TYPE_BEGIN(
    System_Threading_Thread,
    "System.Threading.Thread",
    IL2C_TYPE_REFERENCE | IL2C_TYPE_WITH_MARK_HANDLER,
    sizeof(System_Threading_Thread),
    System_Object,
    System_Threading_Thread_MarkHandler,
    0)
IL2C_RUNTIME_TYPE_END();
