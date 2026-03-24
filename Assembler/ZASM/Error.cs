// .cs
// ZASM Error Logging Functions
// by Kyle Furey

namespace ZASM
{
    /** Static functions for logging errors. */
    public static class Print
    {
        /// <summary>
        /// Returned when the ZASM assembler executes OK.
        /// </summary>
        public const int OK = 0;

        /// <summary>
        /// Returned when the ZASM assembler encounters an error.
        /// </summary>
        public const int ERROR = 'Z';

        /// <summary>
        /// Logs a message to the ZASM assembler.
        /// </summary>
        public static void Log(params object[] msg)
        {
            Console.Out.Write("ZASM LOG:\t");

            foreach (var obj in msg)
            {
                Console.Out.Write(obj.ToString());
            }

            Console.Out.WriteLine();
        }

        /// <summary>
        /// Logs a warning to the ZASM assembler.
        /// </summary>
        public static void Warning(params object[] msg)
        {
            Console.Out.Write("ZASM WARNING:\t");

            foreach (var obj in msg)
            {
                Console.Out.Write(obj.ToString());
            }

            Console.Out.WriteLine();
        }

        /// <summary>
        /// Logs an error to the ZASM assembler.
        /// </summary>
        public static void Error(params object[] msg)
        {
            Console.Error.Write("ZASM ERROR:\t");

            foreach (var obj in msg)
            {
                Console.Error.Write(obj.ToString());
            }

            Console.Error.WriteLine();
        }
    }
}