// .cs
// Z Error Logging Functions
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
    /** Static functions for logging errors. */
    public static class Print
    {
        /// <summary>
        /// Returned when the Z compiler executes OK.
        /// </summary>
        public const int OK = 0;

        /// <summary>
        /// Returned when the Z compiler encounters an error.
        /// </summary>
        public const int ERROR = 'Z';

        /// <summary>
        /// Logs a message to the Z compiler.
        /// </summary>
        public static void Log(params object[] msg)
        {
            Console.Out.Write("Z LOG:\t\t");

            foreach (var obj in msg)
            {
                Console.Out.Write(obj.ToString());
            }

            Console.Out.WriteLine();
        }

        /// <summary>
        /// Logs a warning to the Z compiler.
        /// </summary>
        public static void Warning(params object[] msg)
        {
            Console.Out.Write("Z WARNING:\t");

            foreach (var obj in msg)
            {
                Console.Out.Write(obj.ToString());
            }

            Console.Out.WriteLine();
        }

        /// <summary>
        /// Logs an error to the Z compiler.
        /// </summary>
        public static void Error(params object[] msg)
        {
            Console.Error.Write("Z ERROR:\t");

            foreach (var obj in msg)
            {
                Console.Error.Write(obj.ToString());
            }

            Console.Error.WriteLine();
        }
    }
}