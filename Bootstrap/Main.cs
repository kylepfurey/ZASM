// .cs
// Z Entry Point
// by Kyle Furey

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using ZLang;

/** The class that contains the Main() function. */
public static class Program
{
    /** Compiles a Z assembly, executable, or library. */
    public static int Main(params string[] args)
    {
        int i = 0;
        foreach (string arg in args)
        {
            Print.Log($"Argument {i++}: {arg}");
        }

        if (args.Length <= 0)
        {
            Print.Error("No files were given!");
            return Print.ERROR;
        }

        return Compiler.CompileFile(
            args.Length >= 2 ? args[^2] : args[^1],
            args.Length >= 2 ? args[^1] : null
        );
    }
}