// .cs
// Z Assembler Functions
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
    /** Static functions for the Z assembler. */
    public static class Assembler
    {
        /// <summary>
        /// Z assembly file extension.
        /// </summary>
        public const string ZASM_EXT = ".zasm";

        /// <summary>
        /// Z application code file extension.
        /// </summary>
        public const string ZAC_EXT = ".zac";

        /// <summary>
        /// Z library file extension.
        /// </summary>
        public const string ZLIB_EXT = ".zlib";

        /// <summary>
        /// Compiles the given .zasm file into a .zac executable or .zlib library.
        /// </summary>
        public static int AssembleFile(string source, string? target = null)
        {
            try
            {
                string fromDir = Path.GetDirectoryName(source) ?? "";
                string fromName = Path.GetFileNameWithoutExtension(source) ?? "";
                string fromExt = Path.GetExtension(source) ?? "";

                switch (fromExt.ToLower())
                {
                    case ZASM_EXT:
                        break;
                    case Compiler.ZCODE_EXT:
                        return Compiler.CompileFile(source, target);
                    default:
                        Print.Error("Invalid assembly source extension! { .zasm, .zcode }");
                        return Print.ERROR;
                }

                target ??= (Path.Combine(fromDir, fromName) + ZAC_EXT);
                string targetExt = Path.GetExtension(target) ?? "";

                switch (targetExt.ToLower())
                {
                    case ZAC_EXT:
                    case ZLIB_EXT:
                        break;
                    default:
                        Print.Error("Invalid assembly target extension! { .zac, .zlib }");
                        return Print.ERROR;
                }

                Print.Log($"Assembling \"{source}\" to \"{target}\".");
                string sourceCode = File.ReadAllText(source);
                string? targetCode = AssembleString(sourceCode);

                if (targetCode == null)
                {
                    Print.Error("Failed to assemble source code!");
                    return Print.ERROR;
                }

                File.WriteAllText(target, targetCode);

                return Print.OK;
            }
            catch (Exception e)
            {
                Print.Error(e.Message);
                return Print.ERROR;
            }
        }

        /// <summary>
        /// Assembles the given string into a Z binary.
        /// </summary>
        public static string? AssembleString(string source)
        {
            string? target = "";

            // TODO

            Print.Log("Assembly successful.");
            return target;
        }
    }
}