﻿.class public IL2C.BasicTypes.System_Int64
{
    .method public static bool IsValueType() cil managed
    {
        .maxstack 2
        ldc.i8 1234567890123
        box int64
        isinst [netstandard]System.ValueType
        brnull.s F1
        ldc.i4.1
        ret
    F1:
        ldc.i4.0
        ret
    }

    .method public static int32 SizeOf() cil managed
    {
        .maxstack 1
        sizeof int64
        ret
    }
}
