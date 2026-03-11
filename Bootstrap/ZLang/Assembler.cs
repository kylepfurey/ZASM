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
                byte[]? targetCode = AssembleString(sourceCode);

                if (targetCode == null)
                {
                    Print.Error("Failed to assemble source code!");
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
        /// Assembles the given string into a Z binary.
        /// </summary>
        public static byte[]? AssembleString(string source)
        {
            LinkedList<string> tokens = TokenizeString(InlineCode(source));

            tokens.PrintAll();

            MemoryStream target = new();

            // TODO

            byte[]? result = target.ToArray();

            Print.Log("Assembly successful.");
            return result;
        }

        /// <summary>
        /// Iterates the string and strips out comments, newlines, and whitespace.
        /// </summary>
        public static string InlineCode(string source, char comment = ';', char delim = ' ')
        {
            StringBuilder target = new();

            bool awaitNewline = false;
            bool previousSpace = false;
            foreach (char c in source)
            {
                if (!awaitNewline)
                {
                    if (c == comment)
                    {
                        awaitNewline = true;
                    }
                    else if (char.IsWhiteSpace(c))
                    {
                        if (!previousSpace)
                        {
                            previousSpace = true;
                            target.Append(delim);
                        }
                    }
                    else
                    {
                        previousSpace = false;
                        target.Append(c);
                    }
                }
                else
                {
                    if (c == '\n')
                    {
                        awaitNewline = false;
                        if (!previousSpace)
                        {
                            previousSpace = true;
                            target.Append(delim);
                        }
                    }
                }
            }

            return target.ToString();
        }

        /// <summary>
        /// Takes a string and splits it into tokens.
        /// </summary>
        public static LinkedList<string> TokenizeString(string source, char delim = ' ')
        {
            LinkedList<string> list = new();

            source
                .Split(delim)
                .Where((str) => !string.IsNullOrWhiteSpace(str))
                .ForEach((str) => list.AddLast(str.Trim()));
            // TODO - Combine string tokens, separate operator tokens

            return list;
        }
    }
}