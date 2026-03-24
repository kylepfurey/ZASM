// .cs
// ZASM Entry Point
// by Kyle Furey

using ZASM;

/** The class that contains the Main() function. */
public static class Program
{
    /** Compiles a ZASM assembly or library. */
    public static int Main(params string[] args)
    {
        if (args.Length <= 0)
        {
            Print.Error("No files were given!");
            return Print.ERROR;
        }

        return Assembler.AssembleFile(
            args.Length > 1 ? args[^2] : args[^1],
            args.Length > 1 ? args[^1] : null
        );
    }
}