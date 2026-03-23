// .cs
// ZASM Extension Functions
// by Kyle Furey

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ZASM
{
    /// <summary>
    /// Extension helper methods for the ZASM tools.
    /// </summary>
    public static class Extensions
    {
        /// <summary>
        /// Calls <action> on each element in <collection>.
        /// </summary>
        public static IEnumerable ForEach(this IEnumerable collection, Action<object> action)
        {
            foreach (object elem in collection)
            {
                action(elem);
            }

            return collection;
        }

        /// <summary>
        /// Calls <action> on each element in <collection>.
        /// </summary>
        public static IEnumerable<T> ForEach<T>(this IEnumerable<T> collection, Action<T> action)
        {
            foreach (T elem in collection)
            {
                action(elem);
            }

            return collection;
        }

        /// <summary>
        /// Prints all elements in a collection.
        /// </summary>
        public static IEnumerable PrintAll(this IEnumerable collection)
        {
            return collection.ForEach((object? elem) => Print.Log(elem != null ? elem.ToString()! : "null"));
        }

        /// <summary>
        /// Prints all elements in a collection.
        /// </summary>
        public static IEnumerable<T> PrintAll<T>(this IEnumerable<T> collection)
        {
            return collection.ForEach((T? elem) => Print.Log(elem != null ? elem.ToString()! : "null"));
        }
    }
}