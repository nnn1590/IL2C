﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;

using Mono.Cecil.Cil;
using NUnit.Framework;

using IL2C.ILConverters;

namespace IL2C
{
    [TestFixture]
    public sealed class Generators
    {
        [Test]
        public static async Task DumpSupportedOpCodes()
        {
            var ilConverterType = typeof(ILConverter);
            var ilConverters =
                ilConverterType.Assembly.GetTypes()
                .Where(type => type.IsClass && type.IsSealed
                    && ilConverterType.IsAssignableFrom(type)
                    && (type.GetConstructor(Type.EmptyTypes) != null))
                .Select(type => (ILConverter)Activator.CreateInstance(type))
                .ToDictionary(ilConverter => ilConverter.OpCode.Name, StringComparer.InvariantCultureIgnoreCase);

            var refOpCodeNames =
                typeof(System.Reflection.Emit.OpCodes)
                .GetFields(BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly)
                .Where(field => field.FieldType == typeof(System.Reflection.Emit.OpCode))
                .Select(field =>
                {
                    var opCode = (System.Reflection.Emit.OpCode)field.GetValue(null);
                    return (opCode.Value, field.Name);
                })
                .ToDictionary(entry => entry.Item1, entry => entry.Item2);

            var opCodes =
                typeof(OpCodes)
                .GetFields(BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly)
                .Where(field => field.FieldType == typeof(OpCode))
                .Select(field =>
                {
                    var opCode = (OpCode)field.GetValue(null);
                    var isEmitRelated = refOpCodeNames.TryGetValue(opCode.Value, out var refOpCodeName);
                    var name = isEmitRelated ? refOpCodeName : opCode.Name;
                    return (name, isEmitRelated, opCode);
                })
                .OrderBy(entry => (ushort)entry.opCode.Value)
                .ToArray();

            var ilConverterTests =
                typeof(ILConvertersTest)
                .GetFields(BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly)
                .Where(field => field.IsInitOnly && (field.FieldType == typeof(TestCaseInformation[])))
                .SelectMany(field => (TestCaseInformation[])field.GetValue(null))
                .GroupBy(entry => entry.Method.DeclaringType.Name)
                .ToDictionary(g => g.Key, g => g.Count(), StringComparer.InvariantCultureIgnoreCase);

            var basePath = Path.GetFullPath(
                Path.Combine(
                    Path.GetDirectoryName(typeof(Generators).Assembly.Location),
                    "..",
                    "..",
                    "..",
                    "..",
                    ".."));
            var path = Path.Combine(basePath, "supported-opcodes.md");

            using (var fs = await TestUtilities.CreateStreamAsync(path))
            {
                var tw = new StreamWriter(fs);

                await tw.WriteLineAsync("# Supported IL opcodes");
                await tw.WriteLineAsync();
                await tw.WriteLineAsync("OpCode | Binary | Implement | Test | ILConverter");
                await tw.WriteLineAsync("|:---|:---|:---|:---|:---|");

                foreach (var (name, isEmitRelated, opCode) in opCodes)
                {
                    ilConverters.TryGetValue(opCode.Name, out var ilConverter);

                    var opCodeName = opCode.Name.TrimEnd('.');
                    await tw.WriteLineAsync(
                        string.Format("| {0} | 0x{1:x} | {2} | {3} | {4} |",
                            isEmitRelated ?
                                string.Format(
                                    "[{0}](https://docs.microsoft.com/en-us/dotnet/api/system.reflection.emit.opcodes.{1})",
                                    opCodeName, name.ToLowerInvariant()) :
                                opCodeName,
                            (ushort)opCode.Value,
                            (ilConverter != null) ? "Implemented" : string.Empty,
                            ilConverterTests.TryGetValue(name, out var count) ?
                                string.Format("[Test [{0}]](tests/IL2C.Core.Test.Target/ILConverters/{1})", count, name) :
                                string.Empty,
                            ilConverter?.GetType().FullName ?? string.Empty));
                }

                await tw.FlushAsync();
            }
        }
    }
}