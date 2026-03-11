// .cs
// Z Compiler Functions
// by Kyle Furey

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ZLang
{
    /** Static functions for the Z compiler. */
    public static class Compiler
    {
        /// <summary>
        /// Z code file extension.
        /// </summary>
        public const string ZCODE_EXT = ".zcode";

        /// <summary>
        /// Z application code file extension.
        /// </summary>
        public const string ZAC_EXT = ".zac";

        /// <summary>
        /// Z library file extension.
        /// </summary>
        public const string ZLIB_EXT = ".zlib";

        /// <summary>
        /// The target for a compiled file.
        /// </summary>
        public enum CompilationTarget
        {
            /// <summary>
            /// .zac / .zlib binary.
            /// </summary>
            BINARY,

            /// <summary>
            /// .zasm assembly.
            /// </summary>
            ASSEMBLY,
        }

        /// <summary>
        /// Compiles the given .zcode file into a .zac executable or .zlib library.
        /// </summary>
        public static int CompileFile(string source, string? target = null)
        {
            try
            {
                string sourceDir = Path.GetDirectoryName(source) ?? "";
                string sourceName = Path.GetFileNameWithoutExtension(source) ?? "";
                string sourceExt = Path.GetExtension(source) ?? "";

                switch (sourceExt.ToLower())
                {
                    case ZCODE_EXT:
                        break;
                    case Assembler.ZASM_EXT:
                        return Assembler.AssembleFile(source, target);
                    default:
                        Print.Error("Invalid compiler source extension! { .zcode, .zasm }");
                        return Print.ERROR;
                }

                target ??= (Path.Combine(sourceDir, sourceName) + ZAC_EXT);
                string targetExt = Path.GetExtension(target) ?? "";

                switch (targetExt.ToLower())
                {
                    case Assembler.ZASM_EXT:
                    case ZAC_EXT:
                    case ZLIB_EXT:
                        break;
                    default:
                        Print.Error("Invalid compiler target extension! { .zasm, .zac, .zlib }");
                        return Print.ERROR;
                }

                Print.Log($"Compiling \"{source}\" to \"{target}\".");
                string sourceCode = File.ReadAllText(source);
                byte[]? targetCode = CompileString(
                    sourceCode,
                    targetExt.ToLower() == Assembler.ZASM_EXT ? CompilationTarget.ASSEMBLY : CompilationTarget.BINARY
                );

                if (targetCode == null)
                {
                    Print.Error("Failed to compile source code!");
                    return Print.ERROR;
                }

                File.WriteAllBytes(target, targetCode);

                return Print.OK;
            }
            catch (Exception e)
            {
                Print.Error(e.Message);
                return Print.ERROR;
            }
        }

        /// <summary>
        /// Compiles the given string into a Z binary.
        /// </summary>
        public static byte[]? CompileString(string source, CompilationTarget format = CompilationTarget.BINARY)
        {
            MemoryStream target = new();

            // TODO

            byte[]? result = target.ToArray();

            if (format == CompilationTarget.BINARY)
            {
                result = Assembler.AssembleString(Encoding.UTF8.GetString(result));
            }

            Print.Log("Compilation successful.");
            return result;
        }
    }
}